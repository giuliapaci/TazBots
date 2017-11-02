//
//  grafica.cpp
//  TazBot
//
//  Copyright (c) 2014 TazBot. All rights reserved.
//

#include "grafica.h"


//user interaction
bool paused=false;
bool draw=true;
//for drawing
char buf2[10];
int skipdraw=1;
bool drawFood=true;
int CLOSED=0; //if environment is closed, then no random bots are added per time interval
const int WWIDTH = 1880; //dimensioni finestra
const int WHEIGHT = 880;
char buf[100];


void processNormalKeys(unsigned char key, int x, int y) {
    
	if (key == 27)
		exit(0);
	else if (key=='r' || key=='R') {
		Diavoli.clear();
		addRandomBots(NUMTAZ);
		printf("Diavoli reset\n");
	} else if (key=='p' || key=='P') {
		//pause
		paused= !paused;
	} else if (key=='d' || key=='D') {
		//drawing
		draw= !draw;
//	} else if (key=='s' || key=='S') {
//		//salva cervelli
//		SalvaCervelli();
	} else if(key==43){
		//+
		skipdraw++;
        
	} else if(key==45){
		//-
		skipdraw--;
	} else if(key=='c' || key=='C'){
		CLOSED= 1-CLOSED;
		printf("Environemt closed now= %i\n",CLOSED==0?0:1);
	} else {
		printf("Unknown key pressed: %i\n", key);
	}
	
}




void processMouse(int button, int state, int x, int y) {
	if(state==0 && button==GLUT_LEFT_BUTTON){
//		cout << x << " " << y << endl;
        float mind=1e10;
		float mini=-1;
		float d;
		for(int i=0;i<Diavoli.size();i++){
			d= pow(x-Diavoli[i].pos.x,2)+pow(y-Diavoli[i].pos.y,2);
			if (d<mind){
				mind=d;
				mini=i;
			}
		}
		//toggle selection of this Diavolo
		for(int i=0;i<Diavoli.size();i++) Diavoli[i].selectflag=false;
        if (Diavoli[mini].selectflag== true) 		Diavoli[mini].selectflag= false;
		else Diavoli[mini].selectflag= true;
//        cout << mini << " " << mind << endl;
		//Diavoli[mini].printSelf();
	}
    
//////riproduzione con clic destro//////////
//	if(button==GLUT_RIGHT_BUTTON && state==1){
//		float mind=1e10;
//		float mini=-1;
//		float d;
//		for(int i=0;i<Diavoli.size();i++){
//			d= pow(x-Diavoli[i].pos.x,2)+pow(y-Diavoli[i].pos.y,2);
//			if (d<mind){
//				mind=d;
//				mini=i;
//			}
//		}
//        
//		reproduce(mini, Diavoli[mini].MUTRATE1, Diavoli[mini].MUTRATE2); //si riproduce il diavolo selezionato con click destro
////////////////////////////////////////
    
        
	}

void processMouseActiveMotion(int x, int y) {
}


void changeSize(int w, int h) {
    
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,WWIDTH,WHEIGHT,0,0,1);
}



void RenderString(float x, float y, void *font, const char* string, float r, float g, float b)
{
    glColor3f(r,g,b);
    glRasterPos2f(x, y);
    int len = (int) strlen(string);
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
}

void drawCircle(float x, float y, float r){
	float n;
	for (int k=0;k<17;k++){
		n = k*(M_PI/8);
		glVertex3f(x+r*sin(n),y+r*cos(n),0);
	}
}

void drawArc(float x, float y, float r, float theta1, float theta2){
    glBegin(GL_LINE_STRIP);
    int num_segments=14;
    for(int ii = 0; ii <= num_segments; ii++)
    {
        float theta = abs(theta2) * float(ii) / float(num_segments);//get the current angle
        
        float cx = r * cos(theta+theta1);//calculate the x component
        float cy = r * sin(theta+theta1);//calculate the y component
        
        glVertex2f(x + cx, y - cy);//output vertex
    }
    glEnd();
}

