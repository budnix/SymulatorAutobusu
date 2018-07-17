#include "fmod.h"
#include "Inicjalizacja.h"

//extern HWND		hWnd;
bool keys[256], blok=TRUE, Pasazer=FALSE, Wl=TRUE, Drzwi=FALSE, 
	 BlokZam=TRUE, PokazParametry=FALSE, BlokadaKlaksonu=TRUE,
	 BlokadaSilnika=TRUE, StartStopSilnika=TRUE, BlokadaGazu=TRUE,
	 BlokadaOdtwarzania=TRUE, NieudanyZaplon=TRUE, BlokadaH=FALSE,
	 BlokadaKierP=FALSE, KierP=FALSE, BlokadaKierL=FALSE, KierL=FALSE,
	 StrzalkaKierP=FALSE, StrzalkaKierL=FALSE, BlokadaSygnalu=FALSE,
	 SwiatloWew=FALSE, BlokadaX=FALSE, SwiatloZew=FALSE, BlokadaC=FALSE,
	 SwiatlaStop=FALSE, JakaKamera=TRUE, SwiatlaTyl=FALSE,
	 DrogoweZolty=FALSE,DrogoweZielony=FALSE,DrogoweCzerwony=TRUE,
	 BlokadaL, BlokadaMenuD=TRUE, BlokadaMenuG=TRUE, Wyjscie=FALSE,
	 NowaGra=FALSE, Autobusy=FALSE, OGrze=FALSE, WyjscieDoMenu=FALSE,
	 BlokadaEnter, BlokadaEnterAnim=FALSE, Klawiszologia=FALSE, BlokadaJ=FALSE,
	 PierwszeDrzwi=FALSE, DrugieDrzwi=FALSE, TrzecieDrzwi=FALSE,
	 PasazerowieWsiadajacy=FALSE, PasazerowieWysiadajacy=TRUE,
	 BlokadaPasWys=TRUE, PrzystanekZaliczony=FALSE, NaStacji,
	 PokazRozklad=FALSE, BlokadaR, BlokadaPoprz, BlokadaNast, BlokadaPlay,
	 BlokadaDlaSilnika, PasazerowieWToku, BlokadaP, Pauza=FALSE,
	 Pasazerowie=FALSE, Statystyka=FALSE;
const GLfloat pi180 = 0.0174532925f;
GLfloat obrot_x, x, z, obiekt_x, predkosc, MaxPredkosc,
	  kat_kier, kierownica, pochylenieX, wskazowka,
	  obrot_f125=180, przyspieszenie, MocHamulcow, PochPrzySkrecie,
	  OdcinekX, OdcinekZ, KatWskaznika, PasWy, PasWch, PasSuma,
	  WylosowaniWy, WylosowaniWch, Paliwo, Dystans, Sekundy,
	  Minuty, Godziny, PojBaku, XStacjiPaliw, ZStacjiPaliw,
	  DrzwiAObrot, DrzwiAZ, DrzwiBObrot, DrzwiBZ, DrzwiCObrot,
	  DrzwiCZ;
GLint kierunek, blokada_a, blokada_b, jaki_bieg, nr_bieg=1,
	ostatnia_klatka, fps, gaz, iloscPasazerow, wchIwych=1,
	ilPas, zapBieg, fpsOstateczny, licznik, Czas, CzasOstatni,
	CzasSD, CzasOstatniSD, CzasSwiatel, WyborLuster=1, WyborMenu,
	GranicaGorna=4, GranicaDolna, NumerPrzystanka, NumerPiosenki=1,
	IloscPiosenek, DlugoscPiosenki, AktualnaPozycja;
char *StanDrzwi1="zamkniete", *StanDrzwi2="zamkniete", *StanBiegow="luz",
     *StanDrzwi3="zamkniete";
static GLint klatki;
extern FSOUND_SAMPLE *Dzwieki[13];
extern FSOUND_STREAM *Muzyka[100];
extern bool StartGry, PowrotDoMenu, UruchomBezMenu;
Kamera UstawKamere;
extern Przystanki KolejnePrzystanki;
extern int Joystick;

JOYINFO        Joy_Info;

