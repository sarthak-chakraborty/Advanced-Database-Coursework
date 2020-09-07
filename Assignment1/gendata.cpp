#include <bits/stdc++.h>

#define N 5000000   // N = 5 million (number of data points)
/* Range of random numbers generated*/
#define LOWER 0
#define UPPER 20

using namespace std;


void gen_data(const int n, const char* filename){
    int min_num, max_num;
    FILE* fout = fopen(filename, "w");
    srand(time(0));

    for(int i=0; i < N; i++){
        for(int dim=0; dim < n; dim++){
            do {
                min_num = (rand() % (UPPER - LOWER +1)) + LOWER;
                max_num = (rand() % (UPPER - LOWER +1)) + LOWER;
            }while(max_num < min_num);
            fprintf(fout, "%d %d ", min_num, max_num);  
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
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

    /* Generate Data and write it into a file */
    gen_data(n, filename.c_str());

    cout << "[Success] Data written successfully to " << filename << "!" << endl;
    return 0;
}