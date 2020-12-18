#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
using namespace std;

class Z2Candidates{
    private:  
    public:
    Z2Candidates();
    Z2Candidates(const string &path);
    ~Z2Candidates();
    vector<string> readcands(string q) ;
     
    map<string,vector<string>> _map;
    
};