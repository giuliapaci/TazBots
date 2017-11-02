//
//  environment.cpp
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#include "environment.h"

float INITFOODin=0.05;  //percentuale cibo iniziale
float INITFOODfin=0.05;  //percentuale cibo finale
float FOODINTAKE= 0.3; //how much does every Diavolo consume?
int foodcount=0;
int FW= WIDTH/CZ;
int FH= HEIGHT/CZ;
int fx=0, ffx=0;
int fy=0, ffy=0;
int modcounter=0;
float costo=0;
bool destro;

void processOutputs(bool muove){
	//assign meaning
	//SX    DX    MORSO BOOST
	// 0    1     2     3

    
    if (muove) {
        
        for(int i=0; i<Diavoli.size();i++){
            Diavolo* a= &Diavoli[i];
            
            if(a->boostpassato>0) a->boostpassato-=1;
            
            
            if (fase<3) a->boost=0;
            else {
                a->boost=(a->out[3]);
            }
            
            int ii=1;
            int segno=1;
            
            if(fase>=3){
                if (a->boost>0.1+a->boost_soglia) {
                    ii=2;
                    a->boostpassato=10;
                    if (a->boost<0) segno=-1;
                    if (a->bitten>0) {
                        a->bravura+=10*BRAV;
                    }
                    if (a->in[idCIBOD]>0)
                    {
                        a->bravura+=BRAV;
                    }
                    if (a->morsopassato==0 && a->in[idCIBOD]<=0) {
                        a->bravura-=60*BRAV;
                    }
                }
                if (a->boost>0.4+a->boost_soglia) ii=3;
            }
            
        if(fase==1){
            if(abs(a->w1-a->out[0])<0.001) a->bravura-=BRAV/10;
            if(abs(a->w2-a->out[1])<0.001) a->bravura-=BRAV/10;
        }
        
        //move bots con il boost
        if (ii==1) {
            a->w1= a->out[0];
            a->w2= a->out[1];
        }
        else{
            a->w1= a->out[4];
            a->w2= a->out[5];
        }
        
        
        for(int j=0; j<ii; j++){
            Vector2f v(TAZRAD/2, 0);
            v.rotate(a->angolo + M_PI/2);
            
            Vector2f w1p= - v; //wheel positions
            Vector2f w2p= v;
            
            //intensità delle ruote di destra
            float BW1= BOTSPEED * a->w1;        // il segno è determinato dall'output out[3] (boost)
            //intensità delle ruote di sinistra
            float BW2= BOTSPEED * a->w2;
            
            
            //mia funzione movimento
            Vector2f vv= w1p;
            vv.rotate(+BW1);
            Vector2f D_pos;
            D_pos=vv-w1p;
            
            vv=w2p;
            vv.rotate(-BW2);
            D_pos=D_pos+(vv-w2p);
            
            a->pos.x+=D_pos[0];
            a->pos.y-=D_pos[1];
            a->angolo+=BW1-BW2;
            if(a->angolo>M_PI) a->angolo= -M_PI + (a->angolo-M_PI);
            if(a->angolo<=-M_PI) a->angolo= M_PI - (-M_PI-a->angolo);
            
            
            //wrap around the map
            if(a->pos.x<0) a->pos.x= WIDTH+a->pos.x;
            if(a->pos.x>=WIDTH) a->pos.x= a->pos.x-WIDTH;
            if(a->pos.y<0) a->pos.y= HEIGHT+a->pos.y;
            if(a->pos.y>=HEIGHT) a->pos.y= a->pos.y-HEIGHT;
        }
        }
    }
    
    
    
    else{
        //MORDE
        if (fase>1){
            
            for(int i=0; i<Diavoli.size(); i++){
                Diavoli[i].bitten=0;
                Diavoli[i].morsopassato-=1;
            }
            
            
            float dist=3*TAZRAD;
            
            for(int i=0; i<Diavoli.size();i++){
                Diavolo* a= &Diavoli[i];
                
                a->bit=(a->out[2]);
                a->biter=0;
                if ( (a->bit + a->morso_soglia)>0.8 )
                    {
                        if (randf(0,1)>0.5)     destro=true;
                        else                    destro=false;
                        
                        int j=0;
                        for (int kk=0; kk<2; kk++) {
                            if (destro) j= a->DiavDx;
                            else j= a->DiavSx;
                            
                            if (j==i) continue;
                            
                            Diavolo* a2= &Diavoli[j];
                            
                            float d= (a->pos -a2->pos).length();
                            
                            if(d<dist)
                            {
                                //Se ha morso vicino al cibo viene ulteriormente premiato
                                if(fase!=3)
                                {a->bravura+=10*(tanh(a->odore_cibo_old))*float(BRAV);   //N.B.:qui devo avere come input l'odore del cibo!!
                                    
                                    if((a->out[2]+a->morso_soglia)>1.3) a->bravura+=10*(tanh(a->odore_cibo_old))*float(BRAV);}
                                
                                if(fase==4 && apprendi){
                                    a->brain.colonna[1].apprendimento();
                                }
                                
                                a->biter=2;
                                
                                // però le conseguenze del morso avvengono solo se il diavolo è sufficientemente aggressivo
                                if(randf(0,1)<a->aggress){
                                    if (mode==0)    a->initEvent(TAZRAD*1.4,1,1,0);
                                    a2->morsopassato=10;
                                    a->biter=3;
                                    if (a2->bitten<2) a2->bitten+=1;
                                    if(fase!=2){
                                        a2->salute-=BITELOSS/10;
                                        a2->bravura-=BRAV*20;
                                    }
                                    if (mode==0) 		a2->initEvent(TAZRAD*1.4,1,0.5,0.7);		//alone rosa attorno a chi viene morso
                                    if((a->out[2]+a->morso_soglia)>1.3)
                                    {
                                        if (a2->bitten<2) a2->bitten+=1;
                                        if(fase==4 && apprendi){
                                            a->brain.colonna[1].apprendimento();
                                        }
                                        
                                        if(fase!=2){
                                            a2->salute-=BITELOSS/10;
                                        }
                                        
                                        if (mode==0) 			a2->initEvent(TAZRAD*1.4,1,0,0);		//alone rosso attorno a chi viene morso
                                    }
                                }
                            }
                            
                            destro=!destro;
                        }
                        
                        if (a->biter==0)
                        {
                            a->biter=1;
                            if (fase!=3) {
                                a->bravura-=BRAV*100;
                            }
                            
                            a->salute-=BITELOSS*2;
                            if((a->out[2]+a->morso_soglia)>1.3)
                            {
                                if(fase!=3)     a->bravura-=BRAV*100;
                                a->salute-=BITELOSS*2;
                            }
                            if (mode==0)		a->initEvent(TAZRAD*1.4,0,0,0);
                        }
                        
                    }
                
                if ( (a->bit + a->morso_soglia)<0.8){
                    if (fase!=3) a->bravura+=BRAV;
                    if (fase==4 && apprendi) {
                        a->brain.colonna[1].apprendimento();
                    }
                }
                
                
            }}
        
        
        
        for (int i=0; i<Diavoli.size(); i++) {
            
            if(mode==0){
                if (Diavoli[i].bitten>0 && Diavoli[i].biter>1){
                    Diavoli[i].initEvent(TAZRAD*1.4,0,0,1);
                }
            }
            
        //MANGIA
        if(fase<3 || Diavoli[i].bitten==0)
        {
            Diavoli[i].bitten=0;
            int cx= (int) Diavoli[i].pos.x/CZ;
            int cy= (int) Diavoli[i].pos.y/CZ;
            float f= food[cx][cy];
            
            if(f>0 && Diavoli[i].boostpassato>8){
                Diavoli[i].bravura+=20*BRAV;
                if (fase==4 && apprendi) {
                    Diavoli[i].brain.colonna[2].apprendimento();
                }
            }
            
            if(f>0 && Diavoli[i].salute<SALMAX){
                //Diavolo eats the food
                //                    Diavoli[i].cervello.apprendimento();	//irrobustico la rete visto che ha fatto giusto
                float itk=FOODINTAKE;
                itk=2*itk/(abs(Diavoli[i].w1)*abs(Diavoli[i].w2)+1);
                Diavoli[i].salute+= itk;
                if (fase!=2) Diavoli[i].bravura+=10*BRAV;								//aggiungo quanto cibo ha mangiato questo giro
                if (fase==4 && apprendi){
                    if (Diavoli[i].boostpassato<10) Diavoli[i].brain.colonna[0].apprendimento();
                    else Diavoli[i].brain.colonna[2].apprendimento();
                }
                Diavoli[i].repcounter -= (itk);
                if(Diavoli[i].repcounter<0) Diavoli[i].repcounter=0;
                food[cx][cy]-= FOODMAX/3.0;
                if(food[cx][cy]<(float(FOODMAX)/4.0)){
                    foodcount+=1;
                    food[cx][cy]=0;
                }
            }
        }
            
            Diavolo* a= &Diavoli[i];
            //Bravura boost
            if (a->boostpassato>0) {
                
                if (a->bitten>0) {
                    a->bravura-=BRAV*20*a->boostpassato;
                }
                else{
                    a->bravura+=BRAV;
                }
                if (a->bitten==0 && a->morsopassato>0){
                    a->bravura+=BRAV*20*a->boostpassato;
                    if (fase==4 && apprendi) {
                        a->brain.colonna[2].apprendimento();
                    }
                }
                
            }
        }
    }
}