void drawDiavolo(float x, float y, float r){
    //glClearColor(1, 1, 1, 1); //bianco
    //glClear(GL_COLOR_BUFFER_BIT);
	//glutSwapBuffers();
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);
	
    //NUOVI COMANDI PER CARICARE IMMAGINE PNG
    const char* filename= "./TAZ.png";
    std::vector<unsigned char> image;
    unsigned  width,height;
    unsigned error=lodepng::decode(image, width, height, filename);
    if(error != 0)
    {
//        std::cout<<"error"<<error<<":"<<lodepng_error_text(error)<<std::endl;
    }
    
    
    
	//tex = SOIL_load_OGL_texture("TAZ.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_TEXTURE_REPEATS);
    
	//glBindTexture(GL_TEXTURE_2D, tex);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//set our filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	//set our filter
    
    glTexImage2D(GL_TEXTURE_2D,0,4,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, &image[0]);
    
    //disegno rettangolo
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    float rr=r;
    glTexCoord2f(0.0,0.0);
    glVertex2f(x-rr,y-rr);
    glTexCoord2f(1.0,0.0);
    glVertex2f(x+rr, y-rr);
    glTexCoord2f(1.0,1.0);
    glVertex2f(x+rr,y+rr);
    glTexCoord2f(0.0,1.0);
    glVertex2f(x-rr,y+rr);
    glEnd();
    
	//glutSwapBuffers();
    glFlush(); //draw on screen
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
    
}




