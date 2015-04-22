/******************************************************************
*
* MerryGoRound.cpp
*
* Computer Graphics Proseminar SS 2015
* 
*
*
*******************************************************************/


/* Standard includes */
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>


/* Local includes */
#include "LoadShader.h"   /* Provides loading function for shader code */
#include "Transformation.h"
#include "Shape.h"
#include "Cylinder.h"

using namespace std;

/*----------------------------------------------------------------*/
/* 0: whole housing
 * 1-5: independently moving objects */

/* Define handlers to vertex buffer objects */
GLuint VBO[6];

/* Define handlers to color buffer objects */
GLuint CBO[6];

/* Define handlers to index buffer objects */
GLuint IBO[6];


/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vColor = 1}; 

/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

Transformation ProjectionMatrix; /* Perspective projection matrix */
Transformation ViewMatrix; /* Camera view matrix */ 
Transformation ModelMatrix[6]; /* Model matrix */ 
Transformation InitialTransform;

double height = 1;

/* displayable objects */
Form **objects = new Form*[6];

/*----------------------------------------------------------------*/


/******************************************************************
*
* Display
*
* This function is called when the content of the window needs to be
* drawn/redrawn. It has been specified through 'glutDisplayFunc()';
* Enable vertex attributes, create binding between C program and 
* attribute name in shader
*
*******************************************************************/

void Display()
{
    /* Clear window; color specified in 'Initialize()' */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < 6; i++) {
		if(objects[i] == NULL)
			continue;

        glEnableVertexAttribArray(vPosition);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(vColor);
        glBindBuffer(GL_ARRAY_BUFFER, CBO[i]);
        glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);

        GLint size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

        /* Associate program with shader matrices */
        GLint projectionUniform = glGetUniformLocation(ShaderProgram, "ProjectionMatrix");
        if (projectionUniform == -1)
        {
            cerr << "Could not bind uniform ProjectionMatrix" << endl;
            exit(-1);
        }
        glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, ProjectionMatrix.matrix);
    
		GLint ViewUniform = glGetUniformLocation(ShaderProgram, "ViewMatrix");
		if (ViewUniform == -1) 
		{
			cerr << "Could not bind uniform ViewMatrix" << endl;
			exit(-1);
		}
		glUniformMatrix4fv(ViewUniform, 1, GL_TRUE, ViewMatrix.matrix);
   
		GLint RotationUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
		if (RotationUniform == -1) 
		{
			cerr << "Could not bind uniform ModelMatrix" << endl;
			exit(-1);
		}
		glUniformMatrix4fv(RotationUniform, 1, GL_TRUE, ModelMatrix[i].matrix); 

        /* Issue draw command, using indexed triangle list */
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    }

    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);   

    /* Swap between front and back buffer */ 
    glutSwapBuffers();
}


/******************************************************************
*
* OnIdle
*
* 
*
*******************************************************************/

void OnIdle()
{
	// TODO here we have to set all the ModelMatrices for the independent objects
    // remember to multiply them with ModelMatrix[0] so that they rotate with the housing
    float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI); 
    Transformation RotationMatrixAnim;

    /* Time dependent rotation */
    RotationMatrixAnim.rotateY(angle);
    RotationMatrixAnim.multiply(InitialTransform.matrix);

    /* Apply model rotation; finally move cube down */
    ModelMatrix[0].set_transformation(RotationMatrixAnim.matrix);

    /* Request redrawing of window content */
    glutPostRedisplay();
}


/******************************************************************
*
* SetupDataBuffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void initObjects() {
    objects[0] = new Cylinder(100, 3., 0.2, 0.);
    /* TODO add all the stuff to the object[0] (housing), define independent objects */
}

void SetupDataBuffers()
{

    /* initialize buffers for objects */
    for (int i = 0; i < 6; i++) {
		if(objects[i] == NULL)
			continue;
        glGenBuffers(1, &VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i]->vertex_number * sizeof(GLfloat),objects[i]->vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &IBO[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * objects[i]->triangle_number * sizeof(GLshort), objects[i]->index_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &CBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, CBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i]->vertex_number * sizeof(GLfloat), objects[i]->color_buffer_data, GL_STATIC_DRAW);
    }


}


