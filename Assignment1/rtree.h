#ifndef _RTREE_
#define _RTREE_

#include <bits/stdc++.h>

using namespace std;

struct RTNode;

struct RTNodeList{
   vector<int> dmin, dmax;
   int RTNodeList_num;
   struct RTNode* child;
};
typedef RTNodeList RTNodeList;

struct RTNode{
    vector<RTNodeList> entry;
    int RTNode_num;
    struct RTNode* parent;
};
typedef struct RTNode RTNode;
typedef struct RTNode* RTreePtr;

bool RTNewTree(RTreePtr* T, RTNodeList* list);
bool RTSearchData(RTreePtr* T, int dim, RTNodeList **list, size_t *count);
bool RTInsertData(RTreePtr* T, int dim, int* data);
bool RTFreeTree(RTreePtr* T);

#endif /* _RTREE_*/