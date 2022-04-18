# Door-Operating-System
A computer graphics project on building a GUI of a Operating System.

<br>
  <br>
  #include <stdio.h>
#include <stdlib.h>
#include <windows.h>  // only for windows platform
#include <GL/glut.h>
#include<math.h>

#include<time.h>
time_t t;   // not a primitive datatype


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

int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y
int fullScreenMode = 1; // Full-screen or windowed mode?

GLfloat house[3][2]= {{550,445},{430,550}, {1,1}};
GLfloat rot_mat[3][3]={{0},{0},{0}};
GLfloat result[3][2]={{0},{0},{0}};
GLfloat h=550;
GLfloat k=445;
GLfloat theta=0;
/*
GLfloat house[3][2]= {{550,430},{445,550}, {1,1}};
GLfloat rot_mat[3][3]={{0},{0},{0}};
GLfloat result[3][2]={{0},{0},{0}};
GLfloat h=550;
GLfloat k=445;
GLfloat theta=0;*/
  /* glBegin(GL_POLYGON);
        glVertex2f(550,430);
        glVertex2f(550,480);
        glVertex2f(800,480);
        glVertex2f(800,430);
    glEnd();
*/
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

void clearing(){
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
}

// while booting
void load(){
    /*
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

    sleep(2);
    glBegin(GL_POLYGON);
    glColor3f(0,1,0);
        glVertex2f(819,600);
        glVertex2f(819,650);
        glVertex2f(1092,650);
        glVertex2f(1092,600);
    glEnd();
    glFlush();
    */
    // 1   4
    // 2   3
}
// Home Screen
void mainScreen(){
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

    // Time
    glBegin(GL_POLYGON);
    glColor3f(1,1,0);
        glVertex2f(1205,741);
        glVertex2f(1205,767);
        glVertex2f(1365,767);
        glVertex2f(1365,741);
    glEnd();
    glColor3f(0,0,0);
    int  l,i;
    int x = 1215;
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
    glColor3f(1,1,1);
        glVertex2f(1335,0);
        glVertex2f(1335,26);
        glVertex2f(1365,26);
        glVertex2f(1365,0);
    glEnd();


    glColor3f(1,0,0);


    x = 1339;
    y = 17;
    st="OFF";
    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( x, y); // location to start printing text
    for( i=0; i < l; i++)  // loop until i is greater then l
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, st[i]); // Print a character on the screen
    }
    glFlush();




    glBegin(GL_LINE);
    glColor3f(0,0,0);
        glVertex2f(1335,0);
        glVertex2f(1335,26);
        glVertex2f(1365,26);
        glVertex2f(1365,0);
    glEnd();
    glFlush();
}

void unload(){
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

}


void signup(){
    // Sprint( 660, 250, "Enter password:" );

    glClearColor(0.9961,0.9059,0.0824,0);
    glClear(GL_COLOR_BUFFER_BIT);

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

    glFlush();
}


void exiting(){
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void display()
{



    if(stage==1){
        clearing();
        load();
        sleep(2);
    }
        stage=2;
        clearing();
        signup();
        glFlush();
/*
   clearing();
        signup();
        glFlush();
        */
        /*
        sleep(5);
   // while(stage==3 && ch=='a'){
   ch =getch();
        clearing();
        mainScreen();
        glFlush();
        sleep(5);
*/
    //sleep();
    /*
    stage=1;
    clearing();
    note();
    glFlush();
    stage=0;

    sleep(5);
    */
    /*
    stage=1;
    clearing();
    unload();
    glFlush();
    stage=0;

    exiting();
    glFlush();
    */

}

void clipArea()
{
    glClearColor(0.8, 0.2, 0.2, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1365, 767, 0);
}

void myKey(unsigned char key, int x, int y){
    switch (key) {
        case 27:
            exit(0);
            break;
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
            glVertex2f(550,430);
        glVertex2f(550,480);
        glVertex2f(800,480);
        glVertex2f(800,430);

    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN && stage==2){
        // sign in complete;
        for(int i=550;i<800;i++ ){
            for(int j=430; j<480; j++){
                if(xpos==i && ypos==j)
                    signedIn=1;
            }
        }
    }
    if(btn==GLUT_RIGHT_BUTTON && stage==2 && signedIn==1){
        stage=3;
    }

    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN && stage==3 ){
        mainScreen();
        glFlush();
        for(int i=1345;i<1365;i++ )
            for(int j=0; j<20; j++){
                if(xpos==i && ypos==j)
                    poweroff=1;
            }
    }
    if(btn==GLUT_RIGHT_BUTTON && stage==3 && poweroff==1){
        stage=4;
    }


    if(btn==GLUT_RIGHT_BUTTON && stage==4){
        clearing();
        unload();
        glFlush();
    }

}

int main(int argc, char **argv)
{
    theta = 0;
    theta = (3.14*theta)/180;
    glutInit(&argc, &argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Guest Operating System");
    glutInitWindowPosition(300,300);

    clipArea();
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(myKey);
    glutMouseFunc(myMouse);
    glutFullScreen();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

    /* for(int i=0; i<360; i=i+1){
        GLfloat m,n;
        m=-h*(cos(theta)-1)+k*(sin(theta));
        n=-k*(cos(theta)-1)-h*(sin(theta));
        rot_mat[0][0]=cos(theta);
        rot_mat[0][1]=-sin(theta);
        rot_mat[0][2]=m;
        rot_mat[1][0]=sin(theta);
        rot_mat[1][1]=cos(theta);
        rot_mat[1][2]=n;
        rot_mat[2][0]=0;
        rot_mat[2][1]=0;
        rot_mat[2][2]=1;
        // multiply();

        int p,q,r;
        for(p=0;p<3;p++)
            for(q=0;q<2;q++)
            {
                result[p][q]=0;
                for(r=0;r<3;r++)
                    result[p][q]=result[p][q]+rot_mat[p][r]*house[r][q];
            }
        // drawrotatedhouse();
        glBegin(GL_LINES);
            glVertex2f(result[0][0],result[1][0]);
            glVertex2f(result[0][1],result[1][1]);
        glEnd();
        theta+=i;
        theta = (3.14*theta)/180;
    }*/
    // rectangle here

