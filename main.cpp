

//#ifdef __WIN32__
#include <windows.h>
//#endif

#include <stdlib.h>
#include <GL/glut.h>
#include "load_bmp.h"
#include "load_3ds.h"
#include "object.h"
#include <time.h>   /* 時間相關函數 */
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
using namespace std;


/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/

 // The width and height of your window, change them as you like
int screen_width = 700;
int screen_height = 700;

// Absolute rotation values (0-359 degrees) and rotation increments for each frame
double rotation_x = 0, rotation_x_increment = 0.1;
double rotation_y = 0, rotation_y_increment = 0.05;
double rotation_z = 0, rotation_z_increment = 0.03;

// Flag for rendering as lines or filled polygons
int filling = 1; //0=OFF 1=ON

//Lights settings
GLfloat light_ambient[] = { 0.8f, 0.8f, 0.8f, 0.99f };
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.5f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 0.5f };
GLfloat light_position[] = { 100.0f, 0.0f, 50.0f, 1.0f };

//flighter Materials settings
GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular[] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_shininess[] = { 20.0f };

//my add
double fly_r = 0.0, fly_f = 0.0;
int LR = 0;//L=-1 R=1
int UD = 0;//Down=-1 UP=1

struct Point {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

Point flighter_position;
double flighter_speed = 3.0;

Point H(Point P1, Point P2, Point T1, Point T2, double t) {
    Point out;
    out.x = ((1.0 - 3.0 * pow(t, 2.0) + 2.0 * pow(t, 3.0)) * P1.x + pow(t, 2.0) * (3.0 - 2.0 * t) * P2.x + t * pow(t - 1.0, 2.0) * T1.x + pow(t, 2.0) * (t - 1.0) * T2.x);
    out.y = ((1.0 - 3.0 * pow(t, 2.0) + 2.0 * pow(t, 3.0)) * P1.y + pow(t, 2.0) * (3.0 - 2.0 * t) * P2.y + t * pow(t - 1.0, 2.0) * T1.y + pow(t, 2.0) * (t - 1.0) * T2.y);
    out.z = ((1.0 - 3.0 * pow(t, 2.0) + 2.0 * pow(t, 3.0)) * P1.z + pow(t, 2.0) * (3.0 - 2.0 * t) * P2.z + t * pow(t - 1.0, 2.0) * T1.z + pow(t, 2.0) * (t - 1.0) * T2.z);
    return  out;;
}

float rangom_1_0() {
    return float((double)rand() / (RAND_MAX + 1.0));
}

struct stone {
    const double minP = -100.0, maxP = 100.0;

    Point p = {(maxP - minP) * rand() / (RAND_MAX + 1.0) + minP,
                    (maxP - minP) * rand() / (RAND_MAX + 1.0) + minP,
                    flighter_position.z-1000.0 };

    double color[3] = {(1.0-0.3) * rand() / (RAND_MAX + 1.0)+0.3,
            (1.0 - 0.3)* rand() / (RAND_MAX + 1.0) + 0.3,
            (1.0 - 0.3)* rand() / (RAND_MAX + 1.0) + 0.3 }; 

    GLfloat mat_ambient1 [4] = { float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) };
    GLfloat mat_diffuse1 [4] = { float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) };
    GLfloat mat_specular1 [4] = { float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) };
    
    void draw() {
        //p = H(p1, p2, t1, t2,t);
        //double stone_size = (120 + (p.z - (flighter_position.z + 20))) / 150.0; //0~1
        
        glPushMatrix();
        glColor3dv(color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
        //glMaterialfv(GL_FRONT, GL_SHININESS, shine1);
        glTranslatef(p.x, p.y, p.z);
        glutSolidSphere(3, 30, 30);
        glPopMatrix();
        
    }
};
vector <stone> Stone;

struct coin {
    const double minP = -20.0, maxP = 20.0;

    Point p = { (maxP - minP) * rand() / (RAND_MAX + 1.0) + minP,
                    (maxP - minP) * rand() / (RAND_MAX + 1.0) + minP,
                    flighter_position.z - 1000.0 };

    double color[3] = { (1.0 - 0.3) * rand() / (RAND_MAX + 1.0) + 0.3,
            (1.0 - 0.3) * rand() / (RAND_MAX + 1.0) + 0.3,
            (1.0 - 0.3) * rand() / (RAND_MAX + 1.0) + 0.3 };

