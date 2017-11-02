//
//  diavoli.h
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#ifndef TazBot_diavoli_h
#define TazBot_diavoli_h




//#include <vector>
//#include <math.h>
//#include "vmath.h"
//#include "helpers.h"
#include "brain.h"
using namespace std;

const float REPRATE=20;
extern bool apprendi;
extern float METAMUTRATE1;//= 0.02; //what is the change in MUTRATE1 and 2 on reproduction?
extern float METAMUTRATE2;//= 0.5;
extern float METAMUTRATE_DNA1;
extern float METAMUTRATE_DNA2;
extern float METAMUT_DNA_SOGLIA;
const float DIST= 100; //quanto lontano vede/sente gli altri diavoli
const float SALMAX=10;				//massimo della salute
const float BITELOSS=0.1;			//quanta vita perde perché è stato morso o ha morso il niente
extern float BRAV;//=20;  //incremento della bravura quando mangiano

class Diavolo{
public:
	Vector2f pos;
    
	float bravura; //in [-BRAVMAX,BRAVMAX]
    
	float salute; //in [0,SALMAX]
    float sal_old;
    float odore_cibo_old;
	float angolo;
	
	float w1; //wheel speeds
	float w2;
    
	float bit; //l'output "vecchio" del morso
    float boost;  //se il diavolo "corre" o no
	
	int age;
	float aggress; //aggressivit‡ del diavolo
    float boost_soglia, morso_soglia;
	int biter, bitten;
    int morsopassato;
    int boostpassato;
	bool sesso; //true femmina false maschio
    int genitoreA, genitoreB;
    
	vector<float> in;
	vector<float> out;
    
	float repcounter; //when repcounter gets to 0, this bot reproduces
	int gencount; //generation counter
	float clockf1, clockf2; //the frequencies of the two clocks of this bot
	
	//variables for drawing purposes
	float indicator;
	float ir;float ig;float ib; //indicator colors
	int selectflag; //is this Diavolo selected?
    
	int id;
    
	float MUTRATE1; //how often do mutations occur?
	float MUTRATE2; //how significant are they?
    float MUTRATE_DNA1;
    float MUTRATE_DNA2;
    float MUT_DNA_SOGLIA;
    
    int Dcibo;
    int DciboV;
    float AngoloV;
    float AngoloVmin;
    float Ddiav;
    float DdiavV;
    float cibo[50][50];
    int DiavDx;         //indice del diavolo che vede alla sua destra
    int DiavSx;         //indice del diavolo che vede alla sua sinistra
    
    Cervello brain;
    
    Diavolo(); //costruttore

	void initEvent(float size, float r, float g, float b);
};

extern vector<Diavolo> Diavoli;

void addRandomBots(int num);

Vector2f guarda(int cx, int cy, float angdiav, float norm, int dd, int odx, int osx, Vector2f Coord);

void setInputs(bool muove);

void cervellosTick();


#endif
