//
//  utilities.cpp
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//


#include "utilities.h"

bool controlla_in=false;
bool controlla_out=false;

vector<float> StampaControllo(int D, int mm, vector<Diavolo> DD){
    vector<float> controlla;
    
    for(int k=0;k<INPUTSIZE;k++)        controlla.push_back(randf(-1, 1));

    DD[D].brain.SetInp(controlla);
    vector<float> out_controllo= DD[D].brain.colonna[mm].GetOutput();
    
    for(int k=0;k<DD[D].brain.colonna[mm].n_neur_layers[2];k++) {
        controlla.push_back(out_controllo[k]);
    }
    
    return controlla;

}

int LeggiControllo(int D, int mm, vector<float> leggimi, vector<Diavolo> DD){
    vector<float> controlla;
    
    for(int k=0;k<INPUTSIZE;k++)        controlla.push_back(leggimi[k]);
    
    DD[D].brain.SetInp(controlla);
    vector<float> out_controllo= DD[D].brain.colonna[mm].GetOutput();
    
    for(int k=0;k<DD[D].brain.colonna[mm].n_neur_layers[2];k++) {
        controlla.push_back(out_controllo[k]);
    }
    
    int ok=0, tot=0, ans=0;
    for (int k=0; k<leggimi.size(); k++) {
        if (abs(leggimi[k]-controlla[k])<0.00001) {
            ok++;
            tot++;
        }
        else{
            cout << "!! ERRORE nel controllo:"<<endl<< "il diavolo " << D << " nella colonna " << mm+1 << " doveva avere " << leggimi[k] << " e invece ha prodotto " << controlla[k] <<endl ;
            tot++;
        }
    }
    ans=(tot-ok);
    return ans;
}

void SalvaCervelli(int gen, vector<Diavolo> Diav)
{
    int f=min(fase, 3);
    for(int mm=0; mm<f; mm++)
    {
    char nomefile[32];
        if (controlla_out) {
                snprintf(nomefile, sizeof(char) * 32, "Cervelli_gen_%i_f_%i_C.txt", gen, mm+1);
        }
        else{
                snprintf(nomefile, sizeof(char) * 32, "Cervelli_gen_%i_f_%i.txt", gen, mm+1);
        }
    ofstream fileout(nomefile);
    
    fileout << Diav[0].brain.colonna[mm].n_neur_layers[0] << endl << Diav[0].brain.colonna[mm].n_neur_layers[1] << endl << Diav[0].brain.colonna[mm].n_neur_layers[2] << endl;
    

    for (int D=0;D<Diav.size();D++)
    {
        fileout << D <<endl;
        for (int i=0;i<Diav[D].brain.colonna[mm].net_layers-1;i++)
        {
            for (int j=0;j<Diav[D].brain.colonna[mm].n_neur_layers[i]+1;j++)
            {
                for (int l=1;l<Diav[D].brain.colonna[mm].n_neur_layers[i+1]+1;l++)
                {
                    fileout << Diav[D].brain.colonna[mm].Layers[i].Neurones[j].Dendrites[l].d_peso<< endl;
                }
            }
        }
    
        
        if(controlla_out){
            vector<float> scrivimi=StampaControllo(D, mm, Diav);
            fileout<<scrivimi.size()<<endl;
            for(int i=0;i<scrivimi.size();i++){
                fileout << scrivimi[i]<<endl;
            }
        }

    }
    fileout.close();
    }
    
    char nomefile[32];
    snprintf(nomefile, sizeof(char) * 32, "Cervelli_gen_%i_dna.txt", gen);
    ofstream fileout(nomefile);
    
    for (int D=0;D<Diav.size();D++)
    {
        fileout << D << endl;
        fileout << Diav[D].Dcibo  <<endl;
        fileout << Diav[D].DciboV <<endl;
        fileout << Diav[D].AngoloV <<endl;
        fileout << Diav[D].AngoloVmin << endl;
        fileout << Diav[D].Ddiav <<endl;
        fileout << Diav[D].DdiavV <<endl;
        fileout << Diav[D].morso_soglia <<endl;
        if(fase>=3) fileout << Diav[D].boost_soglia <<endl;
//      fileout << Diav[D]. <<endl;
    }
    
    fileout.close();
    
    cout << endl << " - Salvati i cervelli della generazione "<< gen << " -"<< endl;
    
}