    GLfloat mat_ambient1[4] = { float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) };
    GLfloat mat_diffuse1[4] = { float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) };
    GLfloat mat_specular1[4] = { float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) ,float((double)rand() / (RAND_MAX + 1.0)) };

    float rotate[4] = { float((360.0 - 0.0) * rand() / (RAND_MAX + 1.0)),
                        float((double)rand() / (RAND_MAX + 1.0)) ,
                        float((double)rand() / (RAND_MAX + 1.0)) ,
                        float((double)rand() / (RAND_MAX + 1.0)) };


    void draw() {
        //p = H(p1, p2, t1, t2,t);
        //double stone_size = (120 + (p.z - (flighter_position.z + 20))) / 150.0; //0~1

        glPushMatrix();
        glColor3dv(color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
        //glMaterialfv(GL_FRONT, GL_SHININESS, shine1);
        glTranslatef(p.x, p.y, p.z);
        glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
        glutSolidTeapot(3);
        glPopMatrix();

    }
};
vector <coin> Coin;

int life=3;
float left_time = 40.0;
int score = 0;

#define MAX_CHAR       128
void drawString(const char* str) {

    static int isFirstCall = 1;
    static GLuint lists;

    if (isFirstCall) { // 如果是第一次調用，執行初始化

        // 爲每一個ASCII字符產生一個顯示列表

        isFirstCall = 0;

        // 申請MAX_CHAR個連續的顯示列表編號

        lists = glGenLists(MAX_CHAR);

        // 把每個字符的繪製命令都裝到對應的顯示列表中

        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);

    }

    // 調用每個字符對應的顯示列表，繪製每個字符

    for (; *str != '\0'; ++str)
        glCallList(lists + *str);

}



void selectFont(int size, int charset, const char* face) {

    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,

        charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,

        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);

    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);

    DeleteObject(hOldFont);

}

