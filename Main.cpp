#include "Inicjalizacja.h"

#define PLANSZA 1 
#define KIEROWNICA 2 
#define KABINA 3 
#define NIEBO 4 
#define WSKAZOWKA 5
#define KIERP 6
#define KIERL 7
#define TARCZAPRED 8
#define SWIATLA 9
#define SWIATLASTOP 10
#define SWIATLATYL 11
#define KOLO 12
#define KOLOPRZODP 13
#define KOLOPRZODL 14
#define MODELFIATA 15
#define ZOLTYA 16
#define CZERWONYA 17
#define ZIELONYA 18
#define ZOLTYB 19
#define CZERWONYB 20
#define ZIELONYB 21
#define STANDRZWIA 28
#define STANDRZWIB 29
#define STANDRZWIC 30
#define BELKAGORA 31
#define WSKAZNIK 32
#define ROZKLAD 33
#define KABINAWEW 34
#define DRZWIA 35
#define DRZWIB 36
#define DRZWIC 37
#define DRZWID 38
#define DRZWIE 39
#define DRZWIF 40

extern Kamera UstawKamere;
HGLRC	hRC;
HDC		hDC;
GLuint TexPlanszy[200], TexModelu[100], TexFiata[100], Czcionka[1],
	   TexMenu[20], TexRozne[100], TexRozklady[100];
extern GLfloat obrot_x, x, z, obiekt_x, kierownica, predkosc, wskazowka,
			   pochylenieX, x_f125, z_f125, obrot_f125, WyliczonyKat,
			   Kat, PochPrzySkrecie, KatWskaznika, PasWy, PasWch, PasSuma,
			   Paliwo, OdcinekX, OdcinekZ, Dystans, Sekundy, Minuty,
			   Godziny, DrzwiAObrot, DrzwiAZ, DrzwiBObrot, DrzwiBZ,
			   DrzwiCObrot,	DrzwiCZ;
extern float OdlegloscKolaP, OdlegloscKolaL, OdlegloscKolTyl, OdlegloscDrzwi1P,
			 OdlegloscDrzwi1L, OdlegloscDrzwi2P, OdlegloscDrzwi2L,
			 OdlegloscDrzwi3P, OdlegloscDrzwi3L, wynik;
GLint g_keyColor[3] = { 0, 0, 0 };
extern GLint fpsOstateczny, WyborLuster;
extern bool PokazParametry, StrzalkaKierP, StrzalkaKierL, SwiatloWew,
		    SwiatloZew, SwiatlaStop, JakaKamera, SwiatlaTyl,DrogoweZolty,
			DrogoweZielony, DrogoweCzerwony, Klawiszologia, PierwszeDrzwi,
			DrugieDrzwi, TrzecieDrzwi, PrzystanekZaliczony, PokazRozklad,
			Pauza, Statystyka;
extern char *StanDrzwi1, *StanDrzwi2, *StanDrzwi3, *StanBiegow, *Opinia;
extern DWORD Mipmaping;
float ObrotKola=0;
bool Obraz=TRUE;

