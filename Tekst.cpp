#include "Inicjalizacja.h"

GLuint base;
extern GLuint Czcionka[1];

/*************************** Budowanie czcionek ******************/
GLvoid BudujCzcionki(void)
{
	float	cx;
	float	cy;
	base=glGenLists(256); //Twórz 256 List
	//glBindTexture(GL_TEXTURE_2D, Czcionka[0]);
	for (int loop=0; loop<256; loop++)
	{
		cx=static_cast<float>(loop%16)/16.0f;
		cy=static_cast<float>(loop/16)/16.0f;
			glNewList(base+loop,GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(cx,1-cy-0.0625f);//Dó³ Lewa
				glVertex2i(0,0);
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);//Dó³ Prawa
				glVertex2i(16,0);
				glTexCoord2f(cx+0.0625f,1-cy);//Góra prawa
				glVertex2i(16,16);
				glTexCoord2f(cx,1-cy);//Góra lewa
				glVertex2i(0,16);
			glEnd();
		glTranslated(11,0,0);
		glEndList();		
	}
}
/***************** Usuwanie listy czcionek ***********************/
GLvoid NiszczCzcionki(void)
{
 	glDeleteLists(base,256);
}
/***************** wyœwietlanie czcionek *************************/
GLvoid WyswietlTekst(GLint x, GLint y, int set, char *string, ...)
{

	char		text[256];
	va_list		ap;
	va_start(ap, string);
	vsprintf(text, string, ap);
	va_end(ap);
 
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();	
	glLoadIdentity();
	glOrtho(0,640,0,480,-1,1);
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);
	glListBase(base-32+(128*set));	
	glBindTexture(GL_TEXTURE_2D, Czcionka[0]);
	glCallLists(strlen(text),GL_BYTE,text);
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();	
	glEnable(GL_DEPTH_TEST);

}

