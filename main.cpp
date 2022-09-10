//BENEMERITA UNIVERSIDAD AUTONOMA DE PUEBLA
//FACULTAD DE CIENCIAS DE LA COMPUTACION
//GRAFICACION - JESUS HUERTA AGUILAR

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <math.h>
#include <cmath>  //para operar con las funciones trigonometricas

GLfloat sizep;
///GLOBALES
int i,j,k;
int band = 0;

//DEFAULT VALORES
int ax = -20,ay =-20;  //<--- PIVOTE!!
int bx = 20, by = -20;
int cx = 0, cy = 20;
int TP1 = 0, TP2 = 0;
int scalex = 1; //SIN CAMBIOS
int scaley = 1;
float finaltheta = 0;
//DIFERENCIALES
int DIFtx = 0;
int DIFty = 0;
float DIFsx = 0;
float DIFsy = 0;
float DIFtheta = 0;
//VALORES DE MATRIZ 3X3
const int FIL = 3;
const int COL = 3;

void init(void){
    glColor3f(0.0, 1.0, 0.0);
    glPointSize(1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250, 250, -250, 250);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
///RESERVA MEMORIA
//Reservamos memoria dinamica, ya que asi se pasa por referencia los elementos de la matriz
float** reservarMemoria(size_t FIL, size_t COL) {
    float **matriz = new float *[FIL];

    for (size_t f(0); f < FIL; ++f) {
        matriz[f] = new float[COL];
    }
    return matriz;
}

//BRESENHAM
void Bresenham(int x0, int y0, int x1, int y1){
    int x, y, dx, dy, p, incE, incNE, stepx, stepy;
    dx = (x1 - x0);
    dy = (y1 - y0);
    /* determinar que punto usar para empezar, cual para terminar */
    if (dy < 0) {
        dy = -dy; stepy = -1;
      }
    else
      stepy = 1;
    if (dx < 0) {
        dx = -dx; stepx = -1;
      }
    else
      stepx = 1;
    x = x0;
    y = y0;

    glBegin(GL_POINTS);
        glVertex2i(x0,y0);
    glEnd();
     /* se cicla hasta llegar al extremo de la línea */
      if(dx>dy){
        p = 2*dy - dx;
        incE = 2*dy;
        incNE = 2*(dy-dx);
        while (x != x1){
          x = x + stepx;
          if (p < 0){
            p = p + incE;
          }
          else {
            y = y + stepy;
            p = p + incNE;
          }
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();
        }
      }
      else{
        p = 2*dx - dy;
        incE = 2*dx;
        incNE = 2*(dx-dy);
        while (y != y1){
          y = y + stepy;
          if (p < 0){
            p = p + incE;
          }
          else {
            x = x + stepx;
            p = p + incNE;
          }
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();
        }
      }
 }
//MULTIPLICA MATRICES 3X3
void multMatriz(float **A,float B[3][3]){
    float C[FIL][COL];
    for (int i = 0;i < FIL;i++){
        for (int j = 0;j < COL;j++){
            C[i][j]=0;
            for (k = 0;k < FIL;k++){
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    for(int i = 0; i < FIL;i++){
        for(int j = 0;j < COL;j++){
            A[i][j] = C[i][j];
        }
    }
}
//MULTIPLICA MATRICES PUNTOS
void multMatrizPoints(int p1,int p2,float **A,float **B){
    int points[3][1] = {{p1},
                        {p2},
                        {1}};
    float C[3][1];

    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 1;j++){
            C[i][j]=0;
            for (int k = 0;k < 3;k++){
                C[i][j] += A[i][k] * points[k][j];
            }
        }
    }
    for(int i = 0; i < 3;i++){
        B[i][0] = trunc(C[i][0]);
    }
}

//TRASLACION
void traslation(float tx,float ty,float **in){
    for(int i = 0; i < FIL; i++){
        for(int j = 0;j < FIL;j++){
            if(i == j){
                in[i][j] = 1;
            }
            else{
                in[i][j] = 0;
            }
        }
    }
    in[2][0] = -1*tx;
    in[2][1] = -1*ty;
}
//ROTACION
void rotation(float theta,float **in){
    float rota[FIL][COL] = {{cos(theta),-1*sin(theta), 0 },
                            {sin(theta),  cos(theta) , 0 },
                            {     0    ,      0      , 1 }};
    multMatriz(in,rota);
}
//ESCALAMIENTO
void scaling(float sx,float sy,float **in){
    float scal[FIL][COL] = {{ sx , 0  , 0 },
                            { 0  , sy , 0 },
                            { 0  , 0  , 1 }};
    multMatriz(in,scal);
}
//TRASLACION INVERSA (REGRESA POSICION ORIGINAL)
void traslationInversa(float tx,float ty,float **in){
    float trasInv[FIL][COL] = {{ 1 , 0 ,tx},
                               { 0 , 1 ,ty},
                               { 0 , 0 , 1}};
    multMatriz(in,trasInv);
}
//CREACION MATRIZ DE MODELADO
void modeladoCreation(float tx,float ty,float sx,float sy,float theta,float **modelado){
    //MATRIZ DE TRASLACION
    float **tras = reservarMemoria(FIL,COL);
    //OP de matriz de modelado
    traslation(tx,ty,tras);
    rotation(theta,tras);
    scaling(sx,sy,tras);
    //T' de matriz de modelado
    traslationInversa(tx,ty,tras);
    for(int i = 0; i < FIL;i++){
        for(int j = 0;j < COL;j++){
            modelado[i][j] = tras[i][j];
        }
    }
}
//DIBUJAR POLIGONO
void draw(float Dtx,float Dty,float Dsx,float Dsy,float Dtheta){
    float **points = reservarMemoria(3,1);
    float **MModelado = reservarMemoria(FIL,COL);
    modeladoCreation(ax+Dtx,ay+Dty,scalex+Dsx,scaley+Dsy,finaltheta+Dtheta,MModelado);
    multMatrizPoints(ax,ay,MModelado,points);
    TP1 = points[0][0];
    TP2 = points[1][0];
    multMatrizPoints(bx,by,MModelado,points);
    //Bresenham
    Bresenham(TP1,TP2,points[0][0],points[1][0]);
    TP1 = points[0][0];
    TP2 = points[1][0];
    multMatrizPoints(cx,cy,MModelado,points);
    //Bresenham
    Bresenham(TP1,TP2,points[0][0],points[1][0]);
    TP1 = points[0][0];
    TP2 = points[1][0];
    multMatrizPoints(ax,ay,MModelado,points);
    //Bresenham
    Bresenham(TP1,TP2,points[0][0],points[1][0]);
}
//Dibuja grafica
void dibujaGrafica(){
    glClear(GL_COLOR_BUFFER_BIT);  //borra la ventana de visualizacion
    //Dibujado
    draw(DIFtx,DIFty,DIFsx,DIFsy,DIFtheta); //Dibuja poligono

    ///Activamos las variables control
    //VC Traslacion
    if(band == 0){
        DIFtx++;
        if(DIFtx == 270)
            band = 1;
    }
    if(band == 1){
        DIFtx--;
        if(DIFtx == -270)
            band = 0;
    }

    //VC Rotacion
    if(DIFtheta < 6)
        DIFtheta+=0.2;
    else
        DIFtheta = 0;

    glFlush();
    Sleep(50);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //Posicion de la ventana
    glutInitWindowPosition(400,100);
    //se establece el ancho y la altura de la ventana de visualizacion
    glutInitWindowSize(500,500);
    //se crea la ventana de visualizacion
    glutCreateWindow("Poligono Animado");
    init();
    glutDisplayFunc(dibujaGrafica);
    //Animation
    glutIdleFunc(dibujaGrafica);
    glutMainLoop();
    return 0;
}