/********************** £adowanie tekstury planszy ***************/
void __fastcall LadujTekstury(char *tekstura, int IDTex, int WyborTablicy)
{
    AUX_RGBImageRec *Tekstura_RGB = auxDIBImageLoad( tekstura );
    unsigned char *Tekstura_RGBA = NULL;

    if( Tekstura_RGB != NULL )
    {
        int Rozmiar_RGB  = Tekstura_RGB->sizeX * Tekstura_RGB->sizeY * 3;
        int Rozmiar_RGBA = Tekstura_RGB->sizeX * Tekstura_RGB->sizeY * 4;

        Tekstura_RGBA = new unsigned char[Rozmiar_RGBA];

        int i, j;

        for( i = 0, j = 0; i < Rozmiar_RGB; i += 3, j += 4 )
        {

            if( Tekstura_RGB->data[i]   == g_keyColor[0] &&
                Tekstura_RGB->data[i+1] == g_keyColor[1] &&
                Tekstura_RGB->data[i+2] == g_keyColor[2] )
            {
                Tekstura_RGBA[j+3] = 0;
            }
            else
            {
                Tekstura_RGBA[j+3] = 255;
            }

            Tekstura_RGBA[j]   = Tekstura_RGB->data[i];
            Tekstura_RGBA[j+1] = Tekstura_RGB->data[i+1];
            Tekstura_RGBA[j+2] = Tekstura_RGB->data[i+2];
        }
		switch(WyborTablicy){
		case 0:
			glGenTextures(1, &TexPlanszy[IDTex]);
			glBindTexture(GL_TEXTURE_2D, TexPlanszy[IDTex]);
			break;
		case 1:
			glGenTextures(1, &TexModelu[IDTex]);
			glBindTexture(GL_TEXTURE_2D, TexModelu[IDTex]);
			break;
		case 2:
			glGenTextures(1, &TexFiata[IDTex]);
			glBindTexture(GL_TEXTURE_2D, TexFiata[IDTex]);
			break;
		case 3:
			glGenTextures(1, &Czcionka[IDTex]);
			glBindTexture(GL_TEXTURE_2D, Czcionka[IDTex]);
			break;
		case 4:
			glGenTextures(1, &TexMenu[IDTex]);
			glBindTexture(GL_TEXTURE_2D, TexMenu[IDTex]);
			break;
		case 5:
			glGenTextures(1, &TexRozne[IDTex]);
			glBindTexture(GL_TEXTURE_2D, TexRozne[IDTex]);
			break;
		case 6:
			glGenTextures(1, &TexRozklady[IDTex]);
			glBindTexture(GL_TEXTURE_2D, TexRozklady[IDTex]);
			break;
		}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
			Mipmaping);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Tekstura_RGB->sizeX,
Tekstura_RGB->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, Tekstura_RGBA );
/*glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImage_RGB->sizeX, pImage_RGB->sizeY, 0,
              GL_RGBA, GL_UNSIGNED_BYTE, pImage_RGBA );*/

		/* Wymuszenie wyrównania do 4 bajtów */ 
glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
/**/glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); 
glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); 
glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0) ;

    }

    if( Tekstura_RGB )
    {
        if( Tekstura_RGB->data )
            free( Tekstura_RGB->data );

        free( Tekstura_RGB );
    }

    if( Tekstura_RGBA )
        delete [] Tekstura_RGBA;

}
/************************* Renderowanie **************************/
void __fastcall Renderuj()
{

	Klawiatura();
	Kolizja();

for(int i=0; i<WyborLuster; i++){

	if(i==0){
	glViewport(0, 0, szerokosc, wysokosc);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, static_cast<float>(szerokosc)/static_cast<float>(wysokosc), 0.1, 440/*440*/ );
	}
	if(i==1){
	glViewport (0, wysokosc/2, szerokosc/4, wysokosc/2);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, (szerokosc/1.5)/(wysokosc), 0.1f, 500.0 );
	}
	if(i==2){
	glViewport (szerokosc/1.33, wysokosc/2, szerokosc/1.33, wysokosc/2);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective( 45.0, (szerokosc)/(wysokosc/1.6), 0.1f, 500.0 );
	}

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glClear (GL_DEPTH_BUFFER_BIT/* | GL_COLOR_BUFFER_BIT*/);

	if(i==0){
	gluLookAt(UstawKamere.Obrot, UstawKamere.Wysokosc, static_cast<double>(-z/10000000), UstawKamere.Obrot_a, pochylenieX*2, UstawKamere.PunktPatrzenia, 0,1,0);
	glTranslatef(0.0f, 0.0f, UstawKamere.Odleglosc);
	}
	if(i==1){
	gluLookAt(-1.8, 1.6, static_cast<double>(-z/10000000), 5, -2, 100, 0,1,0);
	glTranslatef(0.0f, 0.0f, 7.0f);
	}
	if(i==2){
	gluLookAt(1.8, 2.0, static_cast<double>(-z/10000000), -50, -20, 100, 0,1,0);
	glTranslatef(0.0f, 0.0f, 7.0f);
	}

	/*********** niebo *************/
	glPushMatrix();
	glRotatef(obrot_x, 0.0f, 1.0f, 0.0f);
	glCallList(NIEBO);
	glPopMatrix();

	/************ ca³y œwiat 3D *********/
	glPushMatrix();
	glRotatef(obrot_x, 0.0f, 1.0f, 0.0f);
	glTranslatef(-x, 0.0f, z);
	glCallList(PLANSZA);
	/**** swiat³a drogowe ********/
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	if(DrogoweZolty==TRUE){
		glCallList(ZOLTYA);
		glCallList(ZOLTYB);
	}
	if(DrogoweCzerwony==TRUE){
		glCallList(CZERWONYA);
		glCallList(ZIELONYB);
	}
	if(DrogoweZielony==TRUE){
		glCallList(ZIELONYA);
		glCallList(CZERWONYB);
	}
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_LIGHTING);
	glPopMatrix();

	/*********** Fiat 125 **********/
	glPushMatrix();
	glRotatef(obrot_x, 0.0f, 1.0f, 0.0f);
	glTranslatef(-x, 0.0f, z);

	glTranslatef(x_f125, 0.0f, z_f125);
	glRotatef(obrot_f125, 0.0f, 1.0f, 0.0f);
	
	glTranslatef(0.0f, 0.0f, 14.0f);
	glCallList(MODELFIATA);
	glPopMatrix();

	/**************** ko³a autobusu ty³ ****************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, OdlegloscKolTyl);
	glTranslatef(0.0f, 0.45f, 0.7f);
	glRotatef(ObrotKola=ObrotKola+wskazowka*-100, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.45f, -0.7f);
	glCallList(KOLO);
	glPopMatrix();

	/**************** ko³a autobusu przód P****************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, OdlegloscKolaP);
	glTranslatef(1.3f, 0.45f, -4.55f);
	glRotatef(kierownica*-130, 0.0f, 1.0f, 0.0f);
	glTranslatef(-1.3f, -0.45f, 4.55f);
	glTranslatef(0.0f, 0.45f, -4.55f);
	glRotatef(ObrotKola=ObrotKola+wskazowka*-40, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.45f, 4.55f);
	glCallList(KOLOPRZODP);
	glPopMatrix();

	/**************** ko³a autobusu przód L****************/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, OdlegloscKolaL);
	glTranslatef(-1.3f, 0.45f, -4.55f);
	glRotatef(kierownica*-130, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.3f, -0.45f, 4.55f);
	glTranslatef(0.0f, 0.45f, -4.55f);
	glRotatef(ObrotKola=ObrotKola+wskazowka*-40, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.45f, 4.55f);
	glCallList(KOLOPRZODL);
	glPopMatrix();

	/*********** wskaŸniki o stanie drzwi i model wew ******/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0);//
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, -2.0);//
	glAlphaFunc(GL_GREATER, 0.5f);
	glEnable(GL_ALPHA_TEST);
	if(SwiatloWew==TRUE){
		glDisable(GL_LIGHTING);
		glCallList(KABINAWEW);
		glEnable(GL_LIGHTING);
	}else{
		glCallList(KABINAWEW);
	}
	if(PierwszeDrzwi==TRUE){
		glCallList(STANDRZWIA);
	}
	if(DrugieDrzwi==TRUE){
		glCallList(STANDRZWIB);
	}
	if(TrzecieDrzwi==TRUE){
		glCallList(STANDRZWIC);
	}
	
	//glEnable( GL_BLEND );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glPopMatrix();

	/***************** Drzwi PIERWSZE *****************/
	glPushMatrix();//Prawe
	glTranslatef(0.0f, 0.0f, -2.0f);//resor
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//resor
	glTranslatef(0.0f, 0.0f, OdlegloscDrzwi1P);//pozycja drzwi w Z
	glTranslatef(0.0f, 0.0f, -DrzwiAZ);//przesuwanie
	glTranslatef(1.5f, 0.0f, -0.31f);//obrót
	glRotatef(DrzwiAObrot, 0.0f, 1.0f, 0.0f);//obrót
	glTranslatef(-1.5f, 0.0f, 0.31f);//obrót
	glCallList(DRZWIA);
	glPopMatrix();

	glPushMatrix();//Lewe
	glTranslatef(0.0f, 0.0f, -2.0f);//resor
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//resor
	glTranslatef(0.0f, 0.0f, OdlegloscDrzwi1L);//pozycja drzwi w Z
	glTranslatef(0.0f, 0.0f, DrzwiAZ);//przesuwanie
	glTranslatef(1.5f, 0.0f, -0.31f);//obrót
	glRotatef(-DrzwiAObrot, 0.0f, 1.0f, 0.0f);//obrót
	glTranslatef(-1.5f, 0.0f, 0.31f);//obrót
	glCallList(DRZWID);
	glPopMatrix();

	/***************** Drzwi DRUGIE *****************/
	glPushMatrix();//Prawe
	glTranslatef(0.0f, 0.0f, -2.0f);//resor
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//resor
	glTranslatef(0.0f, 0.0f, OdlegloscDrzwi2P);//pozycja drzwi w Z
	glTranslatef(0.0f, 0.0f, -DrzwiBZ);//przesuwanie
	glTranslatef(1.5f, 0.0f, -0.31f);//obrót
	glRotatef(DrzwiBObrot, 0.0f, 1.0f, 0.0f);//obrót
	glTranslatef(-1.5f, 0.0f, 0.31f);//obrót
	glCallList(DRZWIB);
	glPopMatrix();

	glPushMatrix();//Lewe
	glTranslatef(0.0f, 0.0f, -2.0f);//resor
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//resor
	glTranslatef(0.0f, 0.0f, OdlegloscDrzwi2L);//pozycja drzwi w Z
	glTranslatef(0.0f, 0.0f, DrzwiBZ);//przesuwanie
	glTranslatef(1.5f, 0.0f, -0.31f);//obrót
	glRotatef(-DrzwiBObrot, 0.0f, 1.0f, 0.0f);//obrót
	glTranslatef(-1.5f, 0.0f, 0.31f);//obrót
	glCallList(DRZWIE);
	glPopMatrix();

	/***************** Drzwi TRZECIE *****************/
	glPushMatrix();//Prawe
	glTranslatef(0.0f, 0.0f, -2.0f);//resor
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//resor
	glTranslatef(0.0f, 0.0f, OdlegloscDrzwi3P);//pozycja drzwi w Z
	glTranslatef(0.0f, 0.0f, -DrzwiCZ);//przesuwanie
	glTranslatef(1.5f, 0.0f, -0.31f);//obrót
	glRotatef(DrzwiCObrot, 0.0f, 1.0f, 0.0f);//obrót
	glTranslatef(-1.5f, 0.0f, 0.31f);//obrót
	glCallList(DRZWIC);
	glPopMatrix();

	glPushMatrix();//Lewe
	glTranslatef(0.0f, 0.0f, -2.0f);//resor
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//resor
	glTranslatef(0.0f, 0.0f, OdlegloscDrzwi3L);//pozycja drzwi w Z
	glTranslatef(0.0f, 0.0f, DrzwiCZ);//przesuwanie
	glTranslatef(1.5f, 0.0f, -0.31f);//obrót
	glRotatef(-DrzwiCObrot, 0.0f, 1.0f, 0.0f);//obrót
	glTranslatef(-1.5f, 0.0f, 0.31f);//obrót
	glCallList(DRZWIF);
	glPopMatrix();
