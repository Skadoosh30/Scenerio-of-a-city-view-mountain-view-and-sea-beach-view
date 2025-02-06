#include<windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<math.h>
#include<cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;


 int lightState = 0;
 static float	tp3	=  0;
 static float	pm =  300.0;
 static float	bm =  350.0;


 /*float spin = 0.0;
 GLfloat	tx	=  5;
 GLfloat	ty	=  5;*/

float sunY = 260.0f; // Initial Y-coordinate of the sun
bool isDay = true; // Tracks whether it's day or night
float backgroundR = 0.3f, backgroundG = 0.65f, backgroundB = 1.0f; // Daytime sky color
// Flags for continuous movement
bool isSunsetActive = false;
bool isSunriseActive = false;

/*void circle(GLfloat rx,GLfloat ry,GLfloat x,GLfloat y)
{
    int i;
    float rad = 0;
    glBegin(GL_POLYGON);
    glVertex2f(x,y);
    for(i = 0;i<=360; i++){
    rad = i*(3.1416f/180.0f);
    glVertex2f(x+rx*cos(rad),y+ry*sin(rad));

    }
    glEnd();


}*/
// Function to draw a circle (or ellipse)
void circle(GLfloat rx, GLfloat ry, GLfloat x, GLfloat y) {
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float rad = i * (3.1416f / 180.0f);
        glVertex2f(x + rx * cos(rad), y + ry * sin(rad));
    }
    glEnd();
}


void sun()
{
    glPushMatrix();
    glTranslated(270.0f, sunY, 0.0f); // Use the dynamic Y-coordinate for the sun
    glColor3f(1.0f, 0.76f, 0.3f);
    circle(20.0f, 20.0f, 0.0f, 0.0f);
    glPopMatrix();
}


// Update the sun's position and background color.
// The parameter moveDown==true means "sunset" (sun moves down), else "sunrise" (sun moves up).
void sunMove(bool moveDown) {
    if (moveDown) {
        sunY -= 0.5f;  // Move sun downward (sunset)
        if (sunY < -100.0f) {
            isDay = false;
        }
    } else {
        sunY += 0.5f;  // Move sun upward (sunrise)
        if (sunY > 260.0f) {
            isDay = true;
        }
    }

    // Adjust background color based on time of day
    if (isDay) {
        // Transition to day (light blue sky)
        backgroundR = fmin(backgroundR + 0.001f, 0.3f);
        backgroundG = fmin(backgroundG + 0.001f, 0.65f);
        backgroundB = fmin(backgroundB + 0.001f, 1.0f);
    } else {
        // Transition to night (darker blue sky)
        backgroundR = fmax(backgroundR - 0.001f, 0.0f);
        backgroundG = fmax(backgroundG - 0.001f, 0.0f);
        backgroundB = fmax(backgroundB - 0.001f, 0.2f);
    }
}

void make_cloud(int x, int y)
{

    glColor3f(1.0, 1.0, 1.0);
    circle(10,10,x,y);
    circle(10,10,x+10,y);
    circle(10,10,x+30,y);
    circle(10,10,x+5,y-10);
    circle(10,10,x+20,y-10);
    circle(10,10,x+5,y+10);
    circle(10,10,x+20,y+10);

}

void cloud_move_right(GLfloat t){
    tp3=tp3+t;
    if(tp3>600)
        tp3 = -300;
    glutPostRedisplay();
}


void cloud()
{
    glPushMatrix();
    glTranslatef(tp3,0,0);
    make_cloud(-300,280);
    make_cloud(-500,280);
    make_cloud(-200,250);
    make_cloud(-400,250);
    make_cloud(200,250);
    make_cloud(-100,250);
    make_cloud(-350,230);
    glPopMatrix();
    cloud_move_right(.1);

    glEnd();
}

void rectangle(int x, int y, int x1, int y1)
{
    glBegin(GL_QUADS);
        glVertex2d(x,y);
        glVertex2d(x,y1);
        glVertex2d(x1,y1);
        glVertex2d(x1,y);


    glEnd();
}

void triangle(int x, int y, int x1, int x2,int y1)
{
    glBegin(GL_POLYGON);
        glVertex2d(x,y);
        glVertex2d(x1,y);

        glVertex2d(x1,y);
        glVertex2d(x2,y1);

        glVertex2d(x2,y1);
        glVertex2d(x,y);
        glEnd();
}
void rectangle1(int x1,int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    glBegin(GL_QUADS);
        glVertex2d(x1,y1);
        glVertex2d(x3,y3);
        glVertex2d(x4,y4);
        glVertex2d(x2,y2);
        glEnd();
}





void ground()
{
     glColor3f(0.0, 0.3, 0.0);
     rectangle(-300,-300,300,-10);

}


void road() {
    glColor3f(0.3, 0.3, 0.3);
    rectangle(-300, -200, 300, -30);

    // Roadside borders
    glColor3f(0.0, 0.0, 0.0);
    rectangle(-300, -30, 300, -33);
    rectangle(-300, -205, 300, -200);

    // Lane dividers
    glColor3f(1.0, 1.0, 1.0);
    rectangle(-270, -120, -230, -110);
    rectangle(-150, -120, -100, -110);
    rectangle(80, -120, 130, -110);
    rectangle(200, -120, 265, -110);

    // Zebra Crossing (Height-wise)
    glColor3f(1.0, 1.0, 1.0);
    for (int i = -180; i <= -30; i += 19) { // Adjust spacing as needed
        rectangle(-50, i, 50, i + 9);  // Width of stripes
    }
}

 // 0 = Red, 1 = Yellow, 2 = Green

void drawTrafficLight() {
    // Draw the pole
    glColor3f(0.0, 0.0, 0.0);
    rectangle(1, -30, 10, 70);  // Pole

    // Draw the light box
    glColor3f(0.1, 0.1, 0.1);
    rectangle(-10, 70, 20, 120);  // Box for lights

    // Draw lights
    glColor3f(lightState == 0 ? 1.0 : 0.3, 0.0, 0.0);  // Red
    circle(10, 10, 5.5, 110);

   // glColor3f(lightState == 1 ? 1.0 : 0.3, 1.0, 0.0);  // Yellow
    //circle(10, 10, 5.5, 100);

    glColor3f(0.0, lightState == 1 ? 1.0 : 0.3, 0.0);  // Green
    circle(10, 10, 5.5, 85);
}


void allVehiclesMove() {
    if (lightState != 0) { // Vehicles move only when not red
        bm -= 0.6;
        if (bm < -350) bm = 350;

        pm += 0.35;
        if (pm > 350) pm = -350;


    }
    glutPostRedisplay();
}


void make_tree2()
{
    glColor3f(0.0, 0.6, 0.0);
    circle(20,30,0,0);
    circle(20,30,20,0);
    circle(20,30,40,0);
    circle(20,30,30,30);
    circle(20,30,10,30);
    circle(20,30,10,-30);
    circle(20,30,30,-30);

    circle(10,20,50,-70);
    circle(10,20,60,-60);
    circle(10,20,60,-80);


    glColor3f(0.75, 0.5, 0.25);
    rectangle1(10,-130,30,-130,10,-30,30,-30);
    rectangle1(10,-30,17,-30,5,0,5,0);
    rectangle1(17,-30,22,-30,19,0,19,0);
    rectangle1(22,-30,30,-30,35,0,35,0);
    rectangle1(30,-90,30,-100,55,-70,55,-75);

}