void info_str() {

    //draw time String
    string str1 = "time:" + to_string(int(left_time));
    const char* cstr1 = str1.c_str();
    selectFont(48, ANSI_CHARSET, "consolas");
    glPushMatrix();
    glTranslatef(10.0, 23.0, flighter_position.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glScaled(30.0, 30.0, 30.0);
    glRasterPos2f(0.0f, 0.0f);
    drawString(cstr1);
    glPopMatrix();
    //END draw time String    

    //draw life String
    string str2 = "HP:";
    for (int i = 0; i < life; i++) {
        str2 += "+";
    }
    str2 += " " + to_string(life);
    const char* cstr2 = str2.c_str();
    selectFont(48, ANSI_CHARSET, "consolas");
    glPushMatrix();
    glTranslatef(-25.0, 23.0, flighter_position.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glScaled(30.0, 30.0, 30.0);
    glRasterPos2f(0.0f, 0.0f);
    drawString(cstr2);
    glPopMatrix();
    //END draw life String     

    //draw score String
    string str3 = "score:";
    str3 += " " + to_string(score);
    const char* cstr3 = str3.c_str();
    selectFont(48, ANSI_CHARSET, "consolas");
    glPushMatrix();
    glTranslatef(-10.0, -25.0, flighter_position.z);
    glColor3f(1.0f, 1.0f, 0.0f);
    glScaled(30.0, 30.0, 30.0);
    glRasterPos2f(0.0f, 0.0f);
    drawString(cstr3);
    glPopMatrix();
    //END draw score String 

    
}

bool collision_detection(Point A, Point B) {
    double distance1= abs(sqrt(pow(A.x - B.x, 2.0) + pow(A.y - B.y, 2.0) + pow(A.z - B.z, 2.0)));
    if (distance1 <= 8.0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}
/**********************************************************
 *
 * SUBROUTINE init(void)
 *
 * Used to initialize OpenGL and to setup our world
 *
 *********************************************************/

void init(void)
{
    srand(time(NULL));

    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black

    // Viewport transformation
    glViewport(0, 0, screen_width, screen_height);

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    //gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,5.0f,10000.0f); // We define the "viewing volume"
    //glOrtho(-20.0, 20.0, -20.0, 20.0, -100.0, 100.0);
    //glFrustum(-20.0, 20.0, -20.0, 20.0, abs(flighter_position.z), abs(flighter_position.z)+150);


    flighter_position.x = 0.0;
    flighter_position.y = 0.0;
    //flighter_position.z = -20.0;
    flighter_position.z = -50.0;


    //Lights initialization and activation
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    //glEnable (GL_LIGHT1);
    //glEnable (GL_LIGHTING);

    //Materials initialization and activation
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    //Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)

    //Objects loading
    ObjLoad("fighter1.3ds", "skull.bmp");

    srand(time(NULL));
}



/**********************************************************
 *
 * SUBROUTINE keyboard(unsigned char p_key, int p_x, int p_y)
 *
 * Used to handle the keyboard input (ASCII Characters)
 *
 * Input parameters: p_key = ascii code of the key pressed
 *					 p_x = not used, keeped to maintain compatibility with glut function
 *                   p_y = not used, keeped to maintain compatibility with glut function
 *
 *********************************************************/

void keyboard(unsigned char p_key, int p_x, int p_y)
{

    switch (p_key)
    {
    case 'w': case 'W':
        LR = 0;
        UD = 1;
        flighter_position.y += 1.0;
        break;
    case 's': case 'S':
        LR = 0;
        UD = -1;
        flighter_position.y -= 1.0;
        break;        
    case 'a': case 'A':
        LR = -1;
        UD = 0;
        flighter_position.x-=1.0;        
        break;
    case 'd': case 'D':
        LR = 1;
        UD = 0;
        flighter_position.x += 1.0;            
        break;
    case 'z': case 'Z':        
        LR = 0;
        UD = 0;
        flighter_speed += 0.5;
        break;
    case 'x': case 'X':
        LR = 0;
        UD = 0;
        flighter_speed -= 0.5;
        if (flighter_speed <= 0.7) {
            flighter_speed = 0.7;
        }
        break;
    }

    if (flighter_position.x >= 20.0) {
        flighter_position.x = 20.0;
    }
    if (flighter_position.x <= -20.0) {
        flighter_position.x = -20.0;
    }
    if (flighter_position.y >= 20.0) {
        flighter_position.y = 20.0;
    }
    if (flighter_position.y <= -20.0) {
        flighter_position.y = -20.0;
    }
    glutPostRedisplay();
}



/**********************************************************
 *
 * SUBROUTINE display(void)
 *
 * This is our main rendering subroutine, called each frame
 *
 *********************************************************/

void display(void)
{         

    int j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
    glMatrixMode(GL_MODELVIEW | GL_PROJECTION); // Modeling transformation
    glLoadIdentity(); // Initialize the model matrix as identity 
    
    glFrustum(-20.0, 20.0, -20.0, 20.0, 50.0, 1000.0);
    gluLookAt(0.0, 5.0, flighter_position.z  +70.0,
        0.0, 0.0, flighter_position.z ,
        0.0,100.0, flighter_position.z  + 50.0);

    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);

    info_str();    

    
    
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    //draw & collision Stone
    for (int i = 0; i < Stone.size(); i++) {
        Stone[i].draw();
        if (collision_detection(flighter_position, Stone[i].p)) {
            life--;

            glDisable (GL_LIGHT1);
            glDisable (GL_LIGHTING);

            glPushMatrix();
            glLineWidth(10);
            glColor3d(1.0, 0.0, 0.0);
            glTranslatef(Stone[i].p.x, Stone[i].p.y, Stone[i].p.z);
            glutWireCube(10);
            glPopMatrix();

            glEnable(GL_LIGHT1);
            glEnable(GL_LIGHTING);

            Stone[i].p.z = flighter_position.z + 1000.0;
        }
    }
    //END draw & collision Stone

    //draw & collision Coin
    for (int i = 0; i < Coin.size(); i++) {
        Coin[i].draw();
        if (collision_detection(flighter_position, Coin[i].p)) {
            score++;

            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHTING);

            glPushMatrix();
            glLineWidth(10);
            glColor3d(1.0, 1.0, 0.0);
            glTranslatef(Coin[i].p.x, Coin[i].p.y, Coin[i].p.z);
            glutWireCube(10);
            glPopMatrix();

            glEnable(GL_LIGHT1);
            glEnable(GL_LIGHTING);

            Coin[i].p.z = flighter_position.z+1000.0;
        }
    }
    //END draw & collision Coin

    //glDisable(GL_LIGHT1);
    //glDisable(GL_LIGHTING);

    //draw flighter
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(flighter_position.x, flighter_position.y, flighter_position.z);
    if (UD == 1) {
        glRotatef(10.0, 1.0, 0.0, 0.0);
    }
    else if (UD == -1) {
        glRotatef(-5.0, 1.0, 0.0, 0.0);
    }
    else {
        glRotatef(0.0, 1.0, 0.0, 0.0);
    }
    glRotatef(0.0, 0.0, 1.0, 0.0);
    if (LR == -1) {
        glRotatef(15, 0.0, 0.0, 1.0);
    }
    else if (LR == 1) {
        glRotatef(-15, 0.0, 0.0, 1.0);
    }
    else {
        glRotatef(0.0, 0.0, 0.0, 1.0);
    }
    glScalef(1, 1, 1);

    if (object.id_texture != -1)
    {
        glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 
        glEnable(GL_TEXTURE_2D); // Texture mapping ON
    }
    else
        glDisable(GL_TEXTURE_2D); // Texture mapping OFF
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glutWireSphere(5, 30, 30);
    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (j = 0; j < object.polygons_qty; j++)
    {
        //----------------- FIRST VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f(object.normal[object.polygon[j].a].x,
            object.normal[object.polygon[j].a].y,
            object.normal[object.polygon[j].a].z);
        // Texture coordinates of the first vertex
        glTexCoord2f(object.mapcoord[object.polygon[j].a].u,
            object.mapcoord[object.polygon[j].a].v);
        // Coordinates of the first vertex
        glVertex3f(object.vertex[object.polygon[j].a].x,
            object.vertex[object.polygon[j].a].y,
            object.vertex[object.polygon[j].a].z);

        //----------------- SECOND VERTEX -----------------
        //Normal coordinates of the second vertex
        glNormal3f(object.normal[object.polygon[j].b].x,
            object.normal[object.polygon[j].b].y,
            object.normal[object.polygon[j].b].z);
        // Texture coordinates of the second vertex
        glTexCoord2f(object.mapcoord[object.polygon[j].b].u,
            object.mapcoord[object.polygon[j].b].v);
        // Coordinates of the second vertex
        glVertex3f(object.vertex[object.polygon[j].b].x,
            object.vertex[object.polygon[j].b].y,
            object.vertex[object.polygon[j].b].z);

        //----------------- THIRD VERTEX -----------------
        //Normal coordinates of the third vertex
        glNormal3f(object.normal[object.polygon[j].c].x,
            object.normal[object.polygon[j].c].y,
            object.normal[object.polygon[j].c].z);
        // Texture coordinates of the third vertex
        glTexCoord2f(object.mapcoord[object.polygon[j].c].u,
            object.mapcoord[object.polygon[j].c].v);
        // Coordinates of the Third vertex
        glVertex3f(object.vertex[object.polygon[j].c].x,
            object.vertex[object.polygon[j].c].y,
            object.vertex[object.polygon[j].c].z);
    }
    glEnd();
    glPopMatrix();
    //END draw flighter
    
    

    glFlush(); // This force the execution of OpenGL commands
    glutSwapBuffers(); // In double buffered mode we invert the positions of the visible buffer and the writing buffer
}


