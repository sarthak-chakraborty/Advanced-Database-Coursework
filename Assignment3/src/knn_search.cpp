#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

#define DIM 256
#define N_Trials 1 // Number of query rectangles searched


using namespace std;

/* Global Variables for Max and Min number of childrens */
int M=-1, m=-1, RTNodeEntryNum=0, RTNodeNum=0;
int nodes_visited;
int k;  /* Number of nearest neighbors */


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
        line >> (cur_entry->RTNodeEntry_num) >> (cur_entry->doc_num) >> (cur_entry->pic_num);

        cur_entry->dmin.resize(DIM);
        cur_entry->dmax.resize(DIM);

        for(int j = 0; j < DIM; j++){
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

    fin >> root_node_num;

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
    vector<int> query_point;

    FILE* fin = fopen("query_image.txt", "r");
    if(fin == NULL){
        cerr << "[ERROR] Cannot Open File" << endl;
        exit(0);
    }
    char* line = new char[5000]; 

     while(fgets(line, 5000, fin)){
        char* token = strtok(line, " ");

        while(token != NULL){
        if(atoi(token) != 0 || !strcmp(token, "0")){
            query_point.push_back(atoi(token));
        }
        token = strtok(NULL, " ");
        }
     }

    return query_point;
}



/* Calculated MINDIST of a point to an MBR R */
ll mindist(vector<int> point, RTNodeEntry* R){
    ll min_dist = 0;

    for(int i = 0; i < point.size(); i++){
        if(point[i] < R->dmin[i]){
            min_dist += pow((point[i] - R->dmin[i]), 2);
        }
        else if(point[i] > R->dmax[i]){
            min_dist += pow((point[i] - R->dmax[i]), 2);
        }
    }

    return min_dist;
}



/* Calculates MINMAXDIST of a point to an MBR R */
ll minmaxdist(vector<int> point, RTNodeEntry* R){
    ll minmax_dist = -1;

    ll S = 0;
    for(int i = 0; i < point.size(); i++){
        if(point[i] >= (float)(R->dmin[i] + R->dmax[i]) / 2)
            S += pow((point[i] - R->dmin[i]), 2);
        else
            S += pow((point[i] - R->dmax[i]), 2);
    }

    for(int i = 0; i < point.size(); i++){
        int rm, rM;

        if(point[i] >= (float)(R->dmin[i] + R->dmax[i]) / 2)
            rM = R->dmin[i];
        else
            rM = R->dmax[i];

        if(point[i] <= (float)(R->dmin[i] + R->dmax[i]) / 2)
            rm = R->dmin[i];
        else
            rm = R->dmax[i];

        ll dist = S - pow((point[i] - rM), 2) + pow((point[i] - rm), 2);

        if(minmax_dist == -1 || dist < minmax_dist)
            minmax_dist = dist;
    }

    return minmax_dist;
}



/* Calculates the actual Object Distance of the query point to an object in R-Tree */
ll objdist(vector<int> point, vector<int> object){
    ll dist = 0;
    for(int i = 0; i < point.size(); i++){
        dist += pow((point[i] - object[i]), 2);
    }

    return dist;
}


/* define order for priority queue */
struct CompareNNeighbor{
    bool operator()(NearestN const &N1, NearestN const &N2){
        return N1.dist < N2.dist;
    }
};


/* define order for Active Branch List */
bool sortbyminmaxdist(const ABLEntry &N1, const ABLEntry &N2){
    if(N1.minmaxdist != N2.minmaxdist)
        return N1.minmaxdist < N2.minmaxdist;

    return N1.mindist < N2.mindist;
}



