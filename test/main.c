#include <stdio.h>
#include <stdlib.h>
#include <windows.h>  // only for windows platform
#include <GL/glut.h>
#include<math.h>
#include<winbase.h>
#include<MMsystem.h>

#include<time.h>
time_t t;   // not a primitive datatype

int flag=0, tempFlag=0; // drop down
char name[64]="Loading...";
int keyboardindex;
float i=0;
int stage=1;
char ch='b';
int poweroff=0,signedIn=0;
int xpos, ypos;
float x1,y1,x2,y2;
float angle;
double radius;
char *st;
char *st1;
int clk =0;
int thisPC=0;
int rebin = 0;
int start=0;
int network=0;
int DclickFlag=0;
int num=1;
int change=0;

int refx1, refy1, refx2,refy2, refx3, refy3, refx4, refy4;

int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y
int fullScreenMode = 1; // Full-screen or windowed mode?

// Print screen
BOOL SaveToFile(HBITMAP hBitmap3, LPCTSTR lpszFileName)
{
  HDC hDC;
  int iBits;
  WORD wBitCount;
  DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
  BITMAP Bitmap0;
  BITMAPFILEHEADER bmfHdr;
  BITMAPINFOHEADER bi;
  LPBITMAPINFOHEADER lpbi;
  HANDLE fh, hDib, hPal,hOldPal2=NULL;
  hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
  iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
  DeleteDC(hDC);
  if (iBits <= 1)
    wBitCount = 1;
  else if (iBits <= 4)
    wBitCount = 4;
  else if (iBits <= 8)
    wBitCount = 8;
  else
    wBitCount = 24;
  GetObject(hBitmap3, sizeof(Bitmap0), (LPSTR)&Bitmap0);
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = Bitmap0.bmWidth;
  bi.biHeight =-Bitmap0.bmHeight;
  bi.biPlanes = 1;
  bi.biBitCount = wBitCount;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrImportant = 0;
  bi.biClrUsed = 256;
  dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount +31) & ~31) /8
                                                * Bitmap0.bmHeight;
  hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
  lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
  *lpbi = bi;

  hPal = GetStockObject(DEFAULT_PALETTE);
  if (hPal)
  {
    hDC = GetDC(NULL);
    hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
    RealizePalette(hDC);
  }


  GetDIBits(hDC, hBitmap3, 0, (UINT) Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
    +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

  if (hOldPal2)
  {
    SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
    RealizePalette(hDC);
    ReleaseDC(NULL, hDC);
  }

  fh = CreateFile(lpszFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (fh == INVALID_HANDLE_VALUE)
    return FALSE;

  bmfHdr.bfType = 0x4D42; // "BM"
  dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
  bmfHdr.bfSize = dwDIBSize;
  bmfHdr.bfReserved1 = 0;
  bmfHdr.bfReserved2 = 0;
  bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

  WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

  WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
  GlobalUnlock(hDib);
  GlobalFree(hDib);
  CloseHandle(fh);

  return TRUE;
}

int screenCapture(int x, int y, int w, int h, LPCSTR fname)
{
    HDC hdcSource = GetDC(NULL);
    HDC hdcMemory = CreateCompatibleDC(hdcSource);

    int capX = GetDeviceCaps(hdcSource, HORZRES);
    int capY = GetDeviceCaps(hdcSource, VERTRES);

    HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, w, h);
    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

    BitBlt(hdcMemory, 0, 0, w, h, hdcSource, x, y, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

    DeleteDC(hdcSource);
    DeleteDC(hdcMemory);

    HPALETTE hpal = NULL;
    if(SaveToFile(hBitmap, fname)) return 1;
    return 0;
}
// print screen ends

void Sprint( int x, int y, char *st)
{
    int  l,i;
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( x, y); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }
}

void intro(){
    clearing();
    glBegin(GL_POLYGON);
        glColor3f(0, 1,1);
        glVertex2f(0,0);
        glVertex2f(1367,0);
        glColor3f(1, 0.8,0.8);
        glVertex2f(1367,383);
        glVertex2f(683,0);
    glEnd();
    glFlush();

    glBegin(GL_POLYGON);
        glColor3f(1,0,1);
        glVertex2f(0,767);
        glVertex2f(1367,767);
        glColor3f(1, 0.8,0.8);
        glVertex2f(1367,383);
        glVertex2f(683,767);
    glEnd();
    glFlush();

        int  l,i;
    glColor3f(0,0,0);
    st = "Door Operating System";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(550, 100); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

    glColor3f(0,0,0);
    st = "Under Guidance of, ";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(150, 300); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }
    glColor3f(0,0,0);
    st = "Mr.Puneeth R.P.";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(200, 330); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
    }



     glColor3f(0,0,0);
    st = "M.Tech. Asst. Prof Gd II";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(200, 360); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15	, st[i]); // Print a character on the screen
    }

     glColor3f(0,0,0);
    st = "NMAMIT, Nitte";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(200, 390); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15	, st[i]); // Print a character on the screen
    }


     glColor3f(0,0,0);
    st = "Submitted by:";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(750, 300); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    glColor3f(0,0,0);
    st = "Akash L M - 4NM19CS012";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(800, 330); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
    }

    glColor3f(0,0,0);
    st = "Aniket Ajit Pai - 4NM19CS020";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(800, 360); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
    }

    glFlush();

    // Map instructions
    glColor3f(0,0,0);
     st = "1 -> Introduction";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 650); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

     st = "2 -> Description";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 680); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

     st = "3 -> Boot";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 710); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    glFlush();

}

void des(){
    clearing();


    glBegin(GL_POLYGON);
        glColor3f(1, 0.65,0);
        glVertex2f(0,0);
        glVertex2f(1367,0);
        glColor3f(1, 0.8,0.8);
        glVertex2f(1367,383);
        glVertex2f(683,0);
    glEnd();
    glFlush();

     glBegin(GL_POLYGON);
        glColor3f(0,0,1);
        glVertex2f(0,767);
        glVertex2f(1367,767);
        glColor3f(1, 0.8,0.8);
        glVertex2f(1367,383);
        glVertex2f(683,767);
    glEnd();
    glFlush();

    int  l,i;
    glColor3f(0,0,1);
    st = "Description";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(630, 100); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

    st = "Topic: Door Operating System";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(550, 140); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

    // description
    st = "Abstract  :";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 200); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

    st = "Here in this project we are emulating GUI of Operating System using OpenGL (Open GL is a software interface to graphics hardware.)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 230); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "Open GL is a cross-language, cross-platform application programming interface (API) for rendering 2D and 3D vector graphics.";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 260); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "Features :";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 290); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "      1) Used Keyboard Interaction -> F1 : Display Mode, Esc : Exit , 1 : Intro, 2 : Synopsis, 3 : Boot";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 320); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "      2) Used Mouse Interaction -> To refresh, Open a directory, To lock screen, To turn OFF, so on...";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 350); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "Extra Features Included : ";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 380); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "          Time(Format : Day Month Date Time Year )";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 410); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "          Start Menu.";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 440); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "          Drop Down Options";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 470); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "          Background Music";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 500); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "          Double Click Functionality";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 530); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "          Two Way Authentication";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 560); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "              -> on Click";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 590); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    st = "              -> Use Password";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(100, 620); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

     st = "Thank You!!!";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(600, 650); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

     st = "Press 3 to proceed ->";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(580, 680); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }


    // Map instructions
    glColor3f(0,0,0);
     st = "1 -> Introduction";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 650); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

     st = "2 -> Description";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 680); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

     st = "3 -> Boot";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 710); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }



    glFlush();

}

