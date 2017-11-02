//
//  environment.h
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#ifndef TazBot_environment_h
#define TazBot_environment_h

//#include <vector>
//#include <math.h>
#include <fstream>
//#include <iostream>
//#include "vmath.h"
//#include "helpers.h"
//#include "cervello.h"
#include "diavoli.h"
using namespace std;

//const int WIDTH = 2800; //dimensioni della simulazione
//const int HEIGHT = 1800;
//const int CZ = 20; //cell size in pixels, for food squares. Should divide well into Width Height
const int NUMTAZ=100; //num iniziale diavoli //250
const float TAZRAD=7; //per disegno cerchio
const float BOTSPEED = 0.8;
extern int modcounter;//=0;
extern float costo;//=0;
//cibo
extern float INITFOODin;//=0.10;  //percentuale cibo iniziale
extern float INITFOODfin;//=0.10;  //percentuale cibo finale
extern float FOODINTAKE;//= 0.3; //how much does every Diavolo consume?
//float FOODMAX= 0.5; //how much food per cell can there be at max?
extern int foodcount;//=0;
extern int FW;//= WIDTH/CZ;
extern int FH;//= HEIGHT/CZ;
//float food[WIDTH/CZ][HEIGHT/CZ];
extern int fx, ffx, fy, ffy;//=0, ffx=0;
extern bool destro;
//int fy=0, ffy=0;


void processOutputs(bool muove);
void ripopola_cibo();
void set_cibo(int ciboiniziale);
void update();
void salva_costi(int gen);





#endif