glDisable(GL_ALPHA_TEST);
	/************* model zew ***********/
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);//
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, -2.0f);//
	glCallList(KABINA);
	glPopMatrix();

if(i==0){

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.0f);//
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0f);//
	glTranslatef(0.0f, 0.0f, -4.0f);

	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	/***** œwiat³a drogowe *****/
	if(SwiatloZew==TRUE){ 
		if(JakaKamera==TRUE){
			glDisable(GL_DEPTH_TEST);
			glCallList(SWIATLA);
			glEnable(GL_DEPTH_TEST);
		}else{
			glCallList(SWIATLA);
		}
	}
	if(StrzalkaKierP==TRUE){/*** kierunkowskazy **/
		glCallList(KIERP);
	}
	if(StrzalkaKierL==TRUE){
		glCallList(KIERL);
	}
	if(SwiatlaTyl==TRUE){/*** œwiat³a tyl ***/
		glCallList(SWIATLATYL);
	}
	if(SwiatlaStop==TRUE){/*** œwiat³a stop ***/
		glCallList(SWIATLASTOP);
	}
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_LIGHTING);
	glPopMatrix();

	/*glPushMatrix();
	glTranslatef(0.0f, 0.0f, -4.0f);
	glDisable(GL_LIGHTING);
	glCallList(TARCZAPRED);/*** prêdkoœciomierz **/
	/*glEnable(GL_LIGHTING);
	glPopMatrix();*/

	glPushMatrix();/***** wskazowka predkosc *****/
	glTranslatef(0.0f, 0.0f, -2.0);//
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glRotatef(PochPrzySkrecie*17, 0.0f, 0.0f, 1.0f);//resory lewa-prawa
	glTranslatef(0.0f, 0.0f, 2.0);//
	glTranslatef(-0.91f, 1.01f, 0.0f);
	glRotatef(wskazowka*-400, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.91f, -1.01f, -4.0f);
	if(SwiatloWew==TRUE){
		glDisable(GL_LIGHTING);
		glCallList(WSKAZOWKA);
		glEnable(GL_LIGHTING);
	}else{
		glCallList(WSKAZOWKA);
	}
	glPopMatrix();

	glPushMatrix(); /******* kierownica *******/
	glTranslatef(0.0f, 0.0f, -2.0);//
	glRotatef(pochylenieX, 1.0f, 0.0f, 0.0f);//resory ty³-przód
	glTranslatef(0.0f, 0.0f, 2.0);//
	glTranslatef(-0.79f, 0.9f, 0.0f);
	glRotatef(-kierownica*1000.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.79f, -0.9f, -4.0f);
	if(SwiatloWew==TRUE){
		glDisable(GL_LIGHTING);
		glCallList(KIEROWNICA);
		glEnable(GL_LIGHTING);
	}else{
		glCallList(KIEROWNICA);
	}
	glPopMatrix();

	/****************** belka u góry i wskaŸnik *************/
	glPushMatrix();
	//glTranslated(0,0,-4.0f);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();	
	glLoadIdentity();
	glOrtho(0,5,0,3.75,-1,1);//rzut ortograficzny
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushMatrix();
	//glTranslated(0,0,1.0f);
	glTranslated(0.3f,3.175f,0);
	glRotatef(KatWskaznika+obrot_x, 0.0f, 0.0f, 1.0f);//wskaŸnik
	glTranslated(-0.3f,-3.175f,0);
	if(PrzystanekZaliczony==TRUE){
		glColor3ub(0, 255, 0);
		glCallList(WSKAZNIK);
		glColor3ub(255, 255, 255);
	}else{
		glColor3ub(255, 0, 0);
		glCallList(WSKAZNIK);
		glColor3ub(255, 255, 255);
	}
	glPopMatrix();
	glPushMatrix();
	if(PokazRozklad==TRUE){
		glCallList(ROZKLAD);//Rozklad
	}
	glCallList(BELKAGORA);//belka u góry
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	/*************** wyœwietlanie param. na belce *****/
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslated(0,0,-4.0f);
	glColor3ub(151, 179, 228);
	WyswietlTekst(445, 462, 0, "%3.0fKM/H", wskazowka*222);
	WyswietlTekst(105, 462, 0, "%2.0f", PasWy);
	WyswietlTekst(145, 462, 0, "%2.0f", PasWch);
	WyswietlTekst(185, 462, 0, "%2.0f", PasSuma);
	WyswietlTekst(390, 462, 0, "%2.0fl", Paliwo);
	WyswietlTekst(255, 462, 0, "%2.0f", Godziny);
	WyswietlTekst(277, 462, 0, ":");
	WyswietlTekst(285, 462, 0, "%2.0f", Minuty);
	WyswietlTekst(307, 462, 0, ":");
	WyswietlTekst(315, 462, 0, "%2.0f", Sekundy);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	/******* parametry gry ************/
	if(PokazParametry){
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0.0f, 0.0f, -4.0);
		WyswietlTekst(0, 17, 0, "z=%3.2f", -z);
		WyswietlTekst(0, 34, 0, "x=%3.2f", x);
		WyswietlTekst(0, 68, 0, "predkosc=%3.1fkm/h", wskazowka*222);
		WyswietlTekst(0, 51, 0, "stan biegow:%s", StanBiegow);
		WyswietlTekst(0, 85, 0, "resor lewo-prawo:%3.5f", PochPrzySkrecie);
		WyswietlTekst(0, 102, 0, "resor przod-tyl:%3.5f", pochylenieX);
		WyswietlTekst(0, 119, 0, "stan drzwi 3:%s", StanDrzwi3);
		WyswietlTekst(0, 136, 0, "stan drzwi 2:%s", StanDrzwi2);
		WyswietlTekst(0, 153, 0, "stan drzwi 1:%s", StanDrzwi1);
		WyswietlTekst(0, 170, 0, "kat wskaznika=%3.2f", KatWskaznika);
		WyswietlTekst(0, 187, 0, "paliwo=%3.2f", Paliwo);
		WyswietlTekst(0, 204, 0, "X=%3.2f", OdcinekX);
		WyswietlTekst(0, 221, 0, "Z=%3.2f", OdcinekZ);
		WyswietlTekst(0, 255, 0, "kierownica=%3.2f", kierownica);
		WyswietlTekst(0, 238, 0, "fps=%2.0i", fpsOstateczny);
		//WyswietlTekst(0, 255, 0, "punkty karne=%3.2f", ObliczajPunktyKarne());
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	/******* statystyka po koñcu misji ************/
	if(Statystyka){
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0.0f, 0.0f, -4.0);
		WyswietlTekst(265, 272, 1, "statystyka:");
		WyswietlTekst(195, 255, 0, "zDOBYTE PUNKTY KARNE:%3.0f", ObliczajPunktyKarne());
		WyswietlTekst(275, 221, 1, "uwagi:");
		WyswietlTekst(140, 204, 0, "%s", Opinia);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	/******* punkty karne ************/
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(0.0f, 0.0f, -4.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	WyswietlTekst(1, 1, 0, "kara:%3.0f", ObliczajPunktyKarne());
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	/******* pauza ************/
	if(Pauza==TRUE){
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0.0f, 0.0f, -4.0);
		WyswietlTekst(280, 255, 0, "pauza");
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	/******* klawiszologia gry ************/
	if(Klawiszologia){
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0.0f, 0.0f, -4.0);
		WyswietlTekst(0, 459, 1, "klawiszologia:");
		WyswietlTekst(0, 425, 1, "***biegi***");
		WyswietlTekst(0, 408, 0, "q - do przodu");
		WyswietlTekst(0, 391, 0, "a - luz");
		WyswietlTekst(0, 374, 0, "z - wsteczny");
		WyswietlTekst(0, 340, 1, "***inne***");
		WyswietlTekst(0, 323, 0, "s - start/stop silnika");
		WyswietlTekst(0, 306, 0, "k - klakson");
		WyswietlTekst(0, 289, 0, "scroll lock - sygnal ostrzegawczy przed zamknieciem drzwi");
		WyswietlTekst(0, 272, 0, "l - lusterka wsteczne");
		WyswietlTekst(0, 255, 0, "c - swiatla drogowe");
		WyswietlTekst(0, 221, 1, "***kierunkowskazy***");
		WyswietlTekst(0, 204, 0, ". - prawy kierunkowskaz");
		WyswietlTekst(0, 187, 0, ", - lewy kierunkowskaz");
		WyswietlTekst(0, 153, 1, "***drzwi***");
		WyswietlTekst(0, 136, 0, "delete, end, page down - otwarcie drzwi");
		WyswietlTekst(0, 119, 0, "insert, home, page up - zamkniecie drzwi");
		WyswietlTekst(0, 85, 1, "***kamera***");
		WyswietlTekst(0, 68, 0, "f1 - kabina");
		WyswietlTekst(0, 51, 0, "f2 - tyl autobusu");
		WyswietlTekst(0, 34, 0, "f3 - kamera na zderzaku");
		WyswietlTekst(0, 17, 0, "f4 - kamera na dachu");
		WyswietlTekst(0, 0, 0, "f5 - przod autobusu");
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
}
}
	glFlush();
	SwapBuffers( hDC );

}

