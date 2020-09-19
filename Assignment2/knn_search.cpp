#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

#define N 500000   // N = 0.5 million (number of data points)
#define N_Trials 50 // Number of query rectangles searched

/* Range of random numbers generated*/
#define LOWER 10
#define UPPER 40

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
void ReadTree(RTNode* node, unordered_map<int, string> &lines){
    
    if(node == NULL){
        return;
    }

    /* Find line from the file corresponding to the current node */

    string line_tmp = lines[node->RTNode_num];
    istringstream line(line_tmp);
    
    int node_num;
    line >> node_num;

    int parent_node_num, num_entry, child_node_num;
    line >> parent_node_num >> num_entry;

    /* Add Rectangles to the node */

    RTNodeEntry* cur_entry;
    for(int i = 0; i < num_entry; i++){
        cur_entry = (RTNodeEntry *)mem_alloc(sizeof(RTNodeEntry));
        line >> (cur_entry->RTNodeEntry_num);

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
            ReadTree(cur_entry->child, lines);
        }
    }

}


/* Read the file and stores it in memory */
int ReadFile(string filename, unordered_map<int, string> &lines){
    ifstream fin(filename);

    if(!fin.is_open()){
        cout << "File could not be opened" << endl;
        exit(0);
    }

    int root_node_num;

    fin>>root_node_num;

    fin.seekg(ios::beg);

    string line;
    istringstream line_tmp;

    int node_num;
    while(getline(fin, line)){ 
        line_tmp.str(line);
        line_tmp >> node_num;
        lines[node_num] = line;
    }

    fin.close();
    return root_node_num;
}



/* Generates a query point to be searched */
vector<int> gen_query_point(){
    vector<int> query_point(n);

    for(int i = 0; i < n; i++){
        query_point[i] = (rand() % (UPPER - LOWER +1)) + LOWER;
    }
    return query_point;
}


int mindist(vector<int> point, RTNodeEntry* R){

}

int minmaxdist(vector<int> point, RTNodeEntry* R){
    
}

int objdist(vector<int> point, vector<int> object){

}

vector<NearestN> Search(RTNode* root, vector<int> point){

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

    unordered_map<int, string> lines;

    cout << "Loading File..." <<  endl;
    int root_node_num = ReadFile(filename, lines);
    cout << "Done Loading" <<  endl;  

    RTNode* root = (RTNode *)mem_alloc(sizeof(RTNode));
    root->parent = NULL;
    root->RTNode_num = root_node_num;

    cout << "Reading Tree..." <<  endl;
    ReadTree(root, lines);
    cout << "Reading Complete\n" << endl;    
    lines.clear();

    srand(time(0));
    vector<int> query_point(n);
    vector<RTNodeEntry> out;

    clock_t start, end;
    double total_time = 0, time_taken;
    int total_nodes_visited = 0;

    for(int i = 0; i < N_Trials; i++){
        query_point = gen_query_point();
        cout << "Query Point " << i << endl;

        nodes_visited = 0;

        start = clock(); 
        //Search(root, &query_rect, out);
        end = clock();

        time_taken = double(end - start) / CLOCKS_PER_SEC;
        total_time += time_taken;
        cout << "Time taken : " << time_taken << endl;

        total_nodes_visited += nodes_visited;
        cout << "Number of Nodes visited : " << nodes_visited << endl;

        cout << "Overlapping Rectangles : " << out.size() << endl;
        cout<<endl;

        out.clear();
    }

    cout << "Average Time Taken : " << total_time/N_Trials << endl;
    cout << "Average Number of Nodes visited : " << total_nodes_visited/N_Trials << endl;

    return 0;
}