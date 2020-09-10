#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

#define N 5000000   // N = 5 million (number of data points)

using namespace std;

/* Global Variables for Max and Min number of childrens */
int M=-1, m=-1, RTNodeEntryNum=0, RTNodeNum=0;


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


/* Choose Leaf Algorithm */
RTNode* ChooseLeaf(RTNode* node, RTNodeEntry* E){

    /* CL2 */
    if(node == NULL || (node->entry[0]).child == NULL){
        return node;
    }

    /* CL3 */
    int dim = (node->entry[0]).dmin.size(), num_entry = (node->entry).size();

    double cur_area, new_area, area_inc, min_inc = 0, min_area = 0;
    int new_dmax, new_dmin;

    RTNodeEntry *F = NULL, *cur_entry;

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

node_num  parent_node_num  num_entries  R1(rect_num, dmin[], dmax[], child_node_num)  R2(dmin[], dmax[], child_node_num)

*/
void WriteTree(RTNode* node, FILE* fout){
    
    if(node == NULL){
        return;
    }
    
    /* Write node_num, parent_node_num, num_entries */

    fprintf(fout, "%d ", node->RTNode_num);

    if(node->parent != NULL){
        fprintf(fout, "%d ", node->parent->RTNode_num);
    }
    else{
        fprintf(fout, "-1 ");
    }

    int dim = (node->entry[0]).dmin.size(), num_entry = (node->entry).size();

    fprintf(fout, "%d ", num_entry);

    /* Write rectangles in the node */

    RTNodeEntry* cur_entry;
    for(int i = 0; i < num_entry; i++){
        cur_entry = &(node->entry[i]);

        /* Write rect_num and rect coodinates */
        fprintf(fout, "%d ", cur_entry->RTNodeEntry_num);
        
        for(int j = 0; j < dim; j++){
            fprintf(fout, "%d %d ", cur_entry->dmin[j], cur_entry->dmax[j]);
        }
        
        /* Write child_node_num */
        int child_node_num = -1;
        if(cur_entry->child != NULL){
            child_node_num = cur_entry->child->RTNode_num;
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


/* Quadratic Pick Seeds Algorithm */
pair<int,int> PickSeeds(vector<RTNodeEntry> all_node_entries){

    int init_entries_idx[2] = {-1, -1}; 

    double max_dist = INT_MIN;

    /* [PS1] */
    for(int i = 0; i < all_node_entries.size(); i++){
        for(int j = i+1; j < all_node_entries.size(); j++){
            RTNodeEntry entry1 = all_node_entries[i];
            RTNodeEntry entry2 = all_node_entries[j];

            assert(entry1.dmax.size() == entry2.dmax.size());
            int dim = entry1.dmax.size();

            /* Get the Combined Node */
            vector<int> dmin_combine(dim, INT_MAX), dmax_combine(dim, INT_MIN);
            for(int idx = 0; idx < dim; idx++){
                dmin_combine[idx] = min(entry1.dmin[idx], entry2.dmin[idx]);
                dmax_combine[idx] = max(entry1.dmax[idx], entry2.dmax[idx]);   
            }

            /* Calculating d = Area(Entry1+Entry2) - Area(Entry1) - Area(Entry2) */
            /* NOTE: log is taken for numerical stability and avoid overflow */
            double area1 = 0.0, area2 = 0.0, area_combine = 0.0;
            for(int idx = 0; idx < dim; idx++){
                area1 += log(entry1.dmax[idx] - entry1.dmin[idx]);
                area2 += log(entry2.dmax[idx] - entry2.dmin[idx]);
                area_combine += log(dmax_combine[idx] - dmin_combine[idx]);
            }
            double d = area_combine - area1 - area2;

            /* [PS2] */
            if(d > max_dist){
                max_dist = d;
                init_entries_idx[0] = i;
                init_entries_idx[1] = j;
            }
        }
    }
    return {init_entries_idx[0],init_entries_idx[1]};
}



/* Quadratic Pick Next Algorithm */
pair<int,int> PickNext(RTNode* node1, RTNode* node2, vector<RTNodeEntry> all_node_entries){
    int next_entry_idx = -1;
    int identify_group = -1;

    int dim = all_node_entries[0].dmax.size();

    vector<double> area_inc1, area_inc2;
    
    /* [PN1] */

    /* Calculate Current Area for bounding box of Group 1 and Group 2 */
    double curr_area1 = 0.0, curr_area2 = 0.0;
    vector<int> dmin_node_curr(dim, INT_MAX), dmax_node_curr(dim, INT_MIN);
    vector<RTNodeEntry>::iterator it;

    /* Group 1 */
    for(it = (node1->entry).begin(); it < (node1->entry).end(); it++){
        for(int idx = 0; idx < dim; idx++){
            dmin_node_curr[idx] = min(dmin_node_curr[idx], (it->dmin)[idx]);
            dmax_node_curr[idx] = max(dmax_node_curr[idx], (it->dmax)[idx]);
        }
    }
    for(int idx = 0; idx < dim; idx++){
        curr_area1 += log(dmax_node_curr[idx] - dmin_node_curr[idx]);
    }

    /* Clear history of Group 1 */
    dmin_node_curr.clear(); dmax_node_curr.clear();
    fill(dmin_node_curr.begin(), dmin_node_curr.end(), INT_MAX);
    fill(dmax_node_curr.begin(), dmax_node_curr.end(), INT_MIN);

    /* Group 2 */
    for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
        for(int idx = 0; idx < dim; idx++){
            dmin_node_curr[idx] = min(dmin_node_curr[idx], (it->dmin)[idx]);
            dmax_node_curr[idx] = max(dmax_node_curr[idx], (it->dmax)[idx]);
        }
    }
    for(int idx = 0; idx < dim; idx++){
        curr_area2 += log(dmax_node_curr[idx] - dmin_node_curr[idx]);
    }


    /* Calculate increase in Area */
    for(int i = 0; i < all_node_entries.size(); i++){
        double new_area = 0.0;

        vector<int> dmin_node(dim), dmax_node(dim);
        vector<RTNodeEntry>::iterator it;

        /* Calculate the increase in area of bounding rectangle for Group 1 */
        for(it = (node1->entry).begin(); it < (node1->entry).end(); it++){
            for(int idx = 0; idx < dim; idx++){
                dmin_node[idx] = min(it->dmin[idx], all_node_entries[i].dmin[idx]);
                dmax_node[idx] = max(it->dmax[idx], all_node_entries[i].dmax[idx]);
            }
        }
        for(int idx = 0; idx < dim; idx++)
            new_area += log(dmax_node[idx] - dmin_node[idx]);
        area_inc1.push_back(new_area - curr_area1);

        /* Clear history of Group 1 */
        dmin_node.clear(); dmax_node.clear();
        new_area = 0.0;

        /* Calculate the increase in area of bounding rectangle for Group 1 */
        for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
            for(int idx = 0; idx < dim; idx++){
                dmin_node[idx] = min(it->dmin[idx], all_node_entries[i].dmin[idx]);
                dmax_node[idx] = max(it->dmax[idx], all_node_entries[i].dmax[idx]);
            }
        }
        for(int idx = 0; idx < dim; idx++)
            new_area += log(dmax_node[idx] - dmin_node[idx]);
        area_inc2.push_back(new_area - curr_area2);
    }


    /* [PN2] */
    double max_dist = INT_MIN;
    for(int i = 0; i < all_node_entries.size(); i++){
        if(abs(area_inc2[i] - area_inc1[i]) > max_dist){
            max_dist = abs(area_inc2[i] - area_inc1[i]);
            next_entry_idx = i;
            if(area_inc1[i] < area_inc2[i])
                identify_group = 0;
            else
                identify_group = 1;   
        }
    }

    return {next_entry_idx, identify_group};
}




/* Split Nodes Algorithm : Quadratic Split strategy */
bool QuadraticSplit(RTNode* node1, RTNode* node2, RTNodeEntry* node_entry_ptr){

    assert((node1->entry).size() == M);
    int num_entries_assigned = 0;

    /* Copy all the entries in a single vector */
    vector<RTNodeEntry> all_node_entries;
    all_node_entries.assign((node1->entry).begin(), (node1->entry).end());
    all_node_entries.insert(all_node_entries.end(), *node_entry_ptr);

    node1->entry.clear();  /* Clear the vector containing the entries, will be inserted again */

    pair<int,int> init_entries_idx = PickSeeds(all_node_entries);
    if(init_entries_idx.first == -1 || init_entries_idx.second == -1){
        cerr << "[ERROR] Cannot find Pick Seeds index!!" << endl;
        return false;
    }

    /* Creating initial two groups */
    node1->entry.push_back(all_node_entries[init_entries_idx.first]);
    node2->entry.push_back(all_node_entries[init_entries_idx.second]);

    /* Delete corresponding elements from all_node_entries */
    all_node_entries.erase(all_node_entries.begin() + init_entries_idx.first);
    if(init_entries_idx.first < init_entries_idx.second){
        all_node_entries.erase(all_node_entries.begin() + init_entries_idx.second-1);
    }
    else{
        all_node_entries.erase(all_node_entries.begin() + init_entries_idx.second);
    }
    num_entries_assigned += 2;


    /* [QS2] */
    while(num_entries_assigned < M){
        /*  If one group has so few entries that all the rest must 
            be assigned to it m order for it to have the minimum
            number m, assign them and stop
        */
        if(node1->entry.size() + (M-num_entries_assigned) == m){
            for(int idx = 0; idx < all_node_entries.size(); idx++){
                node1->entry.push_back(all_node_entries[idx]);
                num_entries_assigned++;
            }
            all_node_entries.clear();
            return true;
        }
        if(node2->entry.size() + (M-num_entries_assigned) == m){
            for(int idx = 0; idx < all_node_entries.size(); idx++){
                node2->entry.push_back(all_node_entries[idx]);
                num_entries_assigned++;
            }
            all_node_entries.clear();
            return true;
        }

        /* [QS3] */
        pair<int,int> next_entry = PickNext(node1, node2, all_node_entries);
        if(next_entry.first == -1 || next_entry.second == -1){
            cerr << "[ERROR] Cannot find Pick Next index!!" << endl;
            return false;
        }
        /* Enter the entry to the corresponding group according to 'identify_group' field of the array */
        if(next_entry.second == 0)
            node1->entry.push_back(all_node_entries[next_entry.first]);
        else
            node2->entry.push_back(all_node_entries[next_entry.first]);

        /* Delete corresponding element from all_node_entries */
        all_node_entries.erase(all_node_entries.begin() + next_entry.first);
        num_entries_assigned++;
    }
    return true;
}



/* Adjust Tree Algorithm */
bool AdjustTree(RTNode* node1, RTNode* node2, int dim){

    /* [AT2] */
    if(node1->parent == NULL){
        return true;
    }

    /* [AT3] */
    RTNode* P = node1->parent, *PP;
    vector<RTNodeEntry>::iterator it;
    for(it = (P->entry).begin(); it < (P->entry).end(); it++){

        /* If node1->parent->child == node1 */
        if(it->child->RTNode_num == node1->RTNode_num){

            /* For all entries in node1, check the minimum and maximum among each dimension */
            vector<int> dmin_node(dim, INT_MAX), dmax_node(dim, INT_MIN);
            vector<RTNodeEntry>::iterator it1;
            for(it1 = (node1->entry).begin(); it1 < (node1->entry).end(); it1++){
                // TODO: Better Implementation
                for(int idx = 0; idx < dmin_node.size(); idx++){
                    dmin_node[idx] = min(dmin_node[idx], (it1->dmin)[idx]);
                    dmax_node[idx] = max(dmax_node[idx], (it1->dmax)[idx]);
                }
            }
            it->dmin.assign(dmin_node.begin(), dmin_node.end());
            it->dmax.assign(dmax_node.begin(), dmax_node.end());
            break;
        }
    }

    /* [AT4] */
    if(node2 != NULL){
        /* For all entries in node2, check the minimum and maximum among each dimension */
        vector<int> dmin_node(dim, INT_MAX), dmax_node(dim, INT_MIN);
        vector<RTNodeEntry>::iterator it;
        for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
            // TODO: Better Implementation
            for(int idx = 0; idx < dmin_node.size(); idx++){
                dmin_node[idx] = min(dmin_node[idx], (it->dmin)[idx]);
                dmax_node[idx] = max(dmax_node[idx], (it->dmax)[idx]);
            }
        }

        /* New NodeList to be inserted in parent node of node2 */
        RTNodeEntry* new_node_entry = (RTNodeEntry *)mem_alloc(sizeof(RTNodeEntry));
        new_node_entry->dmin.assign(dmin_node.begin(), dmin_node.end());
        new_node_entry->dmax.assign(dmax_node.begin(), dmax_node.end());
        new_node_entry->RTNodeEntry_num = ::RTNodeEntryNum;
        ::RTNodeEntryNum++;
        new_node_entry->child = node2;

        /* If there is left in parent node, make an entry */
        if((P->entry).size() < ::M){
            (P->entry).push_back(*new_node_entry);
            PP = NULL;
        }
        else{
            PP = (RTNode *)mem_alloc(sizeof(RTNode));
            PP->RTNode_num = ::RTNodeNum;
            ::RTNodeNum++;
            PP->parent = P->parent;
            QuadraticSplit(P, PP, new_node_entry);
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
    ::M = floor(4096 / (4*n+1));
    ::m = floor(M/2);

    return 0;
}