/******************************************************************
*
* AddShader
*
* This function creates and adds individual shaders
*
*******************************************************************/

void AddShader(GLuint ShaderProgram, const char* ShaderCode, GLenum ShaderType)
{
    /* Create shader object */
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) 
    {
        cerr << "Error creating shader type " << ShaderType << endl;
        exit(0);
    }

    /* Associate shader source code string with shader object */
    glShaderSource(ShaderObj, 1, &ShaderCode, NULL);

    GLint success = 0;
    GLchar InfoLog[1024];

    /* Compile shader source code */
    glCompileShader(ShaderObj);
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        cerr << "Error compiling shader type " << ShaderType << ": '" << InfoLog << "'" << endl;
        exit(1);
    }

    /* Associate shader with shader program */
    glAttachShader(ShaderProgram, ShaderObj);
}


/******************************************************************
*
* CreateShaderProgram
*
* This function creates the shader program; vertex and fragment
* shaders are loaded and linked into program; final shader program
* is put into the rendering pipeline 
*
*******************************************************************/

void CreateShaderProgram()
{
    /* Allocate shader object */
    ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) 
    {
        cerr << "Error creating shader program" << endl;
        exit(1);
    }

    /* Load shader code from file */
    VertexShaderString = LoadShader("vertexshader.vs");
    FragmentShaderString = LoadShader("fragmentshader.fs");

    /* Separately add vertex and fragment shader to program */
    AddShader(ShaderProgram, VertexShaderString, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, FragmentShaderString, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024];

    /* Link shader code into executable shader program */
    glLinkProgram(ShaderProgram);

    /* Check results of linking step */
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

    if (Success == 0)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        cerr << "Error linking shader program: '" << ErrorLog << "'" << endl;
        exit(1);
    }

    /* Check if shader program can be executed */ 
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);

    if (!Success) 
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        cerr << "Invalid shader program: '" << ErrorLog << "'" << endl;
        exit(1);
    }

    /* Put linked shader program into drawing pipeline */
    glUseProgram(ShaderProgram);
}

/******************************************************************
*
* Initialize
*
* This function is called to initialize rendering elements, setup
* vertex buffer objects, and to setup the vertex and fragment shader
*
*******************************************************************/

void Initialize(void)
{   
    
    /* Set background (clear) color to dark blue */ 
    glClearColor(0.0, 0.0, 0.4, 0.0);

	/* Setup Vertex array object */
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);	

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    

    /* Setup vertex, color, and index buffer objects */
    SetupDataBuffers();

    /* Setup shaders and shader program */
    CreateShaderProgram();

    /* Set projection transform */
    float fovy = 45.0;
    float aspect = 2.0;
    float nearPlane = 1.0; 
    float farPlane = 50.0;
    float temp[16];
    SetPerspectiveMatrix(fovy, aspect, nearPlane, farPlane, temp);
	ProjectionMatrix.set_transformation(temp);

    /* Set viewing transform */
    float camera_disp = -10.0;
    ViewMatrix.translate(0.0, 0.0, camera_disp);

    /* Initial transformation matrix */
	InitialTransform.translate(0, -sqrtf(sqrtf(2.0)), 0);
}


/******************************************************************
*
* main
*
* Main function to setup GLUT, GLEW, and enter rendering loop
*
*******************************************************************/

int main(int argc, char** argv)
{

    initObjects();

    /* Initialize GLUT; set double buffered window and RGBA color model */
    glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("CG Proseminar - Merry-Go-Round");

	/* Initialize GL extension wrangler */
	glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        cerr << "Error: '" << glewGetErrorString(res) << "'" << endl;
        return 1;
    }

    /* Setup scene and rendering parameters */
    Initialize();


    /* Specify callback functions;enter GLUT event processing loop, 
     * handing control over to GLUT */
    glutIdleFunc(OnIdle);
    glutDisplayFunc(Display);
    glutMainLoop();

    /* ISO C requires main to return int */
    return EXIT_SUCCESS;
}