void clearing(){
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void exiting(){
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
}
// while booting
void load(){
    intro();
    // screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\intro.bmp");
    sleep(15);

   des();
   // screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\des.bmp");
   sleep(25);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glColor3f(1,1,1);
    int  l,i;
    st = "Press ESC to quit!";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 1200, 730); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }
    st = "Press F1 to switch FullScreen/Window Mode!";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 973, 758); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

    glFlush();
    // screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\BlackScreen.bmp");
    sleep(1);
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1,1,1);
        glVertex2f(30,30);
        glVertex2f(31,30);
        glVertex2f(32,30);
        glVertex2f(33,30);
        glVertex2f(34,30);
        glVertex2f(35,30);
        glVertex2f(36,30);
        glVertex2f(37,30);
    glEnd();
    glFlush();
    sleep(2);

    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(0,0,0);
        glVertex2f(30,30);
        glVertex2f(31,30);
        glVertex2f(32,30);
        glVertex2f(33,30);
        glVertex2f(34,30);
        glVertex2f(35,30);
        glVertex2f(36,30);
        glVertex2f(37,30);
    glEnd();
    glFlush();
    sleep(1);
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1,1,1);
        glVertex2f(30,30);
        glVertex2f(31,30);
        glVertex2f(32,30);
        glVertex2f(33,30);
        glVertex2f(34,30);
        glVertex2f(35,30);
        glVertex2f(36,30);
        glVertex2f(37,30);
    glEnd();
    glFlush();
    sleep(2);


    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(0,0,0);
        glVertex2f(30,30);
        glVertex2f(31,30);
        glVertex2f(32,30);
        glVertex2f(33,30);
        glVertex2f(34,30);
        glVertex2f(35,30);
        glVertex2f(36,30);
        glVertex2f(37,30);
    glEnd();
    glFlush();
    sleep(1);
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1,1,1);
        glVertex2f(30,30);
        glVertex2f(31,30);
        glVertex2f(32,30);
        glVertex2f(33,30);
        glVertex2f(34,30);
        glVertex2f(35,30);
        glVertex2f(36,30);
        glVertex2f(37,30);
    glEnd();
    glFlush();
    sleep(2);
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(0,0,0);
        glVertex2f(30,30);
        glVertex2f(31,30);
        glVertex2f(32,30);
        glVertex2f(33,30);
        glVertex2f(34,30);
        glVertex2f(35,30);
        glVertex2f(36,30);
        glVertex2f(37,30);
    glEnd();
    glFlush();
    sleep(1);

    // shift screen


    glPointSize(2.0);
    glLineWidth(1.0);
    // Background
    glBegin(GL_POLYGON);
    glColor3f(0, 0.90, 0.93);
        glVertex2f(0,0);
        glVertex2f(1365,0);
        glVertex2f(1365,767);
    glColor3f(0.95, 0.90, 0.93);
        glVertex2f(0,767);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0, 0, 0);
        glVertex2f(1280,0);
        glVertex2f(1265,0);
        glVertex2f(1365,100);
        glVertex2f(1365,85);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(1250,0);
        glVertex2f(1235,0);
        glVertex2f(1365,130);
        glVertex2f(1365,115);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(410,0);
        glVertex2f(395,0);
        glVertex2f(1162,767);
        glVertex2f(1177,767);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0, 0, 0);
        glVertex2f(385,0);
        glVertex2f(370,0);
        glVertex2f(1137,767);
        glVertex2f(1152,767);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0, 0, 0);
        glVertex2f(360,0);
        glVertex2f(0,0);
        glVertex2f(0,360);
        glVertex2f(407,767);
        glVertex2f(1127,767);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(0,370);
        glVertex2f(0,385);
        glVertex2f(382,767);
        glVertex2f(397,767);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(0,395);
        glVertex2f(0,410);
        glVertex2f(357,767);
        glVertex2f(372,767);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(0,707);
        glVertex2f(0,722);
        glVertex2f(45,767);
        glVertex2f(60,767);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(0,682);
        glVertex2f(0,697);
        glVertex2f(70,767);
        glVertex2f(85,767);
    glEnd();

    glFlush();

    glColor3f(1,1,1);
    Sprint( 665, 400, "Loading" );
    Sprint( 678, 420, "Door" );
    glFlush();

    glLineWidth(5);
    sleep(2);
    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
        glVertex2f(240,590);
        glVertex2f(240,660);
        glVertex2f(1100,660);
        glVertex2f(1100,590);
    glEnd();
    glFlush();

    glLineWidth(1);
    sleep(2);
    glBegin(GL_POLYGON);
    glColor3f(0,1,0);
        glVertex2f(250,600);
        glVertex2f(250,650);
        glVertex2f(596,650);
        glVertex2f(596,600);
    glEnd();
    glFlush();

    sleep(2);

    glBegin(GL_POLYGON);
    glColor3f(0,1,0);
        glVertex2f(596,600);
        glVertex2f(596,650);
        glVertex2f(819,650);
        glVertex2f(819,600);
    glEnd();
    glFlush();
    // /screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\Loading.bmp");
    sleep(2);

    glBegin(GL_POLYGON);
    glColor3f(0,1,0);
        glVertex2f(819,600);
        glVertex2f(819,650);
        glVertex2f(1092,650);
        glVertex2f(1092,600);
    glEnd();
    glFlush();
    PlaySound(TEXT("start.wav"),NULL,SND_ASYNC);
    Sleep(1);

    // 1   4
    // 2   3
}


