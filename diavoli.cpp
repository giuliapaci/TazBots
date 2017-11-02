//
//  diavoli.cpp
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#include "diavoli.h"
#include <fstream>


vector<Diavolo> Diavoli;

bool apprendi=false;
float METAMUTRATE1= 0.02; //what is the change in MUTRATE1 and 2 on reproduction?
float METAMUTRATE2= 0.5;
float METAMUTRATE_DNA1=0.1;
float METAMUTRATE_DNA2=0.5;
float METAMUT_DNA_SOGLIA=0.05;
float BRAV=20;  //incremento della bravura quando mangiano




Diavolo::Diavolo(){
    //constructor
    genitoreA=-1;
    genitoreB=-1;
    
    float y;
    y=randf(0,HEIGHT);
    float x;
    x=randf(0,WIDTH);
    
    pos= Vector2f(x,y);
    angolo= randf(-M_PI,M_PI);
    salute= 5.0+randf(0,1);
    sal_old=0;
    odore_cibo_old=0;
    bravura=0;
    sesso=(randf(0,1)>0.5); //assegnazione random 50-50 del sesso
    aggress=randf(0,1);
    morso_soglia=randf(0,1);
    boost_soglia=randf(0,0.8);
    if (! sesso) aggress+=randf(0,0.2);
    bitten=0;
    biter=0;
    morsopassato=0;
    boostpassato=0;
    age=0;
    w1=0;
    w2=0;
    bit=0;
    boost=0;
    clockf1= randf(5,100);
    clockf2= randf(5,100);
    indicator=0;
    gencount=0;
    selectflag=0;
    ir=0;ig=0;ib=0;
    //hybrid= false;
    repcounter= randf(REPRATE-0.1,REPRATE+0.1)+randf(REPRATE-0.1,REPRATE+0.1);
    
    id=0;
    
    DiavDx=-2;
    DiavSx=-2;

// per la fase 4 utilizziamo valori più piccoli per le mutazioni in modo che non si sfasi troppo il cervello!
    MUTRATE1=       0.005;
    MUTRATE2=       0.4;
    MUTRATE_DNA1=   0.1;
    MUTRATE_DNA2=   0.05;
    MUT_DNA_SOGLIA= 0.2;
    
//    MUTRATE1= 0.01;     //era 0.001
//    MUTRATE2= 0.8;      //era 0.1
//    MUTRATE_DNA1= 0.3;
//    MUTRATE_DNA2= 0.8;
//    MUT_DNA_SOGLIA= 0.5;

    in.resize(INPUTSIZE, 0);
    out.resize(OUTPUTSIZE, 0);
    
    Dcibo=25;
    DciboV=10;
    AngoloV=M_PI/3;
    AngoloVmin=0;
    Ddiav=30;
    DdiavV=5*CZ;
}

void Diavolo::initEvent(float size, float r, float g, float b){ //for drawing purposes
    indicator=size;
    ir=r;ig=g;ib=b;
}


void addRandomBots(int num){
	for(int i=0;i<num;i++){
		Diavolo a;
		Diavoli.push_back(a);
	}
}

Vector2f guarda_cibo(int cx, int cy, float norm, int dd, int odx, int osx, Vector2f Coord){
    int ix,jy;
    float ang, diff1, diff2;
    float distanza;
    Vector2f occhi;
    occhi[0]=0;
    occhi[1]=0;
    
    ix=cx+int(Coord[0]);
    jy=cy-int(Coord[1]);
    
    while (ix<0)
        ix=(float(WIDTH)/float(CZ))+(cx+Coord[0]);
    while (ix>=(float(WIDTH)/float(CZ)))
        ix=(cx+Coord[0])-(float(WIDTH)/float(CZ));
    
    while (jy<0)
        jy=(float(HEIGHT)/float(CZ))+(cy+Coord[1]);
    while (jy>=(float(HEIGHT)/float(CZ)))
        jy=(cy+Coord[1])-(float(WIDTH)/float(CZ));
    
    ang=Coord.get_angle();
    
    diff1=(Diavoli[dd].angolo + Diavoli[dd].AngoloVmin)-ang;
    diff2=(Diavoli[dd].angolo - Diavoli[dd].AngoloVmin)-ang;
    
    if (diff1>M_PI)      diff1=diff1-2*M_PI;
    if (diff1<=-M_PI)    diff1=diff1+2*M_PI;
    
    if (diff2>M_PI)      diff2=diff2-2*M_PI;
    if (diff2<=-M_PI)    diff2=diff2+2*M_PI;
    
    if (abs(diff1)<Diavoli[dd].AngoloV) {
    if (diff1>=0 && odx==1) {
        distanza=norm*sqrt((ix-cx)*(ix-cx)+(jy-cy)*(jy-cy));
        if(distanza<1) distanza=1;
        occhi[0]=food[ix][jy]/(float(FOODMAX)*distanza);
        if(mode==0 && mostra_vista_cibo)        Diavoli[dd].cibo[int(Coord[0])+Diavoli[dd].DciboV][-int(Coord[1])+Diavoli[dd].DciboV]=occhi[0];
    }
    }
    if (abs(diff2)<Diavoli[dd].AngoloV){
    if (diff2<0 && osx==1) {
        distanza=norm*sqrt((ix-cx)*(ix-cx)+(jy-cy)*(jy-cy));
        if(distanza<1) distanza=1;
        occhi[1]=food[ix][jy]/(float(FOODMAX)*distanza);
        if(mode==0 && mostra_vista_cibo)        Diavoli[dd].cibo[int(Coord[0])+Diavoli[dd].DciboV][-int(Coord[1])+Diavoli[dd].DciboV]=-(occhi[1]);
    }
    }
    
    return occhi;

}