void add_stone_cube(int value) {
    static Point p_ston = flighter_position;
    if (abs(flighter_position.z - p_ston.z) > 10.0) {
        for (int i = 0; i < 5; i++) {
            stone a;
            Stone.push_back(a);
        }
        p_ston = flighter_position;
    }

    static Point p_coin = flighter_position;
    if (abs(flighter_position.z - p_coin.z) > 101.0) {
        for (int i = 0; i < 1; i++) {
            coin a;
            Coin.push_back(a);
        }
        p_coin = flighter_position;
    }

    glutPostRedisplay();

    glutTimerFunc(200, add_stone_cube, 1);
}

void flighter_forward(int value) {
    flighter_position.z -= flighter_speed;

    glutPostRedisplay();

    glutTimerFunc(5, flighter_forward, 1);
}

void timer(int value) {
    static int flag = 0;
    static int  life_copy = 0, score_copy = 0;
    static float left_time_copy = 0.0;
    if (left_time > 0) {
        left_time-=0.1;
    }
    
    if (flag == 1) {

        string str1 = "Remaining time: " + to_string(int(left_time_copy))
            + "\nHP: " + to_string(life_copy)
            + "\nScore: " + to_string(score_copy)
            + "\n\nConfirm and start again.";
        const char* cstr1 = str1.c_str();
        MessageBox(NULL, cstr1, "Game Over!", MB_OK);
        left_time = 40;
        life = 3;
        score = 0;
        flag = 0;
        flighter_speed = 3.0;
    }
    if (life <= 0 || left_time <= 0) {
        flag = 1;
        left_time_copy = left_time; life_copy = life; score_copy = score;
    }
    glutTimerFunc(100, timer, 1);
}

/**********************************************************
 *
 * The main routine
 *
 *********************************************************/

int main(int argc, char** argv)
{
    // We use the GLUT utility to initialize the window, to handle the input and to interact with the windows system
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("1112914宋冠穎");
    glutDisplayFunc(display);

    glutTimerFunc(200, add_stone_cube, 1);
    glutTimerFunc(5, flighter_forward, 1);
    glutTimerFunc(100, timer, 1);
    glutKeyboardFunc(keyboard);
    
    init();
    glutMainLoop();

    return(0);
}