void tree2()
{
    glPushMatrix();
    glTranslated(-320,120,0);
    make_tree2();
    glPopMatrix();

    glPushMatrix();
    glTranslated(270,120,0);
    make_tree2();
    glPopMatrix();

    glPushMatrix();
    glTranslated(150,120,0);
    make_tree2();
    glPopMatrix();

      glPushMatrix();
    glTranslated(-10,120,0);
    make_tree2();
    glPopMatrix();



}

void busmake()
{
     glColor3f(0.7, 0.13, 0.13);
    rectangle(-60,-15,0,15);
    rectangle(-80,-15,-60,25);
    glColor3f(0, 0, 0);
    rectangle(-60,15,-2,50);
    rectangle(-70,25,-60,50);
    glColor3f(0.76, 0.84, 0.84);
    rectangle1(-2,50,-70,50,-7,55,-76,55);

    glColor3f(0.2, 0.8, 1.0);
    rectangle1(-70,25,-74,25,-70,50,-76,55);

    glColor3f(1, 1, 1.0);
    rectangle(-58,0,-50,35);
    glColor3f(0.2, 0.8, 1.0);
    rectangle(-45,30,-40,40);
    rectangle(-35,30,-30,40);
    rectangle(-25,30,-20,40);
    rectangle(-15,30,-10,40);


    glColor3f(0.7, 0.13, 0.13);
    circle(3,15,0,0);
    circle(3,20,-80,5);

    glColor3f(0, 0, 0);
    circle(8,16,-10,-15);
    circle(8,16,-70,-15);


}
void carmake()
{
    glColor3f(0.2, 0.2, 0.8);
    rectangle(-50, -20, 0, 10);
    glColor3f(0.8, 0.8, 0.8);
    rectangle(-45, 10, -5, 20);

    glColor3f(0.0, 0.0, 0.0);
    circle(5, 5, -40, -20);
    circle(5, 5, -10, -20);

    glColor3f(1.0, 1.0, 1.0);
    circle(3, 3, -40, -20);
    circle(3, 3, -10, -20);
}

void bus()
{
    glPushMatrix();
    glTranslatef(bm, -165, 0);
    busmake();
    glPopMatrix();
}

void car()
{
    glPushMatrix();
    glTranslatef(pm, -70, 0); // Adjusted position for marked area
    carmake();
    glPopMatrix();
}



void building(int x1, int y1, int x2, int y2, float r, float g, float b, int window_count) {
    // Building base
    glColor3f(r, g, b);
    rectangle(x1, y1, x2, y2);

    // Windows
    glColor3f(0.95, 0.95, 0.95);
    int window_width = 10;
    int window_height = 10;
    int gap = 15;
    for (int i = x1 + gap; i < x2 - gap; i += window_width + gap) {
        for (int j = y1 + gap; j < y2 - gap; j += window_height + gap) {
            rectangle(i, j, i + window_width, j + window_height);
        }
    }
}

void buildings()
{
    building(-250, -10, -200, 220, 0.5f, 0.2f, 0.8f, 4); // Purple
    building(-200, -10, -140, 170, 0.2f, 0.7f, 0.3f, 3);  // Green
    building(-135, -10, -40, 270, 0.8f, 0.3f, 0.2f, 4);     // Red
    building(100, -10, 170, 250, 1.0f, 0.75f, 0.8f, 4);   // Blue
    building(30, -10, 80, 150, 0.9f, 0.8f, 0.1f, 5);   // Yellow

}
void house()
{
    glColor3f(0.82f, 0.41f, 0.12f);
    rectangle1(200,-9,350,-9,200,80,350,80);
    rectangle1(200,80,250,80,200,120,250,120);
    glColor3f(0.0f, 0.1f, 0.0f);
    triangle(195,120,255,225,150);

    glColor3f(0.4, 1.0, 1.0);
    rectangle1(215,95,235,95,215,115,235,115);

    glColor3f(0.95, 0.95, 0.95);
    rectangle1(220,-9,250,-9,220,40,250,40);
}


void drawBench(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);  // Move the bench to a new position

    // **Bench Legs (Brown)**
    glColor3f(0.5, 0.2, 0.0);
    glBegin(GL_QUADS);

    // Left Leg
    glVertex2f(-220, -220); glVertex2f(-215, -220);
    glVertex2f(-215, -200); glVertex2f(-220, -200);

    // Right Leg
    glVertex2f(-190, -220); glVertex2f(-185, -220);
    glVertex2f(-185, -200); glVertex2f(-190, -200);

    glEnd();

    // **Bench Seat**
    glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(-225, -200); glVertex2f(-180, -200);
    glVertex2f(-180, -190); glVertex2f(-225, -190);
    glEnd();

    glPopMatrix();
}

void drawBenches() {
    drawBench(-50, 0);  // Bench 1
    drawBench(70, 0);   // Bench 2 (Shifted right)
    drawBench(310, 0);// Bench 3 (More to the right)
    drawBench(430, 0);// bench 4
    drawBench(70,200);
    drawBench(365,200);
}

void drawPineTree(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // **Tree Trunk (Brown)**
    glColor3f(0.5, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(-8, -40); glVertex2f(8, -40);
    glVertex2f(8, 0); glVertex2f(-8, 0);
    glEnd();

    // **Tree Leaves (Three Triangles)**
    glColor3f(0.0, 0.6, 0.0);

    // Bottom Triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(-30, 0);  // Left
    glVertex2f(30, 0);   // Right
    glVertex2f(0, 30);   // Top
    glEnd();

    // Middle Triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(-25, 15);
    glVertex2f(25, 15);
    glVertex2f(0, 45);
    glEnd();

    // Top Triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(-20, 30);
    glVertex2f(20, 30);
    glVertex2f(0, 60);
    glEnd();

    glPopMatrix();
}





void drawObjects() {

    drawPineTree(170, -190);// Adjust X and Y to place tree between benches
    drawPineTree(280, -190);
     drawPineTree(55, -190);
    drawPineTree(-75,-190);
    drawPineTree(-190,-190);
    drawPineTree(80,20);

}


// Function to draw a lamp post


// Mouse callback to set movement flags.
// Left button: sunset; Right button: sunrise.
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Start sunset (sun moves down and background transitions to night)
        if (!isSunsetActive) {
            isSunsetActive = true;
            isSunriseActive = false;
            std::cout << "Left mouse click: Starting sunset." << std::endl;
            sunMove(true); // Trigger sunset immediately on click
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        // Start sunrise (sun moves up and background transitions to day)
        if (!isSunriseActive) {
            isSunriseActive = true;
            isSunsetActive = false;
            std::cout << "Right mouse click: Starting sunrise." << std::endl;
             sunMove(true); // Trigger sunrise immediately on click
        }
    }
    // glutPostRedisplay();  // Redraw the scene after mouse action
}
// Timer callback: updates the scene at ~60 FPS.
// If a movement flag is active, it continuously updates the sun's position.
// Mouse interaction for sunrise and sunset
void timer(int) {
    if (isSunsetActive) {
        sunMove(true);
    }
    if (isSunriseActive) {
        sunMove(true);
    }
    glutPostRedisplay();                   // Redraw the scene
    glutTimerFunc(4, timer, 0);           // Set up next timer callback (~60 FPS)
}
/*void myDisplay()
{
     glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);  // Update background color
    glClear(GL_COLOR_BUFFER_BIT);
    sun();
    cloud();

    ground();
    road();

    tree2();
    buildings();
    house();


   drawTrafficLight();
    bus();
    car();

    allVehiclesMove();

   drawBenches();
   drawObjects();
    glFlush();
}*/
void init()
{   glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);

}


