#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

//#define N 5000000   // N = 5 million (number of data points)

#define N 20

using namespace std;

/* Global Variables for Max and Min number of childrens */
int M=-1, m=-1, RTNodeEntryNum=0, RTNodeNum=0;

/* dimension of rectangles */
int n;

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

    /* Find line from the file corresponding to the current node */
    string line_tmp;
    istringstream line;
    
    while(getline(fin, line_tmp)){
        line.str(line_tmp);
        line>>node_num;
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
            line>>(cur_entry->dmin[j])>>(cur_entry->dmax[j]);
        }
        
        /* Add child node to the rectangle */
        line>>child_node_num;

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


void Search(RTNode* node, RTNodeEntry* S, vector<RTNodeEntry> &out){
    if(node == NULL){
        return;
    }

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
            /* S1 */
            if(cur_entry->child != NULL){
                Search(cur_entry->child, S, out);
            }
            else{
            /* S2 */
                out.push_back(*cur_entry);
            }
        }
    }

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

    fin>>root_node_num;    

    RTNode* root = (RTNode *)mem_alloc(sizeof(RTNode));
    root->parent = NULL;
    root->RTNode_num = root_node_num;

    ReadTree(root, fin);

    fin.close();

    return 0;
}