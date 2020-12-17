#include <stdio.h>
#include "Ngram.h"
using namespace std;

int main(int argc, char *argv[])
{
    int ngram_order = 2;
    Vocab voc;
    Ngram lm( voc, ngram_order );

    {
        const char lm_filename[] = "./cor_lm";
        File lmFile( lm_filename, "r" );
        lm.read(lmFile);
        lmFile.close();
    }

    VocabIndex wid = voc.getIndex("Ê¨");
    if(wid == Vocab_None) {
        printf("No word with wid = %d\n", wid);
        printf("where Vocab_None is %d\n", Vocab_None);
    }

    wid = voc.getIndex("«¢");
    cout<<voc.getIndex("«¢")<<endl;
    //VocabIndex context[] = {voc.getIndex("Åo"), voc.getIndex("«¢") };//, voc.getIndex("«¢"), Vocab_None};
    //printf("Log Prob(±wªÌ|¬r-Å}) = %f\n", lm.wordProb(wid, context));
    VocabIndex context[] = {voc.getIndex("<s>")};
    //cout<<lm.wordProb(voc.getIndex("<unk>"), context);
    //VocabIndex context2[5];
    //cout<<lm.wordProb(voc.getIndex("«¢"),context2);

    return 0;
}

