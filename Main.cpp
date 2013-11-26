/*
 *Main.cpp
 *
 *The main program or the interface
 *
 *@author:  zhang haibo
 *@tiem:    2013-11-22
 *@version: 1.0
 */
#include "TBL.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "Usge: train-corpus" << endl;
        return 1;
    }
 
    TBL* tbl = new TBL();
    tbl->Train(argv[1]);
    delete tbl;

    return 0;   
}