// Home Screen
void mainScreen(){

    num=1;
    glClearColor(0, 0.5294, 0.9686,1);
    glClear(GL_COLOR_BUFFER_BIT);
    // Background screen
    //left 3
    glColor3f(0.0235, 0.2784, 0.67058);
    glBegin(GL_POLYGON);
            glVertex2f(0,0);
            glVertex2f(924,0);
            glVertex2f(168,767);
            glVertex2f(0,767);
    glEnd();


    //left 2
    glColor3f(0.0235, 0.39607, 0.81176);
    glBegin(GL_POLYGON);
            glVertex2f(0,0);
            glVertex2f(0,627);
            glVertex2f(619,0);
    glEnd();
    // left 1
    glColor3f(0.01176, 0.466667, 0.90196);
    glBegin(GL_POLYGON);
            glVertex2f(0,0);
            glVertex2f(0,323);
            glVertex2f(320,0);
    glEnd();
    // right 3
    glColor3f(0.98039, 0.75294, 0.12549);
    glBegin(GL_POLYGON);
            glVertex2f(681,250);
            glVertex2f(1365,633);
            glVertex2f(1365,0);
            glVertex2f(924,0);
    glEnd();
    // right 2
    glColor3f(0.929411, 0.85098, 0.172549);
    glBegin(GL_POLYGON);
            glVertex2f(812,120);
            glVertex2f(924,0);
            glVertex2f(1365,0);
            glVertex2f(1365,428);
    glEnd();
    // right 1
     glColor3f(0.9019607, 0.921568, 0.21176);
    glBegin(GL_POLYGON);
            glVertex2f(924,0);
            glVertex2f(1365,0);
            glVertex2f(1365,220);
    glEnd();
    glFlush();

    // border

    // left 3 border
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
            glVertex2f(924,0);
            glVertex2f(168,767);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(186,767);
            glVertex2f(942,0);

    glEnd();
    //left 2 border
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
            glVertex2f(0,627);
            glVertex2f(619,0);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(637,0);
            glVertex2f(0,645);
    glEnd();
    //left 1 border
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
            glVertex2f(0,323);
            glVertex2f(320,0);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(338,0);
            glVertex2f(0,341);
    glEnd();

    //right 3 border
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
            glVertex2f(1365,633);
            glVertex2f(681,250);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(670,260);
            glVertex2f(1365,651);
    glEnd();

     //right 2 border
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
            glVertex2f(1365,428);
            glVertex2f(812,120);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(801,130);
            glVertex2f(1365,446);
    glEnd();
     //right 1 border
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
            glVertex2f(1365,220);
            glVertex2f(952,0);
            glColor3f(0.5,0.5,0.5);
            glVertex2f(925,0);
            glVertex2f(1365,238);
    glEnd();

    glFlush();


    // paint icon
    /*
    for(int k=50; k<700; k=k+100){

        glBegin(GL_POLYGON);
        glColor3f(0,0,0);
            glVertex2f(50,k+50);
            glVertex2f(50,k);
            glVertex2f(100,k);
            glVertex2f(100,k+50);
        glEnd();
        st="Hello World";
         int  l,i;
         glColor3f(1,1,1);
        l=strlen( st ); // see how many characters are in text string.
        glRasterPos2i( 50, k+70); // location to start printing text
        for( i=0; i < l; i++)  // loop until i is greater then l
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, st[i]); // Print a character on the screen
        }
    }*/
    // This PC
    // pole
    glBegin(GL_POLYGON);
    glColor3f(0,0,0.8);
        glVertex2f(72,80);
        glVertex2f(72,90);
        glVertex2f(77,90);
        glVertex2f(77,80);
    glEnd();

    // box
    glBegin(GL_POLYGON);
    glColor3f(0.2,0.2,0.2);
        glVertex2f(60,84);
        glVertex2f(60,60);
        glVertex2f(90,60);
        glVertex2f(90,84);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5,0.5,0.8);
        glVertex2f(63,81);
        glVertex2f(63,63);
        glVertex2f(87,63);
        glVertex2f(87,81);
    glEnd();

    // stand
   glBegin(GL_POLYGON);
    glColor3f(0.3,0.3,0.3);
        glVertex2f(60,90);
        glVertex2f(60,95);
        glVertex2f(90,95);
        glVertex2f(90,90);
    glEnd();
    glFlush();


    st=" This PC";
     int  l,i;
     glColor3f(1,1,1);
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 54, 117); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, st[i]); // Print a character on the screen
    }

    // MY Network

    glBegin(GL_POLYGON);
    glColor3f(0,0.8,0);
        glVertex2f(72,180);
        glVertex2f(72,190);
        glVertex2f(77,190);
        glVertex2f(77,180);
    glEnd();

    // box
    glBegin(GL_POLYGON);
    glColor3f(0.2,0.2,0.2);
        glVertex2f(60,184);
        glVertex2f(60,160);
        glVertex2f(90,160);
        glVertex2f(90,184);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5,1,0.5);
        glVertex2f(63,181);
        glVertex2f(63,163);
        glVertex2f(87,163);
        glVertex2f(87,181);
    glEnd();

    // stand
   glBegin(GL_POLYGON);
    glColor3f(0.3,0.3,0.3);
        glVertex2f(60,190);
        glVertex2f(60,195);
        glVertex2f(90,195);
        glVertex2f(90,190);
    glEnd();
    glFlush();




    st="My Network";

     glColor3f(1,1,1);
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 48, 215); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, st[i]); // Print a character on the screen
    }

    // Recycle Bin
    glBegin(GL_POLYGON);
    glColor3f(0.8,0.8,0.8);
        glVertex2f(65,300);
        glVertex2f(85,300);
        glVertex2f(90,265);
        glVertex2f(60,265);
        glColor3f(0.5,0.5,0.5);

    glEnd();
     glBegin(GL_POLYGON);
        glColor3f(0.9,0.9,0.9);
        glVertex2f(90,265);
        glVertex2f(60,265);
        glVertex2f(65,255);
        glVertex2f(85,255);
    glEnd();
    st="Recycle Bin";

     glColor3f(1,1,1);
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 50, 320); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, st[i]); // Print a character on the screen
    }

    // Task Bar
    glBegin(GL_POLYGON);
    glColor3f(0.5,0.5,0.5);
        glVertex2f(0,741);
        glVertex2f(0,767);
        glVertex2f(1365,767);
        glVertex2f(1365,741);
    glEnd();
    // Start Button
    glBegin(GL_POLYGON);
        glColor3f(0,0.5,0.9);
        glVertex2f(0,741);
        glVertex2f(0,767);
        glVertex2f(60,767);
        glVertex2f(60,741);
    glEnd();
    glColor3f(0,1,0);
    Sprint( 8, 762, "Start" );

    // wifi

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
    glLineWidth(35);

        glVertex2f(1190,747);
        glVertex2f(1190,760);
        glVertex2f(1175,760);
    glEnd();
    glColor3f(0,0,0);

    // battery
     glBegin(GL_POLYGON);
    glColor3f(0,0,0);
        glVertex2f(1160,750);
        glVertex2f(1160,760);
        glVertex2f(1135,760);
        glVertex2f(1135,750);
    glEnd();
    glColor3f(0,0,0);
     glBegin(GL_POLYGON);
    glColor3f(1,1,1);
        glVertex2f(1150,752);
        glVertex2f(1156,758);
        glVertex2f(1137,758);
        glVertex2f(1137,752);
    glEnd();
    glColor3f(0,0,0);

    //cortana
    radius=11;
    glColor3f(.3,.3,.3);
    x1 = 77,y1=755;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();

    radius=9;
    glColor3f(0.8,0.8,0.8);
    x1 = 77,y1=755;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();

    // temperature


    radius=2;
    glColor3f(1,1,1);
    x1 = 1117,y1=748;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();

    st1="25 C";
    l=strlen( st1 ); // see how many characters are in text string.
    glRasterPos2i( 1101, 758); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, st1[i]); // Print a character on the screen
    }
    glFlush();

    // Time
    glBegin(GL_POLYGON);
    glColor3f(1,1,0);
        glVertex2f(1200,741);
        glVertex2f(1200,767);
        glVertex2f(1365,767);
        glVertex2f(1365,741);
    glEnd();
    glColor3f(0,0,0);

   int x = 1205;
    int y = 760;

    time(&t);
    st1=ctime(&t);
    printf("%s\n",st1);
    l=strlen( st1 ); // see how many characters are in text string.
    glRasterPos2i( x, y); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, st1[i]); // Print a character on the screen
    }
    glFlush();

    // OFF button background
    glBegin(GL_POLYGON);
    glColor3f(0.9019607, 0.921568, 0.21176);
        glVertex2f(1335,0);
        glVertex2f(1335,26);
        glVertex2f(1365,26);
        glVertex2f(1365,0);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
        glVertex2f(1340,10);
        glVertex2f(1360,10);
        glVertex2f(1350,20);
    glEnd();
    glFlush();




    glBegin(GL_LINE);
    glColor3f(0,0,0);
        glVertex2f(1335,0);
        glVertex2f(1335,26);
        glVertex2f(1365,26);
        glVertex2f(1365,0);
    glEnd();
    glFlush();

   // screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\MainScreen.bmp");
}

void unload(){
    PlaySound(TEXT("turnoff.wav"),NULL,SND_ASYNC);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
    glColor3f(0.71, 0.13, 0.52);
        glVertex2f(1023.75,0);
        glVertex2f(1365,0);
        glVertex2f(1365,767);
        glVertex2f(341.25,767);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.64, 0.83, 0.93);
        glVertex2f(1023.75,0);
        glVertex2f(0,0);
        glVertex2f(0,767);
        glVertex2f(341.25,767);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0,0,0);

        glVertex2f(1023.75,0);
        glVertex2f(341.25,767);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    Sprint( 620, 383.5, "Shutting down" );
    // sleep(6);
    glFlush();
   //  screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\shuttingDown.bmp");
}

void signup(){
    if(num==1){
        glClearColor(0.9961,0.9059,0.0824,0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
     // wifi

    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glLineWidth(35);

        glVertex2f(1340,747);
        glVertex2f(1340,760);
        glVertex2f(1325,760);
    glEnd();
    glColor3f(0,0,0);

    // battery
     glBegin(GL_POLYGON);
    glColor3f(0,0,0);
        glVertex2f(1310,750);
        glVertex2f(1310,760);
        glVertex2f(1285,760);
        glVertex2f(1285,750);
    glEnd();
    glColor3f(0,0,0);
     glBegin(GL_POLYGON);
    glColor3f(1,1,1);
        glVertex2f(1300,752);
        glVertex2f(1306,758);
        glVertex2f(1287,758);
        glVertex2f(1287,752);
    glEnd();
    glColor3f(0,0,0);

    // insert logo
    glBegin(GL_POLYGON);
    glColor3f(0,0,0);
        glVertex2f(570,100);
        glVertex2f(780,100);
        glVertex2f(780,400);
        glVertex2f(570,400);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.9961,0.9059,0.0824);
        glVertex2f(585,115);
        glVertex2f(765,115);
        glVertex2f(765,415);
        glVertex2f(585,415);
    glEnd();

     glBegin(GL_POLYGON);
    glColor3f(0,0,0);
      glVertex2f(645,145);
        glVertex2f(750,120);
        glVertex2f(750,410);
        glVertex2f(645,385);
    glEnd();


    // outer box
    glColor3f(1,1,1);
    radius=33;
    x1 = 550,y1=455;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();

    radius=33;
    x1 = 800,y1=455;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(550,425);
        glVertex2f(550,485);
        glVertex2f(800,485);
        glVertex2f(800,425);
    glEnd();

    glColor3f(0.06349,0.095238,0.12698);

    // inner box
    radius=25;
    x1 = 550,y1=455;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();

    radius=25;
    x1 = 800,y1=455;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1,y1);
    for (angle=1.0f;angle<361.0f;angle+=0.2)
    {
        x2 = x1+sin(angle)*radius;
        y2 = y1+cos(angle)*radius;
        glVertex2f(x2,y2);
    }
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(550,430);
        glVertex2f(550,480);
        glVertex2f(800,480);
        glVertex2f(800,430);
    glEnd();

    int  l,i;
    glColor3f(1,1,0);
    st = "Sign In";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(640, 460); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

    glColor3f(0,0,0);
    st = "OR";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(655, 550); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
    }

    glColor3f(0,0,0);
    st = "Enter Password:";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(510, 605); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }

     glColor3f(0,0,0);
    st = "Press Del to Clear";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(590, 635); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
    }
     glColor3f(0,0,0);

    glColor3f(0.9961,0.9059,0.0824);
    glBegin(GL_POLYGON);
        glVertex2f(1100,700);
        glVertex2f(1100,600);
        glVertex2f(1367,600);
        glVertex2f(1367,700);
    glEnd();
    glFlush();

    glColor3f(0,0,0);
    time(&t);
    st = ctime(&t);
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i(1100, 700); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }
    glFlush();


}

