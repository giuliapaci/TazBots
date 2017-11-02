//  gen_alg.cpp
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#include "gen_alg.h"



int TOPTAZ= int(float (NUMTAZ) * 0.1);      //n° diavoli di riprodurre
int TOPTOPTAZ=int (float(TOPTAZ) * 0.25);   //n° diavoli da clonare
const int NUMGIRI=10000;
vector<Diavolo> DiaSort;


bool compara_bravura (Diavolo i,Diavolo j) { return (i.bravura>j.bravura); }

int roulette(int max){              //seleziona un intero tra 0 e max-1, con una distribuzione probabilità proporzionale alla bravura
    float sommaFitness = 0.0f;
    for (int i=1; i < max+1; i++){
        sommaFitness += pow(float(i),-3.0/4.0);
    }
    
    // Get a floating point number in the interval 0.0 ... sumFitness**
    float randomNumber = randf(0.0,sommaFitness);
    
    // Translate this number to the corresponding member**
    int indice=0;
    float partialSum=0.0f;
    
    while (randomNumber > partialSum && partialSum<=sommaFitness)
    {
        partialSum += pow(float(indice+1),-3.0/4.0);
        indice++;
    }
    
    return indice;
}




void newgen(int gen, int numgen){
    modcounter=0;
    DiaSort.clear();
    DiaSort = Diavoli;                                              //DiaSort è copia di Diavoli
    Diavoli.clear();                                                //pulisce Diavoli
    sort (DiaSort.begin(), DiaSort.end(), compara_bravura);         //Ordino DiaSort
    
    if(gen%(SaveRate)==0){
        SalvaCervelli(gen, DiaSort);             //SALVA I CERVELLI OGNI SaveRate GENERAZIONI
    };
    
    SalvaMutrate(DiaSort);
    
    ofstream fileout;
    char nomefile[32];
    
    time_t t=time(NULL);
    tm *time;
    time=localtime(&t);
    
    snprintf(nomefile, sizeof(char) * 32, "Albero genealogico.txt");
	fileout.open(nomefile, ios::out | ios::app);
    
    if (gen==0)
    {fileout << "gen"<< '\t'<< "figlio"<< '\t' <<"genitore 1"<<'\t'<<"genitore 2"  <<endl;    //scrive intestazione
    }
	if (gen>0)
    {
        for (int D=0; D<TOPTAZ; D++)
        {
            fileout <<  gen << '\t' << D << '\t'  << DiaSort[D].genitoreA <<'\t'<<  DiaSort[D].genitoreB << endl;
        }
    }
    
	fileout.close();
    
    
    float bravura_media=0, bravura_bravi=0;
    
    for(int i=0; i <DiaSort.size(); i++)
    {
        bravura_media+=DiaSort[i].bravura;
        if (i<TOPTAZ) {
            bravura_bravi+=DiaSort[i].bravura;
        }
        DiaSort[i].genitoreA=i;
        DiaSort[i].genitoreB=i;
    }
    
    bravura_media=bravura_media/DiaSort.size();
    bravura_bravi=bravura_bravi/TOPTAZ;
    
    snprintf(nomefile, sizeof(char) * 32, "Risultati finali.txt");
	fileout.open(nomefile, ios::out | ios::app);
    
    if (gen==0)
    {fileout << "gen"<< '\t'<< "numero diavoli sopravvissuti"<< '\t' <<"bravura 1"<<'\t'<<"bravura 2" << '\t'<< "bravura ultimo"<< '\t'<< "bravura bravi"<< '\t' << "Bravura media"<< '\t' <<"Registrato il: "<< time->tm_year + 1900 <<"/"<< time->tm_mon +1 <<"/"<< time->tm_mday<<" alle ore: " <<time->tm_hour <<":"<< time->tm_min <<endl;    //scrive intestazione
    }
    float perc=(INITFOODfin-INITFOODin)/float(numgen)*gen+INITFOODin;
	
    fileout <<  gen << '\t' <<DiaSort.size()<< '\t'  << DiaSort[0].bravura/(perc*NUMGIRI*(BRAV-1)) <<'\t'<<  DiaSort[1].bravura/(perc*NUMGIRI*(BRAV-1)) <<'\t'<< DiaSort[DiaSort.size()-1].bravura/(perc*NUMGIRI*(BRAV-1)) <<'\t'<< bravura_bravi/(perc*NUMGIRI*(BRAV-1)) << '\t' << bravura_media/(perc*NUMGIRI*(BRAV-1)) <<'\t'<< endl;
    
	fileout.close();
    
    
    snprintf(nomefile, sizeof(char) * 32, "Evoluzione_DNA.txt");
	fileout.open(nomefile, ios::out | ios::app);
    if (gen==0)
    {fileout << "gen" << '\t'<< "Odore cibo"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t' << "Vista cibo"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Angolo vista"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Sovrapposizione angolo"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Odore diavoli"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Vista diavoli"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Aggressività"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t' <<  "Aggressività soglia"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Boost soglia"<< '\t' << '\t' << '\t' <<'\t'<< '\t' <<'\t'  << "Registrato il: "<< time->tm_year + 1900 <<"/"<< time->tm_mon +1 <<"/"<< time->tm_mday<<" alle ore: " <<time->tm_hour <<":"<< time->tm_min <<endl;    //scrive intestazione
        
        fileout << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu" << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu" << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu" << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu"  << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu"  << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu"  << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu"  << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu"  << '\t' << "0" << '\t' << "1" << '\t' << "2" << '\t' << "3" << '\t' << "u" << '\t' << "pu" << '\t' << endl;
    }
    
    fileout << gen << '\t' << DiaSort[0].Dcibo << '\t' << DiaSort[1].Dcibo << '\t' << DiaSort[2].Dcibo << '\t' << DiaSort[3].Dcibo << '\t' << DiaSort[(DiaSort.size()-1)].Dcibo << '\t' << DiaSort[(DiaSort.size()-2)].Dcibo << '\t' << DiaSort[0].DciboV << '\t' << DiaSort[1].DciboV << '\t' << DiaSort[2].DciboV << '\t' << DiaSort[3].DciboV << '\t' << DiaSort[(DiaSort.size()-1)].DciboV << '\t' << DiaSort[(DiaSort.size()-2)].DciboV << '\t' << DiaSort[0].AngoloV << '\t' << DiaSort[1].AngoloV << '\t' << DiaSort[2].AngoloV << '\t' << DiaSort[3].AngoloV << '\t' << DiaSort[(DiaSort.size()-1)].AngoloV << '\t' << DiaSort[(DiaSort.size()-2)].AngoloV << '\t' << DiaSort[0].AngoloVmin << '\t' << DiaSort[1].AngoloVmin << '\t' << DiaSort[2].AngoloVmin << '\t' << DiaSort[3].AngoloVmin << '\t' << DiaSort[(DiaSort.size()-1)].AngoloVmin << '\t' << DiaSort[(DiaSort.size()-2)].AngoloVmin << '\t' << DiaSort[0].Ddiav << '\t' << DiaSort[1].Ddiav << '\t' << DiaSort[2].Ddiav << '\t' << DiaSort[3].Ddiav << '\t' << DiaSort[(DiaSort.size()-1)].Ddiav << '\t' << DiaSort[(DiaSort.size()-2)].Ddiav << '\t' << DiaSort[0].DdiavV << '\t' << DiaSort[1].DdiavV << '\t' << DiaSort[2].DdiavV << '\t' << DiaSort[3].DdiavV << '\t' << DiaSort[(DiaSort.size()-1)].DdiavV << '\t' << DiaSort[(DiaSort.size()-2)].DdiavV << '\t' << DiaSort[0].aggress << '\t' << DiaSort[1].aggress << '\t' << DiaSort[2].aggress << '\t' << DiaSort[3].aggress << '\t' << DiaSort[(DiaSort.size()-1)].aggress << '\t' << DiaSort[(DiaSort.size()-2)].aggress << '\t' << DiaSort[0].morso_soglia << '\t' << DiaSort[1].morso_soglia << '\t' << DiaSort[2].morso_soglia << '\t' << DiaSort[3].morso_soglia << '\t' << DiaSort[(DiaSort.size()-1)].morso_soglia << '\t' << DiaSort[(DiaSort.size()-2)].morso_soglia << '\t' << DiaSort[0].boost_soglia << '\t' << DiaSort[1].boost_soglia << '\t' << DiaSort[2].boost_soglia << '\t' << DiaSort[3].boost_soglia << '\t' << DiaSort[(DiaSort.size()-1)].boost_soglia << '\t' << DiaSort[(DiaSort.size()-2)].boost_soglia << '\t' << endl;
    
    fileout.close();
    
    //// clonazione ////
    for (int i=0; i<3;i++){                 // 3 cloni dei primi due
        Diavoli.push_back(DiaSort[0]);
        Diavoli.push_back(DiaSort[1]);
    }
    
    for (int i=3; i< TOPTOPTAZ; i++){
        Diavoli.push_back(DiaSort[i]);      // 2 clone degli altri
        Diavoli.push_back(DiaSort[i]);
    }
    
    for (int i=0; i<Diavoli.size();i++){    //randomizziamo posizione e angolo dei cloni
        float y;
		y=randf(0,HEIGHT);
		float x;
		x=randf(0,WIDTH);
        
        Diavoli[i].pos= Vector2f(x,y);
		Diavoli[i].angolo= randf(-M_PI,M_PI);
		Diavoli[i].salute= 5.0+randf(0,1);
		Diavoli[i].bravura=0;
		Diavoli[i].sesso=(randf(0,1)>0.5); //assegnazione random 50-50 del sesso
		Diavoli[i].bitten=0;
		Diavoli[i].biter=0;
        Diavoli[i].morsopassato=0;
        Diavoli[i].aggress=randf(0,1);
		Diavoli[i].age=0;
		Diavoli[i].w1=0;
		Diavoli[i].w2=0;
		Diavoli[i].bit=0;
		Diavoli[i].clockf1= randf(5,100);
		Diavoli[i].clockf2= randf(5,100);
		Diavoli[i].indicator=0;
		Diavoli[i].gencount=gen;
		Diavoli[i].selectflag=0;
		Diavoli[i].ir=0;Diavoli[i].ig=0;Diavoli[i].ib=0;
		Diavoli[i].repcounter= randf(REPRATE-0.1,REPRATE+0.1)+randf(REPRATE-0.1,REPRATE+0.1);
		Diavoli[i].id=i;
        
    }
    
    //// crossover ////
    while (Diavoli.size()<NUMTAZ){          //while per riempire tutto il vector
        int i1, i2;
        i1= roulette(TOPTAZ);
        i2= i1;
        while (i1==i2) {i2= roulette(TOPTAZ);}; //mi accerto che i1 =/= i2
        Diavolo a;  //diavolo generato
        a.genitoreA=i1;
        a.genitoreB=i2;
        a.gencount=gen;
        a.MUTRATE1=     randf(0,1)<0.5 ? DiaSort[i1].MUTRATE1 : DiaSort[i2].MUTRATE1;
        a.MUTRATE2=     randf(0,1)<0.5 ? DiaSort[i1].MUTRATE2 : DiaSort[i2].MUTRATE2;
        a.MUTRATE_DNA1= randf(0,1)<0.5 ? DiaSort[i1].MUTRATE_DNA1 : DiaSort[i2].MUTRATE_DNA1;
        a.MUTRATE_DNA2= randf(0,1)<0.5 ? DiaSort[i1].MUTRATE_DNA2 : DiaSort[i2].MUTRATE_DNA2;
        a.MUT_DNA_SOGLIA= randf(0,1)<0.5 ? DiaSort[i1].MUT_DNA_SOGLIA : DiaSort[i2].MUT_DNA_SOGLIA;

        //CrossOver Cervello e trasmissione caratteri ereditari
        a.brain=        randf(0,1)<0.5 ?     DiaSort[i1].brain : DiaSort[i2].brain;
        a.morso_soglia= randf(0,1)<0.5 ?     DiaSort[i1].morso_soglia : DiaSort[i2].morso_soglia;
        a.boost_soglia= randf(0,1)<0.5 ?     DiaSort[i1].boost_soglia : DiaSort[i2].boost_soglia;
        a.Dcibo=        randf(0,1)<0.5 ?     DiaSort[i1].Dcibo : DiaSort[i2].Dcibo;
        a.DciboV=       randf(0,1)<0.5 ?     DiaSort[i1].DciboV : DiaSort[i2].DciboV;
        a.Ddiav=        randf(0,1)<0.5 ?     DiaSort[i1].Ddiav : DiaSort[i2].Ddiav;
        a.DdiavV=       randf(0,1)<0.5 ?     DiaSort[i1].DdiavV : DiaSort[i2].DdiavV;
        a.AngoloV=      randf(0,1)<0.5 ?     DiaSort[i1].AngoloV : DiaSort[i2].AngoloV;
        a.AngoloVmin=   randf(0,1)<0.5 ?     DiaSort[i1].AngoloVmin : DiaSort[i2].AngoloVmin;


        
        int cross;
        
        if(fase<4){
                for (int i=0;i<a.brain.colonna[fase-1].net_layers-1;i++)
                {
                    cross=randi(0,a.brain.colonna[fase-1].n_neur_layers[i]);
                    for (int j=0;j<a.brain.colonna[fase-1].n_neur_layers[i];j++)
                    {
                        if (j<cross)
                            a.brain.colonna[fase-1].Layers[i].Neurones[j]=DiaSort[i1].brain.colonna[fase-1].Layers[i].Neurones[j];
                        else
                            a.brain.colonna[fase-1].Layers[i].Neurones[j]=DiaSort[i2].brain.colonna[fase-1].Layers[i].Neurones[j];
                    }
                }
        }
        else{
            for (int k=0; k<a.brain.colonna.size();k++){
                for (int i=0;i<a.brain.colonna[k].net_layers-1;i++)
                {
                    cross=randi(0,a.brain.colonna[k].n_neur_layers[i]);
                    for (int j=0;j<a.brain.colonna[k].n_neur_layers[i];j++)
                    {
                        if (j<cross)
                            a.brain.colonna[k].Layers[i].Neurones[j]=DiaSort[i1].brain.colonna[k].Layers[i].Neurones[j];
                        else
                            a.brain.colonna[k].Layers[i].Neurones[j]=DiaSort[i2].brain.colonna[k].Layers[i].Neurones[j];
                    }
                }
            }
        }


        Diavoli.push_back(a);
    }
    
    //Mutazione Cervello
    for (int k=0; k<Diavoli.size(); k++){
        if(randf(0,1)<0.2) Diavoli[k].MUTRATE1= randn(Diavoli[k].MUTRATE1, METAMUTRATE1);   //cambiamo i MUTRATE
		if(randf(0,1)<0.2) Diavoli[k].MUTRATE2= randn(Diavoli[k].MUTRATE2, METAMUTRATE2);
        if(randf(0,1)<0.2) Diavoli[k].MUTRATE_DNA1= randn(Diavoli[k].MUTRATE_DNA1, METAMUTRATE_DNA1);
        if(randf(0,1)<0.2) Diavoli[k].MUTRATE_DNA2= randn(Diavoli[k].MUTRATE_DNA2, METAMUTRATE_DNA2);
        if(randf(0,1)<0.2) Diavoli[k].MUT_DNA_SOGLIA= randn(Diavoli[k].MUT_DNA_SOGLIA, METAMUT_DNA_SOGLIA);
        if(Diavoli[k].MUTRATE1< 0.0001)     Diavoli[k].MUTRATE1= 0.0001;
        if(Diavoli[k].MUTRATE1>= 1)          Diavoli[k].MUTRATE1= 0.9;
        if(Diavoli[k].MUTRATE2< 0.01)       Diavoli[k].MUTRATE2= 0.01;
        if(Diavoli[k].MUTRATE2>= 1)          Diavoli[k].MUTRATE2= 0.9;
        if(Diavoli[k].MUTRATE_DNA1< 0.01)   Diavoli[k].MUTRATE_DNA1= 0.01;
        if(Diavoli[k].MUTRATE_DNA1>= 1)      Diavoli[k].MUTRATE_DNA1= 0.9;
        if(Diavoli[k].MUTRATE_DNA2< 0.01)    Diavoli[k].MUTRATE_DNA2= 0.01;
        if(Diavoli[k].MUTRATE_DNA2>= 1)       Diavoli[k].MUTRATE_DNA2= 0.9;
        if(Diavoli[k].MUT_DNA_SOGLIA< 0.001)    Diavoli[k].MUT_DNA_SOGLIA= 0.001;
        if(Diavoli[k].MUT_DNA_SOGLIA>= 1)   Diavoli[k].MUT_DNA_SOGLIA=0.99;
        
        
        
        if(fase<4){
        for (int i=0;i<Diavoli[k].brain.colonna[fase-1].net_layers-1;i++)
        {
            for (int j=0;j<Diavoli[k].brain.colonna[fase-1].n_neur_layers[i]+1;j++)
            {
                for (int l=1;l<Diavoli[k].brain.colonna[fase-1].n_neur_layers[i+1]+1;l++)
                {
                    if (randf(0,1)<Diavoli[k].MUTRATE1)
                        Diavoli[k].brain.colonna[fase-1].Layers[i].Neurones[j].Dendrites[l].d_peso+=randf(-Diavoli[k].MUTRATE2,Diavoli[k].MUTRATE2);
                }
            }
        }

        }
        else{
            for(int m=0;m<Diavoli[k].brain.colonna.size();m++){
            for (int i=0;i<Diavoli[k].brain.colonna[m].net_layers-1;i++)
            {
                for (int j=0;j<Diavoli[k].brain.colonna[m].n_neur_layers[i]+1;j++)
                {
                    for (int l=1;l<Diavoli[k].brain.colonna[m].n_neur_layers[i+1]+1;l++)
                    {
                        if (randf(0,1)<Diavoli[k].MUTRATE1)
                            Diavoli[k].brain.colonna[m].Layers[i].Neurones[j].Dendrites[l].d_peso+=randf(-Diavoli[k].MUTRATE2,Diavoli[k].MUTRATE2);
                    }
                }
            }
            }
        }
        
        //Mutazione Caratteri ereditari
        if (fase!=2 && fase!=3){
        if (randf(0,1)<Diavoli[k].MUTRATE_DNA1)        //    !!! era MUTRATE1
            Diavoli[k].AngoloV+=randf(-0.1*Diavoli[k].MUTRATE_DNA2,0.1*Diavoli[k].MUTRATE_DNA2);
            
        
        if (randf(0,1)<Diavoli[k].MUTRATE_DNA1)        //    !!! era MUTRATE1
            Diavoli[k].AngoloVmin+=randf(-0.1*Diavoli[k].MUTRATE_DNA2,0.1*Diavoli[k].MUTRATE_DNA2);
        
        if (randf(0,1)<Diavoli[k].MUTRATE_DNA1){        //    !!! era MUTRATE1
            float add=randf(-1,1);
            if (add>Diavoli[k].MUT_DNA_SOGLIA)             Diavoli[k].Dcibo+=1;
            else if (add<-Diavoli[k].MUT_DNA_SOGLIA)       Diavoli[k].Dcibo+=-1;
            if(Diavoli[k].Dcibo<2)   Diavoli[k].Dcibo=2;
                }
        
        if (randf(0,1)<Diavoli[k].MUTRATE_DNA1){        //    !!! era MUTRATE1
            float add=randf(-1,1);
            if (add>Diavoli[k].MUT_DNA_SOGLIA)             Diavoli[k].DciboV+=1;
            else if (add<-Diavoli[k].MUT_DNA_SOGLIA)       Diavoli[k].DciboV+=-1;
            if(Diavoli[k].DciboV<1)  Diavoli[k].DciboV=1;
                }
        }
        
        if (fase==2) {
            if (randf(0,1)<Diavoli[k].MUTRATE_DNA1)        //    !!! era MUTRATE1
                Diavoli[k].morso_soglia+=randf(-Diavoli[k].MUTRATE_DNA2,Diavoli[k].MUTRATE_DNA2);
            if (Diavoli[k].morso_soglia>1)      Diavoli[k].morso_soglia=1;
            if (Diavoli[k].morso_soglia<(-1))   Diavoli[k].morso_soglia=-1;
        }
        
        
        if(fase>1){
            if (fase!=3){
        if (randf(0,1)<Diavoli[k].MUTRATE_DNA1){        //    !!! era MUTRATE1
            Diavoli[k].Ddiav+=randf(-CZ*Diavoli[k].MUTRATE_DNA2,CZ*Diavoli[k].MUTRATE_DNA2);
        if(Diavoli[k].Ddiav<10) Diavoli[k].Ddiav=15;
        if(Diavoli[k].Ddiav>32) Diavoli[k].Ddiav=30;
            
            
            if (randf(0,1)<Diavoli[k].MUTRATE_DNA1){        //    !!! era MUTRATE1
                Diavoli[k].DdiavV+=randf(-CZ*Diavoli[k].MUTRATE_DNA2,CZ*Diavoli[k].MUTRATE_DNA2);
                
                if(Diavoli[k].DdiavV<10) Diavoli[k].DdiavV=20;
                if(Diavoli[k].DdiavV>110) Diavoli[k].DdiavV=107;
            }

        }
            }
        
            if (fase==3) {
        if (randf(0,1)<Diavoli[k].MUTRATE_DNA1)        //    !!! era MUTRATE1
            Diavoli[k].boost_soglia+=randf(-Diavoli[k].MUTRATE_DNA2,Diavoli[k].MUTRATE_DNA2);
                if(Diavoli[k].boost_soglia<0) Diavoli[k].boost_soglia=0;
                if(Diavoli[k].boost_soglia>1) Diavoli[k].boost_soglia=1;
        }
        }
        
    }
    
    
    
}