void Debug(){}


void setInputs(bool muove) {
    for(int dd=0;dd<Diavoli.size();dd++){
		Diavolo* a= &Diavoli[dd];
        
    if (mode==0 && mostra_vista_cibo && muove) {
        //annullo matrice cibo visto del diavolo
        for (int i=-a->DciboV; i<=a->DciboV; i++) {
            for (int j=-a->DciboV; j<=a->DciboV; j++) {
                a->cibo[i+a->DciboV][j+a->DciboV]=0;
            }
        }
    }
    
		//salute
        if (muove) {
            if (a->salute>SALMAX)
                a->salute=SALMAX;		//faccio in modo che il massimo per la salute sia SALMAX
            a->in[idSAL]= cap(a-> salute/SALMAX);
            a->in[idDERSAL]= tanh((a->salute - a->sal_old)/SALMAX);
        }
        
		float rcibo=0;
        float lcibo=0;
		int cx= int(a->pos.x/CZ);
		int cy= int(a->pos.y/CZ);
		int ix,jy;
		Vector2f CoordCibo(0,0);
        Vector2f occhi(0,0);
		float ang;
        float norm=0.5;
        float odore=0;
        
		////VISTA e ODORE CIBO
        //normalizzazione per la vista del cibo
        
		for (int i=-a->Dcibo;i<=a->Dcibo;i++)
		{
			for (int j=-a->Dcibo;j<=a->Dcibo;j++)
			{
                //ODORE CIBO
                ix=cx+i;
                jy=cy+j;
                if (ix<0)
                    ix=(WIDTH/CZ)+(cx+i);
                if (ix>=(WIDTH/CZ))
                    ix=(cx+i)-(WIDTH/CZ);
                
                if (jy<0)
                    jy=(HEIGHT/CZ)+(cy-j);
                if (jy>=(HEIGHT/CZ))
                    jy=(cy-j)-(HEIGHT/CZ);
                odore+=0.5*food[ix][jy]/float((FOODMAX)*(abs(i)+1)*(abs(j)+1));
            }
        }
        
        float temp;

        if(odore<0){
            temp=0;
        }
        else if(odore>10){
            temp=10;
        }
        else{
            temp=odore;
        }
        
        odore=15*(temp - a->odore_cibo_old);
        
        a->odore_cibo_old=temp;
        if (fase!=2) {
            if(odore>0) a->bravura+=BRAV;
            if (fase==4 && apprendi) {
                a->brain.colonna[0].apprendimento();
            }
        }
        
        
		a->in[idCIBOD]=tanh(odore);
        
        //VEDO
        if (muove){
        int occhiodx=1;
        int occhiosx=1;
        for (int i=1; i<=a->DciboV; i++) {
            if (occhiodx!=0 || occhiosx!=0) {
            for (int j=-a->DciboV; j<=a->DciboV; j++) {
                //VISTA CIBO
                if (abs(j)<=i) {
                    for (int k=-1; k<=1; k=k+2) {
                        if (!(k*i==0 && j==0)){
                        CoordCibo[0]=k*i;
                        CoordCibo[1]=j;
//                        alg iniz
                            ix=cx+int(CoordCibo[0]);
                            jy=cy-int(CoordCibo[1]);
                            
                            while (ix<0)
                                ix=(float(WIDTH)/float(CZ))+ix;
                            while (ix>=(float(WIDTH)/float(CZ)))
                                ix=ix-(float(WIDTH)/float(CZ));
                            
                            while (jy<0)
                                jy=(float(HEIGHT)/float(CZ))+jy;
                            while (jy>=(float(HEIGHT)/float(CZ)))
                                jy=jy-(float(WIDTH)/float(CZ));
                            
                        if(food[ix][jy]!=0){
                            occhi=guarda_cibo(cx, cy, norm, dd, occhiodx, occhiosx, CoordCibo);
                            if (occhi[0]>rcibo) rcibo=occhi[0];
                            if (occhi[1]>lcibo) lcibo=occhi[1];
                        }
//                        alg fine
                        
                        if (abs(k*i)!=abs(j)) {
                            CoordCibo[0]=j;
                            CoordCibo[1]=k*i;
//                            alg inizio
                            ix=cx+int(CoordCibo[0]);
                            jy=cy-int(CoordCibo[1]);
                            
                            while (ix<0)
                                ix=(float(WIDTH)/float(CZ))+ix;
                            while (ix>=(float(WIDTH)/float(CZ)))
                                ix=ix-(float(WIDTH)/float(CZ));
                            
                            while (jy<0)
                                jy=(float(HEIGHT)/float(CZ))+jy;
                            while (jy>=(float(HEIGHT)/float(CZ)))
                                jy=jy-(float(WIDTH)/float(CZ));
                            
                            if(food[ix][jy]!=0){
                                occhi=guarda_cibo(cx, cy, norm, dd, occhiodx, occhiosx, CoordCibo);
                                if (occhi[0]>rcibo) rcibo=occhi[0];
                                if (occhi[1]>lcibo) lcibo=occhi[1];
                            }
//                            alg fine
                        }
                        }

                    }

                }
            }
            }
            if (rcibo>0) occhiodx=0;
            if (lcibo>0) occhiosx=0;
        }

		a->in[idCIBVID]=cap(rcibo);
		a->in[idCIBVIS]=cap(lcibo);
        }
        
		//ODORE Diavoli
        float odorediav=0;
		float rdiav=0, ldiav=0;
        float dmindx=a->DdiavV;
        float diff1, diff2;
        float dminsx=a->DdiavV;
        
        if (mode==0 && mostra_vista_diavoli) {
            a->DiavDx=-2;
            a->DiavSx=-2;
        }
        
        if(fase>1){
		for(int add=0;add<Diavoli.size();add++){
			if(dd==add) continue;
			Diavolo* a2= &Diavoli[add];
            
            float dist=max(a->DdiavV,a->Ddiav);
            
			if(abs(a->pos.x-a2->pos.x) > dist || abs(a->pos.y-a2->pos.y)> dist) continue;
            
			float d= (a->pos-a2->pos).length();
            
			if(!muove){
			if(d<a->Ddiav)
			{
                odorediav+=(a->Ddiav-d)/a->Ddiav;
            }}
            
            if(d<a->DdiavV){
				//VISTA DIAVOLI
                if (d<dmindx || d<dminsx) {
                Vector2f CoordAnimale;
                CoordAnimale[0]=-a->pos.x + a2->pos.x;
                CoordAnimale[1]= a->pos.y - a2->pos.y;
                
                    ang=CoordAnimale.get_angle();
                    if (ang>M_PI) ang=ang-2*M_PI;
                    if (ang<-M_PI)ang=ang+2*M_PI;
                    
                    
                    diff1=(a->angolo + a->AngoloVmin)-ang;
                    diff2=(a->angolo - a->AngoloVmin)-ang;
                    
                    if (diff1>M_PI)      diff1=diff1-2*M_PI;
                    if (diff1<=-M_PI)    diff1=diff1+2*M_PI;
                    
                    if (diff2>M_PI)      diff2=diff2-2*M_PI;
                    if (diff2<=-M_PI)    diff2=diff2+2*M_PI;
                    
                    if (abs(diff1)<a->AngoloV) {
                        if (diff1>=0 && d<dmindx) {         //dmindx è la distanza minore tra il diavolo e un diavolo da vedere con l'occhio destro
                            dmindx=d;
                            rdiav=(a->DdiavV-d)/a->DdiavV;
                            a->DiavDx=add; //segno il diavolo che viene visto
                        }
                    }
                    
                    if (abs(diff2)<a->AngoloV) {
                        if (diff2<0 && d<dminsx) {
                            dminsx=d;
                            ldiav=(a->DdiavV-d)/a->DdiavV;
                            a->DiavSx=add; //segno il diavolo che viene visto
                        }
                    }
                }
            }
        }
            
		}
        
        if(!muove) a->in[idDIAOD]=tanh(odorediav);
		a->in[idDIAVID]= cap(rdiav);
		a->in[idDIAVIS]= cap(ldiav);
        
		a->in[idBITTEN]=float(a->bitten)/2;		//bitten Ë uno se Ë stato morso e zero altrimenti
        a->in[idBITER]=float(a->biter)/3;
        }
    Debug();
}


void cervellosTick()
{
	for(int i=0;i<Diavoli.size();i++){
		Diavoli[i].brain.SetInp(Diavoli[i].in);
		Diavoli[i].out= Diavoli[i].brain.Aggiorna();
	}
}