void myPC(){
    int l,p;
    // icon on taskbar
    glLineWidth(3);
    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(120,746);
        glVertex2f(120,763);
        glVertex2f(98,763);
        glVertex2f(98,746);
    glEnd();

    // window

    // content

    glBegin(GL_POLYGON);
        glColor3f(0,1,1);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,741);
        glVertex2f(0,741);
    glEnd();

    //1st rectangular box
    glColor3f(1,0,1);
     glBegin(GL_POLYGON);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,52);
        glVertex2f(0,52);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    st="This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 12, 45); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[p]); // Print a character on the screen
    }



    //2nd Rect Box

    glColor3f(0,0,0);
     glBegin(GL_POLYGON);
        glVertex2f(0,52);
        glVertex2f(200,52);
        glVertex2f(200,740);
        glVertex2f(0,740);
    glEnd();
    glFlush();



glColor3f(1,1,1);
    st=" Quick Access";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 70); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 90); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 110); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 130); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 150); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    New Volume(D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 170); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    New Volume(C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 190); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 210); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 230); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    st="    Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 250); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Untitled Document";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 270); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    //This PC Drop Down

    glColor3f(1,1,1);
    st=" This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 300); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 320); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 340); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 360); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 380); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > New Volume(D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 400); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > New Volume(C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 420); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 440); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 460); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    st=" Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 490); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }



//------Contents of 3rd Rect Box

//Desktop File
glColor3f(1,1,1);
    st="Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 263, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(270,100);
        glVertex2f(310,100);
        glVertex2f(310,150);
        glVertex2f(270,150);
    glEnd();

     glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0);
        glVertex2f(270,100);
        glVertex2f(283.33,112.5);
        glVertex2f(283.33,162.5);
        glVertex2f(270,150);
    glEnd();


    //Documents File
    glColor3f(1,1,1);
    st="Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 369, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(380,100);
        glVertex2f(420,100);
        glVertex2f(420,150);
        glVertex2f(380,150);
    glEnd();

     glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0);
        glVertex2f(380,100);
        glVertex2f(393.33,112.5);
        glVertex2f(393.33,162.5);
        glVertex2f(380,150);
    glEnd();


    //Downloads
    glColor3f(1,1,1);
    st="Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 479, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(490,100);
        glVertex2f(530,100);
        glVertex2f(530,150);
        glVertex2f(490,150);
    glEnd();

     glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0);
        glVertex2f(490,100);
        glVertex2f(503.3,112.5);
        glVertex2f(503.33,162.5);
        glVertex2f(490,150);
    glEnd();


    //Music
    glColor3f(1,1,1);
    st="Music";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 598, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(600,100);
        glVertex2f(640,100);
        glVertex2f(640,150);
        glVertex2f(600,150);
    glEnd();

     glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0);
        glVertex2f(600,100);
        glVertex2f(613.3,112.5);
        glVertex2f(613.33,162.5);
        glVertex2f(600,150);
    glEnd();


    //Pictures
    glColor3f(1,1,1);
    st="Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 705, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(710,100);
        glVertex2f(750,100);
        glVertex2f(750,150);
        glVertex2f(710,150);
    glEnd();

     glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0);
        glVertex2f(710,100);
        glVertex2f(723.3,112.5);
        glVertex2f(723.33,162.5);
        glVertex2f(710,150);
    glEnd();


    //Videos
    glColor3f(1,1,1);
    st="Videos";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 815, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(820,100);
        glVertex2f(860,100);
        glVertex2f(860,150);
        glVertex2f(820,150);
    glEnd();

     glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0);
        glVertex2f(820,100);
        glVertex2f(833.33,112.5);
        glVertex2f(833.33,162.5);
        glVertex2f(820,150);
    glEnd();


    //titles 1

    glColor3f(1,1,1);
    st="Folders (6)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 240, 85); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(0.5,0.5,0.5);
        glVertex2f(330,80);
        glVertex2f(1340,80);
    glEnd();


    //titles 2

    glColor3f(1,1,1);
    st="Device and Drivers (2)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 240, 300); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

     glBegin(GL_LINES);
        glColor3f(0.5,0.5,0.5);
        glVertex2f(423,295);
        glVertex2f(1340,295);
    glEnd();
    glFlush();


    //Local Disk File (c:)

    glColor3f(1,1,1);
    st="Local Disk (C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 270, 415); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0.8);
        glVertex2f(270,355);
        glVertex2f(310,375);
        glVertex2f(310,395);
        glVertex2f(270,375);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(0.5,0.5,0.5);
        glVertex2f(270,355);
        glVertex2f(310,375);
        glVertex2f(370,345);
        glVertex2f(330,325);
    glEnd();


    glBegin(GL_POLYGON);
        glColor3f(0,0.5,0.5);
        glVertex2f(370,345);
        glVertex2f(310,375);
        glVertex2f(310,395);
        glVertex2f(370,365);
    glEnd();

     glColor3f(0.2,0.8,0);
      glPointSize(4);
    glBegin(GL_POINTS);
        glVertex2f( 300, 381);
    glEnd();
    glFlush();

    //New Volume (D:)

    glColor3f(1,1,1);
    st="New Volume (D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 430, 415); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0.8);
        glVertex2f(430,355);
        glVertex2f(470,375);
        glVertex2f(470,395);
        glVertex2f(430,375);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(0.5,0.5,0.5);
        glVertex2f(430,355);
        glVertex2f(470,375);
        glVertex2f(530,345);
        glVertex2f(490,325);
    glEnd();


    glBegin(GL_POLYGON);
        glColor3f(0,0.5,0.5);
        glVertex2f(530,345);
        glVertex2f(470,375);
        glVertex2f(470,395);
        glVertex2f(530,365);
    glEnd();

      glColor3f(0.2,0.8,0);
      glPointSize(4);
    glBegin(GL_POINTS);
        glVertex2f( 460, 381);
    glEnd();
    glFlush();


    //------------------//


    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glColor3f(0,0,0);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,741);
        glVertex2f(0,741);
    glEnd();


/*
    // title bar
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glColor3f(0,0,0);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,40);
        glVertex2f(0,40);
    glEnd();
    */



    // close button
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2f(1330,20);
        glVertex2f(1365,20);
        glVertex2f(1365,52);
        glVertex2f(1330,52);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    st="X";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 1340, 43); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[p]); // Print a character on the screen
    }
    //screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\thisPC.bmp");
}

void recycleBin(){
    int l,p;

    // icon on taskbar
     glLineWidth(3);
    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(120,746);
        glVertex2f(120,763);
        glVertex2f(98,763);
        glVertex2f(98,746);
    glEnd();

    // window

    // content

    glBegin(GL_POLYGON);
        glColor3f(0,1,1);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,741);
        glVertex2f(0,741);
    glEnd();





    //1st rectangular box
    glColor3f(1,0,1);
     glBegin(GL_POLYGON);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,52);
        glVertex2f(0,52);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    st="Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 12, 45); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[p]); // Print a character on the screen
    }



    //2nd Rect Box

    glColor3f(0,0,0);
     glBegin(GL_POLYGON);
        glVertex2f(0,52);
        glVertex2f(200,52);
        glVertex2f(200,740);
        glVertex2f(0,740);
    glEnd();
    glFlush();