/*void my_keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'g':
            lightState = 1;
            //glutIdleFunc(allVehiclesMove); // Set a single idle function for all vehicles
             glutPostRedisplay();
            break;
        case 'r':
            lightState = 0;
           // glutIdleFunc(NULL);
            glutPostRedisplay();
            break;

        default:
            break;
    }
}*/


/*int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1500, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Computer Graphics Project");
    init();
    glutDisplayFunc(myDisplay);
   // glutTimerFunc(3000, changeTrafficLight, 0);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
   // glutSpecialFunc(spe_key);
    glutKeyboardFunc(my_keyboard);
    glutMainLoop();
    return 0;
}*/

//***************************************************************************************** //end 3rd scenerio code  ************************



// Global variable to track cloud positions
float cloudmoun1 = -0.82f;
float cloudmoun2 = 0.13f;
float carPosition1 = -1.0f; // Initial car position for left-to-right car
float carPosition2 = 1.0f;  // Initial car position for right-to-left car

bool carVisible1 = true;   // Car 1 visibility flag
bool carVisible2 = true;   // Car 2 visibility flag



// Function to draw a circle (used for sun, clouds, and smoke)
void circlemoun(float cx, float cy, float r, int num_segments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}



// Function to draw the dark blue-gray mountain
void drawBlueMountain() {
    glColor3f(0.2f, 0.2f, 0.3f); // Dark Blue-Gray color
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, 0.2f);
    glVertex2f(1.0f, -0.5f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.2f); // Darker Blue color
    glBegin(GL_POLYGON);
    glVertex2f(-0.8f, -0.5f);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.4f, 0.1f);
    glVertex2f(0.8f, -0.5f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.9f); // Light Gray/White for snow
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.2f, 0.3f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.2f, 0.3f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0.25f, 0.1f);
    glVertex2f(0.35f, 0.2f);
    glVertex2f(0.3f, 0.0f);
    glEnd();
}

// Function to draw the green mountain
void drawGreenMountain(float xOffset) {
    glColor3f(0.0f, 0.3f, 0.0f); // Deep green
    glBegin(GL_POLYGON);
    glVertex2f(-0.8f + xOffset, -0.5f);
    glVertex2f(-0.2f + xOffset, 0.4f);
    glVertex2f(0.0f + xOffset, 0.7f);
    glVertex2f(0.2f + xOffset, 0.5f);
    glVertex2f(0.7f + xOffset, -0.5f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f); // Medium green
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f + xOffset, -0.5f);
    glVertex2f(-0.2f + xOffset, 0.3f);
    glVertex2f(0.0f + xOffset, 0.5f);
    glVertex2f(0.3f + xOffset, 0.4f);
    glVertex2f(0.6f + xOffset, -0.5f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.9f); // Bright white
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.2f + xOffset, 0.3f);
    glVertex2f(0.0f + xOffset, 0.7f);
    glVertex2f(0.2f + xOffset, 0.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0.05f + xOffset, 0.4f);
    glVertex2f(0.15f + xOffset, 0.5f);
    glVertex2f(0.2f + xOffset, 0.35f);
    glEnd();
}

// Function to draw the ground (grass)
void GroundMoun() {
    glColor3f(0.0f, 0.8f, 0.0f); // Green color for grass
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.0f, -0.9f);
    glVertex2f(1.0f, -0.9f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    // Extend road near toll plaza
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray for extended road
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.9f);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(1.0f, -0.9f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // White stripes for extended road
    for (float i = -1.0f; i < 1.0f; i += 0.2f) {
        glBegin(GL_POLYGON);
        glVertex2f(i, -0.7f);
        glVertex2f(i + 0.1f, -0.7f);
        glVertex2f(i + 0.1f, -0.69f);
        glVertex2f(i, -0.69f);
        glEnd();
    }
}

// Function to draw a road
void RoadMoun() {
    glColor3f(0.2f, 0.2f, 0.2f); // Dark Gray color
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.9f);
    glVertex2f(-1.0f, -0.7f);
    glVertex2f(1.0f, -0.7f);
    glVertex2f(1.0f, -0.9f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // White lines
    for (float i = -1.0f; i < 1.0f; i += 0.2f) {
        glBegin(GL_POLYGON);
        glVertex2f(i, -0.8f);
        glVertex2f(i + 0.1f, -0.8f);
        glVertex2f(i + 0.1f, -0.79f);
        glVertex2f(i, -0.79f);
        glEnd();
    }
}
// Function to draw a car
void drawCar(float xOffset, float yOffset, float r, float g, float b) {
    // Body
    glColor3f(r, g, b); // Car color
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f + xOffset, -0.85f + yOffset);
    glVertex2f(-0.1f + xOffset, -0.8f + yOffset);
    glVertex2f(0.1f + xOffset, -0.8f + yOffset);
    glVertex2f(0.1f + xOffset, -0.85f + yOffset);
    glEnd();

    // Roof
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.07f + xOffset, -0.8f + yOffset);
    glVertex2f(-0.05f + xOffset, -0.77f + yOffset);
    glVertex2f(0.05f + xOffset, -0.77f + yOffset);
    glVertex2f(0.07f + xOffset, -0.8f + yOffset);
    glEnd();

    // Wheels
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.08f + xOffset, -0.86f + yOffset);
    glVertex2f(-0.08f + xOffset, -0.88f + yOffset);
    glVertex2f(-0.06f + xOffset, -0.88f + yOffset);
    glVertex2f(-0.06f + xOffset, -0.86f + yOffset);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.06f + xOffset, -0.86f + yOffset);
    glVertex2f(0.06f + xOffset, -0.88f + yOffset);
    glVertex2f(0.08f + xOffset, -0.88f + yOffset);
    glVertex2f(0.08f + xOffset, -0.86f + yOffset);
    glEnd();
}


void drawSun() {
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(0.8f + 0.1f * cos(theta), 0.8f + 0.1f * sin(theta));
    }
    glEnd();
}

