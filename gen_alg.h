//
//  gen_alg.h
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#ifndef TazBot_gen_alg_h
#define TazBot_gen_alg_h

//#include <vector>
//#include <math.h>
//#include <fstream>
//#include <iostream>
//#include "vmath.h"
//#include "helpers.h"
//#include "cervello.h"
//#include "diavoli.h"
#include "utilities.h"
using namespace std;

extern int TOPTAZ;//= int(float (NUMTAZ) * 0.1);      //n° diavoli di riprodurre
extern int TOPTOPTAZ;//=int (float(TOPTAZ) * 0.25);   //n° diavoli da clonare
extern const int NUMGIRI;//=2000;


extern vector<Diavolo> DiaSort;
bool compara_bravura (Diavolo i,Diavolo j);
int roulette(int max);
void newgen(int gen, int numgen);




#endif