glColor3f(1,1,1);
    st=" Quick Access";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 70); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 90); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 110); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 130); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 150); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    New Volume(D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 170); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    New Volume(C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 190); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 210); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 230); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    st="    Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 250); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Untitled Document";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 270); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }



    //Recycle Bin Drop Down

    glColor3f(1,1,1);
    st=" This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 300); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 320); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 340); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 360); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 380); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > New Volume(D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 400); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > New Volume(C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 420); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 440); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 460); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    st=" Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 490); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }



//Contents of 3rd Rect Box


    //Aniket.txt file

glColor3f(1,1,1);
    st="Aniket.txt";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 259, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(0.95, 0.95, 0.95);
        glVertex2f(270,100);
        glVertex2f(300,100);
        glVertex2f(310,110);
        glVertex2f(310,150);
        glVertex2f(270,150);
    glEnd();


    glLineWidth(0.8);
     glBegin(GL_LINE_LOOP);
        glColor3f(0.05, 0.05, 0.05);
        glVertex2f(270,100);
        glVertex2f(300,100);
        glVertex2f(310,110);
        glVertex2f(310,150);
        glVertex2f(270,150);
    glEnd();

     glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0.3, 0.3);
         glVertex2f(300,100);
        glVertex2f(300,110);
        glVertex2f(310,110);
    glEnd();

     glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0.3, 0.3);
        glVertex2f(270,100);
        glVertex2f(300,100);
        glVertex2f(310,110);
        glVertex2f(310,150);
        glVertex2f(270,150);
    glEnd();


        for(int i=105; i<=145; i=i+5){
            if(i>110){
                glBegin(GL_LINES);
                glColor3f(0.05, 0.05, 0.05);
                    glVertex2f(275,i);
                    glVertex2f(305,i);
                glEnd();
                glFlush();
            }
            else{
                glBegin(GL_LINES);
                glColor3f(0.05, 0.05, 0.05);
                    glVertex2f(275,i);
                    glVertex2f(295,i);
                glEnd();
                glFlush();
            }

        }



    //Akash.txt file

    glColor3f(1,1,1);
    st="Akash.txt";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 369, 175); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    glBegin(GL_POLYGON);
        glColor3f(0.95, 0.95, 0.95);
        glVertex2f(380,100);
        glVertex2f(410,100);
        glVertex2f(420,110);
        glVertex2f(420,150);
        glVertex2f(380,150);
    glEnd();
     glLineWidth(0.8);
     glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0.3, 0.3);
         glVertex2f(380,100);
        glVertex2f(410,100);
        glVertex2f(420,110);
        glVertex2f(420,150);
        glVertex2f(380,150);
    glEnd();

     glBegin(GL_LINE_LOOP);
        glColor3f(0.3, 0.3, 0.3);
        glVertex2f(410,100);
        glVertex2f(410,110);
        glVertex2f(420,110);
        glVertex2f(410,100);
    glEnd();


        for(int i=105; i<=145; i=i+5){
            if(i>110){
                glBegin(GL_LINES);
                glColor3f(0.05, 0.05, 0.05);
                    glVertex2f(385,i);
                    glVertex2f(415,i);
                glEnd();
                glFlush();
            }
            else{
                glBegin(GL_LINES);
                glColor3f(0.05, 0.05, 0.05);
                    glVertex2f(385,i);
                    glVertex2f(405,i);
                glEnd();
                glFlush();
            }

        }





//------------------//



    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glColor3f(0,0,0);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,741);
        glVertex2f(0,741);
    glEnd();



    // close button
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2f(1330,20);
        glVertex2f(1365,20);
        glVertex2f(1365,52);
        glVertex2f(1330,52);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    st="X";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 1340, 43); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[p]); // Print a character on the screen
    }
    // screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\recycleBin.bmp");
}

void networkDisp(){
    int l,p;
       // icon on taskbar
    glLineWidth(3);
    glBegin(GL_POLYGON);
        glColor3f(1,1,0);
        glVertex2f(120,746);
        glVertex2f(120,763);
        glVertex2f(98,763);
        glVertex2f(98,746);
    glEnd();

    // window

    // content

    glBegin(GL_POLYGON);
        glColor3f(0,1,1);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,741);
        glVertex2f(0,741);
    glEnd();



    //1st rectangular box
    glColor3f(1,0,1);
     glBegin(GL_POLYGON);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,52);
        glVertex2f(0,52);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    st="Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 12, 45); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[p]); // Print a character on the screen
    }



    //2nd Rect Box

    glColor3f(0,0,0);
     glBegin(GL_POLYGON);
        glVertex2f(0,52);
        glVertex2f(200,52);
        glVertex2f(200,740);
        glVertex2f(0,740);
    glEnd();
    glFlush();



glColor3f(1,1,1);
    st=" Quick Access";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 70); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 90); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 110); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 130); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 150); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    New Volume(D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 170); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    New Volume(C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 190); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 210); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 230); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    st="    Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 250); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="    Untitled Document";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 270); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }



    //My Network Drop Down

    glColor3f(1,1,1);
    st=" This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 300); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Downloads";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 320); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Desktop";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 340); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Documents";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 360); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Pictures";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 380); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > New Volume(D:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 400); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > New Volume(C:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 420); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > This PC";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 440); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    st="   > Recycle Bin";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 460); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }


    st=" Network";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 490); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }



//Contents of 3rd Rect Box


    glColor3f(1,1,1);
    st="Network (E:)";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 270, 190); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[p]); // Print a character on the screen
    }

    glBegin(GL_POLYGON);
        glColor3f(0.8,0.8,0.8);
        glVertex2f(270,130);
        glVertex2f(310,150);
        glVertex2f(310,170);
        glVertex2f(270,150);
    glEnd();

    glBegin(GL_POLYGON);
        glColor3f(0.5,0.5,0.5);
        glVertex2f(270,130);
        glVertex2f(310,150);
        glVertex2f(370,120);
        glVertex2f(330,100);
    glEnd();


    glBegin(GL_POLYGON);
        glColor3f(0,0.5,0.5);
        glVertex2f(370,120);
        glVertex2f(310,150);
        glVertex2f(310,170);
        glVertex2f(370,140);
    glEnd();

     glColor3f(0.2,0.8,0);
      glPointSize(4);
    glBegin(GL_POINTS);
        glVertex2f( 300, 155);
    glEnd();
    glFlush();




//------------------//



    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glColor3f(0,0,0);
        glVertex2f(0,20);
        glVertex2f(1365,20);
        glVertex2f(1365,741);
        glVertex2f(0,741);
    glEnd();



    // close button
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex2f(1330,20);
        glVertex2f(1365,20);
        glVertex2f(1365,52);
        glVertex2f(1330,52);
    glEnd();
    glFlush();

    glColor3f(1,1,1);
    st="X";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 1340, 43); // location to start printing text
    for( p=0; p<l; p++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[p]); // Print a character on the screen
    }
    // screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\myNetwork.bmp");
}

void display(){

    // check this out
    if(stage==1){
        /*
        intro();
        sleep(5);

        des();
        sleep(5);
        */
        clearing();
        load();
        sleep(2);
        stage=2;
    }


    if(stage==2){
        // clearing();
        signup();
        glFlush();
    }
}

void clipArea(){
    glClearColor(0.8, 0.2, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1365, 767, 0);
}

void myKey(unsigned char key, int x, int y){

    switch(key){
        case '1':intro();
                break;
        case '2':des();
                break;
        case '3':
                display();
                glFlush();
                signup();
                glFlush();
                break;
    }
    switch (key) {
        case 27:
            // Esc to exit
            exit(0);
            break;
        case 8:
            // backspace
            if(stage==2){
                num=num-2;
            }
            break;
        case 127:
            // press Del to clear
            if(stage==2){
                num=0;
            }
            break;
    }
    if(stage==2 ){
        signup();
        glFlush();

        if(num>=1){
            glColor3f(0,0,0);
            glPointSize(20);
            glBegin(GL_POINTS);
                glVertex2f(660,600);
            glEnd();
            glFlush();
        }
        if(num>=2){
            glColor3f(0,0,0);
            glPointSize(20);
            glBegin(GL_POINTS);
                glVertex2f(690,600);
            glEnd();
            glFlush();
        }
        if(num>=3){
            glColor3f(0,0,0);
            glPointSize(20);
            glBegin(GL_POINTS);
                glVertex2f(720,600);
            glEnd();
            glFlush();
        }
        if(num>=4){
            glColor3f(0,0,0);
            glPointSize(20);
            glBegin(GL_POINTS);
                glVertex2f(750,600);
            glEnd();
            glFlush();
        }
        if(num>=5){
            glColor3f(0,0,0);
            glPointSize(20);
            glBegin(GL_POINTS);
                glVertex2f(780,600);
            glEnd();
            glFlush();
        }
        if(num>=6){
            glColor3f(0,0,0);
            glPointSize(20);
            glBegin(GL_POINTS);
                glVertex2f(810,600);
            glEnd();
            glFlush();
        screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\signup.bmp");
            sleep(2);

            change=1;
        }
        num++;
    }

    if(change==1){

        signedIn=1;
        stage=3;
        flag=0;

        mainScreen();
        glFlush();

        PlaySound(TEXT("vista.wav"),NULL,SND_ASYNC);
        change=0;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
         }
         break;
    }
}

