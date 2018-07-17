#include "Inicjalizacja.h"
#include "fmod.h"

extern GLuint TexMenu[20];
bool BlokadaRozjas=TRUE, KoniecRozjasniania=TRUE, BlokadaXmenu=TRUE,
     BlokadaXmenuA=TRUE, BlokadaXmenuB=TRUE, BlokadaXmenuC=TRUE,
	 BlokadaXmenuD=TRUE, BlokadaXmenuE=TRUE, AnimujNapisy=FALSE,
	 AnimujAutobusy=FALSE, BlokadaXmenuF=TRUE, BlokadaXmenuG=TRUE,
	 StartGry=FALSE, BlokadaInitGL=TRUE, Ladowanie=FALSE, PowrotDoMenu=FALSE,
	 AnimujNowaGra=FALSE, IniGL=TRUE, ImpulsDlaDzwieku=TRUE;
float Rozjasniacz=0, Xmenu=0, BlokadaAnimuj=TRUE, Xwyjscie=0,Xautobusy=0;
int JakaTekstura=0;
extern int WyborMenu, GranicaGorna, GranicaDolna;
extern GLint gaz;
extern bool Wyjscie, NowaGra, Autobusy, OGrze, WyjscieDoMenu, BlokadaEnterAnim;

extern FSOUND_SAMPLE *Dzwieki[12];
extern FSOUND_STREAM *Muzyka[1];

#define LOGO 22
#define NOWAGRA 23
#define BUSINFO 24
#define OGRZE 25
#define WYJSCIE 26
#define OGRZEANIMACJA 27

