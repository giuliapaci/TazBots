//
//  utilities.h
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#ifndef TazBot_utilities_h
#define TazBot_utilities_h

//#include <vector>
//#include <math.h>
//#include <fstream>
//#include <iostream>
//#include "vmath.h"
//#include "helpers.h"
//#include "cervello.h"
//#include "diavoli.h"
#include "environment.h"
using namespace std;

extern bool controlla_in, controlla_out;
const int SaveRate=50; //ogni quante generazioni salvo i cervelli

void SalvaCervelli(int gen, vector<Diavolo> Diav);
void LoadCervelli();
void SalvaMutrate(vector<Diavolo> Diav);

#endif
