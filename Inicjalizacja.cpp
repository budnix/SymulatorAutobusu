#include "Inicjalizacja.h"

GLfloat KolorDnia[] = {0.75f, 0.75f, 0.75f, 1.0f};
GLfloat KolorNocy[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat gray[] = {0.75f, 0.75f, 0.75f, 1.0f};
GLfloat OtoczenieDzien[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat OtoczenieNoc[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat odbicie[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat pozycja[] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat odblysk[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat odblyskmat[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat spot[] = {0.0f, 1.0f, -1.0f};

LONGLONG cur_time;       // bie¿¹cy czas
DWORD  time_count=40;    // czas klatki (w ms) z wartoœci¹ pocz¹tkow¹ 40
LONGLONG perf_cnt;       // czêstotliwoœæ zegara
LONGLONG next_time=0;    // czas pozosta³y do wyrenderowania nastêpnej klatki

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

DWORD Mipmaping;
GLint szerokosc=640, wysokosc=480, ile_tekstur, Rozdzielczosc, PoraDnia,
	  Szczegolowosc, Mipmap, kolor=16, IleTekstur, KartaDzwiekowa,
      AlphaFunc, Joystick;
float OdlegloscKolaP, OdlegloscKolaL, OdlegloscKolTyl, OdlegloscDrzwi1P,
      OdlegloscDrzwi1L, OdlegloscDrzwi2P, OdlegloscDrzwi2L, OdlegloscDrzwi3P,
	  OdlegloscDrzwi3L;
Obiekt Plansza, Kab, Niebo, Fiat, ZoltyA, ZielonyA, CzerwonyA,
	   ZoltyB, ZielonyB, CzerwonyB, SwiatloStop, SwiatloWsteczne,
	   SwiatloKierL, SwiatloKierP, SwiatloDrogowe, KoloPrzednieP,
	   KoloPrzednieL, KolaTyl, StanDrzwi, Wsk, Rozklad, KabWew,
	   DrzwiA, DrzwiB;
Przystanki KolejnePrzystanki;

HWND	hWnd;

char *lpszAppName = "Symulator Autobusu";
char plik_tex[1000], StringSA[255], *SzczegPlanszy, *SzczegModelu,
	 *SzczegFiata, SciezkaDost[200], FolderMp3[255], SciezkaDoGry[255];
bool Dzien, UruchomBezMenu, TypAutobusu;

extern Kamera UstawKamere;
extern float x, z, obrot_x, MaxPredkosc, przyspieszenie, MocHamulcow,
		     predkosc, PojBaku, Paliwo, Godziny, Minuty, XStacjiPaliw,
			 ZStacjiPaliw;
extern bool StartStopSilnika, StartGry;
extern GLint NumerPrzystanka;
extern GLuint TexPlanszy[200], TexModelu[100], TexFiata[100], TexMenu[20],
		      Czcionka[1], TexRozne[100], TexRozklady[100];
static JOYCAPS Joy_Caps;


/************************** Piksel format ************************/
void SetDCPixelFormat( HDC hDC )
{
	try{
		int	nPixelFormat;

		static PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
			8,
			0, 0, 0, 0, 0, 0,
			0, 0,
			0, 0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		nPixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, nPixelFormat, &pfd);
		}
	catch(...){
		MessageBox(NULL, "Nie mogê zainicjalizowaæ systemu graficznego OpenGL! \nInicjalizacja SetDCPixelFormat nieudana!.", "B³¹d", MB_OK);
		KoniecProgramu();
	}
}
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HANDLE Semaphore;
    Semaphore = CreateSemaphore(NULL, 0, 1, "SA.exe");

	switch (uMsg)
	{
	case WM_CREATE:
		if ((Semaphore != 0) && (GetLastError() == ERROR_ALREADY_EXISTS)){
			hDC = GetDC( hWnd );
			SetDCPixelFormat( hDC );
			hRC = wglCreateContext( hDC );
			wglMakeCurrent( hDC, hRC );
			joyGetDevCaps (JOYSTICKID1, &Joy_Caps, sizeof (JOYCAPS));
			Joy_Caps.wXmin=-1;
			Joy_Caps.wXmax=1;
			//SetPriorityClass(GetCurrentProcess, REALTIME_PRIORITY_CLASS);
			//SetThreadPriority(GetCurrentThread, THREAD_PRIORITY_TIME_CRITICAL);
			Menu();
		}else{
			CloseHandle((void*)Semaphore);
			MessageBox(NULL, "Program ten jest ju¿ uruchomiony!", "B³¹d", MB_OK|MB_ICONHAND);
			KoniecProgramu();
		}
		//InitGL();
		break;

	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;

	case WM_SIZE:
		ResizeGLWindow(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		keys[wParam]=TRUE;
		break;

	case WM_KEYUP:
		keys[wParam]=FALSE;
		break;
	default:
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
	return(0L);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/************ parametry wczytywane z lini komend *******/
	switch(*lpCmdLine){
	case 'V':
		StartGry=TRUE;
		UruchomBezMenu=TRUE;
		TypAutobusu=TRUE;
		;
		break;
	case 'S':
		StartGry=TRUE;
		UruchomBezMenu=TRUE;
		TypAutobusu=FALSE;
		;
		break;
	default:
		StartGry=FALSE;
		UruchomBezMenu=FALSE;
		TypAutobusu=TRUE;
	}/******** koniec ******/

	MSG			msg;
	WNDCLASS	wndclass;

	// czy jest dostêpny dok³adniejszy licznik systemowy
	if (QueryPerformanceFrequency((LARGE_INTEGER *) &perf_cnt)){
		// oblicz czas na klatkê w oparciu o czêstotliwoœæ licznika
		time_count=static_cast<unsigned long>(perf_cnt/50);     
		QueryPerformanceCounter((LARGE_INTEGER *) &next_time);
	}

	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= MainWndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= NULL;
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= lpszAppName;

	GetCurrentDirectory(255, SciezkaDoGry);
	CzytajOpcje(); ///Czytaj opcje (rozdzielczoœæ......itd.)

	if( RegisterClass( &wndclass ) == 0)
		return FALSE;

	hWnd = CreateWindow( lpszAppName, lpszAppName,
							WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							0, 0,
							szerokosc, wysokosc,
							NULL, NULL, hInstance, NULL);

	if( NULL == hWnd )
		return FALSE;

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else{
			// pobranie czasu
			QueryPerformanceCounter((LARGE_INTEGER *) &cur_time);

			// czy ju¿ czas wyrenderowaæ klatkê?
			if (cur_time>next_time){ 

				RenderujMenu();
				//Renderuj();
				
				// ustaw czas dla nastêpnej klatki
				next_time += time_count;
				// Jeœli opuœciliœmy kilka klatek, nie renderujmy ich.
				// Ustawmy prawid³owy czas, gdy¿ w przeciwnym wypadku
				// wydajnoœæ programu spadnie, gdy¿ bêdzie musieli nieustannie
				// renderowaæ klatkê
				if (next_time < cur_time) next_time = cur_time + time_count;
			}
		}
	}
	
	return msg.wParam;
}
/********************** niszcz okno ******************************/
void KoniecProgramu(void)
{
		ChangeDisplaySettings(NULL,0);
		CzyscTabliceTekstur();
		NiszczCzcionki();
		NiszczDzwiek();
		wglMakeCurrent( hDC, NULL );
		wglDeleteContext( hRC );
		hRC=NULL;
		hDC=NULL;
		PostQuitMessage( 0 );
}
/*********************** Wstêpne ustawienia **********************/
void InitGL(void)
{
	//StartStopSilnika=TRUE;// wy³. silnik
	predkosc=0.0f;	// prêdkoœæ startowa równa 0
	UstawKamere.Odleglosc=5;
	UstawKamere.Wysokosc=1.6f;
	UstawKamere.PunktPatrzenia=-40;
	UstawKamere.Obrot=-0.75f;
	UstawKamere.Obrot_a=0;
	NumerPrzystanka=0;

	CzytajTeksury(SzczegPlanszy, 0);
	CzytajTeksury(SzczegModelu, 1);
	CzytajTeksury(SzczegFiata, 2);
	CzytajTeksury("Dane/Wskazniki/Tekstury.tex", 5);
	CzytajTeksury("Dane/Rozklady/128/Tekstury.tex", 6);

	/*************** Miasto *************/
	CzytajSA(Plansza, "Dane/Trasy/Test/Plansza.sa");

	if(TypAutobusu==TRUE){
	/*********** Model autobusu ********/
	CzytajSA(Kab, "Dane/Autobusy/Volvo7000/ModelZew.sa");
	CzytajSA(KabWew, "Dane/Autobusy/Volvo7000/ModelWew.sa");
	CzytajSA(SwiatloStop, "Dane/Autobusy/Volvo7000/Swiatlostop.sa");
	CzytajSA(SwiatloWsteczne, "Dane/Autobusy/Volvo7000/Swiatlowsteczne.sa");
	CzytajSA(SwiatloKierL, "Dane/Autobusy/Volvo7000/SwiatlokierL.sa");
	CzytajSA(SwiatloKierP, "Dane/Autobusy/Volvo7000/SwiatlokierP.sa");
	CzytajSA(SwiatloDrogowe, "Dane/Autobusy/Volvo7000/Swiatlodrogowe.sa");
	CzytajSA(KoloPrzednieL, "Dane/Autobusy/Volvo7000/KoloprzednieL.sa");
	CzytajSA(KoloPrzednieP, "Dane/Autobusy/Volvo7000/KoloprzednieP.sa");
	CzytajSA(KolaTyl, "Dane/Autobusy/Volvo7000/Kolatylnie.sa");
	CzytajSA(StanDrzwi, "Dane/Autobusy/Volvo7000/Standrzwi.sa");
	CzytajSA(DrzwiA, "Dane/Autobusy/Volvo7000/DrzwiA.sa");
	CzytajSA(DrzwiB, "Dane/Autobusy/Volvo7000/DrzwiB.sa");
	}else{
	/*********** Model autobusu ********/
	CzytajSA(Kab, "Dane/Autobusy/Urbino12/ModelZew.sa");
	CzytajSA(KabWew, "Dane/Autobusy/Urbino12/ModelWew.sa");
	CzytajSA(SwiatloStop, "Dane/Autobusy/Urbino12/Swiatlostop.sa");
	CzytajSA(SwiatloWsteczne, "Dane/Autobusy/Urbino12/Swiatlowsteczne.sa");
	CzytajSA(SwiatloKierL, "Dane/Autobusy/Urbino12/SwiatlokierL.sa");
	CzytajSA(SwiatloKierP, "Dane/Autobusy/Urbino12/SwiatlokierP.sa");
	CzytajSA(SwiatloDrogowe, "Dane/Autobusy/Urbino12/Swiatlodrogowe.sa");
	CzytajSA(KoloPrzednieL, "Dane/Autobusy/Urbino12/KoloprzednieL.sa");
	CzytajSA(KoloPrzednieP, "Dane/Autobusy/Urbino12/KoloprzednieP.sa");
	CzytajSA(KolaTyl, "Dane/Autobusy/Urbino12/Kolatylnie.sa");
	CzytajSA(StanDrzwi, "Dane/Autobusy/Urbino12/Standrzwi.sa");
	CzytajSA(DrzwiA, "Dane/Autobusy/Urbino12/DrzwiA.sa");
	CzytajSA(DrzwiB, "Dane/Autobusy/Urbino12/DrzwiB.sa");
	}

	/************* model fiata **************/
	CzytajSA(Fiat, "Dane/Pojazdy/F125/Model.sa");
	/************* niebo **************/
	CzytajSA(Niebo, "Dane/Trasy/Test/Niebo.sa");
	/************* œwiat³a drogowe ********/
	CzytajSA(ZoltyA, "Dane/Trasy/Test/ZoltyA.sa");
	CzytajSA(ZoltyB, "Dane/Trasy/Test/ZoltyB.sa");
	CzytajSA(CzerwonyA, "Dane/Trasy/Test/CzerwonyA.sa");
	CzytajSA(CzerwonyB, "Dane/Trasy/Test/CzerwonyB.sa");
	CzytajSA(ZielonyA, "Dane/Trasy/Test/ZielonyA.sa");
	CzytajSA(ZielonyB, "Dane/Trasy/Test/ZielonyB.sa");
	CzytajSA(Rozklad, "Dane/Rozklady/128/Rozklad.sa");
	CzytajSA(Wsk, "Dane/Wskazniki/Wskazniki.sa");

	TworzListyWyswietlania();

	CzytajParametryBusa();
	CzytajParametryMisji();

	LadujTekstury("Dane/czcionka.gfx", 0, 3);
	BudujCzcionki();

	CzytajPrzystanki();

	SzukajPlikow(FolderMp3);

}
/********************** Zmiana rozmiaru okna *********************/
void ResizeGLWindow(int szerokosc, int wysokosc)
{
	if (wysokosc==0)	wysokosc=1;

	glViewport(0, 0, szerokosc, wysokosc);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective( 45.0f, static_cast<float>(szerokosc)/static_cast<float>(wysokosc), 0.1, 440/*440*/ );
	//glFrustum(-0.5, 0.5, -0.5, 0.5, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
/**************** ³adowanie tekstur ******************************/
void CzytajTeksury(char *sciezka, int Tablica) 
{
//TexPlanszy = new unsigned int[];
	try{
	FILE *tekstury; 
	tekstury = fopen(sciezka, "r"); // otwarcie naszego pliku 

	fscanf(tekstury, "%d\n", &IleTekstur);

	for (int i = 0; i < IleTekstur; i++){
	 fscanf(tekstury, "%s\n", &SciezkaDost);
		char *sciezka_a = SciezkaDost;
		LadujTekstury(sciezka_a, i, Tablica);
	}
	fclose(tekstury); // zamkniêcie pliku
	}catch(...){
		MessageBox(NULL, "Nie mogê za³adowaæ pliku 'Tekstury.tex'.\nUruchamienie programu zostanie teraz zakoñczone.", "B³¹d", MB_OK|MB_ICONHAND);
		KoniecProgramu();
	}
}
/****************** Czyszczenie tablic tekstur *******************/
void CzyscTabliceTekstur()
{

//delete[] TexPlanszy;

}
/**************** ³adowanie X i Z przystanków ********************/
void CzytajPrzystanki() 
{
	try{
	FILE *przystanki; 
	przystanki = fopen("Dane/Rozklady/128/Przystanki.roz", "r"); // otwarcie naszego pliku 

	fscanf(przystanki, "%d\n", &KolejnePrzystanki.IloscPrzystankow);

	for (int i = 0; i < KolejnePrzystanki.IloscPrzystankow; i++){
		fscanf(przystanki, "%f %f", 
		&KolejnePrzystanki.X[i], &KolejnePrzystanki.Z[i]);
	}
	fclose(przystanki); // zamkniêcie pliku
	}catch(...){
		MessageBox(NULL, "Nie mogê za³adowaæ pliku 'Przystanki.roz'!\nUruchamienie programu zostanie zakoñczone.", "B³¹d", MB_OK|MB_ICONHAND);
		KoniecProgramu();
	}
}
/**************** ³adowanie planszy ******************************/
void CzytajSA(Obiekt &JakiObiekt, char *DostDoPliku) 
{
	try{
	FILE *PlikiSA; 
	PlikiSA = fopen(DostDoPliku, "r"); // otwarcie naszego pliku 

	fscanf(PlikiSA, "%d\n", &JakiObiekt.ilosc_prymitywow);

	for (int i = 0; i < JakiObiekt.ilosc_prymitywow; i++){
		fgets(StringSA, 255, PlikiSA);
		if(StringSA!="/" || StringSA!="\n"){
			fscanf(PlikiSA, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d", 
			&JakiObiekt.Wierzcholki[i][0], &JakiObiekt.Wierzcholki[i][1], &JakiObiekt.Wierzcholki[i][2], 
			&JakiObiekt.Wierzcholki[i][3], &JakiObiekt.Wierzcholki[i][4], &JakiObiekt.Wierzcholki[i][5], 
			&JakiObiekt.Wierzcholki[i][6], &JakiObiekt.Wierzcholki[i][7], &JakiObiekt.Wierzcholki[i][8],
			&JakiObiekt.Wektor_tex[i][0], &JakiObiekt.Wektor_tex[i][1], &JakiObiekt.Wektor_tex[i][2], 
			&JakiObiekt.Wektor_tex[i][3], &JakiObiekt.Wektor_tex[i][4], &JakiObiekt.Wektor_tex[i][5],
			&JakiObiekt.ID_TEX[i]);
		}
	}
	fclose(PlikiSA); // zamkniêcie pliku
	}catch(...){
		MessageBox(NULL, "Nie mogê za³adowaæ pliku SA!\nUruchamienie programu zostanie zakoñczone.", "B³¹d", MB_OK|MB_ICONHAND);
		KoniecProgramu();
	}
}
/***************** Czytaj parametry pojazdu ********************/
void CzytajParametryBusa()
{
	try{
	FILE *ParametryBusa; 
	if(TypAutobusu==TRUE){
		ParametryBusa = fopen("Dane/Autobusy/Volvo7000/Parametry.busa", "r"); // otwarcie naszego pliku 
	}else{
		ParametryBusa = fopen("Dane/Autobusy/Urbino12/Parametry.busa", "r"); // otwarcie naszego pliku 
	}

	fscanf(ParametryBusa, "%f\n", &MaxPredkosc);
	fscanf(ParametryBusa, "%f\n", &przyspieszenie);
	fscanf(ParametryBusa, "%f\n", &MocHamulcow);
	fscanf(ParametryBusa, "%f\n", &PojBaku);
	fscanf(ParametryBusa, "%f\n", &OdlegloscKolaL);
	fscanf(ParametryBusa, "%f\n", &OdlegloscKolaP);
	fscanf(ParametryBusa, "%f\n", &OdlegloscKolTyl);
	fscanf(ParametryBusa, "%f\n", &OdlegloscDrzwi1P);
	fscanf(ParametryBusa, "%f\n", &OdlegloscDrzwi1L);
	fscanf(ParametryBusa, "%f\n", &OdlegloscDrzwi2P);
	fscanf(ParametryBusa, "%f\n", &OdlegloscDrzwi2L);
	fscanf(ParametryBusa, "%f\n", &OdlegloscDrzwi3P);
	fscanf(ParametryBusa, "%f\n", &OdlegloscDrzwi3L);
	}catch(...){
		MessageBox(NULL, "Nie mogê za³adowaæ pliku 'Parametry.busa'.\nUruchamienie programu zostanie teraz zakoñczone.", "B³¹d", MB_OK|MB_ICONHAND);
		KoniecProgramu();
	}
}
/***************** Czytaj parametry misji ************************/
void CzytajParametryMisji()
{
	try{
	FILE *ParametryMisji; 
	ParametryMisji = fopen("Dane/Rozklady/128/Parametry.misji", "r"); // otwarcie naszego pliku 

	fscanf(ParametryMisji, "%f\n", &x);
	fscanf(ParametryMisji, "%f\n", &z);
	fscanf(ParametryMisji, "%f\n", &obrot_x);
	fscanf(ParametryMisji, "%f\n", &Paliwo);
	fscanf(ParametryMisji, "%f\n", &Godziny);
	fscanf(ParametryMisji, "%f\n", &Minuty);
	fscanf(ParametryMisji, "%f\n", &XStacjiPaliw);
	fscanf(ParametryMisji, "%f\n", &ZStacjiPaliw);
	}catch(...){
		MessageBox(NULL, "Nie mogê za³adowaæ pliku 'ParametryMisji'.\nUruchamienie programu zostanie teraz zakoñczone.", "B³¹d", MB_OK|MB_ICONHAND);
		KoniecProgramu();
	}
}
/**************** ³adowanie opcji ******************************/
void CzytajOpcje() 
{
	try{
	char SciezkaPelna[255];
	lstrcpy(SciezkaPelna, SciezkaDoGry);
	lstrcat(SciezkaPelna, "/Opcje.ini");

	Rozdzielczosc=GetPrivateProfileInt("Opcje", "Rozdzielczosc", 0, SciezkaPelna);
	Szczegolowosc=GetPrivateProfileInt("Opcje", "JakoscTekstur", 1, SciezkaPelna);
	Mipmap=GetPrivateProfileInt("Opcje", "Mipmaping", 0, SciezkaPelna);
	KartaDzwiekowa=GetPrivateProfileInt("Opcje", "KartaDzwiekowa", 0, SciezkaPelna);
	PoraDnia=GetPrivateProfileInt("Opcje", "Zachmurzenie", 0, SciezkaPelna);
	AlphaFunc=GetPrivateProfileInt("Opcje", "AlphaFunc", 0, SciezkaPelna);
	Joystick=GetPrivateProfileInt("Opcje", "Joystick", 0, SciezkaPelna);
	GetPrivateProfileString("Opcje", "FolderMp3", "Dane/Radio/", FolderMp3, 255, SciezkaPelna);
	//strcpy(StringFolderMp3, FolderMp3 );
	//SzukajPlikow("Dane/Radio/");
	switch(Rozdzielczosc){
	case 0:
		szerokosc=640;
		wysokosc=480;
		kolor=16;
		break;
	case 1:
		szerokosc=800;
		wysokosc=600;
		kolor=16;
		break;
	case 2:
		szerokosc=1024;
		wysokosc=768;
		kolor=16;
		break;
	case 3:
		szerokosc=640;
		wysokosc=480;
		kolor=32;
		break;
	case 4:
		szerokosc=800;
		wysokosc=600;
		kolor=32;
		break;
	case 5:
		szerokosc=1024;
		wysokosc=768;
		kolor=32;
		break;
	}
	switch(Szczegolowosc){
	case 0:
		SzczegPlanszy="Dane/Trasy/Test/TeksturyW.tex";
		if(TypAutobusu==TRUE){
			SzczegModelu="Dane/Autobusy/Volvo7000/TeksturyW.tex";
		}else{
			SzczegModelu="Dane/Autobusy/Urbino12/TeksturyW.tex";
		}
		SzczegFiata="Dane/Pojazdy/F125/TeksturyW.tex";
		break;
	case 1:
		SzczegPlanszy="Dane/Trasy/Test/TeksturyN.tex";
		if(TypAutobusu==TRUE){
			SzczegModelu="Dane/Autobusy/Volvo7000/TeksturyN.tex";
		}else{
			SzczegModelu="Dane/Autobusy/Urbino12/TeksturyN.tex";
		}
		SzczegFiata="Dane/Pojazdy/F125/TeksturyN.tex";
		break;
	}
	switch(PoraDnia){
	case 0:
		Dzien=TRUE;
		break;
	case 1:
		Dzien=FALSE;
		break;
	}
	switch(Mipmap){
	case 0:
		Mipmaping = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case 1:
		Mipmaping = GL_LINEAR;
		break;
	}

	}catch(...){
		MessageBox(NULL, "Nie mogê za³adowaæ pliku 'Opcje.ini'!\nUruchom najpierw program 'Opcje' w celu skonfigurowania gry.", "B³¹d", MB_OK|MB_ICONHAND);
		KoniecProgramu();
	}
}
/******************** Tworzenie list wyœwietlania ****************/
void TworzListyWyswietlania()
{
	glNewList(PLANSZA, GL_COMPILE);  // ca³y œwiat 3D
	for (int i = 0; i < Plansza.ilosc_prymitywow; i++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[Plansza.ID_TEX[i]]);
		glBegin(GL_TRIANGLES);
		//glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(Plansza.Wektor_tex[i][0], Plansza.Wektor_tex[i][1]);
		glVertex3f(Plansza.Wierzcholki[i][0],Plansza.Wierzcholki[i][1],Plansza.Wierzcholki[i][2]);
		glTexCoord2f(Plansza.Wektor_tex[i][2], Plansza.Wektor_tex[i][3]);
		glVertex3f(Plansza.Wierzcholki[i][3],Plansza.Wierzcholki[i][4],Plansza.Wierzcholki[i][5]);
		glTexCoord2f(Plansza.Wektor_tex[i][4], Plansza.Wektor_tex[i][5]);
		glVertex3f(Plansza.Wierzcholki[i][6],Plansza.Wierzcholki[i][7],Plansza.Wierzcholki[i][8]);
		glEnd();
	}
	glEndList();

	glNewList(KIEROWNICA, GL_COMPILE); // kierownica
	for (int q = 0; q < 2; q++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[KabWew.ID_TEX[q]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(KabWew.Wektor_tex[q][0], KabWew.Wektor_tex[q][1]);
		glVertex3f(KabWew.Wierzcholki[q][0],KabWew.Wierzcholki[q][1],KabWew.Wierzcholki[q][2]);
		glTexCoord2f(KabWew.Wektor_tex[q][2], KabWew.Wektor_tex[q][3]);
		glVertex3f(KabWew.Wierzcholki[q][3],KabWew.Wierzcholki[q][4],KabWew.Wierzcholki[q][5]);
		glTexCoord2f(KabWew.Wektor_tex[q][4], KabWew.Wektor_tex[q][5]);
		glVertex3f(KabWew.Wierzcholki[q][6],KabWew.Wierzcholki[q][7],KabWew.Wierzcholki[q][8]);
		glEnd();
	}
	glEndList();

	glNewList(KABINA, GL_COMPILE);  // kabina
	for (int w = 0; w < Kab.ilosc_prymitywow; w++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[Kab.ID_TEX[w]]);
		glBegin(GL_TRIANGLES);
		//glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(Kab.Wektor_tex[w][0], Kab.Wektor_tex[w][1]);
		glVertex3f(Kab.Wierzcholki[w][0],Kab.Wierzcholki[w][1],Kab.Wierzcholki[w][2]);
		glTexCoord2f(Kab.Wektor_tex[w][2], Kab.Wektor_tex[w][3]);
		glVertex3f(Kab.Wierzcholki[w][3],Kab.Wierzcholki[w][4],Kab.Wierzcholki[w][5]);
		glTexCoord2f(Kab.Wektor_tex[w][4], Kab.Wektor_tex[w][5]);
		glVertex3f(Kab.Wierzcholki[w][6],Kab.Wierzcholki[w][7],Kab.Wierzcholki[w][8]);
		glEnd();
	}
	glEndList();

	glNewList(KABINAWEW, GL_COMPILE);  // kabina wew
	for (int D = 6; D < KabWew.ilosc_prymitywow; D++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[KabWew.ID_TEX[D]]);
		glBegin(GL_TRIANGLES);
		//glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(KabWew.Wektor_tex[D][0], KabWew.Wektor_tex[D][1]);
		glVertex3f(KabWew.Wierzcholki[D][0],KabWew.Wierzcholki[D][1],KabWew.Wierzcholki[D][2]);
		glTexCoord2f(KabWew.Wektor_tex[D][2], KabWew.Wektor_tex[D][3]);
		glVertex3f(KabWew.Wierzcholki[D][3],KabWew.Wierzcholki[D][4],KabWew.Wierzcholki[D][5]);
		glTexCoord2f(KabWew.Wektor_tex[D][4], KabWew.Wektor_tex[D][5]);
		glVertex3f(KabWew.Wierzcholki[D][6],KabWew.Wierzcholki[D][7],KabWew.Wierzcholki[D][8]);
		glEnd();
	}
	glEndList();

	glNewList(NIEBO, GL_COMPILE);  // niebo
	for (int n = 0; n < Niebo.ilosc_prymitywow; n++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[Niebo.ID_TEX[n]]);
		glBegin(GL_TRIANGLES);
		//glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(Niebo.Wektor_tex[n][0], Niebo.Wektor_tex[n][1]);
		glVertex3f(Niebo.Wierzcholki[n][0],Niebo.Wierzcholki[n][1],Niebo.Wierzcholki[n][2]);
		glTexCoord2f(Niebo.Wektor_tex[n][2], Niebo.Wektor_tex[n][3]);
		glVertex3f(Niebo.Wierzcholki[n][3],Niebo.Wierzcholki[n][4],Niebo.Wierzcholki[n][5]);
		glTexCoord2f(Niebo.Wektor_tex[n][4], Niebo.Wektor_tex[n][5]);
		glVertex3f(Niebo.Wierzcholki[n][6],Niebo.Wierzcholki[n][7],Niebo.Wierzcholki[n][8]);
		glEnd();
	}
	glEndList();

	glNewList(WSKAZOWKA, GL_COMPILE); // kierownica
	for (int s = 2; s < 4; s++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[KabWew.ID_TEX[s]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(KabWew.Wektor_tex[s][0], KabWew.Wektor_tex[s][1]);
		glVertex3f(KabWew.Wierzcholki[s][0],KabWew.Wierzcholki[s][1],KabWew.Wierzcholki[s][2]);
		glTexCoord2f(KabWew.Wektor_tex[s][2], KabWew.Wektor_tex[s][3]);
		glVertex3f(KabWew.Wierzcholki[s][3],KabWew.Wierzcholki[s][4],KabWew.Wierzcholki[s][5]);
		glTexCoord2f(KabWew.Wektor_tex[s][4], KabWew.Wektor_tex[s][5]);
		glVertex3f(KabWew.Wierzcholki[s][6],KabWew.Wierzcholki[s][7],KabWew.Wierzcholki[s][8]);
		glEnd();
	}
	glEndList();

	glNewList(KIERL, GL_COMPILE); // kierunkowskaz lewy
	for (int x = 0; x < SwiatloKierL.ilosc_prymitywow; x++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[SwiatloKierL.ID_TEX[x]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(SwiatloKierL.Wektor_tex[x][0], SwiatloKierL.Wektor_tex[x][1]);
		glVertex3f(SwiatloKierL.Wierzcholki[x][0],SwiatloKierL.Wierzcholki[x][1],SwiatloKierL.Wierzcholki[x][2]);
		glTexCoord2f(SwiatloKierL.Wektor_tex[x][2], SwiatloKierL.Wektor_tex[x][3]);
		glVertex3f(SwiatloKierL.Wierzcholki[x][3],SwiatloKierL.Wierzcholki[x][4],SwiatloKierL.Wierzcholki[x][5]);
		glTexCoord2f(SwiatloKierL.Wektor_tex[x][4], SwiatloKierL.Wektor_tex[x][5]);
		glVertex3f(SwiatloKierL.Wierzcholki[x][6],SwiatloKierL.Wierzcholki[x][7],SwiatloKierL.Wierzcholki[x][8]);
		glEnd();
	}
	glEndList();

	glNewList(KIERP, GL_COMPILE); // kierunkowskaz prawy
	for (int z = 0; z < SwiatloKierP.ilosc_prymitywow; z++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[SwiatloKierP.ID_TEX[z]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(SwiatloKierP.Wektor_tex[z][0], SwiatloKierP.Wektor_tex[z][1]);
		glVertex3f(SwiatloKierP.Wierzcholki[z][0],SwiatloKierP.Wierzcholki[z][1],SwiatloKierP.Wierzcholki[z][2]);
		glTexCoord2f(SwiatloKierP.Wektor_tex[z][2], SwiatloKierP.Wektor_tex[z][3]);
		glVertex3f(SwiatloKierP.Wierzcholki[z][3],SwiatloKierP.Wierzcholki[z][4],SwiatloKierP.Wierzcholki[z][5]);
		glTexCoord2f(SwiatloKierP.Wektor_tex[z][4], SwiatloKierP.Wektor_tex[z][5]);
		glVertex3f(SwiatloKierP.Wierzcholki[z][6],SwiatloKierP.Wierzcholki[z][7],SwiatloKierP.Wierzcholki[z][8]);
		glEnd();
	}
	glEndList();

	/*glNewList(TARCZAPRED, GL_COMPILE); // Tarcza prêdkoœciomierza
	for (int c = 21; c < 23; c++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[Kab.ID_TEX[c]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(Kab.Wektor_tex[c][0], Kab.Wektor_tex[c][1]);
		glVertex3f(Kab.Wierzcholki[c][0],Kab.Wierzcholki[c][1],Kab.Wierzcholki[c][2]);
		glTexCoord2f(Kab.Wektor_tex[c][2], Kab.Wektor_tex[c][3]);
		glVertex3f(Kab.Wierzcholki[c][3],Kab.Wierzcholki[c][4],Kab.Wierzcholki[c][5]);
		glTexCoord2f(Kab.Wektor_tex[c][4], Kab.Wektor_tex[c][5]);
		glVertex3f(Kab.Wierzcholki[c][6],Kab.Wierzcholki[c][7],Kab.Wierzcholki[c][8]);
		glEnd();
	}
	glEndList();*/

	glNewList(SWIATLA, GL_COMPILE); // œwiat³a drogowe
	for (int v = 0; v < SwiatloDrogowe.ilosc_prymitywow; v++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[SwiatloDrogowe.ID_TEX[v]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(SwiatloDrogowe.Wektor_tex[v][0], SwiatloDrogowe.Wektor_tex[v][1]);
		glVertex3f(SwiatloDrogowe.Wierzcholki[v][0],SwiatloDrogowe.Wierzcholki[v][1],SwiatloDrogowe.Wierzcholki[v][2]);
		glTexCoord2f(SwiatloDrogowe.Wektor_tex[v][2], SwiatloDrogowe.Wektor_tex[v][3]);
		glVertex3f(SwiatloDrogowe.Wierzcholki[v][3],SwiatloDrogowe.Wierzcholki[v][4],SwiatloDrogowe.Wierzcholki[v][5]);
		glTexCoord2f(SwiatloDrogowe.Wektor_tex[v][4], SwiatloDrogowe.Wektor_tex[v][5]);
		glVertex3f(SwiatloDrogowe.Wierzcholki[v][6],SwiatloDrogowe.Wierzcholki[v][7],SwiatloDrogowe.Wierzcholki[v][8]);
		glEnd();
	}
	glEndList();

	glNewList(SWIATLASTOP, GL_COMPILE); // œwiat³a stop (hamulec)
	for (int b = 0; b < SwiatloStop.ilosc_prymitywow; b++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[SwiatloStop.ID_TEX[b]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(SwiatloStop.Wektor_tex[b][0], SwiatloStop.Wektor_tex[b][1]);
		glVertex3f(SwiatloStop.Wierzcholki[b][0],SwiatloStop.Wierzcholki[b][1],SwiatloStop.Wierzcholki[b][2]);
		glTexCoord2f(SwiatloStop.Wektor_tex[b][2], SwiatloStop.Wektor_tex[b][3]);
		glVertex3f(SwiatloStop.Wierzcholki[b][3],SwiatloStop.Wierzcholki[b][4],SwiatloStop.Wierzcholki[b][5]);
		glTexCoord2f(SwiatloStop.Wektor_tex[b][4], SwiatloStop.Wektor_tex[b][5]);
		glVertex3f(SwiatloStop.Wierzcholki[b][6],SwiatloStop.Wierzcholki[b][7],SwiatloStop.Wierzcholki[b][8]);
		glEnd();
	}
	glEndList();

	glNewList(SWIATLATYL, GL_COMPILE); // œwiat³a wsteczny
	for (int m = 0; m < SwiatloWsteczne.ilosc_prymitywow; m++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[SwiatloWsteczne.ID_TEX[m]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(SwiatloWsteczne.Wektor_tex[m][0], SwiatloWsteczne.Wektor_tex[m][1]);
		glVertex3f(SwiatloWsteczne.Wierzcholki[m][0],SwiatloWsteczne.Wierzcholki[m][1],SwiatloWsteczne.Wierzcholki[m][2]);
		glTexCoord2f(SwiatloWsteczne.Wektor_tex[m][2], SwiatloWsteczne.Wektor_tex[m][3]);
		glVertex3f(SwiatloWsteczne.Wierzcholki[m][3],SwiatloWsteczne.Wierzcholki[m][4],SwiatloWsteczne.Wierzcholki[m][5]);
		glTexCoord2f(SwiatloWsteczne.Wektor_tex[m][4], SwiatloWsteczne.Wektor_tex[m][5]);
		glVertex3f(SwiatloWsteczne.Wierzcholki[m][6],SwiatloWsteczne.Wierzcholki[m][7],SwiatloWsteczne.Wierzcholki[m][8]);
		glEnd();
	}
	glEndList();

	glNewList(KOLO, GL_COMPILE); // œwiat³a wsteczny
	for (int l = 0; l < KolaTyl.ilosc_prymitywow; l++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[KolaTyl.ID_TEX[l]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(KolaTyl.Wektor_tex[l][0], KolaTyl.Wektor_tex[l][1]);
		glVertex3f(KolaTyl.Wierzcholki[l][0],KolaTyl.Wierzcholki[l][1],KolaTyl.Wierzcholki[l][2]);
		glTexCoord2f(KolaTyl.Wektor_tex[l][2], KolaTyl.Wektor_tex[l][3]);
		glVertex3f(KolaTyl.Wierzcholki[l][3],KolaTyl.Wierzcholki[l][4],KolaTyl.Wierzcholki[l][5]);
		glTexCoord2f(KolaTyl.Wektor_tex[l][4], KolaTyl.Wektor_tex[l][5]);
		glVertex3f(KolaTyl.Wierzcholki[l][6],KolaTyl.Wierzcholki[l][7],KolaTyl.Wierzcholki[l][8]);
		glEnd();
	}
	glEndList();

	glNewList(KOLOPRZODP, GL_COMPILE); // œwiat³a wsteczny
	for (int k = 0; k < KoloPrzednieP.ilosc_prymitywow; k++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[KoloPrzednieP.ID_TEX[k]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(KoloPrzednieP.Wektor_tex[k][0], KoloPrzednieP.Wektor_tex[k][1]);
		glVertex3f(KoloPrzednieP.Wierzcholki[k][0],KoloPrzednieP.Wierzcholki[k][1],KoloPrzednieP.Wierzcholki[k][2]);
		glTexCoord2f(KoloPrzednieP.Wektor_tex[k][2], KoloPrzednieP.Wektor_tex[k][3]);
		glVertex3f(KoloPrzednieP.Wierzcholki[k][3],KoloPrzednieP.Wierzcholki[k][4],KoloPrzednieP.Wierzcholki[k][5]);
		glTexCoord2f(KoloPrzednieP.Wektor_tex[k][4], KoloPrzednieP.Wektor_tex[k][5]);
		glVertex3f(KoloPrzednieP.Wierzcholki[k][6],KoloPrzednieP.Wierzcholki[k][7],KoloPrzednieP.Wierzcholki[k][8]);
		glEnd();
	}
	glEndList();

	glNewList(KOLOPRZODL, GL_COMPILE); // œwiat³a wsteczny
	for (int j = 0; j < KoloPrzednieL.ilosc_prymitywow; j++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[KoloPrzednieL.ID_TEX[j]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(KoloPrzednieL.Wektor_tex[j][0], KoloPrzednieL.Wektor_tex[j][1]);
		glVertex3f(KoloPrzednieL.Wierzcholki[j][0],KoloPrzednieL.Wierzcholki[j][1],KoloPrzednieL.Wierzcholki[j][2]);
		glTexCoord2f(KoloPrzednieL.Wektor_tex[j][2], KoloPrzednieL.Wektor_tex[j][3]);
		glVertex3f(KoloPrzednieL.Wierzcholki[j][3],KoloPrzednieL.Wierzcholki[j][4],KoloPrzednieL.Wierzcholki[j][5]);
		glTexCoord2f(KoloPrzednieL.Wektor_tex[j][4], KoloPrzednieL.Wektor_tex[j][5]);
		glVertex3f(KoloPrzednieL.Wierzcholki[j][6],KoloPrzednieL.Wierzcholki[j][7],KoloPrzednieL.Wierzcholki[j][8]);
		glEnd();
	}
	glEndList();

	glNewList(MODELFIATA, GL_COMPILE); // Model fiata125
	for (int t = 0; t < Fiat.ilosc_prymitywow; t++){
		glBindTexture(GL_TEXTURE_2D,TexFiata[Fiat.ID_TEX[t]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(Fiat.Wektor_tex[t][0], Fiat.Wektor_tex[t][1]);
		glVertex3f(Fiat.Wierzcholki[t][0],Fiat.Wierzcholki[t][1],Fiat.Wierzcholki[t][2]);
		glTexCoord2f(Fiat.Wektor_tex[t][2], Fiat.Wektor_tex[t][3]);
		glVertex3f(Fiat.Wierzcholki[t][3],Fiat.Wierzcholki[t][4],Fiat.Wierzcholki[t][5]);
		glTexCoord2f(Fiat.Wektor_tex[t][4], Fiat.Wektor_tex[t][5]);
		glVertex3f(Fiat.Wierzcholki[t][6],Fiat.Wierzcholki[t][7],Fiat.Wierzcholki[t][8]);
		glEnd();
	}
	glEndList();

	glNewList(CZERWONYA, GL_COMPILE); // Model fiata125
	for (int y = 0; y < CzerwonyA.ilosc_prymitywow; y++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[CzerwonyA.ID_TEX[y]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(CzerwonyA.Wektor_tex[y][0], CzerwonyA.Wektor_tex[y][1]);
		glVertex3f(CzerwonyA.Wierzcholki[y][0],CzerwonyA.Wierzcholki[y][1],CzerwonyA.Wierzcholki[y][2]);
		glTexCoord2f(CzerwonyA.Wektor_tex[y][2], CzerwonyA.Wektor_tex[y][3]);
		glVertex3f(CzerwonyA.Wierzcholki[y][3],CzerwonyA.Wierzcholki[y][4],CzerwonyA.Wierzcholki[y][5]);
		glTexCoord2f(CzerwonyA.Wektor_tex[y][4], CzerwonyA.Wektor_tex[y][5]);
		glVertex3f(CzerwonyA.Wierzcholki[y][6],CzerwonyA.Wierzcholki[y][7],CzerwonyA.Wierzcholki[y][8]);
		glEnd();
	}
	glEndList();

	glNewList(ZOLTYA, GL_COMPILE); // ¿ó³te œwiat³a drogowe
	for (int e = 0; e < ZoltyA.ilosc_prymitywow; e++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[ZoltyA.ID_TEX[e]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(ZoltyA.Wektor_tex[e][0], ZoltyA.Wektor_tex[e][1]);
		glVertex3f(ZoltyA.Wierzcholki[e][0],ZoltyA.Wierzcholki[e][1],ZoltyA.Wierzcholki[e][2]);
		glTexCoord2f(ZoltyA.Wektor_tex[e][2], ZoltyA.Wektor_tex[e][3]);
		glVertex3f(ZoltyA.Wierzcholki[e][3],ZoltyA.Wierzcholki[e][4],ZoltyA.Wierzcholki[e][5]);
		glTexCoord2f(ZoltyA.Wektor_tex[e][4], ZoltyA.Wektor_tex[e][5]);
		glVertex3f(ZoltyA.Wierzcholki[e][6],ZoltyA.Wierzcholki[e][7],ZoltyA.Wierzcholki[e][8]);
		glEnd();
	}
	glEndList();

	glNewList(ZIELONYA, GL_COMPILE); // Model fiata125
	for (int r = 0; r < ZielonyA.ilosc_prymitywow; r++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[ZielonyA.ID_TEX[r]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(ZielonyA.Wektor_tex[r][0], ZielonyA.Wektor_tex[r][1]);
		glVertex3f(ZielonyA.Wierzcholki[r][0],ZielonyA.Wierzcholki[r][1],ZielonyA.Wierzcholki[r][2]);
		glTexCoord2f(ZielonyA.Wektor_tex[r][2], ZielonyA.Wektor_tex[r][3]);
		glVertex3f(ZielonyA.Wierzcholki[r][3],ZielonyA.Wierzcholki[r][4],ZielonyA.Wierzcholki[r][5]);
		glTexCoord2f(ZielonyA.Wektor_tex[r][4], ZielonyA.Wektor_tex[r][5]);
		glVertex3f(ZielonyA.Wierzcholki[r][6],ZielonyA.Wierzcholki[r][7],ZielonyA.Wierzcholki[r][8]);
		glEnd();
	}
	glEndList();

	glNewList(CZERWONYB, GL_COMPILE); // Model fiata125
	for (int a = 0; a < CzerwonyB.ilosc_prymitywow; a++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[CzerwonyB.ID_TEX[a]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(CzerwonyB.Wektor_tex[a][0], CzerwonyB.Wektor_tex[a][1]);
		glVertex3f(CzerwonyB.Wierzcholki[a][0],CzerwonyB.Wierzcholki[a][1],CzerwonyB.Wierzcholki[a][2]);
		glTexCoord2f(CzerwonyB.Wektor_tex[a][2], CzerwonyB.Wektor_tex[a][3]);
		glVertex3f(CzerwonyB.Wierzcholki[a][3],CzerwonyB.Wierzcholki[a][4],CzerwonyB.Wierzcholki[a][5]);
		glTexCoord2f(CzerwonyB.Wektor_tex[a][4], CzerwonyB.Wektor_tex[a][5]);
		glVertex3f(CzerwonyB.Wierzcholki[a][6],CzerwonyB.Wierzcholki[a][7],CzerwonyB.Wierzcholki[a][8]);
		glEnd();
	}
	glEndList();

	glNewList(ZOLTYB, GL_COMPILE); // ¿ó³te œwiat³a drogowe
	for (int d = 0; d < ZoltyB.ilosc_prymitywow; d++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[ZoltyB.ID_TEX[d]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(ZoltyB.Wektor_tex[d][0], ZoltyB.Wektor_tex[d][1]);
		glVertex3f(ZoltyB.Wierzcholki[d][0],ZoltyB.Wierzcholki[d][1],ZoltyB.Wierzcholki[d][2]);
		glTexCoord2f(ZoltyB.Wektor_tex[d][2], ZoltyB.Wektor_tex[d][3]);
		glVertex3f(ZoltyB.Wierzcholki[d][3],ZoltyB.Wierzcholki[d][4],ZoltyB.Wierzcholki[d][5]);
		glTexCoord2f(ZoltyB.Wektor_tex[d][4], ZoltyB.Wektor_tex[d][5]);
		glVertex3f(ZoltyB.Wierzcholki[d][6],ZoltyB.Wierzcholki[d][7],ZoltyB.Wierzcholki[d][8]);
		glEnd();
	}
	glEndList();

	glNewList(ZIELONYB, GL_COMPILE); // Model fiata125
	for (int f = 0; f < ZielonyB.ilosc_prymitywow; f++){
		glBindTexture(GL_TEXTURE_2D,TexPlanszy[ZielonyB.ID_TEX[f]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(ZielonyB.Wektor_tex[f][0], ZielonyB.Wektor_tex[f][1]);
		glVertex3f(ZielonyB.Wierzcholki[f][0],ZielonyB.Wierzcholki[f][1],ZielonyB.Wierzcholki[f][2]);
		glTexCoord2f(ZielonyB.Wektor_tex[f][2], ZielonyB.Wektor_tex[f][3]);
		glVertex3f(ZielonyB.Wierzcholki[f][3],ZielonyB.Wierzcholki[f][4],ZielonyB.Wierzcholki[f][5]);
		glTexCoord2f(ZielonyB.Wektor_tex[f][4], ZielonyB.Wektor_tex[f][5]);
		glVertex3f(ZielonyB.Wierzcholki[f][6],ZielonyB.Wierzcholki[f][7],ZielonyB.Wierzcholki[f][8]);
		glEnd();
	}
	glEndList();

	glNewList(STANDRZWIA, GL_COMPILE); // Drzwi pierwsze
	for (int h = 0; h < 2; h++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[StanDrzwi.ID_TEX[h]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(StanDrzwi.Wektor_tex[h][0], StanDrzwi.Wektor_tex[h][1]);
		glVertex3f(StanDrzwi.Wierzcholki[h][0],StanDrzwi.Wierzcholki[h][1],StanDrzwi.Wierzcholki[h][2]);
		glTexCoord2f(StanDrzwi.Wektor_tex[h][2], StanDrzwi.Wektor_tex[h][3]);
		glVertex3f(StanDrzwi.Wierzcholki[h][3],StanDrzwi.Wierzcholki[h][4],StanDrzwi.Wierzcholki[h][5]);
		glTexCoord2f(StanDrzwi.Wektor_tex[h][4], StanDrzwi.Wektor_tex[h][5]);
		glVertex3f(StanDrzwi.Wierzcholki[h][6],StanDrzwi.Wierzcholki[h][7],StanDrzwi.Wierzcholki[h][8]);
		glEnd();
	}
	glEndList();

	glNewList(STANDRZWIB, GL_COMPILE); // Drzwi pierwsze
	for (int o = 2; o < 4; o++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[StanDrzwi.ID_TEX[o]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(StanDrzwi.Wektor_tex[o][0], StanDrzwi.Wektor_tex[o][1]);
		glVertex3f(StanDrzwi.Wierzcholki[o][0],StanDrzwi.Wierzcholki[o][1],StanDrzwi.Wierzcholki[o][2]);
		glTexCoord2f(StanDrzwi.Wektor_tex[o][2], StanDrzwi.Wektor_tex[o][3]);
		glVertex3f(StanDrzwi.Wierzcholki[o][3],StanDrzwi.Wierzcholki[o][4],StanDrzwi.Wierzcholki[o][5]);
		glTexCoord2f(StanDrzwi.Wektor_tex[o][4], StanDrzwi.Wektor_tex[o][5]);
		glVertex3f(StanDrzwi.Wierzcholki[o][6],StanDrzwi.Wierzcholki[o][7],StanDrzwi.Wierzcholki[o][8]);
		glEnd();
	}
	glEndList();

	glNewList(STANDRZWIC, GL_COMPILE); // Drzwi pierwsze
	for (int p = 4; p < StanDrzwi.ilosc_prymitywow; p++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[StanDrzwi.ID_TEX[p]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(StanDrzwi.Wektor_tex[p][0], StanDrzwi.Wektor_tex[p][1]);
		glVertex3f(StanDrzwi.Wierzcholki[p][0],StanDrzwi.Wierzcholki[p][1],StanDrzwi.Wierzcholki[p][2]);
		glTexCoord2f(StanDrzwi.Wektor_tex[p][2], StanDrzwi.Wektor_tex[p][3]);
		glVertex3f(StanDrzwi.Wierzcholki[p][3],StanDrzwi.Wierzcholki[p][4],StanDrzwi.Wierzcholki[p][5]);
		glTexCoord2f(StanDrzwi.Wektor_tex[p][4], StanDrzwi.Wektor_tex[p][5]);
		glVertex3f(StanDrzwi.Wierzcholki[p][6],StanDrzwi.Wierzcholki[p][7],StanDrzwi.Wierzcholki[p][8]);
		glEnd();
	}
	glEndList();

	glNewList(BELKAGORA, GL_COMPILE); // Belka góra
	for (int A = 0; A < 4; A++){
		glBindTexture(GL_TEXTURE_2D,TexRozne[Wsk.ID_TEX[A]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(Wsk.Wektor_tex[A][0], Wsk.Wektor_tex[A][1]);
		glVertex3f(Wsk.Wierzcholki[A][0],Wsk.Wierzcholki[A][1],Wsk.Wierzcholki[A][2]);
		glTexCoord2f(Wsk.Wektor_tex[A][2], Wsk.Wektor_tex[A][3]);
		glVertex3f(Wsk.Wierzcholki[A][3],Wsk.Wierzcholki[A][4],Wsk.Wierzcholki[A][5]);
		glTexCoord2f(Wsk.Wektor_tex[A][4], Wsk.Wektor_tex[A][5]);
		glVertex3f(Wsk.Wierzcholki[A][6],Wsk.Wierzcholki[A][7],Wsk.Wierzcholki[A][8]);
		glEnd();
	}
	glEndList();

	glNewList(WSKAZNIK, GL_COMPILE); // WskaŸnik
	for (int B = 4; B < Wsk.ilosc_prymitywow; B++){
		glBindTexture(GL_TEXTURE_2D,TexRozne[Wsk.ID_TEX[B]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(Wsk.Wektor_tex[B][0], Wsk.Wektor_tex[B][1]);
		glVertex3f(Wsk.Wierzcholki[B][0],Wsk.Wierzcholki[B][1],Wsk.Wierzcholki[B][2]);
		glTexCoord2f(Wsk.Wektor_tex[B][2], Wsk.Wektor_tex[B][3]);
		glVertex3f(Wsk.Wierzcholki[B][3],Wsk.Wierzcholki[B][4],Wsk.Wierzcholki[B][5]);
		glTexCoord2f(Wsk.Wektor_tex[B][4], Wsk.Wektor_tex[B][5]);
		glVertex3f(Wsk.Wierzcholki[B][6],Wsk.Wierzcholki[B][7],Wsk.Wierzcholki[B][8]);
		glEnd();
	}
	glEndList();

	glNewList(ROZKLAD, GL_COMPILE); // Rozk³ad jazdy
	for (int C = 0; C < Rozklad.ilosc_prymitywow; C++){
		glBindTexture(GL_TEXTURE_2D,TexRozklady[Rozklad.ID_TEX[C]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(Rozklad.Wektor_tex[C][0], Rozklad.Wektor_tex[C][1]);
		glVertex3f(Rozklad.Wierzcholki[C][0],Rozklad.Wierzcholki[C][1],Rozklad.Wierzcholki[C][2]);
		glTexCoord2f(Rozklad.Wektor_tex[C][2], Rozklad.Wektor_tex[C][3]);
		glVertex3f(Rozklad.Wierzcholki[C][3],Rozklad.Wierzcholki[C][4],Rozklad.Wierzcholki[C][5]);
		glTexCoord2f(Rozklad.Wektor_tex[C][4], Rozklad.Wektor_tex[C][5]);
		glVertex3f(Rozklad.Wierzcholki[C][6],Rozklad.Wierzcholki[C][7],Rozklad.Wierzcholki[C][8]);
		glEnd();
	}
	glEndList();

	glNewList(DRZWIA, GL_COMPILE); // Drzwi pierwsze
	for (int E = 0; E < DrzwiA.ilosc_prymitywow; E++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[DrzwiA.ID_TEX[E]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(DrzwiA.Wektor_tex[E][0], DrzwiA.Wektor_tex[E][1]);
		glVertex3f(DrzwiA.Wierzcholki[E][0],DrzwiA.Wierzcholki[E][1],DrzwiA.Wierzcholki[E][2]);
		glTexCoord2f(DrzwiA.Wektor_tex[E][2], DrzwiA.Wektor_tex[E][3]);
		glVertex3f(DrzwiA.Wierzcholki[E][3],DrzwiA.Wierzcholki[E][4],DrzwiA.Wierzcholki[E][5]);
		glTexCoord2f(DrzwiA.Wektor_tex[E][4], DrzwiA.Wektor_tex[E][5]);
		glVertex3f(DrzwiA.Wierzcholki[E][6],DrzwiA.Wierzcholki[E][7],DrzwiA.Wierzcholki[E][8]);
		glEnd();
	}
	glEndList();

	glNewList(DRZWIB, GL_COMPILE); // Drzwi drugie
	for (int F = 0; F < DrzwiA.ilosc_prymitywow; F++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[DrzwiA.ID_TEX[F]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(DrzwiA.Wektor_tex[F][0], DrzwiA.Wektor_tex[F][1]);
		glVertex3f(DrzwiA.Wierzcholki[F][0],DrzwiA.Wierzcholki[F][1],DrzwiA.Wierzcholki[F][2]);
		glTexCoord2f(DrzwiA.Wektor_tex[F][2], DrzwiA.Wektor_tex[F][3]);
		glVertex3f(DrzwiA.Wierzcholki[F][3],DrzwiA.Wierzcholki[F][4],DrzwiA.Wierzcholki[F][5]);
		glTexCoord2f(DrzwiA.Wektor_tex[F][4], DrzwiA.Wektor_tex[F][5]);
		glVertex3f(DrzwiA.Wierzcholki[F][6],DrzwiA.Wierzcholki[F][7],DrzwiA.Wierzcholki[F][8]);
		glEnd();
	}
	glEndList();

	glNewList(DRZWIC, GL_COMPILE); // Drzwi trzecie
	for (int G = 0; G < DrzwiA.ilosc_prymitywow; G++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[DrzwiA.ID_TEX[G]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(DrzwiA.Wektor_tex[G][0], DrzwiA.Wektor_tex[G][1]);
		glVertex3f(DrzwiA.Wierzcholki[G][0],DrzwiA.Wierzcholki[G][1],DrzwiA.Wierzcholki[G][2]);
		glTexCoord2f(DrzwiA.Wektor_tex[G][2], DrzwiA.Wektor_tex[G][3]);
		glVertex3f(DrzwiA.Wierzcholki[G][3],DrzwiA.Wierzcholki[G][4],DrzwiA.Wierzcholki[G][5]);
		glTexCoord2f(DrzwiA.Wektor_tex[G][4], DrzwiA.Wektor_tex[G][5]);
		glVertex3f(DrzwiA.Wierzcholki[G][6],DrzwiA.Wierzcholki[G][7],DrzwiA.Wierzcholki[G][8]);
		glEnd();
	}
	glEndList();

	glNewList(DRZWID, GL_COMPILE); // Drzwi pierwsze
	for (int H = 0; H < DrzwiB.ilosc_prymitywow; H++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[DrzwiB.ID_TEX[H]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(DrzwiB.Wektor_tex[H][0], DrzwiB.Wektor_tex[H][1]);
		glVertex3f(DrzwiB.Wierzcholki[H][0],DrzwiB.Wierzcholki[H][1],DrzwiB.Wierzcholki[H][2]);
		glTexCoord2f(DrzwiB.Wektor_tex[H][2], DrzwiB.Wektor_tex[H][3]);
		glVertex3f(DrzwiB.Wierzcholki[H][3],DrzwiB.Wierzcholki[H][4],DrzwiB.Wierzcholki[H][5]);
		glTexCoord2f(DrzwiB.Wektor_tex[H][4], DrzwiB.Wektor_tex[H][5]);
		glVertex3f(DrzwiB.Wierzcholki[H][6],DrzwiB.Wierzcholki[H][7],DrzwiB.Wierzcholki[H][8]);
		glEnd();
	}
	glEndList();

	glNewList(DRZWIE, GL_COMPILE); // Drzwi drugie
	for (int I = 0; I < DrzwiB.ilosc_prymitywow; I++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[DrzwiB.ID_TEX[I]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(DrzwiB.Wektor_tex[I][0], DrzwiB.Wektor_tex[I][1]);
		glVertex3f(DrzwiB.Wierzcholki[I][0],DrzwiB.Wierzcholki[I][1],DrzwiB.Wierzcholki[I][2]);
		glTexCoord2f(DrzwiB.Wektor_tex[I][2], DrzwiB.Wektor_tex[I][3]);
		glVertex3f(DrzwiB.Wierzcholki[I][3],DrzwiB.Wierzcholki[I][4],DrzwiB.Wierzcholki[I][5]);
		glTexCoord2f(DrzwiB.Wektor_tex[I][4], DrzwiB.Wektor_tex[I][5]);
		glVertex3f(DrzwiB.Wierzcholki[I][6],DrzwiB.Wierzcholki[I][7],DrzwiB.Wierzcholki[I][8]);
		glEnd();
	}
	glEndList();

	glNewList(DRZWIF, GL_COMPILE); // Drzwi trzecie
	for (int J = 0; J < DrzwiB.ilosc_prymitywow; J++){
		glBindTexture(GL_TEXTURE_2D,TexModelu[DrzwiB.ID_TEX[J]]);
		glBegin(GL_TRIANGLES);
   	    //glColor4f (1.0f, 1.0f, 1.0f, 0.5f); 
		glTexCoord2f(DrzwiB.Wektor_tex[J][0], DrzwiB.Wektor_tex[J][1]);
		glVertex3f(DrzwiB.Wierzcholki[J][0],DrzwiB.Wierzcholki[J][1],DrzwiB.Wierzcholki[J][2]);
		glTexCoord2f(DrzwiB.Wektor_tex[J][2], DrzwiB.Wektor_tex[J][3]);
		glVertex3f(DrzwiB.Wierzcholki[J][3],DrzwiB.Wierzcholki[J][4],DrzwiB.Wierzcholki[J][5]);
		glTexCoord2f(DrzwiB.Wektor_tex[J][4], DrzwiB.Wektor_tex[J][5]);
		glVertex3f(DrzwiB.Wierzcholki[J][6],DrzwiB.Wierzcholki[J][7],DrzwiB.Wierzcholki[J][8]);
		glEnd();
	}
	glEndList();
}
/*********************** Zmiana rozdzielczoœci *******************/
void ZmienRozdzielczosc()
{
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);

	int index = 0;
	while(EnumDisplaySettings(NULL, index, &dm)){
		if(dm.dmBitsPerPel==static_cast<unsigned>(kolor) && dm.dmPelsWidth==static_cast<unsigned>(szerokosc) &&dm.dmPelsHeight==static_cast<unsigned>(wysokosc)){
			dm.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
			LONG result=ChangeDisplaySettings(&dm,CDS_TEST);
			if(result==DISP_CHANGE_SUCCESSFUL){
				ChangeDisplaySettings(&dm, 0);
				break;
			}else{
				MessageBox(hWnd,"B³¹d zmiany rozdzielczoœci!", "B³¹d",MB_OK);
			}
		}
		index++;
	}
}
/********************** Mg³a i œwiat³o ***************************/
void MglaISwiatlo()
{
	/*glEnable( GL_FOG );
	glHint( GL_FOG_HINT, GL_NICEST );
	if(Dzien==TRUE){
		glFogfv( GL_FOG_COLOR, KolorDnia );
	}else{
		glFogfv( GL_FOG_COLOR, KolorNocy );
	}
	glFogf( GL_FOG_START, 30.0f );
	glFogf( GL_FOG_END, 100.0f );
	glFogf( GL_FOG_DENSITY, 0.01f );
	glFogf( GL_FOG_MODE, GL_EXP2 );*/

	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
	if(Dzien==TRUE){
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, OtoczenieDzien);
		glLightfv(GL_LIGHT0,GL_AMBIENT, OtoczenieDzien); 
	}else{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, OtoczenieNoc);
		glLightfv(GL_LIGHT0,GL_AMBIENT, OtoczenieNoc); 
	}
	glLightfv(GL_LIGHT0,GL_DIFFUSE, odbicie);
	glLightfv(GL_LIGHT0,GL_SPECULAR, odblysk) ; 
	glLightfv(GL_LIGHT0,GL_POSITION, pozycja);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, odblyskmat); 
	glMateriali(GL_FRONT,GL_SHININESS, 256);
}
/********************** inicjalizacja menu gry *******************/
void Menu()
{
	InitDzwiek("Volvo7000");

	if(StartGry==FALSE){
	LadujTekstury("Dane/Menu/logo.gfx", 0, 4);
	LadujTekstury("Dane/Menu/tlo.gfx", 1, 4);
	LadujTekstury("Dane/Menu/nowagra_gora.gfx", 2, 4);
	LadujTekstury("Dane/Menu/nowagra_dol.gfx", 3, 4);
	LadujTekstury("Dane/Menu/autobusy_gora.gfx", 4, 4);
	LadujTekstury("Dane/Menu/autobusy_dol.gfx", 5, 4);
	LadujTekstury("Dane/Menu/ogrze_gora.gfx", 6, 4);
	LadujTekstury("Dane/Menu/ogrze_dol.gfx", 7, 4);
	LadujTekstury("Dane/Menu/wyjscie_gora.gfx", 8, 4);
	LadujTekstury("Dane/Menu/wyjscie_dol.gfx", 9, 4);
	LadujTekstury("Dane/Menu/ogrze.gfx", 10, 4);
	LadujTekstury("Dane/Menu/autobusy.gfx", 11, 4);
	LadujTekstury("Dane/Menu/nowagra.gfx", 12, 4);

	TworzListWyswMenu();
	}else{
		InitGL();
	}

	ShowCursor(FALSE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_FLAT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glPolygonMode(GL_FRONT, GL_LINE);

	if(AlphaFunc==0){			//Alpha funkcja
		glAlphaFunc(GL_GREATER, 0.5f);
		glEnable(GL_ALPHA_TEST);
	}else{
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}

	glEnable(GL_TEXTURE_2D);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	MglaISwiatlo();
	ZmienRozdzielczosc();
}