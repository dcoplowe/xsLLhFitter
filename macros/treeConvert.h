#ifndef TREECONVERT_H
#define TREECONVERT_H

#include "TString.h"

class treeConvert {
public:
    treeConvert();
    ~treeConvert();
    
    void Convert(TString inFileName, TString inTreeName, TString outFileName, TString D1NameRec, TString D1NameTrue, TString D2NameRec = "", TString D2NameTrue = "", bool scale = false, double scale_size = 1);
};
#endif