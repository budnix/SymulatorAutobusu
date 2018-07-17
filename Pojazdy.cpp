#include "Inicjalizacja.h"

const GLfloat pi180 = 0.0174532925f;

GLfloat x_f125=4.2f, z_f125=-20, predkosc_f125=0.1f, Kat=0, WspPredk=0,
        WyliczonyKat;
extern GLfloat obrot_f125;

/************************ Obliczenia dla Fiata *******************/
void F125()
{
	x_f125 += static_cast<float>(sin(obrot_f125*pi180) * predkosc_f125); 
	z_f125 += static_cast<float>(cos(obrot_f125*pi180) * predkosc_f125);
	
	if(x_f125>-5 && x_f125<7 && z_f125<-50 && z_f125>-70){
		predkosc_f125=AnimujPredkosc(0.2f, 0.3f, 0);
	}
	if(x_f125>-5 && x_f125<7 && z_f125<-250 && z_f125>-260){
		//predkosc_f125=AnimujPredkosc(0.1f, 0.1f, 0);
		obrot_f125=179/*AnimujSkret(180, 179, 1)*/;
	}
	if(x_f125>-5 && x_f125<7 && z_f125<-300 && z_f125<-310){
		obrot_f125=180;
	}
	if(x_f125>-5 && x_f125<7 && z_f125<-380 && z_f125<-390){
		obrot_f125=181; 
	}
	if(x_f125>-15 && x_f125<10 && z_f125<-443 && z_f125<-447){
		predkosc_f125=AnimujPredkosc(0.1f, 0.05f, 0);
		obrot_f125=AnimujSkret(181, 269, 0);
	}
	if(x_f125>-110 && x_f125<-90 && z_f125>-465 && z_f125<-455){
		predkosc_f125=AnimujPredkosc(0.1f, 0.3f, 0);
		obrot_f125=271.2f;
	}
	if(x_f125<-405 && x_f125>-420 && z_f125>-470 && z_f125<-430){
		predkosc_f125=AnimujPredkosc(0.1f, 0.05f, 0);
		obrot_f125=AnimujSkret(271, 0, 0);
	}
	if(x_f125<-405 && x_f125>-420 && z_f125>-430 && z_f125<-380){
		predkosc_f125=AnimujPredkosc(0.1f, 0.4f, 0);
		obrot_f125=0;
	}
	if(x_f125<-390 && x_f125>-430 && z_f125>-56 && z_f125<-40){
		predkosc_f125=AnimujPredkosc(0.1f, 0.2f, 0);
		obrot_f125=AnimujSkret(0, 10, 0);
	}
	if(x_f125<-387 && x_f125>-430 && z_f125>53 && z_f125<60){
		predkosc_f125=AnimujPredkosc(0.1f, 0.03f, 0);
		obrot_f125=AnimujSkret(10, 44, 0);
	}
	if(x_f125<-360 && x_f125>-389 && z_f125>40 && z_f125<77){
		predkosc_f125=AnimujPredkosc(0.05f, 0.32f, 0);
		obrot_f125=AnimujSkret(44, 90, 0);
	}
	if(x_f125<-10 && x_f125>-55 && z_f125>30 && z_f125<77){
		predkosc_f125=AnimujPredkosc(0.32f, 0.2f, 0);
		obrot_f125=AnimujSkret(90, 151, 0);
	}
	if(x_f125>-10 && x_f125<15 && z_f125>-15 && z_f125<8.7){
		//predkosc_f125=AnimujPredkosc(0.32f, 0.2f, 0);
		obrot_f125=AnimujSkret(151, 180, 0);
	}
}
/************************** Animacja Skrêtu fiata ****************/
float AnimujSkret(float AktualnyKat, float ZamierzanyKat, int KierunekLiczenia)
{
	switch(KierunekLiczenia){
	case 0:
		if(WyliczonyKat!=ZamierzanyKat){
			Kat=Kat+0.5f;
			WyliczonyKat=AktualnyKat+Kat;
			if(WyliczonyKat==360){
				WyliczonyKat=0;
			}
		}else{
			Kat=0;
		}
		break;
	case 1:
		if(WyliczonyKat>=ZamierzanyKat){
			Kat=Kat+0.5f;
			WyliczonyKat=AktualnyKat-Kat;
		}else{
			Kat=0;
		}
		break;
	}
	return WyliczonyKat;

}
/***************** Animacja prêdkoœci ****************************/
float AnimujPredkosc(float AktualnaPredkosc, float ZamierzanaPredkosc, int Kierunek)
{

	switch(Kierunek){
	case 0:
		if(AktualnaPredkosc<=ZamierzanaPredkosc){
			WspPredk=WspPredk+0.001f;
			AktualnaPredkosc=AktualnaPredkosc+WspPredk;
		}else{
			WspPredk=0;
		}
		break;
	case 1:
		if(AktualnaPredkosc>=ZamierzanaPredkosc){
			WspPredk=WspPredk+0.001f;
			AktualnaPredkosc=AktualnaPredkosc-WspPredk;
		}else{
			WspPredk=0;
		}
		break;
	}
	return AktualnaPredkosc;

}