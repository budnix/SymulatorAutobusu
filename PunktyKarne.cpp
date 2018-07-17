#include "Inicjalizacja.h"

/************ istniej�ce dane **********/
extern float wskazowka;
extern bool PrzystanekZaliczony, PierwszeDrzwi, DrugieDrzwi, TrzecieDrzwi,
		    PasazerowieWToku, Pasazerowie;
/********** nowe dane ******************/
float PunktyKarne;
char *Opinia;

/******* Obliczanie punkt�w karnych dla r�nych przekrocze� ******/
float ObliczajPunktyKarne()
{
	/******* punkty karne za przekroczenie 65km/h ****/
	if(wskazowka*222>65){
		PunktyKarne=PunktyKarne+0.02f;
	}
	/**** za otworzenie drzwi poza przystankiem *****/
	if(PrzystanekZaliczony==FALSE){
		if(PierwszeDrzwi==TRUE || DrugieDrzwi==TRUE || TrzecieDrzwi==TRUE){
			PunktyKarne=PunktyKarne+0.02f;
		}
	}
	/******* za zamkni�cie drzwi przed nosem pasa�era ****/
	if(PasazerowieWToku==TRUE && PierwszeDrzwi==FALSE && DrugieDrzwi==FALSE && TrzecieDrzwi==FALSE){
		PunktyKarne=PunktyKarne+10.0f;
	}
	/********** wy�wietlanie UWAG: ************/
	if(PunktyKarne==0){
		Opinia="sWIETNIE, PRZEJECHALES BEZBLEDNIE!!!";
	}
	if(PunktyKarne>=1 && PunktyKarne<=5){
		Opinia="pRAWIE BEZBLEDNIE, WIECEJ TRENINGU!!!";
	}
	if(PunktyKarne>=6 && PunktyKarne<=10){
		Opinia="   nIE WIEM CZY UTRZYMASZ TA PRACE";
	}
	if(PunktyKarne>=11){
		Opinia="      kTO CI DAL PRAWO JAZDY!!!";
	}
	/******* za przejechanie przestanku bez zatrzymania si� ***
	if(PrzystanekZaliczony==TRUE && Pasazerowie==TRUE){
		Pasazerowie=FALSE;
		PunktyKarne=PunktyKarne+10.0f;
	}*/

	return floor(PunktyKarne);
}