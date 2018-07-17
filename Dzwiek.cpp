#include "Inicjalizacja.h"
#include "fmod.h"

extern HWND		hWnd;
extern GLint KartaDzwiekowa;
extern bool TypAutobusu;

int NumerTablicy;
char SciezkaDostDzwieku[255]="Dane/Dzwiek/";
FSOUND_SAMPLE *Dzwieki[14];
FSOUND_STREAM *Muzyka[100];
/******************* Inicjalizacja BASS-a ************************/
void InitDzwiek(char *RodzajAutobusu)
{
	if (FSOUND_GetVersion() < FMOD_VERSION) 
    {
		MessageBox(hWnd, "Niew³aœciwa wersja biblioteki FMOD.DLL", "B³¹d", MB_OK);     
    }
    FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
    FSOUND_SetDriver(KartaDzwiekowa);
    FSOUND_SetMixer(FSOUND_MIXER_QUALITY_MMXP6);
    FSOUND_SetBufferSize(50);

    if (!FSOUND_Init(44100,32,FSOUND_INIT_GLOBALFOCUS)) 
    {
		MessageBox(hWnd, "Inicjalizacja dŸwiêku zakoñczy³a siê niepowodzeniem!", "B³¹d", MB_OK);
    }

     //FSOUND_SetPanSeperation(1);

     //FMUSIC_PlaySong(Mod);
	/*do{
		lstrcat(SciezkaDostDzwieku, RodzajAutobusu);
		lstrcat(RodzajAutobusu, "/kierunkowskaz.sfx");
		
		Dzwieki[NumerTablicy] = FSOUND_Sample_Load(FSOUND_FREE,RodzajAutobusu, FSOUND_LOOP_NORMAL,0,0);*/
		Dzwieki[0] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/skrzynia_biegow.sfx", FSOUND_LOOP_NORMAL, 0, 0);
		Dzwieki[1] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/drzwi_o.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[2] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/drzwi_z.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[3] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/klakson.sfx", FSOUND_LOOP_NORMAL,0,0);
		Dzwieki[4] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/silnik_start.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[5] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/silnik_stop.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[6] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/kierunkowskaz.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[7] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/sygnal.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[8] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/drzwi_o.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[9] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/drzwi_z.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[10] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/drzwi_o.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[11] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/drzwi_z.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[12] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/hamulec_powietrze.sfx", FSOUND_LOOP_OFF,0,0);
		Dzwieki[13] = FSOUND_Sample_Load(FSOUND_FREE,"Dane/Dzwiek/Volvo7000/silnik.sfx", FSOUND_LOOP_NORMAL,0,0);
		/*NumerTablicy++;
	}
	while(NumerTablicy==14);*/

	Muzyka[0] = FSOUND_Stream_Open("Dane/Menu/muzyka.wav", FSOUND_LOOP_NORMAL,0,0);
	//FSOUND_Stream_SetMode(Muzyka[0], FSOUND_LOOP_NORMAL);

}
/****************** Odtwarzanie sampli ***************************/
void Odtwarzanie(int gaz, int sample)
{
	switch(sample){
	case 0:
		FSOUND_SetFrequency(0, gaz);
		break;
	case 1:
		FSOUND_PlaySound(1,Dzwieki[1]);
		break;
	case 2:
		FSOUND_PlaySound(2,Dzwieki[2]);
		break;
	case 3:
		FSOUND_PlaySound(3,Dzwieki[3]);
		break;
	case 4:
		FSOUND_PlaySound(4,Dzwieki[4]);
	break;
	case 5:
		FSOUND_PlaySound(5,Dzwieki[5]);
	break;
	case 6:
		FSOUND_PlaySound(6,Dzwieki[6]);
	break;
	case 7:
		FSOUND_PlaySound(7,Dzwieki[7]);
	break;
	case 8:
		FSOUND_PlaySound(8,Dzwieki[8]);
	break;
	case 9:
		FSOUND_PlaySound(9,Dzwieki[9]);
	break;
	case 10:
		FSOUND_PlaySound(10,Dzwieki[10]);
	break;
	case 11:
		FSOUND_PlaySound(11,Dzwieki[11]);
	break;
	case 12:
		FSOUND_PlaySound(12,Dzwieki[12]);
	break;
	case 13:
		FSOUND_SetFrequency(13, gaz);
		break;
	case 99:
		FSOUND_PlaySound(0,Dzwieki[0]);
	break;
	}
} 
/******************* Koniec u¿ywania dŸwiêku *********************/
void NiszczDzwiek()
{		
	FSOUND_Sample_Free(Dzwieki[0]);
	Dzwieki[0] = NULL;
	FSOUND_Sample_Free(Dzwieki[1]);
	Dzwieki[1] = NULL;
	FSOUND_Sample_Free(Dzwieki[2]);
	Dzwieki[2] = NULL;
	FSOUND_Sample_Free(Dzwieki[3]);
	Dzwieki[3] = NULL;
	FSOUND_Sample_Free(Dzwieki[4]);
	Dzwieki[4] = NULL;
	FSOUND_Sample_Free(Dzwieki[5]);
	Dzwieki[5] = NULL;
	FSOUND_Sample_Free(Dzwieki[6]);
	Dzwieki[6] = NULL;
	FSOUND_Sample_Free(Dzwieki[7]);
	Dzwieki[7] = NULL;
	FSOUND_Sample_Free(Dzwieki[8]);
	Dzwieki[8] = NULL;
	FSOUND_Sample_Free(Dzwieki[9]);
	Dzwieki[9] = NULL;
	FSOUND_Sample_Free(Dzwieki[10]);
	Dzwieki[10] = NULL;
	FSOUND_Sample_Free(Dzwieki[11]);
	Dzwieki[11] = NULL;
	FSOUND_Sample_Free(Dzwieki[12]);
	Dzwieki[12] = NULL;
	FSOUND_Sample_Free(Dzwieki[13]);
	Dzwieki[13] = NULL;
	//FSOUND_Stream_Close(Muzyka[0]);
	Muzyka[0] = NULL;
	FSOUND_Close();
}