void myMouse(int btn, int state, int x, int y){
    xpos= x;
    ypos=y;
    printf("xpos=%d\n",xpos);
    printf("ypos=%d\n\n",ypos);

    // stage 2 is for signin and signedIn==0
    // stage 3 is for main screen and flag==0
    // stage 4 is for main Screen and flag==1( with OFF button Dropdown)

    // stage 10 is for shutdown and poweroff==1

     if(btn==GLUT_LEFT_BUTTON && stage==2 && signedIn==0){
        display();
        // sign in complete;
        for(int i=550;i<800;i++ ){
            for(int j=430; j<480; j++){
                if(xpos==i && ypos==j){
                    glClearColor(0.9961,0.9059,0.0824,0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    PlaySound(TEXT("vista.wav"),NULL,SND_ASYNC);
                    // insert logo
                    glBegin(GL_POLYGON);
                    glColor3f(0,0,0);
                        glVertex2f(570,100);
                        glVertex2f(780,100);
                        glVertex2f(780,400);
                        glVertex2f(570,400);
                    glEnd();

                    glBegin(GL_POLYGON);
                    glColor3f(0.9961,0.9059,0.0824);
                        glVertex2f(585,115);
                        glVertex2f(765,115);
                        glVertex2f(765,415);
                        glVertex2f(585,415);
                    glEnd();

                     glBegin(GL_POLYGON);
                    glColor3f(0,0,0);
                      glVertex2f(645,145);
                        glVertex2f(750,120);
                        glVertex2f(750,410);
                        glVertex2f(645,385);
                    glEnd();
                    glFlush();

                    // outer box
                    glColor3f(0,0,0);
                    radius=33;
                    x1 = 550,y1=455;
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(x1,y1);
                    for (angle=1.0f;angle<361.0f;angle+=0.2)
                    {
                        x2 = x1+sin(angle)*radius;
                        y2 = y1+cos(angle)*radius;
                        glVertex2f(x2,y2);
                    }
                    glEnd();

                    radius=33;
                    x1 = 800,y1=455;
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(x1,y1);
                    for (angle=1.0f;angle<361.0f;angle+=0.2)
                    {
                        x2 = x1+sin(angle)*radius;
                        y2 = y1+cos(angle)*radius;
                        glVertex2f(x2,y2);
                    }
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex2f(550,425);
                        glVertex2f(550,485);
                        glVertex2f(800,485);
                        glVertex2f(800,425);
                    glEnd();

                    glColor3f(1,1,0);

                    // inner box
                    radius=25;
                    x1 = 550,y1=455;
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(x1,y1);
                    for (angle=1.0f;angle<361.0f;angle+=0.2)
                    {
                        x2 = x1+sin(angle)*radius;
                        y2 = y1+cos(angle)*radius;
                        glVertex2f(x2,y2);
                    }
                    glEnd();

                    radius=25;
                    x1 = 800,y1=455;
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(x1,y1);
                    for (angle=1.0f;angle<361.0f;angle+=0.2)
                    {
                        x2 = x1+sin(angle)*radius;
                        y2 = y1+cos(angle)*radius;
                        glVertex2f(x2,y2);
                    }
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex2f(550,430);
                        glVertex2f(550,480);
                        glVertex2f(800,480);
                        glVertex2f(800,430);
                    glEnd();

                    int  l,i;
                    glColor3f(0.06349,0.095238,0.12698);
                    st = "Sign In";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i(640, 460); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
                    }

                    glFlush();
                    sleep(1);

                    signup();
                    sleep(1);


                    signedIn=1;
                    stage=3;
                    flag=0;
                }
            }
        }
    }

    // stage 3 is for main screen
    if(btn==GLUT_LEFT_BUTTON && stage==3 && flag==0){
        mainScreen();
        glFlush();
        // if start button is pressed
        tempFlag=0;
        // if arrow is pressed(OFF Button drop down)
        for(int i=1345;i<1365;i++ )
            for(int j=0; j<20; j++){
                if(xpos==i && ypos==j){

                    flag=1;
                    stage=4;

                    glColor3f(1,1,1);
                    glBegin(GL_POLYGON);
                        glVertex2f(1365,20);
                        glVertex2f(1365,80);
                        glVertex2f(1250,80);
                        glVertex2f(1250,20);
                    glEnd();

                    int  l,i;
                    glColor3f(0,0,0);
                    st = "LOCK";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( 1260, 40); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    st = "Power OFF";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( 1260, 70); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    glFlush();
                    screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\TurnOffOptions.bmp");
                    break;
                }
            }

    }

    if(btn==GLUT_LEFT_BUTTON && stage==4 && flag==1 && state==GLUT_DOWN){
        flag=0;
        stage=3;
        tempFlag=0;

        for(int i=1260;i<1360;i++ ){
            for(int j=50; j<80; j++){
                if(xpos==i && ypos==j){
                        glLineWidth(2);
                    glColor3f(0,0,0);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(1250,55);
                        glVertex2f(1360,55);
                        glVertex2f(1360,75);
                        glVertex2f(1250,75);
                    glEnd();
                    glFlush();
                    sleep(1);

                    stage=10;
                    poweroff=1;
                }
            }
        }
        for(int i=1260;i<1360;i++ ){
            for(int j=20; j<52; j++){
                if(xpos==i && ypos==j){
                        glLineWidth(2);
                    glColor3f(0,0,0);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(1360,25);
                        glVertex2f(1250,25);
                        glVertex2f(1250,45);
                        glVertex2f(1360,45);
                    glEnd();
                    glFlush();
                    sleep(1);

                    stage=2;
                    signedIn=0;
                }
            }
        }
    }

    // if refresh Button is clicked
    if(btn==GLUT_RIGHT_BUTTON && stage==3 && tempFlag==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("in refresh button clicked");
         // if refresh Button is clicked
        for(int i=100;i<1340;i++ ){ // i=0
            if(tempFlag==1 && stage==5) break;
            for(int j=0; j<741; j++){ //j=0
                if(xpos==i && ypos==j){
                    // x1,y1    x2,y2
                    //
                    //
                    //  x4,y4    x3,y3
                    refx1=i; refy1=j;


                    tempFlag=1;
                    stage=5;
                    glLineWidth(4);
                    glColor3f(0,0,0);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(i,j);
                        glVertex2f(i+140,j);
                        glVertex2f(i+140,j+190);
                        glVertex2f(i,j+190);
                    glEnd();

                    glColor3f(1,1,1);
                    glBegin(GL_POLYGON);
                        glVertex2f(i,j);
                        glVertex2f(i+140,j);
                        glVertex2f(i+140,j+190);
                        glVertex2f(i,j+190);
                    glEnd();

                    int  l;
                    glColor3f(0,0,0);
                    st = "View         >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+15); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Sort by      >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+30); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                      i= refx1; j=refy1;
                    st = "Refresh";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+45); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }



                     i= refx1; j=refy1;
                    st = "Paste";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+60); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                     i= refx1; j=refy1;
                    st = "Paste Shortcut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+75); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                     i= refx1; j=refy1;
                    st = "Undo Delete";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+90); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                     i= refx1; j=refy1;
                    st = "New";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+105); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    st = "Git GUI here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+120); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                     i= refx1; j=refy1;
                    st = "Git Bash here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+135); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    st = "Graphics opts.";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+150); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    st = "Personalize";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+165); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    st = "Display sett.";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+180); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }


                    glFlush();
                    screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\refresh.bmp");
                    break;
                }
            }
        }
            printf("End of a refresh  block");
            printf("stage=%d\n",stage);
            printf("tempFlag=%d\n",tempFlag);
    }
    // reaction
    if(btn==GLUT_LEFT_BUTTON && stage==5 && tempFlag==1 && state == GLUT_DOWN){
        flag=0;
        stage=3;
        tempFlag=0;
        printf("in refresh button clicked- reaction");

        for(int i=refx1;i<refx1+140;i++ ){
            for(int j=refy1; j<refy1+190; j++){
                if(refx1==i && refy1==j){
                    glLineWidth(1);
                    glColor3f(0,0,0);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(refx1,refy1+33);
                        glVertex2f(refx1+140,refy1+33);
                        glVertex2f(refx1+140,refy1+48);
                        glVertex2f(refx1,refy1+48);
                    glEnd();
                    glFlush();
                    sleep(1);

                    mainScreen();
                    glFlush();
                }
            }
        }
    }

    // if icon My computer is pressed
    if(btn==GLUT_RIGHT_BUTTON && stage==3 && thisPC==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("My computer is clicked");
         // if refresh Button is clicked
        for(int i=50;i<100;i++ ){
            if(thisPC==1 && stage==6) break;
            for(int j=50; j<100; j++){
                if(xpos==i && ypos==j){
                    // x1,y1    x2,y2
                    //
                    //
                    //  x4,y4    x3,y3
                    refx1=i; refy1=j;


                    thisPC=1;
                    stage=6;

                    glColor3f(0,0,0);
                    glLineWidth(3);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(50,100);
                        glVertex2f(50,50);
                        glVertex2f(100,50);
                        glVertex2f(100,100);
                    glEnd();

                    glColor3f(1,1,1);
                    glBegin(GL_POLYGON);
                        glVertex2f(i,j);
                        glVertex2f(i+200,j);
                        glVertex2f(i+200,j+280);
                        glVertex2f(i,j+280);
                    glEnd();


                    glFlush();

                    int  l;
                    glColor3f(0,0,0);
                    st = "Open";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+15); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    glColor3f(0,0,0);
                    st = "Open File Location";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+30); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Open Folder Location";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+45); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Pin to Quick access";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+60); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Manage";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+75); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Pin to Start";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+90); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Git Bash Here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+105); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Git GUI Here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+120); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    // Line
                    glLineWidth(1);
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+123);
                        glVertex2f(i+190, j+123);
                    glEnd();



                       i= refx1; j=refy1;
                    st = "7 Zip               >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+135); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "CRC SHA             >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+150); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }


                    // Line
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+153);
                        glVertex2f(i+190, j+153);
                    glEnd();

                       i= refx1; j=refy1;
                    st = "open with           >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+165); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Include in Library  >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+180); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    // Line
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+183);
                        glVertex2f(i+190, j+183);
                    glEnd();

                       i= refx1; j=refy1;
                    st = "Cut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+195); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Copy";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+210); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Create Shortcut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+225); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Delete           del";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+240); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Rename            F2";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+255); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Properties";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+270); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    glFlush();
                    screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\onClickIcon.bmp");
                    break;
                }
            }
        }
            printf("End of a MyPC  block");
            printf("stage=%d\n",stage);
            printf("thisPC=%d\n",tempFlag);
    }
    // reaction
    if(btn==GLUT_LEFT_BUTTON && stage==6 && thisPC==1 && state == GLUT_DOWN ){
        flag=0;
        stage=6;
        thisPC=0;
        printf("in This PC is clicked- reaction");
        if(DclickFlag==0){
            for(int i=refx1;i<refx1+130;i++ ){
                for(int j=refy1; j<refy1+200; j++){
                    if(refx1==i && refy1==j){
                        glLineWidth(2);
                        glColor3f(0,0,0);
                        glBegin(GL_LINE_LOOP);
                            glVertex2f(refx1,refy1+2);
                            glVertex2f(refx1+200,refy1+2);
                            glVertex2f(refx1+200,refy1+19);
                            glVertex2f(refx1,refy1+19);
                        glEnd();
                        glFlush();
                        sleep(1);

                        myPC();
                        glFlush();
                    }
                }
            }
        }
    }
    if(btn==GLUT_LEFT_BUTTON && stage==6 && thisPC==0){
        myPC();
        glFlush();

         for(int i=1330;i<1365;i++ ){
                if(stage==3) break;
            for(int j=20; j<52; j++){
                if(x==i && y==j){

                    sleep(1);

                    flag=0;
                    stage=3;
                    thisPC=0;

                    mainScreen();
                    glFlush();
                    break;
                }
            }
         }
    }
    // end of this PC button

    // if icon Recycle bin is pressed
    if(btn==GLUT_RIGHT_BUTTON && stage==3 && rebin==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("Recycle Bin is clicked");
         // if refresh Button is clicked
        for(int i=50;i<100;i++ ){
            if(rebin==1 && stage==7) break;
            for(int j=250; j<300; j++){
                if(xpos==i && ypos==j){
                    // x1,y1    x2,y2
                    //
                    //
                    //  x4,y4    x3,y3
                    refx1=i; refy1=j;


                    rebin=1;
                    stage=7;

                    glColor3f(0,0,0);
                    glLineWidth(3);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(50,150);
                        glVertex2f(50,200);
                        glVertex2f(100,200);
                        glVertex2f(100,150);
                    glEnd();

                    glColor3f(1,1,1);
                    glBegin(GL_POLYGON);
                        glVertex2f(i,j);
                        glVertex2f(i+200,j);
                        glVertex2f(i+200,j+280);
                        glVertex2f(i,j+280);
                    glEnd();


                    glFlush();

                    int  l;
                    glColor3f(0,0,0);
                    st = "Open";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+15); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    glColor3f(0,0,0);
                    st = "Open File Location";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+30); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Open Folder Location";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+45); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Pin to Quick access";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+60); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Manage";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+75); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Pin to Start";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+90); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Git Bash Here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+105); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Git GUI Here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+120); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    // Line
                    glLineWidth(1);
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+123);
                        glVertex2f(i+190, j+123);
                    glEnd();



                       i= refx1; j=refy1;
                    st = "7 Zip               >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+135); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "CRC SHA             >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+150); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }


                    // Line
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+153);
                        glVertex2f(i+190, j+153);
                    glEnd();

                       i= refx1; j=refy1;
                    st = "open with           >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+165); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Include in Library  >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+180); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    // Line
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+183);
                        glVertex2f(i+190, j+183);
                    glEnd();

                       i= refx1; j=refy1;
                    st = "Cut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+195); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Copy";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+210); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Create Shortcut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+225); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Delete       del";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+240); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Rename        F2";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+255); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Properties";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+270); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    glFlush();
                    break;
                }
            }
        }
            printf("End of a Recycle Bin  block");
            printf("stage=%d\n",stage);
            printf("rebin=%d\n",rebin);
    }
    // reaction
    if(btn==GLUT_LEFT_BUTTON && stage==7 && rebin==1 && state == GLUT_DOWN){
        flag=0;
        stage=7;
        rebin=0;
        printf("in recycle bin is clicked- reaction");
        if(DclickFlag==0){
            for(int i=refx1;i<refx1+130;i++ ){
                for(int j=refy1; j<refy1+200; j++){
                    if(refx1==i && refy1==j){
                        glLineWidth(1);
                        glColor3f(0,0,0);
                        glBegin(GL_LINE_LOOP);
                            glVertex2f(refx1,refy1+2);
                            glVertex2f(refx1+200,refy1+2);
                            glVertex2f(refx1+200,refy1+19);
                            glVertex2f(refx1,refy1+19);
                        glEnd();
                        glFlush();
                        sleep(1);

                        recycleBin();
                        glFlush();
                    }
                }
            }
        }
    }
    if(btn==GLUT_LEFT_BUTTON && stage==7 && thisPC==0){
        recycleBin();
        glFlush();

         for(int i=1330;i<1365;i++ ){
                if(stage==3) break;
            for(int j=20; j<52; j++){
                if(x==i && y==j){

                    sleep(1);

                    flag=0;
                    stage=3;
                    rebin=0;

                    mainScreen();
                    glFlush();
                    break;
                }
            }
         }
    }
    // end of recycle bin button

    // if icon Network is pressed
    if(btn==GLUT_RIGHT_BUTTON && stage==3 && network==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("My Network is clicked");
         // if network is clicked
        for(int i=50;i<100;i++ ){
            if(network==1 && stage==8) break;
            for(int j=150; j<200; j++){
                if(xpos==i && ypos==j){
                    // x1,y1    x2,y2
                    //
                    //
                    //  x4,y4    x3,y3
                    refx1=i; refy1=j;


                    network=1;
                    stage=8;


                    glColor3f(0,0,0);
                    glLineWidth(3);
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(50,150);
                        glVertex2f(100,150);
                        glVertex2f(100,200);
                        glVertex2f(50,200);
                    glEnd();

                    glColor3f(1,1,1);
                    glBegin(GL_POLYGON);
                        glVertex2f(i,j);
                        glVertex2f(i+200,j);
                        glVertex2f(i+200,j+280);
                        glVertex2f(i,j+280);
                    glEnd();


                    glFlush();

                    int  l;
                    glColor3f(0,0,0);
                    st = "Open";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+15); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    i= refx1; j=refy1;
                    glColor3f(0,0,0);
                    st = "Open File Location";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+30); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Open Folder Location";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+45); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Pin to Quick access";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+60); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Manage";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+75); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Pin to Start";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+90); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Git Bash Here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+105); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Git GUI Here";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+120); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    // Line
                    glLineWidth(1);
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+123);
                        glVertex2f(i+190, j+123);
                    glEnd();



                       i= refx1; j=refy1;
                    st = "7 Zip               >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+135); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "CRC SHA             >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+150); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }


                    // Line
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+153);
                        glVertex2f(i+190, j+153);
                    glEnd();

                       i= refx1; j=refy1;
                    st = "open with           >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+165); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Include in Library  >";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+180); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    // Line
                    i= refx1; j=refy1;
                    glBegin(GL_LINES);
                    glColor3f(0,0,0);
                        glVertex2f(i+10, j+183);
                        glVertex2f(i+190, j+183);
                    glEnd();

                       i= refx1; j=refy1;
                    st = "Cut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+195); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Copy";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+210); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Create Shortcut";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+225); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Delete       del";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+240); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Rename        F2";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+255); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                       i= refx1; j=refy1;
                    st = "Properties";
                    l=strlen( st ); // see how many characters are in text string.
                    glRasterPos2i( i+10, j+270); // location to start printing text
                    for( i=0; i < l; i++)  // loop until i is greater then l
                    {
                        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]); // Print a character on the screen
                    }

                    glFlush();
                    break;
                }
            }
        }
            printf("End of a Mynetwork  block");
            printf("stage=%d\n",stage);
            printf("network=%d\n",network);
    }
    // reaction
    if(btn==GLUT_LEFT_BUTTON && stage==8 && network==1 && state == GLUT_DOWN){
        flag=0;
        stage=8;
        network=0;
        printf("in My Network is clicked- reaction");
        if(DclickFlag==0){
            for(int i=refx1;i<refx1+130;i++ ){
                for(int j=refy1; j<refy1+200; j++){
                    if(refx1==i && refy1==j){
                        glLineWidth(1);
                        glColor3f(0,0,0);
                        glBegin(GL_LINE_LOOP);
                            glVertex2f(refx1,refy1+2);
                            glVertex2f(refx1+200,refy1+2);
                            glVertex2f(refx1+200,refy1+19);
                            glVertex2f(refx1,refy1+19);
                        glEnd();
                        glFlush();
                        sleep(1);

                        networkDisp();
                        glFlush();
                    }
                }
            }
        }
    }
    if(btn==GLUT_LEFT_BUTTON && stage==8 && thisPC==0){
        networkDisp();
        glFlush();

         for(int i=1330;i<1365;i++ ){
                if(stage==3) break;
            for(int j=20; j<52; j++){
                if(x==i && y==j){

                    sleep(1);

                    flag=0;
                    stage=3;
                    network=0;

                    mainScreen();
                    glFlush();
                    break;
                }
            }
         }
    }
    // end of MyNetwork button

    // if icon Start is pressed
    if(btn==GLUT_LEFT_BUTTON && stage==3 && start==0 && state== GLUT_DOWN){
        printf("Start is clicked");
         // if start is clicked
        for(int i=0;i<60;i++ ){
            for(int j=741; j<767; j++){
                if(xpos==i && ypos==j){
                    // x1,y1    x2,y2
                    //
                    //
                    //  x4,y4    x3,y3
                    // refx1=i; refy1=j;


                    start=1;
                    stage=9;
                    startBtn();
                    glFlush();
                }
            }
        }
            printf("End of a Start  block");
            printf("stage=%d\n",stage);
            printf("start=%d\n",start);
    }
    if(btn==GLUT_RIGHT_BUTTON && stage==9 && start==1 && state == GLUT_DOWN){
         for(int i=0;i<1365;i++ ){
            for(int j=0; j<767; j++){
                if(xpos==i && ypos==j){

                    start=0;
                    stage=3;

                    mainScreen();
                    glFlush();
                }
            }
        }

        printf("in Start button is clicked- reaction");

    }


    // end of start button

    // Double Click
    // My computer icon
    if(btn==GLUT_LEFT_BUTTON && stage==3 && thisPC==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("My computer is clicked");
         // if refresh Button is clicked
        for(int i=50;i<100;i++ ){
            if(thisPC==1 && stage==6) break;
            for(int j=50; j<100; j++){
                if(xpos==i && ypos==j){
                    thisPC=1;
                    stage=6;
                    DclickFlag=1;
                    break;
                }
            }
        }
            printf("End of a MyPC  block");
            printf("stage=%d\n",stage);
            printf("thisPC=%d\n",tempFlag);
    }

    // My network
    if(btn==GLUT_LEFT_BUTTON && stage==3 && network==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("My network is clicked");
         // if refresh Button is clicked
        for(int i=50;i<100;i++ ){
            if(network==1 && stage==8) break;
            for(int j=150; j<200; j++){
                if(xpos==i && ypos==j){
                    flag=0;
                    stage=8;
                    network=1;

                    DclickFlag=1;
                    break;
                }
            }
        }
            printf("End of a Mynetwork  block");
            printf("stage=%d\n",stage);
            printf("network=%d\n",network);
    }

    // Recycle Bin
     if(btn==GLUT_LEFT_BUTTON && stage==3 && rebin==0 && state== GLUT_DOWN){
        mainScreen();
        glFlush();
        printf("Recycle Bin is clicked");
         // if recycle bin is clicked
        for(int i=50;i<100;i++ ){
            if(rebin==1 && stage==7) break;
            for(int j=250; j<300; j++){
                if(xpos==i && ypos==j){
                    flag=0;
                     rebin=1;
                    stage=7;

                    DclickFlag=1;
                    break;
                }
            }
        }
            printf("End of a Recycle Bin  block");
            printf("stage=%d\n",stage);
            printf("rebin=%d\n",rebin);
    }

    if(btn==GLUT_LEFT_BUTTON && stage==10 && poweroff==1){
        clearing();
        unload();
        glFlush();
    }
}
void startBtn(){
    glColor3f(1,1,1);
    glBegin(GL_POLYGON);
        glVertex2f(0,741);
        glVertex2f(300,741);
        glVertex2f(300,310);
        glVertex2f(0,310);
    glEnd();

    int  l, j;
    glColor3f(0,0,0);
    st = "Turbo C++";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 730); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Visual Studio Code";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 710); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Snip & Sketch";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 690); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Settings";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 670); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Python 3.10";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 650); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Photo Gallery";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 630); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Paint 3D";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 610); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "Open VPN";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 590); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

    st = "One Note";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 570); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Office";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 550); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Need For Speed";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 530); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Microsoft Office               >";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 510); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Firefox";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 490); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Eclipse";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 470); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Camera";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 450); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Calender";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 430); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Arduino IDE";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 410); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Android Studio";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 390); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Anaconda Prompt";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 370); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Adobe";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 350); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }

     st = "Avast Free Antivirus";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( 15, 330); // location to start printing text
    for( j=0; j < l; j++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[j]); // Print a character on the screen
    }


    glFlush();
    screenCapture(0, 0, GetSystemMetrics(0), 768, "D:\\startBtn.bmp");

}

void idle() {
   glutPostRedisplay(); // Post a re-paint request to activate display()
}
void timer(){
    time(&t);
    st=ctime(&t);
}

int main(int argc, char **argv)
{
    glutInit(&argc, &argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Guest Operating System");
    glutInitWindowPosition(300,300);

    clipArea();

    glutIdleFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(myKey);
    glutMouseFunc(myMouse);
    glutFullScreen();


    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
