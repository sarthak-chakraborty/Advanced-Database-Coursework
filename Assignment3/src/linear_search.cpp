#include <bits/stdc++.h>
typedef long long ll;
#define DIM 256

using namespace std;

typedef struct{
    vector<int> object;
    ll dist;
    int doc_num;
    int pic_num;
}NearestN;

ll objdist(vector<int> point, vector<int> object){
    ll dist = 0;

    for(int i = 0; i < point.size(); i++){
        dist += pow((point[i] - object[i]), 2);
    }

    return dist;
}

bool compare(const NearestN &N1, const NearestN &N2){
    return N1.dist < N2.dist;
}


void printPoint(vector<int> &point){
    for(int i = 0; i < point.size(); i++){
        cout<<point[i]<<" ";
    }
    cout<<endl;
}


/* Generates a query point to be searched */
vector<int> gen_query_point(const char* filename){
    vector<int> query_point;

    FILE* fin = fopen(filename, "r");
    if(fin == NULL){
        cerr << "[ERROR] Cannot Open File" << endl;
        exit(0);
    }
    char* line = new char[5000]; 

     while(fgets(line, 5000, fin)){
        char* token = strtok(line, " ");

        while(token != NULL){
        if(atoi(token) != 0 || !strcmp(token, "0")){
            query_point.push_back(atoi(token));
        }
        token = strtok(NULL, " ");
        }
     }

    return query_point;
}


int main(int argc, char **argv){

    int k;
    

    if(argc < 2){
        cerr << "[Error] Enter k!!" << endl;
        exit(0);
    }


    k = atoi(argv[1]);

    vector<int> query_point = gen_query_point("query_image.txt");

    ifstream fin("img_data.txt");

    if(!fin.is_open()){
        cout << "File could not be opened" << endl;
        exit(0);
    }

    string line_tmp;
    istringstream line;
    
    vector<NearestN> nearest;
    while(getline(fin, line_tmp)){
        line.str(line_tmp);

        NearestN cur_neighbor;
        cur_neighbor.object.resize(DIM);
        line>>cur_neighbor.doc_num>>cur_neighbor.pic_num;
        for(int i = 0; i < DIM; i++){
            line>>cur_neighbor.object[i]>>cur_neighbor.object[i];
        }
        cur_neighbor.dist = objdist(query_point, cur_neighbor.object);

        nearest.push_back(cur_neighbor);
    }

    sort(nearest.begin(), nearest.end(), compare);

    cout<<"Query Point"<<endl;
    // printPoint(query_point);

    cout<<"NearestN"<<endl;
    for(int i = 0; i < k; i++){
        cout<<nearest[i].dist<<" "<<nearest[i].doc_num<<" "<<nearest[i].pic_num<<endl;
    }

}