void LoadCervelli()
{
    int risp;
    char nomefilet[200];
    char nomefile[200];
    cout<< "Vuoi utilizzare dei cervelli in memoria? (0=no, 1=si')"<< endl;
    cin>> risp;
    if (risp==1)
    {
        cout<< "Inserire il nome del file senza l'estensione"<< endl;
        cin>> nomefilet;
        int old=0;
        int errori=0;
        
        if(mode==1){
            if(fase==1){
                cout<<"Sei in fase = 1, sei sicuro di voler continuare a caricare i cervelli? (0=no, 1=si')"<<endl;
            }
            if(fase>1 && fase<4){
                cout<<"Vuoi iniziare una evoluzione da zero per la colonna " << min(fase,3)<< " o vuoi partire con quella colonna già parzialmente evoluta (0= zero, 1= evoluta)"<<endl;
            cin>> risp;
            if(risp==1){
                old=1;
            }
            
            if(old==0) cout<<"L'evoluzione partirà da zero e se sei in fase 1 non verranno caricati cervelli."<<endl;
            }
            
        
        int f= min(fase+old,4);
        for(int mm=0;mm<f-1;mm++)
        {
            
            if(controlla_in){
                snprintf(nomefile, sizeof(char) * 52, "%s_f_%i_C.txt", nomefilet, mm+1);
                
            }
            else{
                snprintf(nomefile, sizeof(char) * 52, "%s_f_%i.txt", nomefilet, mm+1);
            }
            
        ifstream filein(nomefile);
        
        if(!filein) {
            cout<<"Il file "<< nomefile<< " della colonna "<<mm+1<<" non esiste!"<<endl;
        }
            
        if(filein){
            char causa[52];
            int D=0;

                int in, hi, ou;
            filein >> in;
            filein >> hi;
            filein >> ou;
                
            char struttura1[152];
            char struttura2[152];
            char struttura3[152];
            
            snprintf(struttura1, sizeof(char) * 152, "Struttura:    input   %i", in);
            snprintf(struttura2, sizeof(char) * 152, "              hidden  %i", hi);
            snprintf(struttura3, sizeof(char) * 152, "              output  %i", ou);
            
            if (in!=input[mm]){
                snprintf(struttura1, sizeof(char) * 152, "Problema: il numero di input del file è %i, mentre la rete ne ha %i", in, input[mm]);
            }
            if (hi!=hidden[mm]){
                snprintf(struttura2, sizeof(char) * 152, "Problema: il numero di hidden del file è %i, mentre la rete ne ha %i", hi, hidden[mm]);
            }
            if (ou!=output[mm]){
                snprintf(struttura3, sizeof(char) * 152, "Problema: il numero di output del file è %i, mentre la rete ne ha %i", ou, output[mm]);
            }
            
            cout<<"Colonna "<<mm+1<<endl<<struttura1<<endl <<struttura2<<endl <<struttura3<<endl;
            
            

            if (in==input[mm] && hi==hidden[mm] && ou==output[mm]){
                
                
                while(D<Diavoli.size()){
                    int ID=-3;
                    filein>> ID;
                    
                    if(!filein.good())
                    {
                        filein.clear();
                        filein.seekg(0, ios::beg);
                        
                        filein >> in;
                        filein >> hi;
                        filein >> ou;
                        filein >> ID;
                    }
                    
                    
                    if (mm==0) {
                        Diavoli[D].id=ID; }
                    else{
                        if (ID!=Diavoli[D].id){
                            cout << "Ho un problema: non sto scrivendo le colonne giuste nei diavoli! Nel file c'e' " << ID << " mentre il diavolo e' il " << Diavoli[D].id <<endl;
                        }
                    }
                    
                    for (int i=0;i<Diavoli[D].brain.colonna[mm].net_layers-1;i++)
                    {
                        for (int j=0;j<Diavoli[D].brain.colonna[mm].n_neur_layers[i]+1;j++)
                        {
                            for (int l=1;l<Diavoli[D].brain.colonna[mm].n_neur_layers[i+1]+1;l++)
                            {
                                filein >> Diavoli[D].brain.colonna[mm].Layers[i].Neurones[j].Dendrites[l].d_peso;
                            }
                        }
                    }
                    
                    
                    if(controlla_in){
                        int dim;
                        filein >> dim;
                        vector<float> leggimi;
                        for(int i=0;i<dim;i++){
                            float temp;
                            filein >> temp;
                            leggimi.push_back(temp);
                        }
                        int temp;
                        temp=LeggiControllo(D, mm, leggimi, Diavoli);
                        errori+=temp;
                    }
                    
                    
                    D++;

                    }
                
            }
            
            
            if(D>=Diavoli.size()){
                snprintf(causa, sizeof(char) * 52, " perché ho finito i diavoli.");
            }
            else{
                snprintf(causa, sizeof(char) * 52, " non so perché.");
            }
            cout<< "Per la colonna " << mm+1<< " ho utilizzato i cervelli salvati per i primi "<< D <<" diavoli su " << Diavoli.size()<< causa <<endl;
        }
        filein.close();
            
        }
            
            cout<<"Nel controllo ho trovato " << errori << " errori"<<endl;
        }
        
        else if (mode==0){
            int f=min(fase, 3);
            for(int mm=0;mm<f;mm++)
            {
                if(controlla_in){
                    snprintf(nomefile, sizeof(char) * 52, "%s_f_%i_C.txt", nomefilet, mm+1);
                }
                else{
                    snprintf(nomefile, sizeof(char) * 52, "%s_f_%i.txt", nomefilet, mm+1);
                }
                
                ifstream filein(nomefile);
                
                
                
                if(!filein) {
                    cout<<"Il file " << nomefile<< " della colonna "<<mm+1<<" non esiste!"<<endl;
                }
                
                if(filein){
                    
                    char causa[52];
                    int D=0;
                    
                    int in, hi, ou;
                    filein >> in;
                    filein >> hi;
                    filein >> ou;
                    
                    char struttura1[152];
                    char struttura2[152];
                    char struttura3[152];
                    
                    snprintf(struttura1, sizeof(char) * 152, "Struttura:    input   %i", in);
                    snprintf(struttura2, sizeof(char) * 152, "              hidden  %i", hi);
                    snprintf(struttura3, sizeof(char) * 152, "              output  %i", ou);
                    
                    if (in!=input[mm]){
                        snprintf(struttura1, sizeof(char) * 152, "Problema: il numero di input del file è %i, mentre la rete ne ha %i", in, input[mm]);
                    }
                    if (hi!=hidden[mm]){
                        snprintf(struttura2, sizeof(char) * 152, "Problema: il numero di hidden del file è %i, mentre la rete ne ha %i", hi, hidden[mm]);
                    }
                    if (ou!=output[mm]){
                        snprintf(struttura3, sizeof(char) * 152, "Problema: il numero di output del file è %i, mentre la rete ne ha %i", ou, output[mm]);
                    }
                    
                    cout<<"Colonna "<<mm+1<<endl<<struttura1<<endl <<struttura2<<endl <<struttura3<<endl;
                    
                    
                    
                    if (in==input[mm] && hi==hidden[mm] && ou==output[mm]){
                        
                        
                        while(D<Diavoli.size()){
                            int ID=-3;
                            filein>> ID;
                            
                            if(!filein.good())
                            {
                                filein.clear();
                                filein.seekg(0, ios::beg);
                                
                                filein >> in;
                                filein >> hi;
                                filein >> ou;
                                filein >> ID;
                            }
                            
                            if (mm==0) {
                                Diavoli[D].id=ID; }
                            else{
                                if (ID!=Diavoli[D].id){
                                    cout << "Ho un problema: non sto scrivendo le colonne giuste nei diavoli! Nel file c'e' " << ID << " mentre il diavolo e' il " << Diavoli[D].id <<endl;
                                }
                            }
                            
                            
                            for (int i=0;i<Diavoli[D].brain.colonna[mm].net_layers-1;i++)
                            {
                                for (int j=0;j<Diavoli[D].brain.colonna[mm].n_neur_layers[i]+1;j++)
                                {
                                    for (int l=1;l<Diavoli[D].brain.colonna[mm].n_neur_layers[i+1]+1;l++)
                                    {
                                        filein >> Diavoli[D].brain.colonna[mm].Layers[i].Neurones[j].Dendrites[l].d_peso;
                                    }
                                }
                            }
                            
                        
                            if(controlla_in){
                                int dim, temp;
                                filein >> dim;
                                vector<float> leggimi;
                                for(int i=0;i<dim;i++){
                                    float temp;
                                    filein >> temp;
                                    leggimi.push_back(temp);
                                }
                                temp=LeggiControllo(D, mm, leggimi, Diavoli);
                                errori+=temp;
                            }
                            
                            D++;

                        }
                    }
                    
                    
                    if(D>=Diavoli.size()){
                        snprintf(causa, sizeof(char) * 52, " perché ho finito i diavoli.");
                    }
                    else{
                        snprintf(causa, sizeof(char) * 52, " non so perché.");
                    }
                    cout<< "Per la colonna " << mm+1<< " ho utilizzato i cervelli salvati per i primi "<< D <<" diavoli su " << Diavoli.size()<< causa <<endl;
                }
                filein.close();
                
            }
        
            cout<<"Nel controllo ho trovato " << errori << " errori"<<endl;
        }
        
        
        char nomefile[52];
        snprintf(nomefile, sizeof(char) * 52, "%s_dna.txt", nomefilet);
        ifstream fileinput(nomefile);
        
        if (!fileinput) {
            cout<< "Manca il file " <<nomefile <<" del DNA, non verranno caricati aggressività e altri parametri ereditari" <<endl;
        }
        
        if(fileinput){
            int D=0;
        while (D<Diavoli.size())
        {
            int ID=-3;
            float cancella;
            fileinput >> ID ;
            
            if(!fileinput.good())
            {
                fileinput.clear();
                fileinput.seekg(0, ios::beg);
                fileinput >> ID ;
            }
            
        
            
            
            if (ID!=Diavoli[D].id) {
                cout << "Non sto caricando il DNA al diavolo con l'ID corretto: nel file c'e' " << ID<< " mentre il diavolo è il "<< Diavoli[D].id <<endl;            }
            else {
            fileinput >> Diavoli[D].Dcibo ;
            fileinput >> Diavoli[D].DciboV ;
            fileinput >> Diavoli[D].AngoloV ;
            fileinput >> Diavoli[D].AngoloVmin;
            
            if (fase==2 && (old==1 || mode==0)) {
                fileinput >> Diavoli[D].Ddiav ;
                fileinput >> Diavoli[D].DdiavV ;
                fileinput >> Diavoli[D].morso_soglia ;
            }
            else if (fase<=2) {
                fileinput >> cancella;
                fileinput >> cancella;
                fileinput >> cancella;
            }
            else if ((fase>3) || (fase==3 && (old==1 || mode==0))){
                fileinput >> Diavoli[D].Ddiav ;
                fileinput >> Diavoli[D].DdiavV ;
                fileinput >> Diavoli[D].morso_soglia ;
                fileinput >> Diavoli[D].boost_soglia;
            }
            else if (fase==3){
                fileinput >> Diavoli[D].Ddiav ;
                fileinput >> Diavoli[D].DdiavV ;
                fileinput >> Diavoli[D].morso_soglia ;
            }

            D++;
                
            }
            
        }

        }
        

    
    
        fileinput.close();
    
    }
}

