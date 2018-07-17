#ifndef _INICJALIZACJA_H
#define _INICJALIZACJA_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "fmodvc.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <winbase.h>
#include <winerror.h>
#include <fstream.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glaux.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdarg.h>

/*********************** Klasy ***********************************/

class Obiekt{
public:
	float Wierzcholki[10000][9]; 
	float Wektor_tex[10000][6];
	int ID_TEX[10000];
	int ilosc_prymitywow;
};
class Przystanki{
public:
	float X[100]; 
	float Z[100];
	int IloscPrzystankow;
};
class Kamera{
public:
	float Wysokosc, Odleglosc, Obrot, PunktPatrzenia;
	float Obrot_a;
};

/************************ Zmienne ********************************/

extern int szerokosc, wysokosc;
extern bool keys[256];
extern HGLRC	hRC;
extern HDC		hDC;

/************************* Funkcje *******************************/
/*** Main.cpp ***/
void __fastcall Renderuj();
void __fastcall LadujTekstury(char *tekstura, int IDTex, int WyborTablicy);
void ObliczAlpha(void);
/*** Klawiatura.cpp ***/
void SzukajPlikow(char szSciezkaDost[]);
void __fastcall Klawiatura();
void __fastcall KlawiaturaMenu(void);
void WyliczCzas(void);
/*** DŸwiêk.cpp ***/
void InitDzwiek(char *RodzajAutobusu);
void Odtwarzanie(int gaz, int sample);
void NiszczDzwiek();
/*** Tekst.cpp ***/
GLvoid BudujCzcionki(void);
GLvoid NiszczCzcionki(void);
GLvoid WyswietlTekst(GLint x, GLint y, int set, char *string, ...);
/*** Inicjalizacja.cpp ***/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CzytajSA(Obiekt &JakiObiekt, char *DostDoPliku);
void CzytajTeksury(char *sciezka, int Tablica);
void CzytajParametryBusa(void);
void CzytajParametryMisji(void);
void CzytajPrzystanki(void);
void SetDCPixelFormat( HDC hDC );
void InitGL(void);
void TworzListyWyswietlania(void);
void ZmienRozdzielczosc(void);
void ResizeGLWindow(int szerokosc, int wysokosc);
void MglaISwiatlo(void);
void CzytajOpcje(void);
void Menu(void);
void CzyscTabliceTekstur(void);
void KoniecProgramu(void);
/*** Kolizja.cpp ***/
void Kolizja(void);
/*** Pojazdy.cpp ***/
void F125(void);
float AnimujSkret(float AktualnyKat, float ZamierzanyKat, int KierunekLiczenia);
float AnimujPredkosc(float AktualnaPredkosc, float ZamierzanaPredkosc, int Kierunek);
/*** Menu.cpp ***/
void __fastcall RenderujMenu(void);
void TworzListWyswMenu(void);
void Logo(int Kierunek);
void AnimujMenu(int Kierunek);
/*** PunktyKarne ***/
float ObliczajPunktyKarne();



#endif 