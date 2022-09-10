# Animated Polygon

## Repository: animated-polygon

### Description
This project, completed as part of the coursework at the Benemérita Universidad Autónoma de Puebla, focuses on animating a polygon using transformations such as translation, rotation, and scaling. The aim is to create an animated polygon by applying these transformations dynamically in the Code::Blocks IDE using OpenGL.

### Overview
This project utilizes various matrix operations to animate a polygon. The primary transformations include translation, rotation, and scaling. The Bresenham algorithm is employed to draw lines efficiently, ensuring minimal computational cost.

### Objectives
- Implement and animate a polygon using matrix transformations in OpenGL.
- Apply learned concepts to dynamically translate, rotate, and scale the polygon.
- Develop an understanding of matrix operations and their applications in computer graphics.

### Key Features
- **Initialization**: Set up the OpenGL environment and window properties.
- **Matrix Operations**: Implement functions for translation, rotation, and scaling.
- **Bresenham Algorithm**: Efficiently draw lines between points using integer calculations.
- **Animation Loop**: Continuously apply transformations to animate the polygon.

### Project Structure
The project includes the following main components:

#### Initialization
This function sets up the OpenGL environment, defining the color of the window and the projection parameters.

```cpp
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200, 200, -200, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
```

#### Memory Reservation

This function dynamically allocates memory for matrices.

```cpp
float** reservarMemoria(size_t FIL, size_t COL) {
    float **matriz = new float *[FIL];
    for (size_t f(0); f < FIL; ++f) {
        matriz[f] = new float[COL];
    }
    return matriz;
}
``` 

#### Matrix Multiplication

This function multiplies two 3x3 matrices.

```cpp
void multMatriz(float **A, float B[3][3]) {
    float C[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            C[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            A[i][j] = C[i][j];
        }
    }
}
```

#### Point Matrix Multiplication

This function multiplies a 3x3 matrix with a 3x1 matrix (point).

```cpp
void multMatrizPoints(int p1, int p2, float **A, float **B) {
    int points[3][1] = {{p1}, {p2}, {1}};
    float C[3][1];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 1; j++) {
            C[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                C[i][j] += A[i][k] * points[k][j];
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        B[i][0] = trunc(C[i][0]);
    }
}
```

#### Translation

This function creates a translation matrix.

```cpp
void traslation(float tx, float ty, float **in) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            in[i][j] = (i == j) ? 1 : 0;
        }
    }
    in[2][0] = -tx;
    in[2][1] = -ty;
}
```

#### Rotation

This function creates a rotation matrix and multiplies it with the input matrix.

```cpp
void rotation(float theta, float **in) {
    float rota[3][3] = {{cos(theta), -sin(theta), 0}, {sin(theta), cos(theta), 0}, {0, 0, 1}};
    multMatriz(in, rota);
}
```

#### Scaling

This function creates a scaling matrix and multiplies it with the input matrix.

```cpp
void scaling(float sx, float sy, float **in) {
    float scal[3][3] = {{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}};
    multMatriz(in, scal);
}
```

#### Inverse Translation

This function creates an inverse translation matrix and multiplies it with the input matrix.

```cpp
void traslationInversa(float tx, float ty, float **in) {
    float trasInv[3][3] = {{1, 0, tx}, {0, 1, ty}, {0, 0, 1}};
    multMatriz(in, trasInv);
}
``` 

#### Modeling Matrix Creation

This function creates a modeling matrix by combining translation, rotation, and scaling transformations.

```cpp
void modeladoCreation(float tx, float ty, float sx, float sy, float theta, float **modelado) {
    float **tras = reservarMemoria(3, 3);
    traslation(tx, ty, tras);
    rotation(theta, tras);
    scaling(sx, sy, tras);
    traslationInversa(tx, ty, tras);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            modelado[i][j] = tras[i][j];
        }
    }
}
```

#### Drawing Polygon

This function draws a polygon by applying the modeling matrix to its points and using the Bresenham algorithm for line drawing.

```cpp
void draw(float Dtx, float Dty, float Dsx, float Dsy, float Dtheta) {
    float **points = reservarMemoria(3, 1);
    float **MModelado = reservarMemoria(3, 3);
    modeladoCreation(ax + Dtx, ay + Dty, scalex + Dsx, scaley + Dsy, finaltheta + Dtheta, MModelado);
    multMatrizPoints(ax, ay, MModelado, points);
    TP1 = points[0][0];
    TP2 = points[1][0];
    multMatrizPoints(bx, by, MModelado, points);
    Bresenham(TP1, TP2, points[0][0], points[1][0]);
    TP1 = points[0][0];
    TP2 = points[1][0];
    multMatrizPoints(cx, cy, MModelado, points);
    Bresenham(TP1, TP2, points[0][0], points[1][0]);
    TP1 = points[0][0];
    TP2 = points[1][0];
    multMatrizPoints(ax, ay, MModelado, points);
    Bresenham(TP1, TP2, points[0][0], points[1][0]);
}
```

#### Animation Function

This function animates the polygon by applying dynamic transformations.

```cpp
void dibujaGrafica() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the display window
    draw(DIFtx, DIFty, DIFsx, DIFsy, DIFtheta);  // Draw polygon
    if (band == 0) {
        DIFtx++;
        if (DIFtx == 270) band = 1;
    }
    if (band == 1) {
        DIFtx--;
        if (DIFtx == -270) band = 0;
    }
    if (DIFtheta < 6) DIFtheta += 0.2;
    else DIFtheta = 0;
    glFlush();
    Sleep(50);
}
``` 

#### Main Function

This function initializes the graphics window and starts the main loop.

```cpp
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Polygon Animation");
    init();
    glutDisplayFunc(dibujaGrafica);
    glutMainLoop();

    return 0;
}
```

### Execution

The project initializes a graphical window and animates a polygon by continuously applying translation, rotation, and scaling transformations. The animation is smooth and visually demonstrates the effects of these transformations.

<p align= "center">
    <img src="https://github.com/KPlanisphere/binary-tree-operations/assets/60454942/69d22864-42e9-436e-87ba-2688fe4cb0be" style="width: 70%; height: auto;">
</p>