/********************* Klawiatura i ca³a fizyka ******************/
void __fastcall Klawiatura()
{
	//joyGetDevCaps(0, MozliwosciJoysticka, sizeof(MozliwosciJoysticka));
	joyGetPos (JOYSTICKID1, &Joy_Info);
	//kierownica=DaneJoysticka->wXpos*0.01f;
	/*** poka¿/ukryj napisy parametrów gry ***/
	if(keys['J']==TRUE && BlokadaJ==FALSE){
		PokazParametry=!PokazParametry;
		Klawiszologia=FALSE;
		BlokadaJ=TRUE;
	}
	if(keys['J']==FALSE){
		BlokadaJ=FALSE;
	}
	/*** pauzowanie gry ***/
	if(keys['P']==TRUE && BlokadaP==FALSE){
		Pauza=!Pauza;
		BlokadaP=TRUE;
	}
	if(keys['P']==FALSE){
		BlokadaP=FALSE;
	}
	/*** poka¿/ukryj rozk³ad jazdy ***/
	if(keys['R']==TRUE && BlokadaR==FALSE){
		PokazRozklad=!PokazRozklad;
		BlokadaR=TRUE;
	}
	if(keys['R']==FALSE){
		BlokadaR=FALSE;
	}
	/*** poka¿/ukryj napisy klawiszologi gry ***/
	if(keys['H']==TRUE && BlokadaH==FALSE){
		Klawiszologia=!Klawiszologia;
		PokazParametry=FALSE;
		BlokadaH=TRUE;
	}
	if(keys['H']==FALSE){
		BlokadaH=FALSE;
	}
	/********** wyjœcie z gry ***********/
	if(keys[VK_ESCAPE]==TRUE){
		if(UruchomBezMenu==TRUE){
			KoniecProgramu();
		}else{
			FSOUND_Stream_Stop(Muzyka[NumerPiosenki]);
			CzyscTabliceTekstur();
			WyborLuster=1;
			BlokadaSilnika=TRUE;// wy³. silnik
			StartStopSilnika=TRUE;// wy³. silnik
			BlokadaGazu=TRUE;
			predkosc=0.0f;	// prêdkoœæ startowa równa 0
			PasSuma=0;
			PasWy=0;
			PasWch=0;
			NumerPrzystanka=0;// zerowanie przystanków
			Sekundy=0;
			PierwszeDrzwi=FALSE;// zamkniêcie drzwi
			DrugieDrzwi=FALSE;// zamkniêcie drzwi
			TrzecieDrzwi=FALSE;// zamkniêcie drzwi
			StanDrzwi1="zamkniete";
			StanDrzwi2="zamkniete";
			StanDrzwi3="zamkniete";
			StartGry=FALSE;//prze³¹czenie na MENU
			PowrotDoMenu=TRUE;
			Statystyka=FALSE;// wy³ statystyki
			//PunktyKarne=0;//wyzerowanie punktów karnych
		}
	}
if(Pauza==FALSE && Statystyka==FALSE){
	F125();// Animacja Fiata
	/************ obs³uga joya *************/
	if(Joystick==1){
		int liczba=Joy_Info.wXpos;
		kierownica=(((liczba)-32768)*0.000012);
		int liczba_a=Joy_Info.wYpos;
		float przys;
		przys=(((liczba_a)-32768)*0.00000001);
		/*if(predkosc<0.001 && predkosc>-0.02){
			predkosc=0.0f;
		}*/
		switch(nr_bieg){
		case 0:
			if(predkosc<=0.03){
				obrot_x=obrot_x+kierownica*0.0000001f*predkosc;
			}else{
				obrot_x=obrot_x+kierownica;
			}
			if(przys>=0){
				if(predkosc>=0){
					if(pochylenieX>0){	// resor przód-ty³
						pochylenieX=pochylenieX-0.02f;
					}
					predkosc=predkosc-przys*4;
					x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
					z += static_cast<float>(cos(obrot_x*pi180) * predkosc);
				}
			}else{
				if(pochylenieX<0.1){	// resor przód-ty³
					pochylenieX=pochylenieX+0.02f;
				}
				predkosc=predkosc-przys*1.5;
				x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
				z += static_cast<float>(cos(obrot_x*pi180) * predkosc); 
			}
		break;
		case 1:
			if(predkosc>0){
				obrot_x=obrot_x+kierownica;
				predkosc=predkosc-0.00008f;
				x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
				z += static_cast<float>(cos(obrot_x*pi180) * predkosc);  
				if(pochylenieX>0){
					pochylenieX=pochylenieX-0.02f;
				}
				if(pochylenieX<0){
					pochylenieX=pochylenieX+0.02f;
				}
			}
			if(predkosc==0){
				if(pochylenieX>0){
					pochylenieX=pochylenieX-0.02f;
				}
				if(pochylenieX<0){
					pochylenieX=pochylenieX+0.02f;
				}
			}
		break;
		}
	}else{
	/***** skrêt w lewo *****/
		if(keys[VK_LEFT]==TRUE){
			kierownica=kierownica-0.01f;
				if(kierownica<=-kat_kier){
					kierownica=-kat_kier;
				}
			blokada_b=1;
		}
		/***** skrêt w prawo *****/
		if(keys[VK_RIGHT]==TRUE){
			kierownica=kierownica+0.01f;
				if(kierownica>=kat_kier){
					kierownica=kat_kier;
				}
			blokada_b=1;
		}
	}
	/******* Pomiar klatek na sekunde *****/
	fps++;
	klatki=static_cast<int>((GetTickCount() * 0.001f));
	if(klatki-ostatnia_klatka>=1.0){
		//ostatnia_klatka=static_cast<int>((GetTickCount() * 0.001f));
		ostatnia_klatka=klatki;
		fpsOstateczny=fps;
		fps=0;
		WyliczCzas();
	}
	/******* pochylenie przy skrêcie ******/
	if(Drzwi==FALSE){
		PochPrzySkrecie=kierownica*predkosc;
	}
	/******* animacja drzwi PIERWSZYCH drzwi ******/
	if(PierwszeDrzwi==TRUE){
		if(DrzwiAObrot<=90){
			DrzwiAObrot=DrzwiAObrot+0.75f;
		}
		if(DrzwiAZ<=0.3){
			DrzwiAZ=DrzwiAZ+0.0025f;
		}
	}
	if(PierwszeDrzwi==FALSE){
		if(DrzwiAObrot>=0){
			DrzwiAObrot=DrzwiAObrot-0.75f;
		}
		if(DrzwiAZ>=0){
			DrzwiAZ=DrzwiAZ-0.0025f;
		}
	}
	/******* animacja drzwi DRUGICH drzwi ******/
	if(DrugieDrzwi==TRUE){
		if(DrzwiBObrot<=90){
			DrzwiBObrot=DrzwiBObrot+0.75f;
		}
		if(DrzwiBZ<=0.3){
			DrzwiBZ=DrzwiBZ+0.0025f;
		}
	}
	if(DrugieDrzwi==FALSE){
		if(DrzwiBObrot>=0){
			DrzwiBObrot=DrzwiBObrot-0.75f;
		}
		if(DrzwiBZ>=0){
			DrzwiBZ=DrzwiBZ-0.0025f;
		}
	}
	/******* animacja drzwi TRZECICH drzwi ******/
	if(TrzecieDrzwi==TRUE){
		if(DrzwiCObrot<=90){
			DrzwiCObrot=DrzwiCObrot+0.75f;
		}
		if(DrzwiCZ<=0.3){
			DrzwiCZ=DrzwiCZ+0.0025f;
		}
	}
	if(TrzecieDrzwi==FALSE){
		if(DrzwiCObrot>=0){
			DrzwiCObrot=DrzwiCObrot-0.75f;
		}
		if(DrzwiCZ>=0){
			DrzwiCZ=DrzwiCZ-0.0025f;
		}
	}
	if(keys[VK_RIGHT]==FALSE && keys[VK_LEFT]==FALSE){
		blokada_b=0;
	}
	if(keys[VK_UP]==FALSE && keys[VK_DOWN]==FALSE){
		blokada_a=0;
	/***** powrót gazu *****/
	if(gaz>4000 && blokada_a==0){
		gaz=gaz-200;
		Odtwarzanie(gaz, 0);
	}
	}
	/**************** oblicznie k¹ta dla wskaŸnika ********/
	OdcinekX=x+(-KolejnePrzystanki.X[NumerPrzystanka]);
	OdcinekZ=z+KolejnePrzystanki.Z[NumerPrzystanka];
	if(OdcinekZ>=0){
		KatWskaznika=-(atanf(OdcinekX/OdcinekZ)*57.29577951f)+180;
	}else{
		KatWskaznika=-(atanf(OdcinekX/OdcinekZ)*57.29577951f);
	}
	/************** oblicznie odleg³oœci od przystanku *****/
	Dystans=static_cast<float>(sqrt((OdcinekX*OdcinekX)+(OdcinekZ*OdcinekZ)));
	if(Dystans<5 && Dystans>-5){
		PrzystanekZaliczony=TRUE;
	}else{
		PrzystanekZaliczony=FALSE;
	}
	/************** oblicznie odleg³oœci od stacji paliw *****/
	float OdcinekXStacja=x+(-(XStacjiPaliw));
	float OdcinekZStacja=z+(ZStacjiPaliw);
	float DystansOdStacji=static_cast<float>(sqrt((OdcinekXStacja*OdcinekXStacja)+(OdcinekZStacja*OdcinekZStacja)));
	if(DystansOdStacji<10 && DystansOdStacji>-10){
		NaStacji=TRUE;
	}else{
		NaStacji=FALSE;
	}
	if(NaStacji==TRUE && StartStopSilnika==TRUE && predkosc==0){
		if(Paliwo<PojBaku){// tankowanie paliwa
			Paliwo=Paliwo+0.03f;
		}
	}
	/***** luz *****/
	if(predkosc<0 && kierunek==0 && blokada_a==0){
		obrot_x=obrot_x-kierownica;
		predkosc=predkosc+0.00008f;
		x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
		z += static_cast<float>(cos(obrot_x*pi180) * predkosc); 
		/*if(predkosc<=0.01){
			predkosc=predkosc+0.0002f;
		}*/
		if(pochylenieX>0){
			pochylenieX=pochylenieX-0.02f;
		}
		if(pochylenieX<0){
			pochylenieX=pochylenieX+0.02f;
		}
	}
	if(predkosc>0 && kierunek==1 && blokada_a==0){
		obrot_x=obrot_x+kierownica;
		predkosc=predkosc-0.00008f;
		x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
		z += static_cast<float>(cos(obrot_x*pi180) * predkosc);  
		/*if(predkosc<=0.01){
			predkosc=predkosc-0.0002f;
		}*/
		if(pochylenieX>0){
			pochylenieX=pochylenieX-0.02f;
		}
		if(pochylenieX<0){
			pochylenieX=pochylenieX+0.02f;
		}
	}
	if(predkosc==0){
		if(pochylenieX>0){
			pochylenieX=pochylenieX-0.02f;
		}
		if(pochylenieX<0){
			pochylenieX=pochylenieX+0.02f;
		}
	}
	/***** powrót gazu *****/
	if(nr_bieg==0 && keys[VK_UP]==FALSE){
		if(predkosc>=0 && predkosc<=0.05){
			gaz=static_cast<int>(predkosc*500000);
		}
		if(predkosc>=0.05 && predkosc<=0.16){
			gaz=static_cast<int>(predkosc*170000);
		}
		if(predkosc>=0.16 && predkosc<=0.26){
			gaz=static_cast<int>(predkosc*95000);
		}
		if(predkosc>=0.26 && predkosc<=MaxPredkosc){
			gaz=static_cast<int>(predkosc*50000);
		}	
		//Odtwarzanie(gaz, 0);
	}
	/***** powrót kierownicy *****/
	if(kierownica<0 && blokada_b==0){
		kierownica=kierownica+0.005f;
	}
	if(kierownica<0.005 && kierownica>-0.005){
		kierownica=0.0f;
	}
	if(kierownica>0 && blokada_b==0){
		kierownica=kierownica-0.005f;
	}
	//kat_kier=0.2f;
	/***** blokada kier. dla ma³ych prêdkoœci *****/
	if(predkosc>0){
		kat_kier=predkosc*10.0f;
	}else{
		kat_kier=predkosc*-10.0f;
	}
	if(kat_kier>=0.4f){
		kat_kier=0.4f;	
	}
	/***** do ty³u *****/
	if(keys[VK_UP]==FALSE){
		if(keys[VK_DOWN]==TRUE){
			blokada_a=1;
			SwiatlaStop=TRUE;
			/***** powrót gazu *****/
			if(gaz>4000){
				gaz=gaz-200;
				Odtwarzanie(gaz, 0);
				//Odtwarzanie(gaz, 13);
			}
			/**** hamowanie ****/
			if(nr_bieg==0 || nr_bieg==1){
				if(predkosc>0 && kierunek==1){
					obrot_x=obrot_x+kierownica;
					predkosc=predkosc-MocHamulcow;
					x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
					z += static_cast<float>(cos(obrot_x*pi180) * predkosc);
				}
				if(predkosc<0 && kierunek==0){
					obrot_x=obrot_x+kierownica;
					predkosc=predkosc+MocHamulcow;
					x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
					z += static_cast<float>(cos(obrot_x*pi180) * predkosc); 
				}
			}
			/***** hamowanie dla wstecznego *****/
			if(nr_bieg==2){
				if(predkosc>0 && kierunek==1){
					obrot_x=obrot_x-kierownica;
					predkosc=predkosc-MocHamulcow;
					x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
					z += static_cast<float>(cos(obrot_x*pi180) * predkosc);
				}
				if(predkosc<0 && kierunek==0){
					obrot_x=obrot_x-kierownica;
					predkosc=predkosc+MocHamulcow;
					x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
					z += static_cast<float>(cos(obrot_x*pi180) * predkosc);
				}
				if(predkosc<0){
					if(pochylenieX<0.1){
						pochylenieX=pochylenieX+0.02f;
					}
				}
			}
			if(predkosc<0.01 && predkosc>-0.01){
			predkosc=0.0f;
			}
			if(predkosc!=0){
				if(pochylenieX>-0.1){
						pochylenieX=pochylenieX-0.02f;
				}
			}
		}else{
			SwiatlaStop=FALSE;
		}
	}
	if(BlokadaGazu==FALSE){
		/***** gaz *****/
		if(keys[VK_UP]==TRUE){
			if(BlokadaDlaSilnika==TRUE){
				BlokadaDlaSilnika=FALSE;
				FSOUND_PlaySound(13,Dzwieki[13]);
			}
			switch(nr_bieg){
			case 0:			// do przódu
				if(predkosc<=MaxPredkosc){
					predkosc=(predkosc+przyspieszenie)-(PasSuma*0.000002f);
				}
				if(predkosc>=0 && predkosc<=0.05){
					gaz=static_cast<int>(predkosc*500000);
				}
				if(predkosc>=0.05 && predkosc<=0.16){
					gaz=static_cast<int>(predkosc*170000);
				}
				if(predkosc>=0.16 && predkosc<=0.26){
					gaz=static_cast<int>(predkosc*95000);
				}
				if(predkosc>=0.26 && predkosc<=MaxPredkosc){
					gaz=static_cast<int>(predkosc*50000);
				}
				Odtwarzanie(gaz, 0);
				Odtwarzanie(gaz, 13);
				obrot_x=obrot_x+kierownica;
				kierunek=1;
				blokada_a=1;
				x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
				z += static_cast<float>(cos(obrot_x*pi180) * predkosc); 
				if(pochylenieX<0.1){	// resor przód-ty³
					pochylenieX=pochylenieX+0.02f;
				}
				zapBieg=0;				//zapamiêtaie biegu
				iloscPasazerow=rand()/3000;
				WylosowaniWy=static_cast<float>(rand()/1000);//losowanie pasa¿erów wychodz¹cych
				WylosowaniWch=static_cast<float>(rand()/1000);// --------//----- wchodz¹cych
				StanBiegow="przod";
			break;
			case 1:
				if(gaz<30000){	// luz 
					gaz=gaz+300;
					Odtwarzanie(gaz, 0);
					Odtwarzanie(gaz, 13);
				}
				if(pochylenieX>0){
					pochylenieX=pochylenieX-0.02f;
				}
				if(pochylenieX<0){
					pochylenieX=pochylenieX+0.02f;
				}
				blokada_a=0;
				StanBiegow="luz";
			break;
			case 2:			// wsteczny
				if(predkosc>-0.05){
					predkosc=predkosc-przyspieszenie;
				}
				gaz=static_cast<int>(-predkosc*450000);
				Odtwarzanie(gaz, 0);
				Odtwarzanie(gaz, 13);
				obrot_x=obrot_x-kierownica;
				kierunek=0;
				blokada_a=1;
				x += static_cast<float>(sin(obrot_x*pi180) * predkosc); 
				z += static_cast<float>(cos(obrot_x*pi180) * predkosc); 
				if(pochylenieX>-0.1){
					pochylenieX=pochylenieX-0.02f;
				}
				zapBieg=2;
				StanBiegow="wsteczny";
			break;
			}
		}
		if(keys[VK_UP]==FALSE){
			BlokadaDlaSilnika=TRUE;
			FSOUND_StopSound(13);
		}
	}
	/***** biegi ****/
	if(keys['Q']==TRUE){
		if(predkosc>=0){
			StanBiegow="przod";
			nr_bieg=0;
			SwiatlaTyl=FALSE;
		}
	}
	if(keys['A']==TRUE){
		StanBiegow="luz";
		nr_bieg=1;
		SwiatlaTyl=FALSE;
	}
	if(keys['Z']==TRUE){
		if(predkosc<=0){
			StanBiegow="wsteczny";
			nr_bieg=2;
			SwiatlaTyl=TRUE;
		}
	}
	/******************** paliwo **************/
	if(StartStopSilnika==FALSE){
		Paliwo=Paliwo-((gaz+1)*0.0000001f);//zu¿ywanie paliwa
	}
	if(Paliwo<=0){//gdy pusty bak
		Paliwo=0;
		StartStopSilnika=FALSE;//zatrzymaj silnik
		keys['S']=TRUE;
	}
	/**** gdy przejechany przystanek bez zatrzymania ***
	if(PrzystanekZaliczony==FALSE){
		if(BlokadaPrzystanku==FALSE){
			Pasazerowie=TRUE;
		}
	}*/
	/************ wirtualni pasa¿erowie *********/
	if(Drzwi==TRUE && PrzystanekZaliczony==TRUE && NumerPrzystanka<=(KolejnePrzystanki.IloscPrzystankow-2)){
		//BlokadaPrzystanku=TRUE;
		if(BlokadaPasWys==TRUE){//Blokada pasa¿erów wychodz¹cych
			if(PasazerowieWysiadajacy==TRUE){
				if(PasWy<=WylosowaniWy){//Wychodz¹cy
					PasazerowieWToku=TRUE;
					//Pasazerowie=FALSE;
					if(PierwszeDrzwi==TRUE){
						PasWy=PasWy+0.01f;
						PasSuma=PasSuma-0.01f;
					}
					if(DrugieDrzwi==TRUE){
						PasWy=PasWy+0.02f;
						PasSuma=PasSuma-0.02f;
					}
					if(TrzecieDrzwi==TRUE){
						PasWy=PasWy+0.02f;
						PasSuma=PasSuma-0.02f;
					}
				}
				if(PasWy>=WylosowaniWy){
					PasazerowieWsiadajacy=TRUE;
					PasazerowieWysiadajacy=FALSE;
					PasazerowieWToku=FALSE;
				}
			}
		}
			if(PasazerowieWsiadajacy==TRUE){//Wchodz¹cy
				if(PasWch<=WylosowaniWch){
					PasazerowieWToku=TRUE;
					Pasazerowie=FALSE;
					if(PierwszeDrzwi==TRUE){
						PasWch=PasWch+0.01f;
						PasSuma=PasSuma+0.01f;
					}
					if(DrugieDrzwi==TRUE){
						PasWch=PasWch+0.02f;
						PasSuma=PasSuma+0.02f;
					}
					if(TrzecieDrzwi==TRUE){
						PasWch=PasWch+0.02f;
						PasSuma=PasSuma+0.02f;
					}
				}
				if(PasWch>=WylosowaniWch){
					PasazerowieWsiadajacy=FALSE;
					PasazerowieWysiadajacy=TRUE;
					PasazerowieWToku=FALSE;
				}
			}
		//}
		if(PasSuma<=0 && NumerPrzystanka<(KolejnePrzystanki.IloscPrzystankow-1)){//gdy 0 osób w busie wtedy ustawiaj, ¿e pasa¿erowie wchodz¹
			PasSuma=0; // a blokuj wychodz¹cych
			BlokadaPasWys=FALSE;
			PasazerowieWsiadajacy=TRUE;
			PasazerowieWysiadajacy=FALSE;
			PasazerowieWToku=FALSE;
		}
		if(NumerPrzystanka>=(KolejnePrzystanki.IloscPrzystankow-2)){//ostatni przystanek
			BlokadaPasWys=FALSE;
			PasazerowieWsiadajacy=FALSE;
			PasazerowieWysiadajacy=FALSE;
			PasazerowieWToku=TRUE;
			if(PasSuma>0){// wyrzucaj wszystkich pasa¿erów w busie
				if(PierwszeDrzwi==TRUE){
					PasWy=PasWy+0.01f;
					PasSuma=PasSuma-0.01f;
				}
				if(DrugieDrzwi==TRUE){
					PasWy=PasWy+0.02f;
					PasSuma=PasSuma-0.02f;
				}
				if(TrzecieDrzwi==TRUE){
					PasWy=PasWy+0.02f;
					PasSuma=PasSuma-0.02f;
				}
			}
		}
	}
	/********* ostatni cel i pokazanie statystyki ****/
	if(SwiatloZew==FALSE && BlokadaSilnika==FALSE && PrzystanekZaliczony==TRUE && NumerPrzystanka>=(KolejnePrzystanki.IloscPrzystankow-1) && predkosc==0){//ostatni przystanek
		BlokadaPasWys=FALSE;
		PasazerowieWsiadajacy=FALSE;
		PasazerowieWysiadajacy=FALSE;
		Statystyka=TRUE;
	}
	if(Drzwi==TRUE && PierwszeDrzwi==FALSE && DrugieDrzwi==FALSE && TrzecieDrzwi==FALSE){
		Drzwi=FALSE;						//Drzwi zamkniête
		nr_bieg=zapBieg;					//wrzut na bieg DO PRZODU po zamkniêciu drzwi
		PasazerowieWToku=FALSE;
		PasWch=0;
		PasWy=0;
		if(PrzystanekZaliczony==TRUE){		//gdy na przystanku
			PasazerowieWsiadajacy=FALSE;	//najpierw wysiadaj¹
			PasazerowieWysiadajacy=TRUE;	//puŸniej wsiadaj¹
			BlokadaPasWys=TRUE;//
			if(NumerPrzystanka<(KolejnePrzystanki.IloscPrzystankow-1)){
				NumerPrzystanka++;			//gdy zaliczony przystanek przechodz do nastêpnego
			}
		}
		//PrzystanekZaliczony=FALSE;			//uniemo¿liwia ³adowanie pasa¿erów na tym samym przystanku
	}
	if(predkosc<=0.0005 && predkosc>=-0.0005){
		/***** otwarcie drzwi *******/
		if(keys[VK_DELETE] && PierwszeDrzwi==FALSE){
			Odtwarzanie(gaz, 1);//pierwsze drzwi
			PierwszeDrzwi=TRUE;
			ilPas=0;
			Pasazer=TRUE;
			Drzwi=TRUE;
			StanDrzwi1="otwarte";
			//GetKeyboardState(StanKlawisza);
			//StanKlawisza[VK_NUMLOCK]=0x0001;
			//SetKeyboardState((unsigned char*)VK_NUMLOCK)=TRUE;
		}
		if(keys[VK_END] && DrugieDrzwi==FALSE){
			Odtwarzanie(gaz, 8);//drugie drzwi
			DrugieDrzwi=TRUE;
			ilPas=0;
			Pasazer=TRUE;
			Drzwi=TRUE;
			StanDrzwi2="otwarte";
			//SetKeyboardState((unsigned char*)"0x14FF");
			//SetKeyboardState((unsigned char*)"0x9001");
			/*SetKeyboardState((unsigned char*)"0x91")==0;*/
		}
		if(keys[VK_NEXT] && TrzecieDrzwi==FALSE){
			Odtwarzanie(gaz, 10);//trzecie drzwi
			TrzecieDrzwi=TRUE;
			ilPas=0;
			Pasazer=TRUE;
			Drzwi=TRUE;
			StanDrzwi3="otwarte";
			/*SetKeyboardState((unsigned char*)"VK_CAPITAL")==0;
			SetKeyboardState((unsigned char*)"0x90")==0;
			SetKeyboardState((unsigned char*)"0x91")==1;*/
		}
		/********** zamkniêcie drzwi ********/
		if(keys[VK_INSERT] && PierwszeDrzwi==TRUE){
			Odtwarzanie(gaz, 2);//pierwsze drzwi
			if(BlokZam){
				Wl=FALSE;
			}
			PierwszeDrzwi=FALSE;
			StanDrzwi1="zamkniete";
			//GetKeyboardState(StanKlawisza);
			//StanKlawisza[VK_NUMLOCK]=0;
			//SetKeyboardState((unsigned char*)VK_NUMLOCK+0);
		}
		if(keys[VK_HOME] && DrugieDrzwi==TRUE){
			Odtwarzanie(gaz, 9);// drugie drzwi
			if(BlokZam){
				Wl=FALSE;
			}
			DrugieDrzwi=FALSE;
			StanDrzwi2="zamkniete";
			//SetKeyboardState((unsigned char*)"0x14")==1;
			//SetKeyboardState((unsigned char*)"0x9000");
			/*SetKeyboardState((unsigned char*)"0x91")==1;*/
		}
		if(keys[VK_PRIOR] && TrzecieDrzwi==TRUE){
			Odtwarzanie(gaz, 11);// trzecie drzwi
			if(BlokZam){
				Wl=FALSE;
			}
			TrzecieDrzwi=FALSE;
			StanDrzwi3="zamkniete";
			/*SetKeyboardState((unsigned char*)"0x14")=true;
			SetKeyboardState((unsigned char*)"0x90");
			SetKeyboardState((unsigned char*)"0x91");*/
		}
	}
	/*** po otwarciu drzwi wrzucaj LUZ ***/
	if(Drzwi==TRUE){
		nr_bieg=1;
	}
	/*** odchylenia bus-a przez pasa¿erów ***/
	if(Pasazer==TRUE && PrzystanekZaliczony==TRUE){
		if(ilPas<=iloscPasazerow){
			if(wchIwych<104){
			wchIwych++;
				if(PochPrzySkrecie>=-0.01 && blok==TRUE){
					PochPrzySkrecie=PochPrzySkrecie-0.0002f;
				}else{
					blok=FALSE;
				}
				if(PochPrzySkrecie<=0 && blok==FALSE){
					PochPrzySkrecie=PochPrzySkrecie+0.0002f;
				}else{
					blok=TRUE;
				}
			}else{
				if(Wl==FALSE){
					Pasazer=FALSE;
					Wl=TRUE;
				}
				wchIwych=1;
				ilPas++;
				BlokZam=TRUE;
			}
		}else{
			BlokZam=FALSE;
			Pasazer=FALSE;
		}
	}
	/********** klakson *********/
	if(keys['K']==TRUE && BlokadaKlaksonu==TRUE){
		BlokadaKlaksonu=FALSE;
		Odtwarzanie(gaz, 3);
	}
	if(keys['K']==FALSE && BlokadaKlaksonu==FALSE){
		BlokadaKlaksonu=TRUE;
		FSOUND_StopSound(3);
	}
	/********** sygna³ drzwi *********/
	if(keys[VK_SCROLL]==TRUE && BlokadaSygnalu==TRUE){
		BlokadaSygnalu=FALSE;
		Odtwarzanie(gaz, 7);
	}
	if(keys[VK_SCROLL]==FALSE && BlokadaSygnalu==FALSE){
		BlokadaSygnalu=TRUE;
		FSOUND_StopSound(7);
	}
	/********** start silnika *********/
	if(StartStopSilnika==TRUE){
		if(keys['S']==TRUE && BlokadaSilnika==TRUE){
			licznik++;
			if(BlokadaOdtwarzania==TRUE){
				Odtwarzanie(gaz, 4);
				NieudanyZaplon=TRUE;
				BlokadaOdtwarzania=FALSE;
			}
			if(FSOUND_IsPlaying(4)==FALSE && licznik<33){
				BlokadaOdtwarzania=TRUE;
			}
			if(FSOUND_IsPlaying(4)==FALSE && licznik>33){
				licznik=0;
				NieudanyZaplon=FALSE;
				gaz=4000;//ustawienie obrotów silnika
				Odtwarzanie(gaz, 99);// strat silnika
				Odtwarzanie(gaz, 0);//czêstotliwoœæ silnika
				Odtwarzanie(gaz, 13);//czêstotliwoœæ skrzyni biegów
				BlokadaSilnika=FALSE;
			}
		}
		if(NieudanyZaplon==FALSE){
			if(keys['S']==FALSE && BlokadaSilnika==FALSE){
				BlokadaSilnika=TRUE;
				BlokadaOdtwarzania=TRUE;
				StartStopSilnika=FALSE;
				BlokadaGazu=FALSE;
			}
		}
	}
	/********** stop silnika *********/
	if(StartStopSilnika==FALSE){
		if(keys['S']==TRUE && BlokadaSilnika==TRUE){
			if(NieudanyZaplon==FALSE){
				blokada_a=0;
				BlokadaSilnika=FALSE;
				FSOUND_StopSound(0);
				Odtwarzanie(gaz, 5);
				BlokadaGazu=TRUE;
			}
		}
		if(keys['S']==FALSE && BlokadaSilnika==FALSE){
			BlokadaSilnika=TRUE;
			StartStopSilnika=TRUE;
		}
	}
	/**** algorytm odpowiadaj¹cy za to, aby 'wskazowka' by³a ci¹gle 
			dodatnia *****/
	if(predkosc>=0){
		wskazowka=predkosc;
	}else{
		wskazowka=-predkosc;
	}
	/****** kierunkowskaz Prawy **********/
	if(KierP==TRUE){// tykanie co sek.
		Czas=static_cast<int>(GetTickCount() * 0.001f);
		if(Czas-CzasOstatni>=1.0){
			CzasOstatni=static_cast<int>(GetTickCount() * 0.001f);
			CzasOstatni=Czas;
			Odtwarzanie(gaz, 6);
			StrzalkaKierP=!StrzalkaKierP;
		}
		if(kierownica<0){
			KierP=FALSE;
		}
		KierL=FALSE;
	}else{
		StrzalkaKierP=FALSE;
	}
	if(keys[0xBE]==TRUE && BlokadaKierP==FALSE){
		BlokadaKierP=TRUE;
		KierP=!KierP;
	}
	if(keys[0xBE]==FALSE && BlokadaKierP==TRUE){
		BlokadaKierP=FALSE;
	}
	/****** kierunkowskaz Lewy**********/
	if(KierL==TRUE){// tykanie co sek.
		Czas=static_cast<int>(GetTickCount() * 0.001f);
		if(Czas-CzasOstatni>=1.0){
			CzasOstatni=static_cast<int>(GetTickCount() * 0.001f);
			CzasOstatni=Czas;
			Odtwarzanie(gaz, 6);
			StrzalkaKierL=!StrzalkaKierL;
		}
		KierP=FALSE;
		if(kierownica>0){
			KierL=FALSE;
		}
	}else{
		StrzalkaKierL=FALSE;
	}
	if(keys[0xBC]==TRUE && BlokadaKierL==FALSE){
		BlokadaKierL=TRUE;
		KierL=!KierL;
	}
	if(keys[0xBC]==FALSE && BlokadaKierL==TRUE){
		BlokadaKierL=FALSE;
	}
	/****** œwiat³o wewnêtrzne ******/
	if(keys['X']==TRUE && BlokadaX==FALSE){
		SwiatloWew=!SwiatloWew;
		BlokadaX=TRUE;
	}
	if(keys['X']==FALSE){
		BlokadaX=FALSE;
	}
	/****** œwiat³o zewnêtrzne ******/
	if(keys['C']==TRUE && BlokadaC==FALSE){
		SwiatloZew=!SwiatloZew;
		BlokadaC=TRUE;
	}
	if(keys['C']==FALSE){
		BlokadaC=FALSE;
	}
	/******* Zmiana kamery: Kokpit *****/
	if(keys[VK_F1]==TRUE){
		UstawKamere.Odleglosc=5;
		UstawKamere.Wysokosc=1.6f;
		UstawKamere.PunktPatrzenia=-40;
		UstawKamere.Obrot=-0.75f;
		JakaKamera=TRUE;
	}
	if(keys[VK_F2]==TRUE){ /*** z ty³u ***/
		UstawKamere.Odleglosc=-14;
		UstawKamere.Wysokosc=4;
		UstawKamere.PunktPatrzenia=-100;
		UstawKamere.Obrot=0.0f;
		JakaKamera=FALSE;
	}
	if(keys[VK_F3]==TRUE){ /*** kamera na zderzalu ***/
		UstawKamere.Odleglosc=7;
		UstawKamere.Wysokosc=0.5f;
		UstawKamere.PunktPatrzenia=-100;
		UstawKamere.Obrot=0.0f;
		JakaKamera=TRUE;
	}
	if(keys[VK_F4]==TRUE){ /*** kamera na dachu ***/
		UstawKamere.Odleglosc=0;
		UstawKamere.Wysokosc=4;
		UstawKamere.PunktPatrzenia=-100;
		UstawKamere.Obrot=0;
		JakaKamera=FALSE;
	}
	if(keys[VK_F5]==TRUE){ /*** Kamera z przodu pojazdu ***/
		UstawKamere.Odleglosc=20;
		UstawKamere.Wysokosc=3;
		UstawKamere.PunktPatrzenia=100;
		UstawKamere.Obrot=0;
		JakaKamera=FALSE;
	}
	if(keys[VK_F6]==TRUE){ /*** widok pasa¿era ***/
		UstawKamere.Odleglosc=0;
		UstawKamere.Wysokosc=1.7f;
		UstawKamere.PunktPatrzenia=-50;
		UstawKamere.Obrot=-0.9f;
		JakaKamera=FALSE;
	}
	if(keys[VK_NUMPAD7]==TRUE){ /*obrot kamery*/
		UstawKamere.Obrot=UstawKamere.Obrot-0.1f;
	}
	if(keys[VK_NUMPAD9]==TRUE){ /*obrot kamery*/
		UstawKamere.Obrot=UstawKamere.Obrot+0.1f;
	}
	if(keys[VK_NUMPAD1]==TRUE){ /*obrot kamery*/
		UstawKamere.Obrot_a=UstawKamere.Obrot_a-1;
	}
	if(keys[VK_NUMPAD3]==TRUE){ /*obrot kamery*/
		UstawKamere.Obrot_a=UstawKamere.Obrot_a+1;
	}
	if(keys[VK_NUMPAD8]==TRUE){ /*obrot kamery*/
		UstawKamere.Odleglosc=UstawKamere.Odleglosc+0.1f;
	}
	if(keys[VK_NUMPAD2]==TRUE){ /*obrot kamery*/
		UstawKamere.Odleglosc=UstawKamere.Odleglosc-0.1f;
	}
	if(keys[VK_NUMPAD5]==TRUE){ /*centrowanie kamery*/
		switch(static_cast<int>(UstawKamere.Odleglosc)){
		case 5:
			UstawKamere.Obrot_a=0;
			UstawKamere.Obrot=-0.75f;
			//UstawKamere.Odleglosc=
			break;
		case 0:
			UstawKamere.Obrot_a=0;
			UstawKamere.Obrot=-0.9f;
			break;
		default:
			UstawKamere.Obrot_a=0;
			UstawKamere.Obrot=0;
			break;
		}
	}
	/*if(keys[VK_NUMPAD4]==true){ obrot kamery*
		obrot_f125=obrot_f125-1;
	}
	if(keys[VK_NUMPAD6]==true){ /*obrot kamery*
		obrot_f125=obrot_f125+1;
	}
	/***************** swiat³a sygnalizacyjne *********/
	CzasSD=static_cast<int>(GetTickCount() * 0.001f);
	if(CzasSD-CzasOstatniSD>=1.0){
		CzasOstatniSD=static_cast<int>(GetTickCount() * 0.001f);
		CzasOstatniSD=CzasSD;
		CzasSwiatel++;
		switch(CzasSwiatel){
		case 20:
			DrogoweCzerwony=TRUE;
			DrogoweZolty=TRUE;
			DrogoweZielony=FALSE;
			break;
		case 23:
			DrogoweZielony=TRUE;
			DrogoweCzerwony=FALSE;
			DrogoweZolty=FALSE;
			break;
		case 43:
			DrogoweZolty=TRUE;
			DrogoweZielony=FALSE;
			DrogoweCzerwony=FALSE;
			break;
		case 46:
			DrogoweCzerwony=TRUE;
			DrogoweZolty=FALSE;
			DrogoweZielony=FALSE;
			CzasSwiatel=0;
			break;
		}
	}
	/*********** Lusterka wsteczne ******************/
	if(keys['L']==TRUE && BlokadaL==FALSE){ 
		WyborLuster=WyborLuster+1;
		BlokadaL=TRUE;
		if(WyborLuster>=4){
			WyborLuster=1;
		}
	}
	if(keys['L']==FALSE){
		BlokadaL=FALSE;
	}
/******************* Odtwarzacz *********************************/
	/*********** Poprzednia ******************/
	if(keys['1']==TRUE && BlokadaPoprz==TRUE){
		BlokadaPoprz=FALSE;
		if(NumerPiosenki>2){
			NumerPiosenki--;
			FSOUND_Stream_SetTime(Muzyka[NumerPiosenki], 0);
			FSOUND_Stream_Play(21, Muzyka[NumerPiosenki]);
		}
	}
	if(keys['1']==FALSE){
		BlokadaPoprz=TRUE;
	}
	/*********** Play ******************/
	if(keys['2']==TRUE && BlokadaPlay==TRUE){ 
		if(NumerPiosenki>=3){
			BlokadaPlay=FALSE;
			FSOUND_Stream_SetTime(Muzyka[NumerPiosenki], 0);
			FSOUND_Stream_Play(21, Muzyka[NumerPiosenki]);
		}
	}
	if(keys['2']==FALSE){
		BlokadaPlay=TRUE;
	}
	/*********** Stop ******************/
	if(keys['3']==TRUE){ 
		FSOUND_Stream_Stop(Muzyka[NumerPiosenki]);
	}
	/*********** Nastêpna ******************/
	if(keys['4']==TRUE && BlokadaNast==TRUE){
		BlokadaNast=FALSE;
		if(NumerPiosenki<IloscPiosenek){
			NumerPiosenki++;
			FSOUND_Stream_SetTime(Muzyka[NumerPiosenki], 0);
			FSOUND_Stream_Play(21, Muzyka[NumerPiosenki]);
		}
	}
	if(keys['4']==FALSE){
		BlokadaNast=TRUE;
	}
	AktualnaPozycja = FSOUND_Stream_GetTime(Muzyka[NumerPiosenki]);
	DlugoscPiosenki = FSOUND_Stream_GetLengthMs(Muzyka[NumerPiosenki]);
	if(AktualnaPozycja>=DlugoscPiosenki){
		if(NumerPiosenki<IloscPiosenek && NumerPiosenki>=3){
			NumerPiosenki++;
			FSOUND_Stream_Play(21, Muzyka[NumerPiosenki]);
		}
	}
}
}
/************************* szukacz plików ************************/
void SzukajPlikow(char szSciezkaDost[])
{
WIN32_FIND_DATA DanePliku;
HANDLE UchwytSzukania;
char szNowaSciezka[MAX_PATH];
char szSciezka2[MAX_PATH];

	lstrcpy(szSciezka2, szSciezkaDost);
	lstrcat(szSciezka2, "*.mp3");

	UchwytSzukania = FindFirstFile(szSciezka2, &DanePliku); 

	do{
			NumerPiosenki++;
			lstrcpy(szNowaSciezka, szSciezkaDost); 
			lstrcat(szNowaSciezka, DanePliku.cFileName); 
			Muzyka[NumerPiosenki] = FSOUND_Stream_Open(szNowaSciezka, FSOUND_LOOP_NORMAL,0,0);
	}
	while(FindNextFile(UchwytSzukania, &DanePliku));
	FindClose(UchwytSzukania);
	IloscPiosenek=NumerPiosenki;
	NumerPiosenki=2;
}
/**************************** wyliczanie czasu *******************/
void WyliczCzas()
{

	Sekundy=Sekundy+1;
	if(Sekundy==60){
		Sekundy=0;
		Minuty=Minuty+1;
	}
	if(Minuty==60){
		Minuty=0;
		Godziny=Godziny+1;
	}
	if(Godziny==24){
		Godziny=0;
	}

}
/********************* Klawiatura Menu ***************************/
void __fastcall KlawiaturaMenu()
{

	if(keys[VK_DOWN]==TRUE && BlokadaMenuD==FALSE){
		WyborMenu++;
		BlokadaMenuD=TRUE;
	}
	if(keys[VK_UP]==TRUE && BlokadaMenuG==FALSE){
		WyborMenu--;
		BlokadaMenuG=TRUE;
	}
	if(keys[VK_UP]==FALSE){
		BlokadaMenuG=FALSE;
	}
	if(keys[VK_DOWN]==FALSE){
		BlokadaMenuD=FALSE;
	}
	if(WyborMenu<=GranicaDolna-1){
		WyborMenu=GranicaDolna;
	}
	if(WyborMenu>=GranicaGorna){
		WyborMenu=GranicaGorna-1;
	}
	if(keys[VK_RETURN]==TRUE && BlokadaEnter==FALSE && BlokadaEnterAnim==FALSE){
		BlokadaEnter=TRUE;
		switch(WyborMenu){
		case 0:
			NowaGra=TRUE;
			break;
		case 1:
			Autobusy=TRUE;
			break;
		case 2:
			OGrze=TRUE;
			break;
		case 3:
			Wyjscie=TRUE;
			break;
		case 4:
			WyjscieDoMenu=TRUE;
			break;
		}
	}
	if(keys[VK_RETURN]==FALSE){
		BlokadaEnter=FALSE;
	}

}