/* kNN Search - Returns k Nearest Neighbors - objects and distances */
void kNN_Search(RTNode* node, vector<int> &point, priority_queue<NearestN, vector<NearestN>, CompareNNeighbor> &nearest){
    if(node == NULL){
        return;
    }

    nodes_visited++;

    int num_entry = node->entry.size();

    
    /* Leaf Node */
    if(node->entry[0].child == NULL){        
        for(int i = 0; i < num_entry; i++){
            RTNodeEntry* cur_entry = &(node->entry[i]);
            ll dist = objdist(point, cur_entry->dmin);

            NearestN cur_neighbor;
            cur_neighbor.object = cur_entry;
            cur_neighbor.dist = dist;

            /* less than k nearest neighbors found */
            if(nearest.size() < k){
                nearest.push(cur_neighbor);
            }
            /* current neighbor is nearer than furthest nearest neighbor */
            else if(dist < nearest.top().dist){
                nearest.pop();
                nearest.push(cur_neighbor);
            }
        }
        return;
    }

    /* Non-leaf Node */
    vector<ABLEntry> ABL;

    /* Calculate MINDIST and MINMAXDIST for all entries in the node */
    for(int i = 0; i < num_entry; i++){
        RTNodeEntry* cur_entry = &(node->entry[i]);

        ABLEntry cur_ablentry;
        cur_ablentry.entry = cur_entry;
        cur_ablentry.mindist = mindist(point, cur_entry);
        cur_ablentry.minmaxdist = minmaxdist(point, cur_entry);

        ABL.push_back(cur_ablentry);
    }

    sort(ABL.begin(), ABL.end(), sortbyminmaxdist);

    for(int i = 0; i < num_entry; i++){
        /* Pruning Strategy 1 */
        if(i >= k && ABL[i].mindist > ABL[0].minmaxdist){
            continue;
        }
        /* Pruning Strategy 3 */
        if(nearest.size() >= k && ABL[i].mindist > nearest.top().dist){ 
            continue;
        }
        else{
            kNN_Search(ABL[i].entry->child, point, nearest);
        }
    }
}


int main(int argc, char** argv){
    k = 1;
    string filename;

    /* Sanity Check for CLI*/
    if(argc < 2){
        cerr << "[Error] Enter k!!" << endl;
        exit(0);
    }
    else if(argc > 2){
        cerr << "[Error] Extra command line arguments added!!" << endl;
        exit(0);
    }

    k = atoi(argv[1]);
    if(k == 0 && strcmp(argv[1], "0")){
        cerr << "[Error] Integer command line argument expected!!" << endl;
        exit(0);
    }

    /* Create filename */
    filename = "RTree_dim=" + to_string(DIM) + ".txt";

    /* Initialize M and m (Max and Min number of childrens for a node) */
    ::M = floor(4096 / (2*DIM+1));
    ::m = floor(M/2);

    unordered_map<int, string> lines;

    // cout << "Loading File..." <<  endl;
    int root_node_num = ReadFile(filename, lines);
    // cout << "Done Loading" <<  endl;  

    RTNode* root = (RTNode *)mem_alloc(sizeof(RTNode));
    root->parent = NULL;
    root->RTNode_num = root_node_num;

    // cout << "Reading Tree..." <<  endl;
    ReadTree(root, lines);
    // cout << "Reading Complete\n" << endl;    
    lines.clear();

    srand(time(0));
    vector<int> query_point;

    clock_t start, end;
    double total_time = 0, time_taken;
    int total_nodes_visited = 0;

    for(int i = 0; i < N_Trials; i++){
        query_point = gen_query_point();
        // cout << "Query Point " << i << endl;

        nodes_visited = 0;

        priority_queue<NearestN, vector<NearestN>, CompareNNeighbor> nearest;

        start = clock(); 
        kNN_Search(root, query_point, nearest);
        end = clock();

        for(int j = 0; j < k; j++){
            cout << nearest.top().object->doc_num << " " << nearest.top().object->pic_num << endl;
            nearest.pop();
        }

        time_taken = double(end - start) / CLOCKS_PER_SEC;
        total_time += time_taken;
        // cout << "Time taken : " << time_taken << endl;

        total_nodes_visited += nodes_visited;
        // cout << "Number of Nodes visited : " << nodes_visited << endl;

        // cout<<endl;

    }

    // cout << "Average Time Taken : " << total_time/N_Trials << endl;
    // cout << "Average Number of Nodes visited : " << total_nodes_visited/N_Trials << endl;

    return 0;
}