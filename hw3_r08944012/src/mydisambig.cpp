#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Ngram.h"
#include "../inc/hw3src.h"

using namespace std;
    
#ifndef MAX_LEN
#	define MAX_LEN    128
#endif
const int pZero = -100;
Vocab voc;//
Ngram lm(voc, 2);//

//Get P(W2 | W1) -- bigram
double getBigramProb(const string &w1, const string &w2)
{
    VocabIndex wid1 = voc.getIndex(w1.c_str());//
    VocabIndex wid2 = voc.getIndex(w2.c_str());
    //cout<<"id"<<" "<<wid1<<" "<<wid2<<" ";

    if(wid1 == Vocab_None)  //OOV//
        wid1 = voc.getIndex(Vocab_Unknown);//
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);
    
    VocabIndex context[] = { wid1, Vocab_None };
    return lm.wordProb( wid2, context);
}
// Get P(W3 | W1, W2) -- trigram
double getTrigramProb(const string &w1, const string &w2, const string &w3) 
{
    VocabIndex wid1 = voc.getIndex(w1.c_str());//
    VocabIndex wid2 = voc.getIndex(w2.c_str());
    VocabIndex wid3 = voc.getIndex(w3.c_str());

    if(wid1 == Vocab_None)  //OOV
        wid1 = voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = voc.getIndex(Vocab_Unknown);
    if(wid3 == Vocab_None)  //OOV
        wid3 = voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid2, wid1, Vocab_None };
    return lm.wordProb( wid3, context );
}
string disambigSentence3(const string &sent, Z2Candidates &mapping){
    vector<vector<vector<double>>> prob((sent.length()/2)+1);
    vector<vector<vector<int>>> path((sent.length()/2)+1);
    vector<string> candidates;
    vector<string> p_candidates;
    vector<string> pp_candidates;
    double ngram_prob;
    //init
    candidates = mapping.readcands(sent.substr(0,2));//get cands of the first token
    //prob[0].push_back(vector<vector<double>>(candidates.size()));
    prob[0].resize(candidates.size());
    //path[0].push_back(vector<vector<int>>(candidates.size()));
    path[0].resize(candidates.size());
    for(int i=0;i<candidates.size();i++){
        ngram_prob = getBigramProb("<s>", candidates[i]);
        prob[0][i].push_back(ngram_prob);
        path[0][i].push_back(-1);
    }
    p_candidates = candidates;
    //return "a";
    candidates = mapping.readcands(sent.substr(2,2));//get cands of the second token
    //prob[1].push_back(vector<vector<double>>(candidates.size()));
    prob[1].resize(candidates.size());
    //path[1].push_back(vector<vector<int>>(candidates.size()));
    path[1].resize(candidates.size());
    for(int i=0;i<candidates.size();i++){
        for(int j=0;j<p_candidates.size();j++){
            //ngram_prob = getBigramProb(p_candidates[j],candidates[i]) + getBigramProb("<s>", p_candidates[j]);
            ngram_prob = getBigramProb("<s>", p_candidates[j])+getTrigramProb("<s>",p_candidates[j],candidates[i]);
            prob[1][i].push_back(ngram_prob);
            path[1][i].push_back(-1);
        }
    }
    pp_candidates = p_candidates;
    p_candidates = candidates;
    //dp
    for(int s=2;s<(sent.length()/2)+1;s++){
        if(s==sent.length()/2) candidates = mapping.readcands("</s>");
        else candidates = mapping.readcands(sent.substr(s*2,2));
        //prob[s].push_back(vector<vector<double>>(candidates.size()));
        prob[s].resize(candidates.size());
        //path[s].push_back(vector<vector<int>>(candidates.size()));
        path[s].resize(candidates.size());
        for(int i=0;i<candidates.size();i++){
            for(int j=0;j<p_candidates.size();j++){//[i][j]
                double _m_p = -1/0.0;//-inf
                int _m_i = 0;
                for(int k=0;k<pp_candidates.size();k++){
                    ngram_prob = getTrigramProb(pp_candidates[k],p_candidates[j],candidates[i]) + prob[s-1][j][k];
                    if(ngram_prob>_m_p){
                        _m_p = ngram_prob;
                        _m_i = k;
                    }
                }
                if(_m_i==-1) cout<<_m_p<<" "<<_m_i;
                prob[s][i].push_back(_m_p);//prob[s][i][j]
                path[s][i].push_back(_m_i);
            }
        }
        pp_candidates = p_candidates;
        p_candidates = candidates;
    }
    //backtracking
    double _mval=-1/0.0;
    int ii,jj,temp;
    int si = (sent.length()/2);
    for(int i=0;i<prob[si].size();i++){
        for(int j=0;j<prob[si][i].size();j++){
            //cout<<i<<" "<<j<<endl;
            if(prob[si][i][j]>_mval){
                _mval = prob[si][i][j];
                ii = i;
                jj = j;
            }
        }
    }
    string out = "</s>";
    //out.insert(0," ");
    //out.insert(0,candidates[ii]);
    temp = path[si][ii][jj];
    for(int s=si-1;s>0;s--){
        ii = jj;
        jj = temp;
        candidates = mapping.readcands(sent.substr(s*2,2));
        out.insert(0," ");
        out.insert(0,candidates[ii]);
        temp = path[s][ii][jj];
        //cout<<temp<<" ";
    }
    candidates = mapping.readcands(sent.substr(0,2));
//     _mval = -1/0.0;
//     candidates = mapping.readcands(sent.substr(0,2));
//     for(int i=0;i<candidates.size();i++){
//         ngram_prob = getBigramProb("<s>", candidates[i]);
//         if(ngram_prob>_mval){
//             _mval = ngram_prob;
//             jj = i;
//         }
//     }
    out.insert(0," ");
    out.insert(0,candidates[jj]);
    out.insert(0," ");
    out.insert(0,"<s>");
    return out;
}


