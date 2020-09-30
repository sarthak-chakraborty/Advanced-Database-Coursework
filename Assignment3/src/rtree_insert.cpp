#include <bits/stdc++.h>
#include <math.h>
#include "rtree.h"

#define DIM 256
#define MAXLINE 5000

using namespace std;

/* Global Variables for Max and Min number of childrens */
int M=-1, m=-1;
int RTNodeEntryNum=0, RTNodeNum=0;


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



/* Quadratic Pick Seeds Algorithm */
vector<int> PickSeeds(vector<RTNodeEntry>& all_node_entries){
    vector<int> init_entries_idx = {-1, -1}; 

    double max_dist = INT_MIN;

    /* [PS1] */
    for(int i = 0; i < all_node_entries.size(); i++){
        for(int j = i+1; j < all_node_entries.size(); j++){
            RTNodeEntry entry1 = all_node_entries[i];
            RTNodeEntry entry2 = all_node_entries[j];

            assert(entry1.dmax.size() == entry2.dmax.size());

            /* Get the Combined Node */
            vector<int> dmin_combine(DIM, INT_MAX), dmax_combine(DIM, INT_MIN);
            for(int idx = 0; idx < DIM; idx++){
                dmin_combine[idx] = min(entry1.dmin[idx], entry2.dmin[idx]);
                dmax_combine[idx] = max(entry1.dmax[idx], entry2.dmax[idx]);   
            }

            /* Calculating d = Area(Entry1+Entry2) - Area(Entry1) - Area(Entry2) */
            /* NOTE: log is taken for numerical stability and avoid overflow */
            double area1 = 0.0, area2 = 0.0, area_combine = 0.0;
            for(int idx = 0; idx < DIM; idx++){
                if(entry1.dmax[idx] != entry1.dmin[idx])
                    area1 += log(entry1.dmax[idx] - entry1.dmin[idx]);
                if(entry2.dmax[idx] != entry2.dmin[idx])
                    area2 += log(entry2.dmax[idx] - entry2.dmin[idx]);
                if(dmax_combine[idx] != dmin_combine[idx])
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
vector<int> PickNext(RTNode* node1, RTNode* node2, vector<RTNodeEntry>& all_node_entries){
    int next_entry_idx = -1;
    int identify_group = -1;

    vector<double> area_inc1, area_inc2;
    

    /* [PN1] */

    /* Calculate Current Area for bounding box of Group 1 and Group 2 */
    double curr_area1 = 0.0, curr_area2 = 0.0;
    vector<int> dmin_node_curr1(DIM, INT_MAX), dmax_node_curr1(DIM, INT_MIN);
    vector<int> dmin_node_curr2(DIM, INT_MAX), dmax_node_curr2(DIM, INT_MIN);
    vector<RTNodeEntry>::iterator it;

    /* Group 1 */
    for(it = (node1->entry).begin(); it < (node1->entry).end(); it++){
        /* TODO: Can be parallelised for optimization */
        for(int idx = 0; idx < DIM; idx++){
            dmin_node_curr1[idx] = min(dmin_node_curr1[idx], (it->dmin)[idx]);
            dmax_node_curr1[idx] = max(dmax_node_curr1[idx], (it->dmax)[idx]);
        }
    }
    for(int idx = 0; idx < DIM; idx++){
        if(dmax_node_curr1[idx] > dmin_node_curr1[idx])
            curr_area1 += log(dmax_node_curr1[idx] - dmin_node_curr1[idx]);
    }

    /* Group 2 */
    for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
        /* TODO: Can be parallelised for optimization */
        for(int idx = 0; idx < DIM; idx++){
            dmin_node_curr2[idx] = min(dmin_node_curr2[idx], (it->dmin)[idx]);
            dmax_node_curr2[idx] = max(dmax_node_curr2[idx], (it->dmax)[idx]);
        }
    }
    for(int idx = 0; idx < DIM; idx++){
        if(dmax_node_curr2[idx] > dmin_node_curr2[idx])
            curr_area2 += log(dmax_node_curr2[idx] - dmin_node_curr2[idx]);
    }


    /* Calculate increase in Area */
    for(int i = 0; i < all_node_entries.size(); i++){
        double new_area1 = 0.0, new_area2 = 0.0;
        int dmin_node, dmax_node;

        for(int idx = 0; idx < DIM; idx++){
            /* Group 1 */
            dmin_node = min(dmin_node_curr1[idx], all_node_entries[i].dmin[idx]);
            dmax_node = max(dmax_node_curr1[idx], all_node_entries[i].dmax[idx]);
            if(dmax_node != dmin_node)
                new_area1 += log(dmax_node - dmin_node);

            /* Group 2 */
            dmin_node = min(dmin_node_curr2[idx], all_node_entries[i].dmin[idx]);
            dmax_node = max(dmin_node_curr2[idx], all_node_entries[i].dmax[idx]);
            if(dmax_node != dmin_node)
                new_area2 += log(dmax_node - dmin_node);
        }
        area_inc1.push_back(new_area1 - curr_area1);
        area_inc2.push_back(new_area2 - curr_area2);
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

    vector<int> init_entries_idx = PickSeeds(all_node_entries);

    if(init_entries_idx[0] == -1 || init_entries_idx[1] == -1){
        cerr << "[ERROR] Cannot find Pick Seeds index!!" << endl;
        return false;
    }


    /* Creating initial two groups */
    node1->entry.push_back(all_node_entries[init_entries_idx[0]]);
    node2->entry.push_back(all_node_entries[init_entries_idx[1]]);

    if(node1->entry.back().child != NULL)
        node1->entry.back().child->parent = node1;
    if(node2->entry.back().child != NULL)
        node2->entry.back().child->parent = node2;

    // cout << all_node_entries[init_entries_idx[0]].RTNodeEntry_num << " NodeEntry is put in Node " << node1->RTNode_num << " after split" << endl;
    // cout << all_node_entries[init_entries_idx[1]].RTNodeEntry_num << " NodeEntry is put in Node " << node2->RTNode_num << " after split" << endl;

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
    while(num_entries_assigned < M+1){
        /*  If one group has so few entries that all the rest must 
            be assigned to it m order for it to have the minimum
            number m, assign them and stop
        */
        if(node1->entry.size() + (M+1-num_entries_assigned) == m){
            for(int idx = 0; idx < all_node_entries.size(); idx++){
                node1->entry.push_back(all_node_entries[idx]);
                if(node1->entry.back().child != NULL)
                    node1->entry.back().child->parent = node1;
                // cout << all_node_entries[idx].RTNodeEntry_num << " NodeEntry is put in Node " << node1->RTNode_num << " after split" << endl;
                num_entries_assigned++;
            }
            all_node_entries.clear();
            return true;
        }
        if(node2->entry.size() + (M+1-num_entries_assigned) == m){
            for(int idx = 0; idx < all_node_entries.size(); idx++){
                node2->entry.push_back(all_node_entries[idx]);
                if(node2->entry.back().child != NULL)
                    node2->entry.back().child->parent = node2;
                // cout << all_node_entries[idx].RTNodeEntry_num << " NodeEntry is put in Node " << node2->RTNode_num << " after split" << endl;
                num_entries_assigned++;
            }
            all_node_entries.clear();
            return true;
        }
        
        /* [QS3] */
        vector<int> next_entry = PickNext(node1, node2, all_node_entries);
        if(next_entry[0] == -1 || next_entry[1] == -1){
            cerr << "[ERROR] Cannot find Pick Next index!!" << endl;
            return false;
        }
        /* Enter the entry to the corresponding group according to 'identify_group' field of the array */
        if(next_entry[1] == 0){
            node1->entry.push_back(all_node_entries[next_entry[0]]);
            if(node1->entry.back().child != NULL)
                node1->entry.back().child->parent = node1;
            // cout << all_node_entries[next_entry[0]].RTNodeEntry_num << " NodeEntry is put in Node " << node1->RTNode_num << " after split" << endl;
        }
        else{
            node2->entry.push_back(all_node_entries[next_entry[0]]);
            if(node2->entry.back().child != NULL)
                node2->entry.back().child->parent = node2;
            // cout << all_node_entries[next_entry[0]].RTNodeEntry_num << " NodeEntry is put in Node " << node2->RTNode_num << " after split" << endl;
        }

        /* Delete corresponding element from all_node_entries */
        all_node_entries.erase(all_node_entries.begin() + next_entry[0]);
        num_entries_assigned++;
    }
    return true;
}



/* Adjust Tree Algorithm */
vector<RTNode*> AdjustTree(RTNode* node1, RTNode* node2){

    /* [AT2] */
    if(node1->parent == NULL){
        return {node1, node2};
    }

    /* [AT3] */
    RTNode* P = node1->parent, *PP = NULL;
    vector<RTNodeEntry>::iterator it;
    for(it = (P->entry).begin(); it < (P->entry).end(); it++){

        /* If node1->parent->child == node1 */
        if(it->child->RTNode_num == node1->RTNode_num){

            /* For all entries in node1, check the minimum and maximum among each dimension */
            vector<int> dmin_node(DIM, INT_MAX), dmax_node(DIM, INT_MIN);
            vector<RTNodeEntry>::iterator it1;
            for(it1 = (node1->entry).begin(); it1 < (node1->entry).end(); it1++){
                /* TODO: Can be parallelised for optimization */
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
        vector<int> dmin_node(DIM, INT_MAX), dmax_node(DIM, INT_MIN);
        vector<RTNodeEntry>::iterator it;
        for(it = (node2->entry).begin(); it < (node2->entry).end(); it++){
            /* TODO: Can be parallelised for optimization */
            for(int idx = 0; idx < dmin_node.size(); idx++){
                dmin_node[idx] = min(dmin_node[idx], (it->dmin)[idx]);
                dmax_node[idx] = max(dmax_node[idx], (it->dmax)[idx]);
            }
        }

        /* New NodeList to be inserted in parent node of node2 */
        RTNodeEntry* new_node_entry = (RTNodeEntry *)mem_alloc(sizeof(RTNodeEntry));
        new_node_entry->dmin.assign(dmin_node.begin(), dmin_node.end());
        new_node_entry->dmax.assign(dmax_node.begin(), dmax_node.end());
        new_node_entry->RTNodeEntry_num = ::RTNodeEntryNum++;
        new_node_entry->child = node2;

        /* If there is left in parent node, make an entry */
        if((P->entry).size() < ::M){
            (P->entry).push_back(*new_node_entry);
            // cout << new_node_entry->RTNodeEntry_num << " NodeEntry is put in Node " << P->RTNode_num << " while adjusting" << endl;
            PP = NULL;
        }
        else{
            PP = (RTNode *)mem_alloc(sizeof(RTNode));
            PP->RTNode_num = ::RTNodeNum++;
            // cout << "New Node " << PP->RTNode_num << " is getting created while adjusting" << endl;
            PP->parent = P->parent;
            QuadraticSplit(P, PP, new_node_entry);
        }
    }
    return AdjustTree(P, PP);
}



/* Choose Leaf Algorithm */
RTNode* ChooseLeaf(RTNode* node, RTNodeEntry* E){

    /* [CL2] */
    if(node == NULL || (node->entry[0]).child == NULL){
        return node;
    }

    /* [CL3] */
    int num_entry = (node->entry).size();

    double cur_area, new_area, area_inc, min_inc = INT_MAX, min_area = INT_MAX;
    int new_dmax, new_dmin;

    RTNodeEntry* F = NULL, *cur_entry;

    for(int i = 0; i < num_entry; i++){
        cur_area = new_area = 0;
        cur_entry = &(node->entry[i]);

        /* For current entry, find current area and new area. (Take log to avoid overflow) */
        for(int j = 0; j < DIM; j++){
            if(cur_entry->dmax[j] != cur_entry->dmin[j])
                cur_area += log(cur_entry->dmax[j] - cur_entry->dmin[j]);

            new_dmin = min(cur_entry->dmin[j], E->dmin[j]);
            new_dmax = max(cur_entry->dmax[j], E->dmax[j]);
            if(new_dmax != new_dmin)
                new_area += log(new_dmax - new_dmin);
        }

        area_inc = abs(new_area - cur_area);
        if(F == NULL || area_inc < min_inc || (area_inc == min_inc && cur_area < min_area)){
            min_inc = area_inc;
            min_area = cur_area;
            F = cur_entry;
        }
    }
    // cout << "Minimum area increase for Choose Leaf is for NodeEntry " << F->RTNodeEntry_num << endl;
    
    /* [CL4] */
    return ChooseLeaf(F->child, E);
}


/* Insert Data into R-Tree */
RTNode* Insert(const char* filename){
    FILE* fin = fopen(filename, "r");
    if(fin == NULL){
        cerr << "[ERROR] Cannot Open File" << endl;
        return NULL;
    }
    char* line = new char[MAXLINE]; 
    RTNode* root = NULL;
    
    int c = 1;
    while(fgets(line, MAXLINE, fin)){
        // cout << c++ << endl;

        /* Make new RTNodeEntry */
        RTNodeEntry* entry = (RTNodeEntry*)mem_alloc(sizeof(RTNodeEntry));
        entry->RTNodeEntry_num = ::RTNodeEntryNum++;
        // cout << "NodeEntry " << entry->RTNodeEntry_num << " for new data" << endl;
        entry->child = NULL;

        vector<int> dmin, dmax;
        int flag = 0, count = 0;
        char* token = strtok(line, " ");
        while(token != NULL){
            if(atoi(token) != 0 || !strcmp(token, "0")){
                if(count == 0){
                    entry->doc_num = atoi(token);
                    count++;
                }
                else if(count == 1){
                    entry->pic_num = atoi(token);
                    count++;
                }
                else{
                    if(!flag){
                        dmin.push_back(atoi(token));
                        flag = 1;
                    }
                    else{
                        dmax.push_back(atoi(token));
                        flag = 0;
                    }
                }
            }
            token = strtok(NULL, " ");
        }
        entry->dmin.assign(dmin.begin(), dmin.end());
        entry->dmax.assign(dmax.begin(), dmax.end());

        /* [I1] */
        RTNode* entry_node = ChooseLeaf(root, entry);      

        if(entry_node == NULL){
            entry_node = (RTNode*)mem_alloc(sizeof(RTNode));
            // cout << "Node num " << ::RTNodeNum << " is current root" << endl;
            entry_node->RTNode_num = ::RTNodeNum++;
            entry_node->parent = NULL;
            root = entry_node;
        }

        // cout << "Node " << entry_node->RTNode_num << " is chosen as leaf" << endl;
        
        /* [I2] */
        RTNode* new_node = NULL;
        if(entry_node->entry.size() < ::M){
            // cout << entry->RTNodeEntry_num << " NodeEntry in Node " << entry_node->RTNode_num << " originally" << endl;
            entry_node->entry.push_back(*entry);
            vector<RTNode*> split_nodes = AdjustTree(entry_node, NULL);
        }
        else{
            // cout << "New Node to be created" << endl;
            new_node = (RTNode*)mem_alloc(sizeof(RTNode));
            new_node->RTNode_num = ::RTNodeNum++;
            // cout << "New Node " << new_node->RTNode_num << " is getting created during Insertion" << endl;
            new_node->parent = entry_node->parent;

            bool split = QuadraticSplit(entry_node, new_node, entry);

            /* [I3] */
            vector<RTNode*> split_nodes = AdjustTree(entry_node, new_node);

            /* [I4] */
            if(split_nodes[1] != NULL && split_nodes[0]->parent == NULL && split_nodes[1]->parent == NULL){
                /* Create new root node */
                RTNode* new_root = (RTNode *)mem_alloc(sizeof(RTNode));
                new_root->parent = NULL;
                new_root->RTNode_num = ::RTNodeNum++;
                // cout << "New Root creation necessary" << endl;

                /* Create 2 RTNodeEntry to  the new root */
                /* Node1 */
                RTNodeEntry* entry1 = (RTNodeEntry *)mem_alloc(sizeof(RTNodeEntry));
                entry1->RTNodeEntry_num = ::RTNodeEntryNum++;
                entry1->child = split_nodes[0];

                vector<int> dmin_node(DIM, INT_MAX), dmax_node(DIM, INT_MIN);
                vector<RTNodeEntry>::iterator it;
                for(it = split_nodes[0]->entry.begin(); it < split_nodes[0]->entry.end(); it++){
                    for(int idx = 0; idx < DIM; idx++){
                        dmin_node[idx] = min(dmin_node[idx], it->dmin[idx]);
                        dmax_node[idx] = max(dmax_node[idx], it->dmax[idx]);
                    }
                }
                entry1->dmin.assign(dmin_node.begin(), dmin_node.end());
                entry1->dmax.assign(dmax_node.begin(), dmax_node.end());

                /* Node2 */
                RTNodeEntry* entry2 = (RTNodeEntry *)mem_alloc(sizeof(RTNodeEntry));
                entry2->RTNodeEntry_num = ::RTNodeEntryNum++;
                entry2->child = split_nodes[1];

                fill(dmin_node.begin(), dmin_node.end(), INT_MAX);
                fill(dmax_node.begin(), dmax_node.end(), INT_MIN);
                for(it = split_nodes[1]->entry.begin(); it < split_nodes[1]->entry.end(); it++){
                    for(int idx = 0; idx < DIM; idx++){
                        dmin_node[idx] = min(dmin_node[idx], it->dmin[idx]);
                        dmax_node[idx] = max(dmax_node[idx], it->dmax[idx]);
                    }
                }
                entry2->dmin.assign(dmin_node.begin(), dmin_node.end());
                entry2->dmax.assign(dmax_node.begin(), dmax_node.end());

                /* Add RTNodeEntry to the new root */
                new_root->entry.push_back(*entry1);
                new_root->entry.push_back(*entry2);

                // cout << "NodeEntry" << entry1->RTNodeEntry_num << " and " << entry2->RTNodeEntry_num << " pointing Node " << entry1->child->RTNode_num << " and " << entry2->child->RTNode_num << " in New Root Node " << new_root->RTNode_num << endl;

                split_nodes[0]->parent = new_root;
                split_nodes[1]->parent = new_root;

                root = new_root;
                // cout << "Node " << root->RTNode_num << " is new root" << endl;
            }
        }
    }
    fclose(fin);
    return root;
}




/* Write tree to a file 
Each line corresponds to a node
node_num  parent_node_num  num_entries  R1(rect_num, dmin[], dmax[], child_node_num)  R2(dmin[], dmax[], child_node_num)
*/
void WriteTree(RTNode* node, FILE* fout){

    if(node == NULL){
        cout << "No RTree formed" << endl;
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

    int num_entry = (node->entry).size();

    fprintf(fout, "%d ", num_entry);

    /* Write rectangles in the node */
    RTNodeEntry* cur_entry;
    for(int i = 0; i < num_entry; i++){
        cur_entry = &(node->entry[i]);

        /* Write rect_num and rect coodinates */
        fprintf(fout, "%d %d %d ", cur_entry->RTNodeEntry_num, cur_entry->doc_num, cur_entry->pic_num);
        
        for(int j = 0; j < DIM; j++){
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




int main(int argc, char** argv){
    string in_filename, out_filename;

    /* Create filename */
    in_filename = "img_data.txt";
    out_filename = "RTree_dim=" + to_string(DIM) + ".txt";


    /* Initialize M and m (Max and Min number of childrens for a node) */
    ::M = floor(4096 / (2*DIM+1));
    ::m = floor(M/2);

    RTNode* root_node = Insert(in_filename.c_str());

    WriteTree(root_node, fopen(out_filename.c_str(), "w"));

    return 0;
}