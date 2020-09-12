#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

#define N 100000   // N = 5 million (number of data points)
// #define N 20
#define N_Trials 50 // Number of query rectangles searched

/* Range of random numbers generated*/
#define LOWER 0
#define UPPER 20

using namespace std;

/* Global Variables for Max and Min number of childrens */
int M=-1, m=-1, RTNodeEntryNum=0, RTNodeNum=0;
int n;  /* dimension of rectangles */
int nodes_visited;


/*Wrapper for malloc checks for out of memory*/
static void* mem_alloc(size_t size) {
   void* mem = malloc(size);
   if (!mem) {
      cerr << "[ERROR!] Fatal: Out Of Memory." << endl;
      exit(EXIT_FAILURE);
   }
   memset(mem, 0, size);
   return mem;
}


/* Read tree from a file 
Each line corresponds to a node
node_num  parent_node_num  num_entries  R1(rect_num, dmin[], dmax[], child_node_num)  R2(dmin[], dmax[], child_node_num)
*/
void ReadTree(RTNode* node, ifstream &fin){
    
    if(node == NULL){
        return;
    }

    fin.seekg(ios::beg);

    int node_num;
    static int c=1;

    /* Find line from the file corresponding to the current node */
    string line_tmp;
    istringstream line;
    
    while(getline(fin, line_tmp)){
        cout << c++ << endl;
        line.str(line_tmp);
        line >> node_num;
        if(node_num == node->RTNode_num)
            break;
    }

    /* Add Rectangles to the node */
    int parent_node_num, num_entry, child_node_num;
    line>>parent_node_num>>num_entry;

    RTNodeEntry* cur_entry;
    for(int i = 0; i < num_entry; i++){
        cur_entry = (RTNodeEntry *)mem_alloc(sizeof(RTNodeEntry));
        line>>(cur_entry->RTNodeEntry_num);

        cur_entry->dmin.resize(n);
        cur_entry->dmax.resize(n);

        for(int j = 0; j < n; j++){
            line >> (cur_entry->dmin[j]) >> (cur_entry->dmax[j]);
        }
        
        /* Add child node to the rectangle */
        line >> child_node_num;

        if(child_node_num != -1){
            cur_entry->child = (RTNode *)mem_alloc(sizeof(RTNode));
            cur_entry->child->RTNode_num = child_node_num;
            cur_entry->child->parent = node;
        }
        else{
            cur_entry->child = NULL;
        }
        
        node->entry.push_back(*cur_entry);
    }

    /* Read subtrees */
    for(int i = 0; i < num_entry; i++){
        cur_entry = &(node->entry[i]);
        if(cur_entry->child != NULL){
            ReadTree(cur_entry->child, fin);
        }
    }

}

/* Search RTree Algorithm */
void Search(RTNode* node, RTNodeEntry* S, vector<RTNodeEntry> &out){
    if(node == NULL){
        return;
    }

    nodes_visited++;

    int num_entry = node->entry.size();

    RTNodeEntry* cur_entry;
    for(int i = 0; i < num_entry; i++){
        cur_entry = &(node->entry[i]);

        /* Check overlap between current entry and query rectangle */
        bool overlap = true;
        for(int j = 0; j < n; j++){
            if(cur_entry->dmax[j] > S->dmin[j] && S->dmax[j] > cur_entry->dmin[j]){
                continue;
            }
            else{
                overlap = false;
                break;
            }
        }

        if(overlap){
            /* [S1] */
            if(cur_entry->child != NULL){
                Search(cur_entry->child, S, out);
            }
            else{
            /* [S2] */
                out.push_back(*cur_entry);
            }
        }
    }

}


RTNodeEntry gen_query_rect(){
    int min_num, max_num;

    RTNodeEntry query_rect;
    query_rect.child = NULL;
    query_rect.RTNodeEntry_num = -1;
    
    query_rect.dmin.resize(n);
    query_rect.dmax.resize(n);

    for(int i = 0; i < n; i++){
        do {
            min_num = (rand() % (UPPER - LOWER +1)) + LOWER;
            max_num = (rand() % (UPPER - LOWER +1)) + LOWER;
        }while(max_num < min_num);
        query_rect.dmin[i] = min_num;
        query_rect.dmax[i] = max_num;
    }
    return query_rect;
}


void printEntry(RTNodeEntry &R){
    for(int i = 0; i < n; i++){
        cout<<R.dmin[i]<<" "<<R.dmax[i]<<" ";
    }
    cout<<endl;
}


int main(int argc, char** argv){
    n = -1;
    string filename;

    /* Sanity Check for CLI*/
    if(argc < 2){
        cerr << "[Error] Enter the number of dimensions to generate data!!" << endl;
        exit(0);
    }
    else if(argc > 2){
        cerr << "[Error] Extra command line arguments added!!" << endl;
        exit(0);
    }

    n = atoi(argv[1]);
    if(n == 0 && strcmp(argv[1], "0")){
        cerr << "[Error] Integer command line argument expected!!" << endl;
        exit(0);
    }

    /* Create filename */
    filename = "RTree_dim=" + to_string(n) + "_N=" + to_string(N) + ".txt";

    /* Initialize M and m (Max and Min number of childrens for a node) */
    ::M = floor(4096 / (4*n+1));
    ::m = floor(M/2);


    ifstream fin(filename);

    if(!fin.is_open()){
        cout<<"File could not be opened"<<endl;
        exit(0);
    }
    
    int root_node_num;

    fin >> root_node_num;    

    RTNode* root = (RTNode *)mem_alloc(sizeof(RTNode));
    root->parent = NULL;
    root->RTNode_num = root_node_num;

    cout << "Reading Tree..." <<  endl;
    ReadTree(root, fin);

    fin.close();
    cout << "Reading Complete\n" << endl;

    srand(time(0));
    RTNodeEntry query_rect;
    vector<RTNodeEntry> out;

    clock_t start, end;
    double total_time = 0, time_taken;
    int total_nodes_visited = 0;

    for(int i = 0; i < N_Trials; i++){
        query_rect = gen_query_rect();
        cout << "Query Rectangle " << i << endl;
        printEntry(query_rect);

        nodes_visited = 0;

        start = clock(); 
        Search(root, &query_rect, out);
        end = clock();

        time_taken = double(end - start) / CLOCKS_PER_SEC;
        total_time += time_taken;
        cout << "Time taken : " << time_taken << endl;

        total_nodes_visited += nodes_visited;
        cout << "Number of Nodes visited : " << nodes_visited << endl;

        cout << "Overlapping Rectangles : " << out.size() << endl;
        // for(int j = 0; j < out.size(); j++){
        //     printEntry(out[j]);
        // }
        cout<<endl;

        out.clear();
    }

    cout << "Average Time Taken : " << total_time/N_Trials << endl;
    cout << "Average Number of Nodes visited : " << total_nodes_visited/N_Trials << endl;

    return 0;
}