void ripopola_cibo(){
    while(foodcount>=4){         //ripopolo il cibo come da condizione iniziale
        fx=randi(0,FW);
        fy=randi(0,FH);
        if (food[fx][fy]==0)
        {
            food[fx][fy]=FOODMAX;
            foodcount-=1;
        }
        ffx= fx +1, ffy=fy;
        if(ffx>=FW) ffx=ffx-FW;
        if(ffy>=FH) ffy=ffy-FH;
        if(food[ffx][ffy]==0){
            food[ffx][ffy]=FOODMAX;
            foodcount-=1;}
        ffx= fx +1, ffy=fy-1;
        if(ffx>=FW) ffx=ffx-FW;
        if(ffy>=FH) ffy=ffy-FH;
        if(food[ffx][ffy]==0){
            food[ffx][ffy]=FOODMAX;
            foodcount-=1;}
        ffx= fx , ffy=fy-1;
        if(ffx>=FW) ffx=ffx-FW;
        if(ffy>=FH) ffy=ffy-FH;
        if(food[ffx][ffy]==0){
            food[ffx][ffy]=FOODMAX;
            foodcount-=1;}
        
    }
}


void set_cibo(int ciboiniziale){
    
    while(ciboiniziale>=4){         //ripopolo il cibo come da condizione iniziale
        fx=randi(0,FW);
        fy=randi(0,FH);
        if (food[fx][fy]==0)
        {
            food[fx][fy]=FOODMAX;
            ciboiniziale-=1;
        }
        ffx= fx +1, ffy=fy;
        if(ffx>=FW) ffx=ffx-FW;
        if(ffy>=FH) ffy=ffy-FH;
        if(food[ffx][ffy]==0){
            food[ffx][ffy]=FOODMAX;
            ciboiniziale-=1;}
        ffx= fx +1, ffy=fy-1;
        if(ffx>=FW) ffx=ffx-FW;
        if(ffy>=FH) ffy=ffy-FH;
        if(food[ffx][ffy]==0){
            food[ffx][ffy]=FOODMAX;
            ciboiniziale-=1;}
        ffx= fx , ffy=fy-1;
        if(ffx>=FW) ffx=ffx-FW;
        if(ffy>=FH) ffy=ffy-FH;
        if(food[ffx][ffy]==0){
            food[ffx][ffy]=FOODMAX;
            ciboiniziale-=1;}
    }
    
    while (ciboiniziale>0) {
        fx=randi(0,FW);
        fy=randi(0,FH);
        if(food[fx][fy]==0){
            food[fx][fy]=FOODMAX;
            ciboiniziale-=1;
        }
    }
    
    
}