// Function to draw the toll plaza
void drawTollPlaza() {
    float scale = 0.5f;
    glColor3f(0.5f, 0.3f, 0.1f); // Brown color for structure
    glBegin(GL_POLYGON);
    glVertex2f(-0.5f * scale, -0.9f);
    glVertex2f(-0.5f * scale, -0.5f);
    glVertex2f(0.5f * scale, -0.5f);
    glVertex2f(0.5f * scale, -0.9f);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.7f); // Gray for roof
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f * scale, -0.5f);
    glVertex2f(-0.6f * scale, -0.4f);
    glVertex2f(0.6f * scale, -0.4f);
    glVertex2f(0.6f * scale, -0.5f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.9f); // White for booth windows
    glBegin(GL_POLYGON);
    glVertex2f(-0.45f * scale, -0.7f);
    glVertex2f(-0.45f * scale, -0.6f);
    glVertex2f(-0.3f * scale, -0.6f);
    glVertex2f(-0.3f * scale, -0.7f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-0.05f * scale, -0.7f);
    glVertex2f(-0.05f * scale, -0.6f);
    glVertex2f(0.1f * scale, -0.6f);
    glVertex2f(0.1f * scale, -0.7f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.3f * scale, -0.7f);
    glVertex2f(0.3f * scale, -0.6f);
    glVertex2f(0.45f * scale, -0.6f);
    glVertex2f(0.45f * scale, -0.7f);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f); // Black for sign
    glBegin(GL_POLYGON);
    glVertex2f(-0.3f * scale, -0.4f);
    glVertex2f(-0.3f * scale, -0.35f);
    glVertex2f(0.3f * scale, -0.35f);
    glVertex2f(0.3f * scale, -0.4f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // White color for text
    glRasterPos2f(-0.08f * scale, -0.389f);
    const char* text = "TOLL PLAZA";
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Function to draw the lamp post
void drawLampPost1() {
    // Draw the pole
    glColor3f(0.5f,0.2f,0.1f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(-0.91f, -0.9f); // Bottom left
    glVertex2f(-0.89f, -0.9f); // Bottom right
    glVertex2f(-0.89f, -0.55f); // Top right
    glVertex2f(-0.91f, -0.55f); // Top left
    glEnd();

    // Draw the lamp head (circle)
    glColor3f(1.0f, 1.0f, 1.0f); // Bright white color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(-0.9f + 0.04f * cos(theta), -0.55f + 0.04f * sin(theta));
    }
    glEnd();
}

void drawLampPost2() {
    // Draw the pole
    glColor3f(0.5f,0.2f,0.1f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(-0.61f, -0.9f); // Bottom left
    glVertex2f(-0.59f, -0.9f); // Bottom right
    glVertex2f(-0.59f, -0.55f); // Top right
    glVertex2f(-0.61f, -0.55f); // Top left
    glEnd();

    // Draw the lamp head (circle)
    glColor3f(1.0f, 1.0f, 1.0f); // Bright yellow color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(-0.6f + 0.04f * cos(theta), -0.55f + 0.04f * sin(theta));
    }
    glEnd();
}

void drawLampPost3() {
    // Draw the pole
    glColor3f(0.5f,0.2f,0.1f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(0.91f, -0.9f); // Bottom left
    glVertex2f(0.89f, -0.9f); // Bottom right
    glVertex2f(0.89f, -0.55f); // Top right
    glVertex2f(0.91f, -0.55f); // Top left
    glEnd();

    // Draw the lamp head (circle)
    glColor3f(1.0f, 1.0f, 1.0f); // Bright yellow color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(0.9f + 0.04f * cos(theta), -0.55f + 0.04f * sin(theta));
    }
    glEnd();
}

void drawLampPost4() {
    // Draw the pole
    glColor3f(0.5f,0.2f,0.1f); // Dark gray color
    glBegin(GL_QUADS);
    glVertex2f(0.61f, -0.9f); // Bottom left
    glVertex2f(0.59f, -0.9f); // Bottom right
    glVertex2f(0.59f, -0.55f); // Top right
    glVertex2f(0.61f, -0.55f); // Top left
    glEnd();

    // Draw the lamp head (circle)
    glColor3f(1.0f, 1.0f, 1.0f); // Bright yellow color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(0.6f + 0.04f * cos(theta), -0.55f + 0.04f * sin(theta));
    }
    glEnd();
}
void circlemoun(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Function to draw the car
void drawCar() {
    // Car Body
    glColor3f(1.0, 0.0, 0.0); // Red
    glBegin(GL_QUADS);
    glVertex2f(-0.635, -0.8);
    glVertex2f(-0.87, -0.8);
    glVertex2f(-0.87, -0.75);
    glVertex2f(-0.635, -0.75);
    glEnd();

    // Car Roof
    glBegin(GL_QUADS);
    glVertex2f(-0.65, -0.75);
    glVertex2f(-0.85, -0.75);
    glVertex2f(-0.83, -0.715);
    glVertex2f(-0.68, -0.715);
    glEnd();

    // Windows
    glColor3f(0.8, 0.8, 0.8); // Gray
    glBegin(GL_QUADS);
    glVertex2f(-0.725, -0.725);
    glVertex2f(-0.785, -0.725);
    glVertex2f(-0.8, -0.75);
    glVertex2f(-0.71, -0.75);
    glEnd();

    // Wheels
    glColor3f(0.0, 0.0, 0.0); // Black
    circlemoun(-0.82, -0.81, 0.03, 20);
    circlemoun(-0.69, -0.81, 0.03, 20);

    // Wheel Centers
    glColor3f(0.8, 0.8, 0.8); // Gray
    circlemoun(-0.82, -0.81, 0.015, 20);
    circlemoun(-0.69, -0.81, 0.015, 20);
}


/*void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // Light blue background

    drawBlueMountain();
    drawGreenMountain(-1.0f);
    drawGreenMountain(0.5f);

    GroundMoun();
    RoadMoun();
    drawTollPlaza();
    drawLampPost1(); // Draw the lamp post after the toll plaza
    drawLampPost2(); // Draw the lamp post after the toll plaza
    drawLampPost3(); // Draw the lamp post after the toll plaza
    drawLampPost4(); // Draw the lamp post after the toll plaza
    drawSun();

    //glPushMatrix();
    //glColor3f(1.0, 0.0, 0.0); // Car color (Red)
   // glTranslatef(carX, roadY, 0.0); // Adjust `roadY` to match the road level
    drawCar();
    glPopMatrix();




    drawCar();

    // Draw clouds
    circlemoun(cloudmoun1, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun1 + 0.04f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun1 + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    circlemoun(cloudmoun2, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun2 + 0.05f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun2 + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    glutSwapBuffers();
    glFlush();
}*/
// Idle function to update cloud positions
/*void updatemoun(int value) {
    cloudmoun1 += 0.004f;
    cloudmoun2 += 0.004f;

    if (cloudmoun1 > 1.8f) cloudmoun1 = -1.5f;
    if (cloudmoun2 > 1.5f) cloudmoun2 = -1.5f;

    glutPostRedisplay();
    glutTimerFunc(16, updatemoun, 0);
}*/

void initOpenGL() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

/*int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mountain Landscape with Toll Plaza and Lamp Post");
    initOpenGL();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}*/

//******************************************************************************** end scenerio 1st ********************

// Global variable to track cloud positions
float cloud1X = -0.82f;
float cloud2X = 0.13f;

// Global variables to track boat positions
float boat1X = -0.6f;
float boat2X = -0.4f;

// Global variable to track ship position
float shipX = 0.4f;

// Global variable to track day/night mode
enum Mode { DAY, EVENING, NIGHT };
Mode currentMode = DAY;
float transitionFactor = 0.0f; // 0.0 = day, 1.0 = night

// Star structure and array
struct Star {
    float x;
    float y;
};
Star stars[20];

// Function to interpolate between two colors based on a factor
void interpolateColor(float factor, float r1, float g1, float b1, float r2, float g2, float b2, float &r, float &g, float &b) {
    r = r1 + (r2 - r1) * factor;
    g = g1 + (g2 - g1) * factor;
    b = b1 + (b2 - b1) * factor;
}

// Function to draw a circle (used for sun, clouds, and smoke)
void Circlesea(float cx, float cy, float r, int num_segments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawFilledHalfCircle(GLfloat x, GLfloat y, GLfloat radius){ //umbrella
    int i;
    int triangleAmount = 200;

    GLfloat twicePi = 2.0f * 3.1415926f/2;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for(i = 0; i <= triangleAmount;i++) {
        glVertex2f(
            x + (radius * cos(i *  twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

// Function to draw a rectangle (used for sea, beach, ship hull, and cabin)
void Rectanglesea(float x, float y, float width, float height, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Function to draw a boat at a given position
void drawBoat(float offsetX, float offsetY, float scale) {
    // Boat base
    glColor3f(0.5f, 0.2f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f((-0.1f * scale) + offsetX, (0.0f * scale) + offsetY);
    glVertex2f((0.1f * scale) + offsetX, (0.0f * scale) + offsetY);
    glVertex2f((0.08f * scale) + offsetX, (-0.03f * scale) + offsetY);
    glVertex2f((-0.08f * scale) + offsetX, (-0.03f * scale) + offsetY);
    glEnd();

    // Mast
    glColor3f(0.3f, 0.1f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f((0.0f * scale) + offsetX, (0.0f * scale) + offsetY);
    glVertex2f((0.0f * scale) + offsetX, (0.15f * scale) + offsetY);
    glEnd();

    // Sail
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f((0.0f * scale) + offsetX, (0.15f * scale) + offsetY);
    glVertex2f((0.07f * scale) + offsetX, (0.03f * scale) + offsetY);
    glVertex2f((0.0f * scale) + offsetX, (0.03f * scale) + offsetY);
    glEnd();
}

// Function to draw a highly detailed ship
void drawShip(float offsetX, float offsetY, float scale) {
    // Draw the hull (curved and realistic)
    float hullR, hullG, hullB;
    interpolateColor(transitionFactor, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f, 0.1f, hullR, hullG, hullB);
    glColor3f(hullR, hullG, hullB);
    glBegin(GL_POLYGON);
    glVertex2f(offsetX - 0.3f * scale, offsetY);
    glVertex2f(offsetX + 0.3f * scale, offsetY);
    glVertex2f(offsetX + 0.28f * scale, offsetY + 0.05f * scale);
    glVertex2f(offsetX - 0.28f * scale, offsetY + 0.05f * scale);
    glEnd();

    // Draw the keel (bottom part of the hull)
    glColor3f(0.05f, 0.05f, 0.05f);
    glBegin(GL_POLYGON);
    glVertex2f(offsetX - 0.3f * scale, offsetY);
    glVertex2f(offsetX + 0.3f * scale, offsetY);
    glVertex2f(offsetX + 0.28f * scale, offsetY - 0.02f * scale);
    glVertex2f(offsetX - 0.28f * scale, offsetY - 0.02f * scale);
    glEnd();

    // Draw the main deck
    float deckR, deckG, deckB;
    interpolateColor(transitionFactor, 0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, deckR, deckG, deckB);
    Rectanglesea(offsetX - 0.28f * scale, offsetY + 0.05f * scale, 0.56f * scale, 0.03f * scale, deckR, deckG, deckB);

    // Draw the superstructure (cabins and bridge)
    float cabinR, cabinG, cabinB;
    interpolateColor(transitionFactor, 0.9f, 0.9f, 0.9f, 0.5f, 0.5f, 0.5f, cabinR, cabinG, cabinB);
    Rectanglesea(offsetX - 0.2f * scale, offsetY + 0.08f * scale, 0.4f * scale, 0.1f * scale, cabinR, cabinG, cabinB);
    Rectanglesea(offsetX - 0.15f * scale, offsetY + 0.18f * scale, 0.3f * scale, 0.05f * scale, cabinR, cabinG, cabinB);

    // Draw the smokestacks
    Rectanglesea(offsetX - 0.1f * scale, offsetY + 0.23f * scale, 0.05f * scale, 0.1f * scale, 0.6f, 0.1f, 0.1f);
    Rectanglesea(offsetX + 0.05f * scale, offsetY + 0.23f * scale, 0.05f * scale, 0.1f * scale, 0.6f, 0.1f, 0.1f);

    // Draw smoke from the smokestacks
    Circlesea(offsetX - 0.075f * scale, offsetY + 0.33f * scale, 0.02f * scale, 20, 0.9f, 0.9f, 0.9f);
    Circlesea(offsetX + 0.075f * scale, offsetY + 0.33f * scale, 0.02f * scale, 20, 0.9f, 0.9f, 0.9f);

    // Draw windows on the cabins
    float windowR, windowG, windowB;
    if (currentMode == NIGHT) {
        windowR = 1.0f; windowG = 1.0f; windowB = 0.8f;
    } else {
        windowR = 0.0f; windowG = 0.0f; windowB = 0.0f;
    }
    for (int i = 1; i < 5; i++) {
        Rectanglesea(offsetX - 0.25f * scale + i * 0.1f * scale, offsetY + 0.1f * scale, 0.03f * scale, 0.02f * scale, windowR, windowG, windowB);
    }
    for (int i = 1; i < 5; i++) {
        Rectanglesea(offsetX - 0.2f * scale + i * 0.08f * scale, offsetY + 0.2f * scale, 0.02f * scale, 0.015f * scale, windowR, windowG, windowB);
    }

    // Draw lifeboats
    Rectanglesea(offsetX - 0.25f * scale, offsetY + 0.05f * scale, 0.06f * scale, 0.02f * scale, 0.8f, 0.0f, 0.0f);
    Rectanglesea(offsetX + 0.19f * scale, offsetY + 0.05f * scale, 0.06f * scale, 0.02f * scale, 0.8f, 0.0f, 0.0f);

    // Draw railings
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(offsetX - 0.3f * scale, offsetY + 0.05f * scale);
    glVertex2f(offsetX + 0.3f * scale, offsetY + 0.05f * scale);
    glEnd();
}

// Function to draw a watching tower
void drawWatchingTower(float x, float y, float scale) {
    // Base of the tower (wider)
    float baseR, baseG, baseB;
    interpolateColor(transitionFactor, 0.5f, 0.35f, 0.05f, 0.3f, 0.2f, 0.05f, baseR, baseG, baseB);
    glColor3f(baseR, baseG, baseB);
    Rectanglesea(x - 0.12f * scale, y, 0.24f * scale, 0.2f * scale, baseR, baseG, baseB);

    // Pillars of the tower (wider spacing)
    glColor3f(0.4f, 0.3f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.08f * scale, y + 0.2f * scale);
    glVertex2f(x - 0.08f * scale, y + 0.5f * scale);
    glVertex2f(x + 0.08f * scale, y + 0.2f * scale);
    glVertex2f(x + 0.08f * scale, y + 0.5f * scale);
    glEnd();

    // Top platform of the tower (wider)
    float platformR, platformG, platformB;
    interpolateColor(transitionFactor, 0.6f, 0.45f, 0.1f, 0.4f, 0.3f, 0.1f, platformR, platformG, platformB);
    Rectanglesea(x - 0.12f * scale, y + 0.5f * scale, 0.24f * scale, 0.05f * scale, platformR, platformG, platformB);

    // Enclosed cabin at the top (wider)
    float cabinR, cabinG, cabinB;
    interpolateColor(transitionFactor, 0.7f, 0.5f, 0.3f, 0.5f, 0.4f, 0.2f, cabinR, cabinG, cabinB);
    Rectanglesea(x - 0.12f * scale, y + 0.55f * scale, 0.24f * scale, 0.1f * scale, cabinR, cabinG, cabinB);

    // Windows on the cabin (wider spacing)
    float windowR, windowG, windowB;
    if (currentMode == NIGHT) {
        windowR = 1.0f; windowG = 1.0f; windowB = 0.8f;
    } else {
        windowR = 0.9f; windowG = 0.9f; windowB = 0.9f;
    }
    Rectanglesea(x - 0.11f * scale, y + 0.56f * scale, 0.03f * scale, 0.03f * scale, windowR, windowG, windowB);
    Rectanglesea(x - 0.06f * scale, y + 0.56f * scale, 0.03f * scale, 0.03f * scale, windowR, windowG, windowB);
    Rectanglesea(x - 0.01f * scale, y + 0.56f * scale, 0.03f * scale, 0.03f * scale, windowR, windowG, windowB);
    Rectanglesea(x + 0.04f * scale, y + 0.56f * scale, 0.03f * scale, 0.03f * scale, windowR, windowG, windowB);

    // Roof of the tower (wider)
    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.12f * scale, y + 0.65f * scale);
    glVertex2f(x + 0.12f * scale, y + 0.65f * scale);
    glVertex2f(x, y + 0.75f * scale);
    glEnd();

    // Ladder leading up to the cabin
    glColor3f(0.4f, 0.3f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.04f * scale, y + 0.2f * scale);
    glVertex2f(x - 0.04f * scale, y + 0.55f * scale);
    glVertex2f(x + 0.04f * scale, y + 0.2f * scale);
    glVertex2f(x + 0.04f * scale, y + 0.55f * scale);
    glEnd();

    // Ladder rungs (wider)
    glBegin(GL_LINES);
    for (float i = y + 0.23f * scale; i <= y + 0.52f * scale; i += 0.05f * scale) {
        glVertex2f(x - 0.04f * scale, i);
        glVertex2f(x + 0.04f * scale, i);
    }
    glEnd();
}

// Function to draw a coconut tree
void drawCoconutTree(float x, float y, float scale) {
    // Trunk
    Rectanglesea(x, y, 0.03f * scale, 0.2f * scale, 0.5f, 0.25f, 0.0f);

    // Leaves
    Circlesea(x + 0.015f * scale, y + 0.22f * scale, 0.05f * scale, 10, 0.0f, 0.5f, 0.0f);
    Circlesea(x - 0.02f * scale, y + 0.2f * scale, 0.05f * scale, 10, 0.0f, 0.5f, 0.0f);
    Circlesea(x + 0.05f * scale, y + 0.2f * scale, 0.05f * scale, 10, 0.0f, 0.5f, 0.0f);
    Circlesea(x + 0.015f * scale, y + 0.25f * scale, 0.05f * scale, 10, 0.0f, 0.5f, 0.0f);
}

void sea_beach_chair1() {
    glColor3ub(102, 68, 28);
    glBegin(GL_QUADS);
    glVertex2f(-0.67f, -0.61f);
    glVertex2f(-0.58f, -0.61f);
    glVertex2f(-0.58f, -0.59f);
    glVertex2f(-0.67f, -0.59f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-0.65f, -0.59f);
    glVertex2f(-0.635f, -0.59f);
    glVertex2f(-0.655f, -0.55f);
    glVertex2f(-0.67f, -0.55f);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-.66f,-.61f);
    glVertex2f(-.66f,-0.64f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.6f,-.61f);
    glVertex2f(-.6f,-0.64f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.65f,-.61f);
    glVertex2f(-.65f,-0.63f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.61f,-.61f);
    glVertex2f(-.61f,-0.63f);
    glEnd();
}

void sea_beach_chair2() {
    glColor3ub(102, 68, 28);
    glBegin(GL_QUADS);
    glVertex2f(-0.47f, -0.61f);
    glVertex2f(-0.38f, -0.61f);
    glVertex2f(-0.38f, -0.59f);
    glVertex2f(-0.47f, -0.59f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-0.45f, -0.59f);
    glVertex2f(-0.435f, -0.59f);
    glVertex2f(-0.455f, -0.55f);
    glVertex2f(-0.47f, -0.55f);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-.46f,-.61f);
    glVertex2f(-.46f,-0.64f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.4f,-.61f);
    glVertex2f(-.4f,-0.64f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.45f,-.61f);
    glVertex2f(-.45f,-0.63f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.41f,-.61f);
    glVertex2f(-.41f,-0.63f);
    glEnd();
}

void sea_beach_chair3() {
    glColor3ub(102, 68, 28);
    glBegin(GL_QUADS);
    glVertex2f(-0.27f, -0.61f);
    glVertex2f(-0.18f, -0.61f);
    glVertex2f(-0.18f, -0.59f);
    glVertex2f(-0.27f, -0.59f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-0.25f, -0.59f);
    glVertex2f(-0.235f, -0.59f);
    glVertex2f(-0.255f, -0.55f);
    glVertex2f(-0.27f, -0.55f);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-.26f,-.61f);
    glVertex2f(-.26f,-0.64f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.2f,-.61f);
    glVertex2f(-.2f,-0.64f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.25f,-.61f);
    glVertex2f(-.25f,-0.63f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-.21f,-.61f);
    glVertex2f(-.21f,-0.63f);
    glEnd();
}

void sea_beach_umbrella1() {
    glColor3ub(255,39,64);
    drawFilledHalfCircle(-.59,-.52,.075);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-.59f,-.68f);
    glVertex2f(-.59f,-0.52f);
    glEnd();
}

void sea_beach_umbrella2() {
    glColor3ub(238,255,64);
    drawFilledHalfCircle(-.39,-.52,.075);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-.39f,-.68f);
    glVertex2f(-.39f,-0.52f);
    glEnd();
}

void sea_beach_umbrella3() {
    glColor3ub(238,39,255);
    drawFilledHalfCircle(-.19,-.52,.075);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(-.19f,-.68f);
    glVertex2f(-.19f,-0.52f);
    glEnd();
}

// Display function
/*void display() {


    if(timeChange>0){
        glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);  // Update background color
    glClear(GL_COLOR_BUFFER_BIT);
    sun();
    cloud();

    ground();
    road();

    tree2();
    buildings();
    house();


   drawTrafficLight();
    bus();
    car();

    allVehiclesMove();

   drawBenches();
   drawObjects();
    }

   //city display end........***********************************************

    else if(timeChange>60){

         glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // Light blue background

    drawBlueMountain();
    drawGreenMountain(-1.0f);
    drawGreenMountain(0.5f);

    GroundMoun();
    RoadMoun();
    drawTollPlaza();
    drawLampPost1(); // Draw the lamp post after the toll plaza
    drawLampPost2(); // Draw the lamp post after the toll plaza
    drawLampPost3(); // Draw the lamp post after the toll plaza
    drawLampPost4(); // Draw the lamp post after the toll plaza
    drawSun();

    //glPushMatrix();
    //glColor3f(1.0, 0.0, 0.0); // Car color (Red)
   // glTranslatef(carX, roadY, 0.0); // Adjust `roadY` to match the road level
    drawCar();
    glPopMatrix();




    drawCar();

    // Draw clouds
    circlemoun(cloudmoun1, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun1 + 0.04f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun1 + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    circlemoun(cloudmoun2, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun2 + 0.05f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun2 + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    glutSwapBuffers();

    }
//*************************************************************mountain end*************************************

    // sea start from here ***************************************************************

   else if(timeChange>120){
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the sky
    float skyR, skyG, skyB;
    if (currentMode == DAY) {
        interpolateColor(transitionFactor, 0.5f, 0.8f, 1.0f, 0.2f, 0.2f, 0.5f, skyR, skyG, skyB);
    } else if (currentMode == EVENING) {
        interpolateColor(transitionFactor, 0.2f, 0.2f, 0.5f, 0.0f, 0.0f, 0.1f, skyR, skyG, skyB);
    } else {
        skyR = 0.0f; skyG = 0.0f; skyB = 0.1f;
    }
    Rectanglesea(-1.0f, 0.0f, 2.0f, 1.0f, skyR, skyG, skyB);

    // Draw the sun/moon
    if (currentMode == DAY || currentMode == EVENING) {
        float sunR, sunG, sunB;
        interpolateColor(transitionFactor, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.0f, sunR, sunG, sunB);
        Circlesea(0.7f, 0.7f, 0.1f, 50, sunR, sunG, sunB);
    } else {
        Circlesea(0.7f, 0.7f, 0.1f, 50, 0.9f, 0.9f, 0.9f);
    }

    // Draw fixed stars at night
    if (currentMode == NIGHT) {
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 20; i++) {
            Circlesea(stars[i].x, stars[i].y, 0.005f, 10, 1.0f, 1.0f, 1.0f);
        }
    }

    // Draw clouds
    Circlesea(cloud1X, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud1X + 0.04f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud1X + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    Circlesea(cloud2X, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud2X + 0.05f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud2X + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    // Draw the sea
    float seaR, seaG, seaB;
    if (currentMode == DAY) {
        interpolateColor(transitionFactor, 0.0f, 0.5f, 0.8f, 0.0f, 0.2f, 0.4f, seaR, seaG, seaB);
    } else if (currentMode == EVENING) {
        interpolateColor(transitionFactor, 0.0f, 0.2f, 0.4f, 0.0f, 0.1f, 0.2f, seaR, seaG, seaB);
    } else {
        seaR = 0.0f; seaG = 0.1f; seaB = 0.2f;
    }
    Rectanglesea(-1.0f, -0.5f, 2.0f, 0.5f, seaR, seaG, seaB);

    // Draw the beach
    Rectanglesea(-1.0f, -1.0f, 2.0f, 0.5f, 0.9f, 0.7f, 0.4f);

    // Draw boats and ships
    drawBoat(boat1X, -0.3f, 1.0f);
    drawBoat(boat2X, -0.1f, 0.7f);
    drawShip(shipX, -0.2f, 0.7f);

    // Draw watching tower and coconut trees
    drawWatchingTower(0.8f, -0.8f, 0.8f);
    drawCoconutTree(-0.95f, -0.85f, 1.0f);
    drawCoconutTree(-0.88f, -0.88f, 1.0f);
    drawCoconutTree(-0.82f, -0.86f, 1.0f);

    // Draw beach chairs and umbrellas
    sea_beach_chair1();
    sea_beach_chair2();
    sea_beach_chair3();
    sea_beach_umbrella1();
    sea_beach_umbrella2();
    sea_beach_umbrella3();
    }
     if (timeChange>180)
     {
         timeChange=0;
     }

    timeChange++;

    cout<<timeChange<<endl;

    glFlush();

}*/

int scenarioIndex = 0; // 0: City, 1: Mountain, 2: Sea


void display() {
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (scenarioIndex == 0) {
        // City Scene
        //glClearColor(0.3f, 0.65f, 1.0f, 1.0f);
        cout << "Displaying City Scene" << endl;
        // Call all functions related to City scene

    glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);  // Update background color
    glClear(GL_COLOR_BUFFER_BIT);
    sun();
    cloud();

    ground();
    road();

    tree2();
    buildings();
    house();


   drawTrafficLight();
    bus();
    car();

    allVehiclesMove();

   drawBenches();
   drawObjects();

    }

     else if (scenarioIndex == 1) {
        // Mountain Scene
        glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
        cout << "Displaying Mountain Scene" << endl;
        // Call all functions related to Mountain scene

        glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glClearColor(0.8f, 0.9f, 1.0f, 1.0f); // Light blue background

    drawBlueMountain();
    drawGreenMountain(-1.0f);
    drawGreenMountain(0.5f);

    GroundMoun();
    RoadMoun();

    // Draw cars if visible
    if (carVisible1) {
        drawCar(carPosition1, 0.0f, 0.8f, 0.0f, 0.0f); // Red car (left-to-right)
    }
    if (carVisible2) {
        drawCar(carPosition2, 0.0f, 0.0f, 0.0f, 0.8f); // Blue car (right-to-left)
    }

    drawTollPlaza();
    drawLampPost1(); // Draw the lamp post after the toll plaza
    drawLampPost2(); // Draw the lamp post after the toll plaza
    drawLampPost3(); // Draw the lamp post after the toll plaza
    drawLampPost4(); // Draw the lamp post after the toll plaza
    drawSun();

    //glPushMatrix();
    //glColor3f(1.0, 0.0, 0.0); // Car color (Red)
   // glTranslatef(carX, roadY, 0.0); // Adjust `roadY` to match the road level
    //drawCar();
    glPopMatrix();
    //drawCar();

    // Draw clouds
    circlemoun(cloudmoun1, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun1 + 0.04f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun1 + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    circlemoun(cloudmoun2, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun2 + 0.05f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    circlemoun(cloudmoun2 + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    glutSwapBuffers();



    }




    else if (scenarioIndex == 2) {
        // Sea Scene
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        cout << "Displaying Sea Scene" << endl;
        // Call all functions related to Sea scene

         glClear(GL_COLOR_BUFFER_BIT);

    // Draw the sky
    float skyR, skyG, skyB;
    if (currentMode == DAY) {
        interpolateColor(transitionFactor, 0.5f, 0.8f, 1.0f, 0.2f, 0.2f, 0.5f, skyR, skyG, skyB);
    } else if (currentMode == EVENING) {
        interpolateColor(transitionFactor, 0.2f, 0.2f, 0.5f, 0.0f, 0.0f, 0.1f, skyR, skyG, skyB);
    } else {
        skyR = 0.0f; skyG = 0.0f; skyB = 0.1f;
    }
    Rectanglesea(-1.0f, 0.0f, 2.0f, 1.0f, skyR, skyG, skyB);

    // Draw the sun/moon
    if (currentMode == DAY || currentMode == EVENING) {
        float sunR, sunG, sunB;
        interpolateColor(transitionFactor, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.0f, sunR, sunG, sunB);
        Circlesea(0.7f, 0.7f, 0.1f, 50, sunR, sunG, sunB);
    } else {
        Circlesea(0.7f, 0.7f, 0.1f, 50, 0.9f, 0.9f, 0.9f);
    }

    // Draw fixed stars at night
    if (currentMode == NIGHT) {
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 20; i++) {
            Circlesea(stars[i].x, stars[i].y, 0.005f, 10, 1.0f, 1.0f, 1.0f);
        }
    }

    // Draw clouds
    Circlesea(cloud1X, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud1X + 0.04f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud1X + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    Circlesea(cloud2X, 0.8f, 0.1f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud2X + 0.05f, 0.87f, 0.08f, 50, 1.0f, 1.0f, 1.0f);
    Circlesea(cloud2X + 0.1f, 0.8f, 0.09f, 50, 1.0f, 1.0f, 1.0f);

    // Draw the sea
    float seaR, seaG, seaB;
    if (currentMode == DAY) {
        interpolateColor(transitionFactor, 0.0f, 0.5f, 0.8f, 0.0f, 0.2f, 0.4f, seaR, seaG, seaB);
    } else if (currentMode == EVENING) {
        interpolateColor(transitionFactor, 0.0f, 0.2f, 0.4f, 0.0f, 0.1f, 0.2f, seaR, seaG, seaB);
    } else {
        seaR = 0.0f; seaG = 0.1f; seaB = 0.2f;
    }
    Rectanglesea(-1.0f, -0.5f, 2.0f, 0.5f, seaR, seaG, seaB);

    // Draw the beach
    Rectanglesea(-1.0f, -1.0f, 2.0f, 0.5f, 0.9f, 0.7f, 0.4f);

    // Draw boats and ships
    drawBoat(boat1X, -0.3f, 1.0f);
    drawBoat(boat2X, -0.1f, 0.7f);
    drawShip(shipX, -0.2f, 0.7f);

    // Draw watching tower and coconut trees
    drawWatchingTower(0.8f, -0.8f, 0.8f);
    drawCoconutTree(-0.95f, -0.85f, 1.0f);
    drawCoconutTree(-0.88f, -0.88f, 1.0f);
    drawCoconutTree(-0.82f, -0.86f, 1.0f);

    // Draw beach chairs and umbrellas
    sea_beach_chair1();
    sea_beach_chair2();
    sea_beach_chair3();
    sea_beach_umbrella1();
    sea_beach_umbrella2();
    sea_beach_umbrella3();
    }

    glFlush();
}
/*void Testkeyboard(unsigned char key, int x, int y) {
    if (key == 'n' || key == 'N') {
        scenarioIndex = (scenarioIndex + 1) % 3; // Cycle through scenarios
    } else if (key == 'p' || key == 'P') {
        scenarioIndex = (scenarioIndex - 1 + 3) % 3; // Cycle backward
    }
    glutPostRedisplay(); // Refresh display
}*/
// Idle function to update cloud positions
void update(int value) {
    cloud1X += 0.003f;
    cloud2X += 0.003f;
    boat1X += 0.0015f;
    boat2X += 0.002f;
    shipX += 0.0015f;
    cloudmoun1 += 0.0005f;
    cloudmoun2 += 0.0005f;
    carPosition1 += 0.005f; // Move car 1 to the right
    carPosition2 -= 0.005f; // Move car 2 to the left

    // Hide car 1 when it enters the toll plaza
    if (carPosition1 > -0.2f && carPosition1 < 0.2f) {
        carVisible1 = false;
    } else {
        carVisible1 = true;
    }

    // Hide car 2 when it enters the toll plaza
    if (carPosition2 > -0.2f && carPosition2 < 0.2f) {
        carVisible2 = false;
    } else {
        carVisible2 = true;
    }

    // Reset car 1 position after it exits the screen
    if (carPosition1 > 1.0f) {
        carPosition1 = -1.0f;
    }

    // Reset car 2 position after it exits the screen
    if (carPosition2 < -1.0f) {
        carPosition2 = 1.0f;
    }




    if (cloudmoun1 > 1.8f) cloudmoun1 = -1.5f;
    if (cloudmoun2 > 1.5f) cloudmoun2 = -1.5f;


    if (cloud1X > 1.8f) cloud1X = -1.5f;
    if (cloud2X > 1.5f) cloud2X = -1.5f;
    if (boat1X > 1.5f) boat1X = -1.5f;
    if (boat2X > 1.5f) boat2X = -1.5f;
    if (shipX > 1.5f) shipX = -1.5f;

    glutPostRedisplay();
    glutTimerFunc(6, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    cout << "Before Key Press: scenarioIndex = " << scenarioIndex << endl; // Debugging output

    // **Scenario Switching (Global)**
    if (key == 'n' || key == 'N') {
        scenarioIndex = (scenarioIndex + 1) % 3; // Move to the next scenario
    }
    else if (key == 'p' || key == 'P') {
        scenarioIndex = (scenarioIndex - 1 + 3) % 3; // Move to the previous scenario
    }

    cout << "After Key Press: scenarioIndex = " << scenarioIndex << endl; // Debugging output

    // **City Scenario Controls (scenarioIndex = 0)**
    if (scenarioIndex == 0) {
        switch (key) {
            case 'g':
                lightState = 1;  // Change traffic light to Green
                cout << "Traffic light: GREEN" << endl;
                glutPostRedisplay();
                break;
            case 'r':
                lightState = 0;  // Change traffic light to Red
                cout << "Traffic light: RED" << endl;
                glutPostRedisplay();
                break;
            default:
                break;
        }
    }

    // **Sea Scenario Controls (scenarioIndex = 2)**
    else if (scenarioIndex == 2) {
        switch (key) {
            case 'd':
            case 'D':
                currentMode = DAY;
                transitionFactor = 0.0f;
                cout << "Switched to DAY mode" << endl;
                break;
            case 'e':
            case 'E':
                currentMode = EVENING;
                transitionFactor = 0.5f;
                cout << "Switched to EVENING mode" << endl;
                break;
            case 'o':
            case 'O':
                currentMode = NIGHT;
                transitionFactor = 1.0f;
                cout << "Switched to NIGHT mode" << endl;
                break;
            default:
                break;
        }
        glutPostRedisplay();
    }

    // Ensure the display updates after key press
    glutPostRedisplay();
}



// Initialization function
void initsea() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    // Initialize fixed star positions
    srand(time(0));
    for (int i = 0; i < 20; i++) {
        stars[i].x = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        stars[i].y = static_cast<float>(rand()) / RAND_MAX;
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1500, 700);
    //glutInitWindowPosition(0, 0);
    glutCreateWindow("all Scenerio");
    initsea();
    initOpenGL();
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, update, 0);
    glutTimerFunc(0, timer, 0);


    glutMainLoop();
    return 0;
}


