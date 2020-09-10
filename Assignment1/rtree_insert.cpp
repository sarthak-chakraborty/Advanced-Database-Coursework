#include <bits/stdc++.h>
#include "rtree.h"

#define N 5000000   // N = 5 million (number of data points)

using namespace std;

/* Global Variables for Max and Min number of children */
int M=-1, m=-1;

/* Counts for Nodes and Rectangles */
int node_count = 0, rect_count = 0;


/* Choose Leaf Algorithm */
RTNode* ChooseLeaf(RTNode* node, Rect* E){

    /* CL2 */
    if(node == NULL || (node->entry[0]).child == NULL){
        return node;
    }

    /* CL3 */
    int dim = (node->entry[0]).dmin.size(), num_entry = (node->entry).size();

    double cur_area, new_area, area_inc, min_inc = 0, min_area = 0;
    int new_dmax, new_dmin;

    Rect *F = NULL, *cur_entry;

    for(int i = 0; i < num_entry; i++){
        cur_area = new_area = 0;
        cur_entry = &(node->entry[i]);

        /* For current entry, find current area and new area. (Take log to avoid overflow) */
        for(int j = 0; j < dim; j++){
            cur_area += log(cur_entry->dmax[j] - cur_entry->dmin[j]);

            new_dmin = min(cur_entry->dmin[j], E->dmin[j]);
            new_dmax = max(cur_entry->dmax[j], E->dmax[j]);
            new_area += log(new_dmax - new_dmin);
        }

        area_inc = new_area - cur_area;
        if(F == NULL || area_inc < min_inc || (area_inc == min_inc && cur_area < min_area)){
            min_inc = area_inc;
            min_area = cur_area;
            F = cur_entry;
        }
    }

    /* CL4 */
    return ChooseLeaf(F->child, E);

}


/* Write tree to a file 

Each line corresponds to a node

node_num  parent_rect_num  R1(rect_num, dmin[], dmax[], child_node_num)  R2(dmin[], dmax[], child_node_num)

*/
void WriteTree(RTNode* node, FILE* fout){
    
    if(node == NULL){
        return;
    }
    
    /* Write node_num and parent_rect_num */

    if(node->parent == NULL){
        node->RTNode_num = node_count++;
    }

    fprintf(fout, "%d ", node->RTNode_num);

    if(node->parent != NULL){
        fprintf(fout, "%d ", node->parent->Rect_num);
    }
    else{
        fprintf(fout, "-1 ");
    }

    /* Write rectangles in the node */
    int dim = (node->entry[0]).dmin.size(), num_entry = (node->entry).size();

    Rect* cur_entry;
    for(int i = 0; i < num_entry; i++){
        cur_entry = &(node->entry[i]);

        /* Write rect_num and rect coodinates */
        cur_entry->Rect_num = rect_count++;        
        fprintf(fout, "%d ", cur_entry->Rect_num);
        
        for(int j = 0; j < dim; j++){
            fprintf(fout, "%d %d ", cur_entry->dmin[j], cur_entry->dmax[j]);
        }
        
        /* Write child_node_num */
        int child_node_num = -1;
        if(cur_entry->child != NULL){
            child_node_num = node_count++;
            cur_entry->child->RTNode_num = child_node_num;
        }
        fprintf(fout, "%d ", child_node_num);
    }

    fprintf(fout, "\n");
    
    /* Write subtrees */
    for(int i = 0; i < num_entry; i++){
        cur_entry = &(node->entry[i]);
        if(cur_entry->child != NULL){
            WriteTree(cur_entry->child, fout);
        }
    }
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
    filename = "rtree_dim=" + to_string(n) + ".txt";

    /* Initialize M and m (Max and Min number of children for a node) */
    M = floor(4096 / (4*n+1));
    m = floor(M/2);
    
    /*
    FILE* fout = fopen(filename.c_str(), "w");

    WriteTree(root, filename);

    fclose(fout);
    */

    return 0;
}