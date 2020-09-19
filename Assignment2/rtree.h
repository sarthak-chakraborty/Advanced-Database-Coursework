#ifndef _RTREE_
#define _RTREE_

#include <bits/stdc++.h>

using namespace std;

struct RTNode;

struct RTNodeEntry{
   vector<int> dmin, dmax;
   int RTNodeEntry_num;
   struct RTNode* child;
};
typedef struct RTNodeEntry RTNodeEntry;

struct RTNode{
    vector<RTNodeEntry> entry;
    int RTNode_num;
    struct RTNode* parent;
};
typedef struct RTNode RTNode;

typedef struct{
    RTNodeEntry* object;
    int dist;
}NearestN;

typedef struct{
    RTNodeEntry* entry;
    int mindist;
    int minmaxdist;
}ABLEntry;


#endif /* _RTREE_*/