/********************* Renderowanie Menu *************************/
void __fastcall RenderujMenu()
{

if(StartGry==FALSE){	/*prze³¹czanie miêdzy MENU a SYMULATOREM*/
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	KlawiaturaMenu();
	glDisable(GL_LIGHTING);

	if(KoniecRozjasniania==TRUE){
		Logo(0);
		//FSOUND_Stream_Play(20, Muzyka[0]);
	}else{
	
		glBindTexture(GL_TEXTURE_2D,TexMenu[1]);
		glCallList(LOGO);

		if(Wyjscie==TRUE){	/*wyjœcie z gry*/
			BlokadaEnterAnim=TRUE;
			Logo(1);
			AnimujMenu(1);
		}
		if(BlokadaAnimuj==TRUE){
			BlokadaEnterAnim=TRUE;
			AnimujMenu(0);
		}
		if(NowaGra==TRUE){	/*gdy wybrano NOWA GRA*/
			BlokadaEnterAnim=TRUE;
			AnimujNowaGra=TRUE;
			AnimujNapisy=FALSE;
			AnimujAutobusy=FALSE;
			AnimujMenu(6);
			AnimujMenu(1);
			if(BlokadaInitGL==TRUE && Ladowanie==TRUE){
				BlokadaInitGL=FALSE;
				Ladowanie=FALSE;
				InitGL();
				StartGry=TRUE;
				AnimujNowaGra=FALSE;
				Xautobusy=0;
				FSOUND_Stream_Stop(Muzyka[0]);
				ImpulsDlaDzwieku=TRUE;
			}
		}
		if(PowrotDoMenu==TRUE){	/*powrót do MENU z SYMULATORA*/
			CzyscTabliceTekstur();
			NiszczCzcionki();
			if(ImpulsDlaDzwieku==TRUE){/*jeden impuls do tych funkcji*/
				ImpulsDlaDzwieku=FALSE;
				FSOUND_Stream_Play(20, Muzyka[0]);
				FSOUND_StopSound(0);
				FSOUND_StopSound(1);
				FSOUND_StopSound(2);
				FSOUND_StopSound(3);
				FSOUND_StopSound(4);
				FSOUND_StopSound(5);
				FSOUND_StopSound(6);
				FSOUND_StopSound(7);
				FSOUND_StopSound(8);
				FSOUND_StopSound(9);
				FSOUND_StopSound(10);
				FSOUND_StopSound(11);
			}
			BlokadaEnterAnim=TRUE;
			BlokadaInitGL=TRUE;
			Ladowanie=FALSE;
			AnimujNowaGra=FALSE;
			AnimujMenu(0);
		}
		if(OGrze==TRUE){	/*gdy wybrano O GRZE*/
			BlokadaEnterAnim=TRUE;
			AnimujNapisy=TRUE;
			AnimujAutobusy=FALSE;
			AnimujMenu(6);
			AnimujMenu(4);
			AnimujMenu(1);
			//WyborMenu=4;
			GranicaGorna=5;
			GranicaDolna=4;
		}
		if(Autobusy==TRUE){	/*gdy wybrano AUTOBUSY*/
			BlokadaEnterAnim=TRUE;
			AnimujAutobusy=TRUE;
			AnimujNapisy=FALSE;
			AnimujMenu(6);
			AnimujMenu(4);
			AnimujMenu(1);
			//WyborMenu=4;
			GranicaGorna=5;
			GranicaDolna=4;
		}
		if(WyjscieDoMenu==TRUE){	/*powrot do MENU g³ownego*/
			BlokadaEnterAnim=TRUE;
			AnimujMenu(5);
			AnimujMenu(7);
			AnimujMenu(0);
			WyborMenu=0;
			GranicaGorna=4;
			GranicaDolna=0;
		}

		glPushMatrix();
		glTranslatef(Xmenu, 0.0f, 0.0f);
		switch(WyborMenu){
		case 0:
			glBindTexture(GL_TEXTURE_2D,TexMenu[2]);
			glCallList(NOWAGRA);
			glBindTexture(GL_TEXTURE_2D,TexMenu[5]);
			glCallList(BUSINFO);
			glBindTexture(GL_TEXTURE_2D,TexMenu[7]);
			glCallList(OGRZE);
			glBindTexture(GL_TEXTURE_2D,TexMenu[9]);
			glCallList(WYJSCIE);
			break;
		case 1:
			glBindTexture(GL_TEXTURE_2D,TexMenu[3]);
			glCallList(NOWAGRA);
			glBindTexture(GL_TEXTURE_2D,TexMenu[4]);
			glCallList(BUSINFO);
			glBindTexture(GL_TEXTURE_2D,TexMenu[7]);
			glCallList(OGRZE);
			glBindTexture(GL_TEXTURE_2D,TexMenu[9]);
			glCallList(WYJSCIE);
			break;
		case 2:
			glBindTexture(GL_TEXTURE_2D,TexMenu[3]);
			glCallList(NOWAGRA);
			glBindTexture(GL_TEXTURE_2D,TexMenu[5]);
			glCallList(BUSINFO);
			glBindTexture(GL_TEXTURE_2D,TexMenu[6]);
			glCallList(OGRZE);
			glBindTexture(GL_TEXTURE_2D,TexMenu[9]);
			glCallList(WYJSCIE);
			break;
		case 3:
			glBindTexture(GL_TEXTURE_2D,TexMenu[3]);
			glCallList(NOWAGRA);
			glBindTexture(GL_TEXTURE_2D,TexMenu[5]);
			glCallList(BUSINFO);
			glBindTexture(GL_TEXTURE_2D,TexMenu[7]);
			glCallList(OGRZE);
			glBindTexture(GL_TEXTURE_2D,TexMenu[8]);
			glCallList(WYJSCIE);
			break;
		case 4:
			glBindTexture(GL_TEXTURE_2D,TexMenu[3]);
			glCallList(NOWAGRA);
			glBindTexture(GL_TEXTURE_2D,TexMenu[5]);
			glCallList(BUSINFO);
			glBindTexture(GL_TEXTURE_2D,TexMenu[7]);
			glCallList(OGRZE);
			glBindTexture(GL_TEXTURE_2D,TexMenu[9]);
			glCallList(WYJSCIE);
			break;
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(Xwyjscie, -0.7f, 0.0f);
		//switch(WyborMenu){
			//case 4:
			glBindTexture(GL_TEXTURE_2D,TexMenu[8]);
			glCallList(WYJSCIE);
			//break;
		//}
		glPopMatrix();

		if(AnimujNapisy==TRUE){ /*animacja 'o grze'*/
		glPushMatrix();
		glTranslatef(-Xautobusy, 0.0f, 0.0f);
		glPushMatrix();
		//glEnable(GL_TEXTURE_2D);
		glMatrixMode (GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity ();
		glTranslatef(0.0, -0.0004f, 0.0f);
		//glLoadIdentity ();
		glBindTexture(GL_TEXTURE_2D,TexMenu[10]);
		glCallList(OGRZEANIMACJA);
		glPopMatrix();
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix();
		//glLoadIdentity ();
		glPopMatrix();
		}

		if(AnimujAutobusy==TRUE){ /*animacja 'autobusy'*/
		glPushMatrix();
		glTranslatef(-Xautobusy, 0.0f, 0.0f);
		glBindTexture(GL_TEXTURE_2D,TexMenu[11]);
		glCallList(OGRZEANIMACJA);
		glPopMatrix();
		}

		if(AnimujNowaGra==TRUE){ /*animacja 'nowa gra'*/
		glPushMatrix();
		glTranslatef(-Xautobusy, 0.0f, 0.0f);
		glBindTexture(GL_TEXTURE_2D,TexMenu[12]);
		glCallList(OGRZEANIMACJA);
		glPopMatrix();
		}

	}
	glEnable(GL_LIGHTING);
	glFlush();
	SwapBuffers( hDC );

}else{
	Renderuj();
}

}
/**************************** wyœwietl logo **********************/
void Logo(int Kierunek)
{
	switch(Kierunek){
	case 0:
		if(Rozjasniacz<1 && BlokadaRozjas==TRUE){
			Rozjasniacz=Rozjasniacz+0.005f;
			if(Rozjasniacz>=1){
				BlokadaRozjas=FALSE;
			}
			if(JakaTekstura==1 && Rozjasniacz>=1){
				Rozjasniacz=1.0f;
				KoniecRozjasniania=FALSE;
			}
		}
		if(Rozjasniacz>0 && BlokadaRozjas==FALSE){
			Rozjasniacz=Rozjasniacz-0.005f;
			if(Rozjasniacz<=0){
				BlokadaRozjas=TRUE;
				JakaTekstura=1;
				FSOUND_Stream_Play(20, Muzyka[0]);
			}
		}
		break;
	case 1:
		if(Rozjasniacz>0 && BlokadaRozjas==FALSE){
			Rozjasniacz=Rozjasniacz-0.005f;
			if(Rozjasniacz<=0){
				BlokadaRozjas=TRUE;
				JakaTekstura=1;
				/**** wyjscie ****/
				CzyscTabliceTekstur();
				NiszczCzcionki();
				NiszczDzwiek();
				wglMakeCurrent( hDC, NULL );
				wglDeleteContext( hRC );
				ChangeDisplaySettings(NULL,0);
				PostQuitMessage( 0 );
			}
		}
		break;
	}
	glBindTexture(GL_TEXTURE_2D,TexMenu[JakaTekstura]);
	glColor3f(Rozjasniacz,Rozjasniacz,Rozjasniacz);
	glCallList(LOGO);

}
/****************************** animuacja menu *******************/
void AnimujMenu(int Kierunek)
{

	switch(Kierunek){
	case 0:
		if(Xmenu<1.495 && BlokadaXmenu==TRUE){
			Xmenu=Xmenu+0.012f;
			if(Xmenu>=1.495){
				BlokadaXmenu=FALSE;
				BlokadaAnimuj=FALSE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuB=TRUE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuF=TRUE;
				BlokadaXmenuG=TRUE;
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				PowrotDoMenu=FALSE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 1:
		if(Xmenu>0 && BlokadaXmenuA==TRUE){
			Xmenu=Xmenu-0.012f;
			if(Xmenu<=0){
				BlokadaXmenuA=FALSE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuB=TRUE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuF=TRUE;
				BlokadaXmenuG=TRUE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				NowaGra=FALSE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 2:
		if(Xmenu>1.2 && BlokadaXmenuB==TRUE){
			Xmenu=Xmenu-0.012f;
			if(Xmenu<=1.2){
				BlokadaXmenuB=FALSE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuF=TRUE;
				BlokadaXmenuG=TRUE;
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 3:
		if(Xmenu<1.495 && BlokadaXmenuC==TRUE){
			Xmenu=Xmenu+0.012f;
			if(Xmenu>=1.495){
				BlokadaXmenuC=FALSE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuB=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuF=TRUE;
				BlokadaXmenuG=TRUE;
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 4:
		if(Xwyjscie<1.25 && BlokadaXmenuD==TRUE){
			Xwyjscie=Xwyjscie+0.012f;
			if(Xwyjscie>=1.25){
				BlokadaXmenuD=FALSE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuB=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuF=TRUE;
				BlokadaXmenuG=TRUE;
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 5:
		if(Xwyjscie>0 && BlokadaXmenuE==TRUE){
			Xwyjscie=Xwyjscie-0.012f;
			if(Xwyjscie<=0){
				BlokadaXmenuE=FALSE;
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuB=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuF=TRUE;
				BlokadaXmenuG=TRUE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 6:
		if(Xautobusy<2 && BlokadaXmenuF==TRUE){
			Xautobusy=Xautobusy+0.019f;
			if(Xautobusy>=2){
				BlokadaXmenuF=FALSE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuG=TRUE;
				if(NowaGra==TRUE){
					Ladowanie=TRUE;
				}
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				BlokadaEnterAnim=FALSE;/*odblokuj klawisz enter*/
			}
		}
		break;
	case 7:
		if(Xautobusy>0 && BlokadaXmenuG==TRUE){
			Xautobusy=Xautobusy-0.019f;
			if(Xautobusy<=0){
				BlokadaXmenuG=FALSE;
				BlokadaXmenu=TRUE;
				BlokadaXmenuA=TRUE;
				BlokadaXmenuC=TRUE;
				BlokadaXmenuD=TRUE;
				BlokadaXmenuE=TRUE;
				BlokadaXmenuF=TRUE;
				NowaGra=FALSE;
				Autobusy=FALSE;
				OGrze=FALSE;
				WyjscieDoMenu=FALSE;
				BlokadaEnterAnim=FALSE; /*odblokuj klawisz enter*/
				AnimujAutobusy=FALSE; /*Koniec animacji 'Autobusy'*/
				AnimujNapisy=FALSE;  /*koniec animacji 'O grze'*/
			}
		}
		break;
	}

}
/****************** Twórz listy wyœwietlania menu ****************/
void TworzListWyswMenu()
{

glNewList(LOGO, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-1,1,-1);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1 , -1);
	glTexCoord2f(1, 0);
	glVertex3f(1, -1 , -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1 , -1);
	glTexCoord2f(1, 0);
	glVertex3f(1, -1 , -1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1 , -1);
	glEnd();
glEndList();

glNewList(NOWAGRA, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, 0.3f, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-2.18f, 0.2f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.26f, 0.2f , -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, 0.3f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.26f, 0.2f, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-1.26f, 0.3f , -1);
	glEnd();
glEndList();

glNewList(BUSINFO, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, 0.15f, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-2.18f, 0.05f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.26f, 0.05f , -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, 0.15f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.26f, 0.05f , -1);
	glTexCoord2f(1, 1);
	glVertex3f(-1.26f, 0.15f , -1);
	glEnd();
glEndList();

glNewList(OGRZE, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, 0.0f, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-2.18f, -0.1f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.5f, -0.1f , -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, 0.0f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.5f, -0.1f , -1);
	glTexCoord2f(1, 1);
	glVertex3f(-1.5f, 0.0f , -1);
	glEnd();
glEndList();

glNewList(WYJSCIE, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, -0.15f, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-2.18f, -0.25f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.44f, -0.25f , -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(-2.18f, -0.15f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1.44f, -0.25f , -1);
	glTexCoord2f(1, 1);
	glVertex3f(-1.44f, -0.15f , -1);
	glEnd();
glEndList();

glNewList(OGRZEANIMACJA, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(1.1f,0.4f,-1);
	glTexCoord2f(0, 0);
	glVertex3f(1.1f, -0.8f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(2.9f, -0.8f , -1);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(1.1f, 0.4f , -1);
	glTexCoord2f(1, 0);
	glVertex3f(2.9f, -0.8f , -1);
	glTexCoord2f(1, 1);
	glVertex3f(2.9f, 0.4f , -1);
	glEnd();
glEndList();

}