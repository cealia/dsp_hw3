#include "hw3src.h"

Z2Candidates::Z2Candidates(){}
Z2Candidates::Z2Candidates(const string &path){
    int count;
    ifstream file(path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream in(line);
            count = 0;
            string t;
            string q;
            while (getline(in, t, ' ')) {
                //if 
                if(count==0){
                    _map[t] = vector<string>();
                    q = t;
                    count ++;
                }
                else{
                    _map[q].push_back(t);
                }
            }
            //cout<<_map[q].size()<<" ";
        }
        file.close();
    }
}
Z2Candidates::~Z2Candidates(){}
//const vector<string>& readcands(const string &q) const;
vector<string> Z2Candidates::readcands(string q){
    return _map[q];
}
vector<string> read_file(const string &path){
    vector<string> obss; //empty vector of strings
    ifstream file(path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            obss.push_back(line);
        }
        file.close();
    }
    return obss;
}
// FILE *fp = open_or_die( filename, "r");
// static FILE *open_or_die( const char *filename, const char *ht )
// {
//     FILE *fp = fopen( filename, ht );
//     if( fp == NULL ){
//         perror( filename);
//         exit(1);
//     }

//     return fp;
// }