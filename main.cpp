//
//  main.cpp
//  TazBot
//
//  Created by Arianna Pace, Giulia Paci e Manami Zanzi.
//  Copyright (c) 2014 TazBot. All rights reserved.
//

//#include <vector>
//#include <math.h>
//#include <fstream>
//#include <iostream>
//#include "vmath.h"
//#include "helpers.h"
//#include "cervello.h"
//#include "diavoli.h"
//#include "environment.h"
//#include "gen_alg.h"
#include "grafica.h"
using namespace std;

#define RANDMAX 0x7fff

// returns a float in the range -1.0f -> - 1.0f
#define RANDOM_CLAMP    (((float)rand()-(float)rand())/RANDMAX)

//returns a float between 0 & 1
#define RANDOM_NUM		((float)rand()/(RANDMAX+1))


int main(int argc, char **argv) {
    
    float perc;
    int ciboiniziale;
    clock_t t1, t2;
    int seed=int(time(NULL));
    srand(seed);
    
    cout << "Scrivi 0 per modalità grafica e 1 per modalità evoluzione" << endl;
    cin >> mode;
    
	if(WIDTH%CZ!=0 || HEIGHT%CZ!=0) printf("CAREFUL! The cell size variable CZ should divide evenly into  both WIDTH and HEIGHT! It doesn't right now!");
	
	//int fx,fy;
    
    cout << endl << "In che fase vuoi fare sviluppare i cervelli? (1= movimento, 2= morso, 3= completo)" << endl;//, 4= apprendimento
    cin >> fase;
    
    addRandomBots(NUMTAZ);
    
    for (int h=0; h<Diavoli.size(); h++) {
        Diavoli[h].id=h;
    }
    
    LoadCervelli();
    
    
    
    if (mode==1){
    
        int gen=0;  //contatore generazioni
        int count=0;//contatore tick di ogni generazione
        int ans=1;
        
        
        while(ans){
        t1=clock();
        int numgen=1;
        cout << endl << "Dimmi quante generazioni vuoi fare: "<< endl;
        cin >> numgen;
        numgen=numgen+gen;

        while(gen<numgen) {
        
            for(int i=0; i<FW; i++){                //azzero la matrice del cibo
                for(int j=0; j<FH; j++){
                    food[i][j]=0;
                }
            }
        
            float dperc=(INITFOODfin-INITFOODin)/float(numgen);
            perc=dperc*float(gen)+INITFOODin;
            ciboiniziale=int(float(FW*FH)*perc);
        
            set_cibo(ciboiniziale); //come da condizione iniziale
        
            while (ciboiniziale>0) {
                fx=randi(0,FW);
                fy=randi(0,FH);
                if(food[fx][fy]==0){
                    food[fx][fy]=FOODMAX;
                    ciboiniziale-=1;
                }
            }
        
        
            cout << endl << "Generazione " << gen << endl;
            count=0;
            while(count<NUMGIRI){
                update();
                if (gen%5==0) {
                    salva_costi(gen);   //scommentare anche in update() la somma delle bravure per avere il costo!
                }
                if(Diavoli.size()<=TOPTAZ+(TOPTAZ/2)) count=NUMGIRI;
                    count++;
            }

            newgen(gen, numgen);
            gen++;
    }
            
    
        SalvaCervelli(-gen, Diavoli);      //salva i cervelli finali con generazione=-gen
    
    t2=clock();
    float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
    cout<< endl<< "Ho impiegato "<< diff << " secondi."<< endl;
    cout<< endl<< "Vuoi chiudere? (0=chiudi, 1=continua)"<<endl;
    cin>>ans;
        }
    }
    
    else if (mode==0){

        
        float dperc=(INITFOODfin-INITFOODin)/float(1);
        perc=dperc*1+INITFOODin;
        ciboiniziale=int(float(FW*FH)*perc);
        
        set_cibo(ciboiniziale);
        
        //GLUT SETUP
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(30,30);
        glutInitWindowSize(WWIDTH,WHEIGHT);
        glutCreateWindow("TEST");
        glClearColor(0.9f, 0.9f, 1.0f, 0.0f);
        glutDisplayFunc(renderScene);
        glutIdleFunc(handleIdle);
        glutReshapeFunc(changeSize);
        
        glutKeyboardFunc(processNormalKeys);
        glutMouseFunc(processMouse);
        glutMotionFunc(processMouseActiveMotion);
        
        glutMainLoop();
    
    }
    
    else cout << "Hai inserito un comando non valido" << endl;
    return 0;
}
