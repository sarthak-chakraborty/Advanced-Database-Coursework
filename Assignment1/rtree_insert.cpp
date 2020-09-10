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
            vector<int> dmin_node(dim, 0), dmax_node(dim, 0);
            vector<RTNodeEntry>::iterator it1;
            for(it1 = (node1->entry).begin(); it1 < (node1->entry).end(); it1++){
                // TODO: Better Implementation
                for(int idx = 0; idx < dmin_node.size(); idx++){
                    dmin_node[idx] = min(dmin_node[idx], (it1->dmin)[idx]);
                    dmax_node[idx] = max(dmax_node[idx], (it1->dmax)[idx]);
                }
            }
            it->dmin.insert((it->dmin).begin(), dmin_node.begin(), dmin_node.end());
            it->dmax.insert((it->dmax).begin(), dmax_node.begin(), dmax_node.end());
            break;
        }
    }

    /* [AT4] */
    if(node2 != NULL){
        /* For all entries in node2, check the minimum and maximum among each dimension */
        vector<int> dmin_node(dim, 0), dmax_node(dim, 0);
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
        new_node_entry->child = node2;

        /* If there is left in parent node, make an entry */
        if((P->entry).size() < ::M){
            (P->entry).push_back(*new_node_entry);
            PP = NULL;
        }
        else{
            /* TODO: split_node(P, PP, new_node_list) */
        }
    }

    return AdjustTree(P, PP, dim);
}



/* Quadratic Pick Seeds Algorithm */
int* PickSeeds(vector<RTNodeEntry> all_node_entries){

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
            vector<int> dmin_combine(dim, 0), dmax_combine(dim, 0);
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
    return init_entries_idx;
}



/* Quadratic Pick Next Algorithm */
int* PickNext(RTNode* node1, RTNode* node2, vector<RTNodeEntry> all_node_entries){
    int next_entry_idx = -1;
    int identify_group = -1;

    int dim = all_node_entries[0].dmax.size();

    vector<double> area_inc1, area_inc2;
    
    /* [PN1] */
    for(int i = 0; i < all_node_entries.size(); i++){

        double curr_area = 0.0, new_area = 0.0;

        vector<int> dmin_node(dim), dmax_node(dim);
        vector<int> dmin_node_curr(dim, 0), dmax_node_curr(dim, 0);
        vector<RTNodeEntry>::iterator it;

        /* Calculate the increase in area of bounding rectangle for Group 1 */
        for(it = (node1->entry).begin(); it < (node1->entry).end(); it++){
            for(int idx = 0; idx < dim; idx++){
                dmin_node_curr[idx] = min(dmin_node_curr[idx], (it->dmin)[idx]);
                dmax_node_curr[idx] = max(dmax_node_curr[idx], (it->dmax)[idx]);

                dmin_node[idx] = min(it->dmin[idx], all_node_entries[i].dmin[idx]);
                dmax_node[idx] = max(it->dmax[idx], all_node_entries[i].dmax[idx]);
            }
        }
        for(int idx = 0; idx < dim; idx++){
            curr_area += log(dmax_node_curr[idx] - dmin_node_curr[idx]);
            new_area += log(dmax_node[idx] - dmin_node[idx]);
        }
        area_inc1.push_back(new_area - curr_area);

        /* Clear history of Group 1 */
        dmin_node.clear(); dmax_node.clear();
        dmin_node_curr.clear(); dmax_node_curr.clear();
        
        fill(dmin_node_curr.begin(), dmin_node_curr.end(), 0);
        fill(dmax_node_curr.begin(), dmax_node_curr.end(), 0);
        curr_area = new_area = 0.0;

        /* Calculate the increase in area of bounding rectangle for Group 1 */
        for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
            for(int idx = 0; idx < dim; idx++){
                dmin_node_curr[idx] = min(dmin_node_curr[idx], (it->dmin)[idx]);
                dmax_node_curr[idx] = max(dmax_node_curr[idx], (it->dmax)[idx]);

                dmin_node[idx] = min(it->dmin[idx], all_node_entries[i].dmin[idx]);
                dmax_node[idx] = max(it->dmax[idx], all_node_entries[i].dmax[idx]);
            }
        }
        for(int idx = 0; idx < dim; idx++){
            curr_area += log(dmax_node_curr[idx] - dmin_node_curr[idx]);
            new_area += log(dmax_node[idx] - dmin_node[idx]);
        }
        area_inc2.push_back(new_area - curr_area);
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

    static int arr[2] = {next_entry_idx, identify_group};
    return arr;
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

    int* init_entries_idx = PickSeeds(all_node_entries);
    if(init_entries_idx[0] == -1 || init_entries_idx[1] == -1){
        cerr << "[ERROR] Cannot find Pick Seeds index!!" << endl;
        return false;
    }

    /* Creating initial two groups */
    node1->entry.push_back(all_node_entries[init_entries_idx[0]]);
    node2->entry.push_back(all_node_entries[init_entries_idx[1]]);
    node2->RTNode_num = ::RTNodeNum;
    node2->parent = node1->parent;

    /* Delete corresponding elements from all_node_entries */
    all_node_entries.erase(all_node_entries.begin() + init_entries_idx[0]);
    if(init_entries_idx[0] < init_entries_idx[1]){
        all_node_entries.erase(all_node_entries.begin() + init_entries_idx[1]-1);
    }
    else{
        all_node_entries.erase(all_node_entries.begin() + init_entries_idx[1]);
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
        int* next_entry = PickNext(node1, node2, all_node_entries);
        if(next_entry[0] == -1 || next_entry[1] == -1){
            cerr << "[ERROR] Cannot find Pick Next index!!" << endl;
            return false;
        }
        /* Enter the entry to the corresponding group according to 'identify_group' field of the array */
        if(next_entry[1] == 0)
            node1->entry.push_back(all_node_entries[next_entry[0]]);
        else
            node2->entry.push_back(all_node_entries[next_entry[0]]);

        /* Delete corresponding element from all_node_entries */
        all_node_entries.erase(all_node_entries.begin() + next_entry[0]);
        num_entries_assigned++;
    }
    return true;
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