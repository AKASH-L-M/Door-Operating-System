
#include <windows.h>  // only for windows platform
#include <GL/glut.h>
void display()
{
    // code to create a model
    glClearColor(0,1,0,0); //R,G,B,Alpha to set background color
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush(); // send info from frame buffer to screen
}

int main(int argc, char **argv) // initiating communication of openGL with OS
{
    glutInit(&argc, &argv); // initializing
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); // color or Black White image
                            // GLUT_DOUBLE - for 2D Buffers required to send information from hardware
    glutInitWindowSize(600,400); // (w,H)
    glutInitWindowPosition(0,0);

    glutCreateWindow("Creating a window"); // Giving a name to a window
    glutDisplayFunc(display); // repetitively calls display function
    glutMainLoop(); // gets into infinite loop waiting for mouse or keyboard event
    return 0;

}