string disambigSentence(const string &sent, Z2Candidates &mapping){//pass by reference
    vector<vector<double>> prob(sent.length()+1);
    vector<vector<int>> path(sent.length()+1); //vocab of max
    vector<string> candidates;
    vector<string> pre_candidates;
    double ngram_p = 0;
    double pZero = -1/0.0;//-inf
    
    //init
    candidates = mapping.readcands(sent.substr(0,1*2));
    for(int i=0;i<candidates.size();i++){
        //cout<<candidates[i];
        ngram_p = getBigramProb("<s>", candidates[i]);
        //cout<<ngram_p<<endl;
        prob[0].push_back(ngram_p);
        path[0].push_back(-1);
    }
    pre_candidates = candidates;
    //dp
    for(int s=1;s<(sent.length()/2)+1;s++){
        //cout<<s;
        //cout<<sent.substr(s*2,2)<<endl;
        if(s==sent.length()/2) candidates = mapping.readcands("</s>");
        else candidates = mapping.readcands(sent.substr(s*2,2));
        //cout<<candidates.size()<<endl;
        for(int i=0;i<candidates.size();i++){
            double max_inc = pZero;
            int max_pre = -1;
            for(int j=0;j<pre_candidates.size();j++){
                //cout<<pre_candidates[j];
                ngram_p = getBigramProb(pre_candidates[j], candidates[i])+prob[s-1][j];
                if(ngram_p>max_inc){
                    max_inc = ngram_p;
                    max_pre = j;
                }
            }
            prob[s].push_back(max_inc);
            path[s].push_back(max_pre);
        }
        pre_candidates = candidates;
    }
    //cout<<pre_candidates.size()<<" "<<candidates.size()<<endl;
    //return "a";
    //backtracking
    double _max = -10000000;
    int m_i = -1;
    for(int i=0;i<pre_candidates.size();i++){
        if(prob[(sent.length()/2)][i]>_max){
            _max = prob[(sent.length()/2)][i];
            m_i = i;
        }
    }
    //cout<<_max<<" "<<m_i;
    //return "a";
    string out = "";
    int s = (sent.length()/2);
    while(1){
        if(s==(sent.length()/2)) candidates = mapping.readcands("</s>");
        else candidates = mapping.readcands(sent.substr(s*2,2));
        //cout<<candidates[m_i]<<" ";
        if(s!=(sent.length()/2)) out.insert(0," ");
        out.insert(0,candidates[m_i]);
        if(path[s][m_i]==-1) break;
        m_i = path[s][m_i];
        s--;
    }
    out.insert(0,"<s> ");
    //reverse(out.begin(), out.end());//ruin <s>,</s>
    return out;
}

static FILE *open_or_die( const char *filename, const char *ht )
{
    FILE *fp = fopen( filename, ht );
    if( fp == NULL ){
        perror( filename);
        exit(1);
    }

    return fp;
}

int main(int argc, char *argv[])
{
    {
        //const char lm_filename[] = argv[1];//lm
        File lmFile(argv[3], "r"); //distruct file object when leaving block (like context manager)
        lm.read(lmFile);
        lmFile.close();
    }
    Z2Candidates mapping(argv[2]);//Zue mapping
    ifstream file(argv[1]);//segemented file
    //FILE *gt = open_or_die(argv[5], "r");
    FILE *output_result = open_or_die(argv[4], "w");
    char g[MAX_LEN*2];
    if (file.is_open()) {
        string line;
        //while (getline(file, line) && fscanf(gt, "%[^\n]%*c", g)==1) {
        while (getline(file, line)) {
            line.erase( remove( line.begin(), line.end(), ' ' ), line.end() );
            //cout<<line<<endl;
            string out;
            if(argc==5){
                //cout<<"Use bigram"<<endl;
                out = disambigSentence(line, mapping);
            } 
            else out = disambigSentence3(line, mapping);
            //cout<<out<<endl;
            fprintf(output_result, "%s\n", out.c_str());
            //cout<<g<<endl;
//             if(strcmp(g,out.c_str())!=0){
//                 cout<<g<<endl;
//                 cout<<out<<endl;
//                 cout<<"-------------"<<endl;
//             }
        }
        file.close();   
    }
    //fclose(gt);
    fclose(output_result);
    
    
//     gt = open_or_die(argv[5], "r");
//     output_result = open_or_die(argv[4], "r");
//     char my[MAX_LEN*2];
//     while (fscanf(output_result, "%[^\n]%*c", my)==1 && fscanf(gt, "%[^\n]%*c", g)==1) {
//         if(strcmp(g,my)==0){
//             cout<<g<<endl;
//             cout<<my<<endl;
//             cout<<"-------------"<<endl;
//             break;
//         }
//     }
//     fclose(gt);
//     fclose(output_result);
    
    
    cout<<"end"<<endl;
    return 0;
    
}
