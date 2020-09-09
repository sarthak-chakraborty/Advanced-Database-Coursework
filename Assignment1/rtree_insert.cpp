#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

#define N 5000000   // N = 5 million (number of data points)

using namespace std;

/* Global Variables for Max and Min number of childrens */
int M=-1, m=-1, RTNodeListNum=0;


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


/* Adjust Tree Algorithm */
bool AdjustTree(RTNode* node1, RTNode* node2, int dim){

    /* [AT2] */
    if(node1->parent == NULL){
        return true;
    }

    /* [AT3] */
    RTNode* P = node1->parent, *PP;
    vector<RTNodeList>::iterator it;
    for(it = (P->entry).begin(); it < (P->entry).end(); it++){

        /* If node1->parent->child == node1 */
        if(it->child->RTNode_num == node1->RTNode_num){

            /* For all entries in node1, check the minimum and maximum among each dimension */
            vector<int> dmin_node(dim, 0), dmax_node(dim, 0);
            vector<RTNodeList>::iterator it1;
            for(it1 = (node1->entry).begin(); it1 < (node1->entry).end(); it1++){
                // TODO: Better Implementation
                for(int idx = 0; idx < dmin_node.size(); idx++){
                    dmin_node[idx] = min(dmin_node[idx], (it1->dmin)[idx]);
                    dmax_node[idx] = max(dmax_node[idx], (it1->dmax)[idx]);
                }
            }
            it->dmin = dmin_node;
            it->dmax = dmax_node;
            break;
        }
    }

    /* [AT4] */
    if(node2 != NULL){
        /* For all entries in node2, check the minimum and maximum among each dimension */
        vector<int> dmin_node(dim, 0), dmax_node(dim, 0);
        vector<RTNodeList>::iterator it;
        for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
            // TODO: Better Implementation
            for(int idx = 0; idx < dmin_node.size(); idx++){
                dmin_node[idx] = min(dmin_node[idx], (it->dmin)[idx]);
                dmax_node[idx] = max(dmax_node[idx], (it->dmax)[idx]);
            }
        }

        /* New NodeList to be inserted in parent node of node2 */
        RTNodeList* new_node = (struct RTNodeList *)mem_alloc(sizeof(struct RTNodeList));
        new_node->dmin = dmin_node;
        new_node->dmax = dmax_node;
        new_node->RTNodeList_num = ::RTNodeListNum;
        new_node->child = node2;

        /* If there is left in parent node, make an entry */
        if((P->entry).size() < ::M){
            (P->entry).push_back(*new_node);
            PP = NULL;
        }
        else{
            /* TODO: split_node(P, new_node) */
        }
    }

    return AdjustTree(P, PP, dim);
}



int main(int argc, char** argv){
    int n = -1;
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
    filename = "testdata_dim=" + to_string(n) + ".txt";

    /* Initialize M and m (Max and Min number of childrens for a node) */
    M = floor(4096 / (4*n+1));
    m = floor(M/2);

    return 0;
}