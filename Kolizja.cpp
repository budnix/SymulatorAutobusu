#include "Inicjalizacja.h"

extern GLfloat x, z, predkosc;
extern Obiekt Plansza;
float A,B,C,D,wynik;

/************************ kolizja ********************************/
void Kolizja()
{
	
	/*if(x-1.5<18 && x+1.5>8 && -z-2.5<-40 && -z-7.5>-80){
		predkosc=0;
	}
	for (int i = 0; i < Plansza.ilosc_prymitywow; i++){
		A=(Plansza.Wierzcholki[i][4]-Plansza.Wierzcholki[i][1])*(Plansza.Wierzcholki[i][8]-Plansza.Wierzcholki[i][2])-(Plansza.Wierzcholki[i][7]-Plansza.Wierzcholki[i][1])*(Plansza.Wierzcholki[i][5]-Plansza.Wierzcholki[i][2]);
		B=(Plansza.Wierzcholki[i][5]-Plansza.Wierzcholki[i][2])*(Plansza.Wierzcholki[i][6]-Plansza.Wierzcholki[i][0])-(Plansza.Wierzcholki[i][8]-Plansza.Wierzcholki[i][2])*(Plansza.Wierzcholki[i][3]-Plansza.Wierzcholki[i][0]);
		C=(Plansza.Wierzcholki[i][3]-Plansza.Wierzcholki[i][0])*(Plansza.Wierzcholki[i][7]-Plansza.Wierzcholki[i][1])-(Plansza.Wierzcholki[i][6]-Plansza.Wierzcholki[i][0])*(Plansza.Wierzcholki[i][4]-Plansza.Wierzcholki[i][1]);
		D=-(Plansza.Wierzcholki[i][0]*A)+(Plansza.Wierzcholki[i][1]*B)+(Plansza.Wierzcholki[i][2]*C);

		wynik=A*x+B*10+C*z+D;
	}*/

}