void update() {
//    cout << modcounter << endl;
    bool muove;
    costo=0;
	modcounter++;
	//Process periodic events
	
    if(modcounter>1){
        muove=false;
        setInputs(muove);
        cervellosTick();
        //read output and process consequences of bots on environment. requires out[]
        processOutputs(muove);
        
        //remove dead Diavoli
        vector<Diavolo>::iterator iter= Diavoli.begin();
        while(iter != Diavoli.end()){
            if(iter->salute <=0) {// || iter->bravura <=-BRAVMAX){
                iter= Diavoli.erase(iter);
            }
            else {
                ++iter;
            }
        }
        
        
        //environment tick
        ripopola_cibo();
    }
	
    muove=true;
	//give input to every Diavolo. Sets in[] array
	setInputs(muove);
    
	//cervellos tick. computes in[] -> out[]
	cervellosTick();
    processOutputs(muove);

    //baseloss
    for(int i=0;i<Diavoli.size();i++){
        int ii=1;
        if(fase>=3){
            if (abs(Diavoli[i].boost)>0.2+Diavoli[i].boost_soglia) ii=10;
            if (abs(Diavoli[i].boost)>0.6+Diavoli[i].boost_soglia) ii=12;
        }
        
        float baseloss= ii*(BITELOSS/20 + BITELOSS/200*(abs(Diavoli[i].w1) + abs(Diavoli[i].w2)));
        Diavoli[i].sal_old=Diavoli[i].salute;
        Diavoli[i].salute -= baseloss;
        if (fase==1) Diavoli[i].bravura -= 1;
    }
    
	
	for(int i=0;i<Diavoli.size();i++){
        costo+=float(Diavoli[i].bravura);
	}
    
    
    
}



void salva_costi(int gen) {
    ofstream fileout;
    char nomefile[32];
    snprintf(nomefile, sizeof(char) * 32, "Costo_gen_%i.txt", gen);
    
    fileout.open(nomefile, ios::out | ios::app);
    time_t t=time(NULL);
    tm *time;
    time=localtime(&t);
    
    if (modcounter==1) fileout <<"Costo della popolazione"<<'\t'<<"Diavoli.size" << '\t' << "modcounter" << '\t' <<"Registrato il: "<< time->tm_year + 1900 <<"/"<< time->tm_mon +1 <<"/"<< time->tm_mday<<" alle ore: " <<time->tm_hour <<":"<< time->tm_min <<endl;    //scrive intestazione
    
    fileout  << costo/float(modcounter) <<'\t'<<Diavoli.size() << '\t' << modcounter << endl;
    
    fileout.close();
    
}
