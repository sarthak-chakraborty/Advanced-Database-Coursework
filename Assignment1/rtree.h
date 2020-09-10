#ifndef _RTREE_
#define _RTREE_

#include <bits/stdc++.h>

using namespace std;

struct RTNode;

struct Rect{
   vector<int> dmin, dmax;
   int Rect_num;
   struct RTNode* child;
};

struct RTNode{
    vector<Rect> entry;
    int RTNode_num;
    struct Rect* parent;
};

typedef struct Rect Rect;
typedef struct RTNode RTNode;
typedef struct RTNode* RTreePtr;

bool RTNewTree(RTreePtr* T, Rect* list);
bool RTSearchData(RTreePtr* T, int dim, Rect **list, size_t *count);
bool RTInsertData(RTreePtr* T, int dim, int* data);
bool RTFreeTree(RTreePtr* T);

#endif /* _RTREE_*/