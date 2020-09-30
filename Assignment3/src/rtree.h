#ifndef _RTREE_
#define _RTREE_

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct RTNode;

struct RTNodeEntry{
   vector<int> dmin, dmax;
   int doc_num;
   int pic_num;
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
    ll dist;
}NearestN;


typedef struct{
    RTNodeEntry* entry;
    ll mindist;
    ll minmaxdist;
}ABLEntry;


#endif /* _RTREE_*/