void renderScene() {
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glPushMatrix();
    
	//draw food
	if(drawFood){
		glBegin(GL_QUADS);
		for(int i=0;i<FW;i++){
			for(int j=0;j<FH;j++){
				float f= 0.4*food[i][j]/FOODMAX;
				glColor3f(0+f,0.6-f,0);					//colore che sfuma da marrone a verde
				glVertex3f(i*CZ,j*CZ,0);
				glVertex3f(i*CZ+CZ,j*CZ,0);
				glVertex3f(i*CZ+CZ,j*CZ+CZ,0);
				glVertex3f(i*CZ,j*CZ+CZ,0);
			}
		}
		glEnd();
	}
    
	float n;
	float r= TAZRAD;
    
    //cibo che vede! Aggiunto da me!!
    for (int i =0; i<Diavoli.size(); i++) {
        if(Diavoli[i].selectflag>0 && mostra_vista_cibo){
            glBegin(GL_QUADS);
            float f;
            for(int m=-Diavoli[i].DciboV;m<=Diavoli[i].DciboV;m++){
                for(int j=-Diavoli[i].DciboV;j<=Diavoli[i].DciboV;j++){
                    int x=m+Diavoli[i].DciboV;
                    int y=j+Diavoli[i].DciboV;
                    
                    if(x<0) x=x+WIDTH;
                    if(x>=WIDTH) x=x-WIDTH;
                    if(y<0) y=y+HEIGHT;
                    if(y>=HEIGHT) y=y-HEIGHT;
                    
                    f= 0.5*Diavoli[i].cibo[x][y];
                    if (f>0) glColor3f(1-f,0.2,0.5+f);					//colore che sfuma da marrone a viola
                    if (f<0) glColor3f(0.5+f, 0.2, 0.5-f);              //colore che sfuma da marrone a blu
                    
                    if (f!=0){
                    glVertex3f(m*CZ+int(Diavoli[i].pos.x/CZ)*CZ,j*CZ+int(Diavoli[i].pos.y/CZ)*CZ,0);
                    glVertex3f(m*CZ+CZ+int(Diavoli[i].pos.x/CZ)*CZ,j*CZ+int(Diavoli[i].pos.y/CZ)*CZ,0);
                    glVertex3f(m*CZ+CZ+int(Diavoli[i].pos.x/CZ)*CZ,j*CZ+CZ+int(Diavoli[i].pos.y/CZ)*CZ,0);
                    glVertex3f(m*CZ+int(Diavoli[i].pos.x/CZ)*CZ,j*CZ+CZ+int(Diavoli[i].pos.y/CZ)*CZ,0);
                    }
                }
            }
            glEnd();
        }
    }
    
	//draw every Diavolo
	for(int i=0;i<Diavoli.size();i++){
        
		//handle selected Diavolo
		if(Diavoli[i].selectflag>0){
			
			//draw selection
			glBegin(GL_POLYGON);
			glColor3f(1,1,1);
			drawCircle(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD+5);
			//drawDiavolo(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD+5);
			glEnd();
            
            
            //Segnaliamo anche i due diavoli che vede
            if(mostra_vista_diavoli){
                if(Diavoli[i].DiavDx>=0){
                    float f;
                    f=Diavoli[i].in[idDIAVID];
                    glBegin(GL_POLYGON);
                    glColor3f(1-f,0.2,0.5+f);
                    drawCircle(Diavoli[Diavoli[i].DiavDx].pos.x, Diavoli[Diavoli[i].DiavDx].pos.y, TAZRAD+5);
                    //drawDiavolo(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD+5);
                    glEnd();
                }
                if(Diavoli[i].DiavSx>=0){
                    float f;
                    f=Diavoli[i].in[idDIAVID];
                    glBegin(GL_POLYGON);
                    glColor3f(0.5-f, 0.2, 0.5+f);
                    drawCircle(Diavoli[Diavoli[i].DiavSx].pos.x, Diavoli[Diavoli[i].DiavSx].pos.y, TAZRAD+5);
                    //drawDiavolo(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD+5);
                    glEnd();
                }
            }
			
			glPushMatrix();
			glTranslatef(Diavoli[i].pos.x-80,Diavoli[i].pos.y+20,0);
			//draw inputs, outputs
			float col;
			float yy=15;
			float xx=15;
			float ss=16;
			glBegin(GL_QUADS);
			for(int j=0;j<INPUTSIZE;j++){
				col= Diavoli[i].in[j];
				if (col<0)
                    glColor3f(1-abs(col),1-abs(col),1);
				else
                    glColor3f(1,1-abs(col),1-abs(col));
                
				glVertex3f(0+ss*j, 0, 0.0f);
				glVertex3f(xx+ss*j, 0, 0.0f);
				glVertex3f(xx+ss*j, yy, 0.0f);
				glVertex3f(0+ss*j, yy, 0.0f);
			}
			yy+=5;
			for(int j=0;j<OUTPUTSIZE;j++){
                if (j<=fase){
				col= Diavoli[i].out[j];
				if (col<0)
                    glColor3f(1-abs(col),1-abs(col),1);
				else
                    glColor3f(1,1-abs(col),1-abs(col));
                }
                else glColor3f(1,1,1);
                
				glVertex3f(0+ss*j, yy, 0.0f);
				glVertex3f(xx+ss*j, yy, 0.0f);
				glVertex3f(xx+ss*j, yy+ss, 0.0f);
				glVertex3f(0+ss*j, yy+ss, 0.0f);
			}
			yy+=ss*2;
            
            
			glEnd();
			glPopMatrix();
		}
        
		//draw indicator of this Diavolo... used for various events
		if(Diavoli[i].indicator>0){
			glBegin(GL_POLYGON);
            glColor3f(Diavoli[i].ir,Diavoli[i].ig,Diavoli[i].ib);
            drawCircle(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD+((int)Diavoli[i].indicator));
			glEnd();
			Diavoli[i].indicator-=1;
		}
        
//		//viewcone of this Diavolo
//		glBegin(GL_LINES);
//        //and view cones
//        glColor3f(0,0,0);
        
        
        for(int j=-1;j<2;j=j+2){
            glBegin(GL_LINES);
                    glColor3f(0,0,0);
            glVertex3f(Diavoli[i].pos.x,Diavoli[i].pos.y,0);
            glVertex3f(Diavoli[i].pos.x+3*Diavoli[i].DciboV*cos(Diavoli[i].angolo+j*Diavoli[i].AngoloV),Diavoli[i].pos.y-3*Diavoli[i].DciboV*sin(Diavoli[i].angolo+j*Diavoli[i].AngoloV),0);
            
            glVertex3f(Diavoli[i].pos.x,Diavoli[i].pos.y,0);
            glVertex3f(Diavoli[i].pos.x+3*Diavoli[i].DciboV*cos(Diavoli[i].angolo+j*Diavoli[i].AngoloVmin),Diavoli[i].pos.y-3*Diavoli[i].DciboV*sin(Diavoli[i].angolo+j*Diavoli[i].AngoloVmin),0);
            glEnd();
            
        }
        
        glColor3f(1,0,0);
        drawArc(Diavoli[i].pos.x, Diavoli[i].pos.y, 2*Diavoli[i].DciboV, (Diavoli[i].angolo-Diavoli[i].AngoloVmin), abs(Diavoli[i].AngoloVmin+Diavoli[i].AngoloV));
        glColor3f(0,0,1);
        drawArc(Diavoli[i].pos.x, Diavoli[i].pos.y, 2*Diavoli[i].DciboV, (Diavoli[i].angolo- abs(Diavoli[i].AngoloV)), abs(Diavoli[i].AngoloVmin+Diavoli[i].AngoloV));
        
        
        if(Diavoli[i].AngoloVmin>0){
            glColor3f(1,0,1);
            drawArc(Diavoli[i].pos.x, Diavoli[i].pos.y, 2*Diavoli[i].DciboV, (Diavoli[i].angolo - Diavoli[i].AngoloVmin), 2*Diavoli[i].AngoloVmin);
        }
        glEnd();
        
        if(Diavoli[i].boostpassato>0 && Diavoli[i].boostpassato>5){
            glColor3f(1,1,1);
            drawArc(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD*180/((Diavoli[i].boostpassato)*(Diavoli[i].boostpassato)), 0, 2*M_PI);
            drawArc(Diavoli[i].pos.x, Diavoli[i].pos.y, (TAZRAD*180/((Diavoli[i].boostpassato)*(Diavoli[i].boostpassato))+TAZRAD/2), 0, 2*M_PI);
        }


        //ORIGINALE!!
//        for(int j=-2;j<3;j++){
//            if(j==0)continue;
//            glVertex3f(Diavoli[i].pos.x,Diavoli[i].pos.y,0);
//            glVertex3f(Diavoli[i].pos.x+(TAZRAD*4)*cos(Diavoli[i].angolo+j*M_PI/8),Diavoli[i].pos.y+(TAZRAD*4)*sin(Diavoli[i].angolo+j*M_PI/8),0);
//        }
        
        //and eye to the back
        //glVertex3f(Diavoli[i].pos.x,Diavoli[i].pos.y,0);
        //glVertex3f(Diavoli[i].pos.x+(TAZRAD*1.5)*cos(Diavoli[i].angolo+M_PI+3*M_PI/16),Diavoli[i].pos.y+(TAZRAD*1.5)*sin(Diavoli[i].angolo+M_PI+3*M_PI/16),0);
        //glVertex3f(Diavoli[i].pos.x,Diavoli[i].pos.y,0);
        //glVertex3f(Diavoli[i].pos.x+(TAZRAD*1.5)*cos(Diavoli[i].angolo+M_PI-3*M_PI/16),Diavoli[i].pos.y+(TAZRAD*1.5)*sin(Diavoli[i].angolo+M_PI-3*M_PI/16),0);
//		glEnd();
        
		//glBegin(GL_POLYGON); //body
        //glColor3f(1,0,0);
        //drawCircle(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD);
        drawDiavolo(Diavoli[i].pos.x, Diavoli[i].pos.y, TAZRAD);
		//glEnd();
        
		glBegin(GL_LINES);
        //outline
        glColor3f(0,0,0);
        
        for (int k=0;k<17;k++)
        {
            n = k*(M_PI/8);
            glVertex3f(Diavoli[i].pos.x+r*sin(n),Diavoli[i].pos.y+r*cos(n),0);
            n = (k+1)*(M_PI/8);
            glVertex3f(Diavoli[i].pos.x+r*sin(n),Diavoli[i].pos.y+r*cos(n),0);
        }
        //and spike
        //glColor3f(0.5,0,0);
        //glVertex3f(Diavoli[i].pos.x,Diavoli[i].pos.y,0);
        
        
		//and salute
		int xo=18;
		int yo=-15;
		glBegin(GL_QUADS);
        //black background
        //glColor3f(0,0,0);
        //glVertex3f(Diavoli[i].pos.x+xo,Diavoli[i].pos.y+yo,0);
        //glVertex3f(Diavoli[i].pos.x+xo+5,Diavoli[i].pos.y+yo,0);
        //glVertex3f(Diavoli[i].pos.x+xo+5,Diavoli[i].pos.y+yo+40,0);
        //glVertex3f(Diavoli[i].pos.x+xo,Diavoli[i].pos.y+yo+40,0);
        
        //sesso
//        if(Diavoli[i].sesso)
//            glColor3f(1,0.4,1);
//        else
//            glColor3f(0.2,0.6,1);
        
        //glVertex3f(Diavoli[i].pos.x+xo,Diavoli[i].pos.y+yo+20*(2-Diavoli[i].salute),0);
        //glVertex3f(Diavoli[i].pos.x+xo+5,Diavoli[i].pos.y+yo+20*(2-Diavoli[i].salute),0);
        //glVertex3f(Diavoli[i].pos.x+xo+5,Diavoli[i].pos.y+yo+40,0);
        //glVertex3f(Diavoli[i].pos.x+xo,Diavoli[i].pos.y+yo+40,0);
        
//        
//        glVertex3f(Diavoli[i].pos.x+xo+3,Diavoli[i].pos.y+yo+12,0);
//        glVertex3f(Diavoli[i].pos.x+xo+3,Diavoli[i].pos.y+yo+22,0);
//        glVertex3f(Diavoli[i].pos.x+xo+4,Diavoli[i].pos.y+yo+12,0);
//        glVertex3f(Diavoli[i].pos.x+xo+4,Diavoli[i].pos.y+yo+22,0);
        
        //salute
        float col;
        col= (Diavoli[i].salute/SALMAX);
        if (col<0)
			glColor3f(1-abs(col),1-abs(col),1);
        else
			glColor3f(1,1-abs(col),1-abs(col));
        
        glVertex3f(Diavoli[i].pos.x+xo+3,Diavoli[i].pos.y+yo+12,0);
        glVertex3f(Diavoli[i].pos.x+xo+3,Diavoli[i].pos.y+yo+22,0);
        glVertex3f(Diavoli[i].pos.x+xo+4,Diavoli[i].pos.y+yo+12,0);
        glVertex3f(Diavoli[i].pos.x+xo+4,Diavoli[i].pos.y+yo+22,0);
        
        
		glEnd();
        
		//print stats - questi sono i numerini a sinistra
		//generation count
		//sprintf(buf2, "%i", Diavoli[i].id); //qui c'era il gen counter
		//RenderString(Diavoli[i].pos.x-TAZRAD*1.5, Diavoli[i].pos.y+TAZRAD*1.8, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
		
		//id
		sprintf(buf2, "%i", Diavoli[i].id);
		RenderString(Diavoli[i].pos.x-TAZRAD*1.5, Diavoli[i].pos.y+TAZRAD*1.8+12, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
		
//		//salute
//		sprintf(buf2, "%.2f", Diavoli[i].salute);
//		RenderString(Diavoli[i].pos.x-TAZRAD*1.5, Diavoli[i].pos.y+TAZRAD*1.8+24, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
//        
//		//bravura
//		sprintf(buf2, "%.2f", Diavoli[i].bravura);
//		RenderString(Diavoli[i].pos.x-TAZRAD*1.5, Diavoli[i].pos.y+TAZRAD*1.8+36, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
//        
		//repcounter
		//sprintf(buf2, "%.2f", Diavoli[i].repcounter);
		//RenderString(Diavoli[i].pos.x-TAZRAD*1.5, Diavoli[i].pos.y+TAZRAD*1.8+48, GLUT_BITMAP_TIMES_ROMAN_24, buf2, 0.0f, 0.0f, 0.0f);
    }
    
	glPopMatrix();
	glutSwapBuffers();
    
}

void handleIdle(){
    sprintf( buf, "NumDiavoli:  %lu         Step:   %d", Diavoli.size(), modcounter );
    glutSetWindowTitle( buf );
    if(!paused) update();
	if(draw){
		if(skipdraw>0){if(modcounter%skipdraw==0) renderScene();} //increase fps by skipping drawing
		else renderScene(); //we will decrease fps by waiting using clocks
	}
}