void SalvaMutrate(vector<Diavolo> Diav){
    char nomefile[32];
    snprintf(nomefile, sizeof(char) * 32, "Mutrate.txt");
    ofstream fileout(nomefile);
    float agg1=0, agg2=0, agg3=0, agg4=0, agg5=0;
    
    for (int i=0; i<Diav.size(); i++) {
        agg1+=Diav[i].MUTRATE_DNA1;
        agg2+=Diav[i].MUTRATE_DNA2;
        agg3+=Diav[i].MUT_DNA_SOGLIA;
        agg4+=Diav[i].MUTRATE1;
        agg5+=Diav[i].MUTRATE2;
    }
    
    agg1=agg1/Diav.size();
    agg2=agg2/Diav.size();
    agg3=agg3/Diav.size();
    agg4=agg2/Diav.size();
    agg5=agg3/Diav.size();
   
    fileout <<"Il MUTRATE1 dei diavoli in media è: " << agg4<<endl;
    fileout <<"Il MUTRATE2 dei diavoli in media è: " << agg5<<endl;
    fileout <<"Il mutrate_DNA1 dei diavoli in media è: "<< agg1<<endl;
    fileout <<"Il mutrate_DNA2 dei diavoli in media è: "<< agg2<<endl;
    fileout <<"Il mut_DNA_soglia dei diavoli in media è: "<< agg3<<endl;
    
    for (int i=0; i<Diav.size(); i++) {
        fileout << i <<endl;
        fileout << Diav[i].MUTRATE1 <<endl;
        fileout << Diav[i].MUTRATE2 <<endl;
        fileout << Diav[i].MUTRATE_DNA1 << endl ;
        fileout << Diav[i].MUTRATE_DNA2 << endl ;
        fileout << Diav[i].MUT_DNA_SOGLIA << endl <<endl ;
    }
    fileout.close();

}

