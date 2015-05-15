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

#define GLM_FORCE_RADIANS

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/* Local includes */
#include "LoadShader.h"   /* Provides loading function for shader code */
#include "Transformation.h"
#include "Shape.h"
#include "Cylinder.h"
#include "Cube.h"
#include "Block.h"
#include "Model.h"
#include "OBJParser.h"
#include "Camera.h"
#include "CameraMode.h"



using namespace std;

/*----------------------------------------------------------------*/
/* 0: whole housing
 * 1-5: independently moving objects */

/* Flag for starting/stopping animation */
GLboolean anim = GL_TRUE;

/* Define handlers to vertex buffer objects */
GLuint VBO[5];

/* Define handlers to color buffer objects */
GLuint CBO[5];

/* Define handlers to index buffer objects */
GLuint IBO[5];

/*Define handlers to vertex buffer room_components*/
GLuint VBR;

/*Define handlers to color buffer room_components*/
GLuint CBR;

/*Define handlers to index buffer room_components*/
GLuint IBR;

/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vColor = 1}; 

/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

Transformation ProjectionMatrix; /* Perspective projection matrix */
Transformation ViewMatrix; /* Camera view matrix */ 
Transformation ModelMatrix[5]; /* Model matrix */
Transformation RoomMatrix[1];
Transformation IdentityMatrix;
Transformation InitialTransform;
Camera camera(glm::vec3(0,0,10));
enum CameraMode camera_mode = MANUAL;

/* camera mode auto stuff */
double auto_speed;

double height = 1;

/* displayable objects */
Shape **objects = new Shape*[5];

/* walls and floor */
Shape **room_components = new Shape*[0];

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
    glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    for (int i = 0; i < 5; i++) {

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, CBO[i]);
        glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);

        GLint size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

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
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBR);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, CBR);
    glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBR);

    GLint size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
    GLint RoomUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
	if (RoomUniform == -1){
		cerr << "Could not bind uniform ModelMatrix" << endl;
		exit(-1);
	}
	glUniformMatrix4fv(RoomUniform, 1, GL_FALSE, RoomMatrix[0].matrix); 

    /* Issue draw command, using indexed triangle list */
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);   

    /* Swap between front and back buffer */ 
    glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y)
{

    switch(button) {
        case 3:
            camera.forward(0.1);
            break;
        case 4:
            camera.back(0.1);
            break;
    }
}


/******************************************************************
*
* Keyboard
*
* Function to be called on key press in window; set by
* glutKeyboardFunc(); x and y specify mouse position on keypress;
* not used in this example 
*
*******************************************************************/

void Keyboard(unsigned char key, int x, int y)   
{
    /* switch camera mode */
    switch(key) {
        case '0':
            camera = Camera(glm::vec3(0,0,10));
            return;
        case '1':
            camera_mode = AUTO;
            auto_speed = 0.;
            cout << "AUTO" << endl;
            return;
        case '2':
            camera_mode = SEMI;
            cout << "SEMI-AUTO (fixed focus)" << endl;
            return;
        case '3':
            camera_mode = MANUAL;
            cout << "MANUAL (first person)" << endl;
            return;
    }

    switch(camera_mode) {
        case AUTO:
            switch(key){
				case 'w': auto_speed += 0.0001;
						  break;
				case 's': auto_speed -= 0.0001;
						  break;
			}
            break;
        case SEMI:
            switch( key )
            {
                case 'w':
                    camera.forward(0.1);
                    break;

                case 'a':
                    camera.left(0.1);
                    break;

                case 's':
                    camera.back(0.1);
                    break;

                case 'd':
                    camera.right(0.1);
                    break;

                case 'q': case 'Q':
                    exit(0);
                    break;
            }
            break;
        case MANUAL:
            switch( key )
            {
                case 'w':
                    camera.forward(0.1);
                    break;

                case 'a':
                    camera.left(0.1);
                    break;

                case 's':
                    camera.back(0.1);
                    break;

                case 'd':
                    camera.right(0.1);
                    break;

                case 'q': case 'Q':
                    exit(0);
                    break;
            }

            break;
    }


    glutPostRedisplay();
}

void KeyboardSpecialKeys(int key, int xp, int yp) {
    switch(camera_mode) {
        case AUTO:
            // nothing
            break;
        case SEMI:
            switch (key) {
                case GLUT_KEY_UP :
                    camera.rotateAroundCenter(-1,camera.u);
                    break;
                case GLUT_KEY_DOWN :
                    camera.rotateAroundCenter(1,camera.u);
                    break;
                case GLUT_KEY_LEFT :
                    camera.rotateAroundCenter(-1,camera.v);
                    break;
                case GLUT_KEY_RIGHT:
                    camera.rotateAroundCenter(1,camera.v);
                    break;
            }
            break;
        case MANUAL:
            switch (key) {
                case GLUT_KEY_UP :
                    camera.rotate(1,camera.u);
                    break;
                case GLUT_KEY_DOWN :
                    camera.rotate(-1,camera.u);
                    break;
                case GLUT_KEY_LEFT :
                    camera.rotate(1,camera.v);
                    break;
                case GLUT_KEY_RIGHT:
                    camera.rotate(-1,camera.v);
                    break;
            }
            break;
    }
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

    /* rotation */
    if(anim){

		float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI);
		Transformation RotationMatrixAnim;
		Transformation TranslationMatrixAnim1;
		Transformation TranslationMatrixAnim2;
		Transformation TranslationMatrixAnim3;
		Transformation TranslationMatrixAnim4;
		
		if(camera_mode == AUTO){
			camera.rotateAroundCenter(angle * auto_speed, glm::vec3(0,1,0));
		}

		RotationMatrixAnim.rotateY(angle);
		RotationMatrixAnim.multiply(InitialTransform.matrix);

		TranslationMatrixAnim1.translate(0.0, cos(angle/50)/5., 0.0);
		TranslationMatrixAnim1.multiply(RotationMatrixAnim.matrix);
		TranslationMatrixAnim1.translate(objects[1]->center_x, objects[1]->center_y, objects[1]->center_z);
		TranslationMatrixAnim1.rotateY(5 * angle);
		TranslationMatrixAnim1.translate(-objects[1]->center_x, -objects[1]->center_y, -objects[1]->center_z);

		TranslationMatrixAnim2.translate(0.0, cos(angle/50.)/5., 0.0);
		TranslationMatrixAnim2.multiply(RotationMatrixAnim.matrix);
		TranslationMatrixAnim2.translate(objects[2]->center_x, objects[2]->center_y, objects[2]->center_z);
		TranslationMatrixAnim2.rotateY(5 * angle);
		TranslationMatrixAnim2.translate(-objects[2]->center_x, -objects[2]->center_y, -objects[2]->center_z);

		TranslationMatrixAnim3.translate(0.0, sin(angle/50.)/5., 0.0);
		TranslationMatrixAnim3.multiply(RotationMatrixAnim.matrix);
		TranslationMatrixAnim3.translate(objects[3]->center_x, objects[3]->center_y, objects[3]->center_z);
		TranslationMatrixAnim3.rotateY(-5 * angle);
		TranslationMatrixAnim3.translate(-objects[3]->center_x, -objects[3]->center_y, -objects[3]->center_z);

		TranslationMatrixAnim4.translate(0.0, sin(angle/50.)/5., 0.0);
		TranslationMatrixAnim4.multiply(RotationMatrixAnim.matrix);
		TranslationMatrixAnim4.translate(objects[4]->center_x, objects[4]->center_y, objects[4]->center_z);
		TranslationMatrixAnim4.rotateY(-5 * angle);
		TranslationMatrixAnim4.translate(-objects[4]->center_x, -objects[4]->center_y, -objects[4]->center_z);

		/* Apply model rotation; finally move cube down */
		ModelMatrix[0].set_transformation(RotationMatrixAnim.matrix);
		ModelMatrix[1].set_transformation(TranslationMatrixAnim1.matrix);
		ModelMatrix[2].set_transformation(TranslationMatrixAnim2.matrix);
		ModelMatrix[3].set_transformation(TranslationMatrixAnim3.matrix);
		ModelMatrix[4].set_transformation(TranslationMatrixAnim4.matrix);
	}

    /* Request redrawing of window content */
    glutPostRedisplay();
}

/******************************************************************
*
* initObjects
*
* initialize shapes
*
*******************************************************************/

void initObjects() {
	/* create the basic shape */
    objects[0] = new Cylinder(200, 3., 0.2, 0., 0., 0., 0.);
    objects[0]->add_shape(new Cylinder(200, 3., 0.2, 0., 2., 0., 1.));
    objects[0]->add_shape(new Cylinder(50, 0.3, 1.8, 0., 0.2, 0., 0.));

    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, 2., 0.2, 0., 0.));
    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, -2., 0.2, 0., 0.));
    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, 0., 0.2, 2., 0.));
    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, 0., 0.2, -2., 0.));
  
    
    /* create the 4 cubes */
    obj_scene_data horse;
    /* Load horse OBJ model */
    char filename[] = "models/horse.obj";
    if(!parse_obj_scene(&horse, filename)){
        cerr << "Could not load file. Exiting." << endl;
        exit(EXIT_FAILURE);
	}
    objects[1] = new Model(horse, 2., 0.4, 0., .6);
    objects[2] = new Model(horse, -2., 0.4, 0., .6);
    objects[3] = new Model(horse, 0., 0.4, 2., .6);
    objects[4] = new Model(horse, 0., 0.4, -2., .6);
    
    room_components[0] = new Block(0.0, 3.0, -4.0, 10.0, 12.0, 0.1);
    room_components[0]->add_shape(new Block(0.0, -2.0, 3.0, 0.1, 12.0, 14.0));
}

/******************************************************************
*
* SetupDataBuffers
*
* Create buffer objects and load data into buffers
*
*******************************************************************/

void SetupDataBuffers() {

    /* initialize buffers for objects */
    for (int i = 0; i < 5; i++) {

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
    
    /* initialize buffers for room components */
    glGenBuffers(1, &VBR);
    glBindBuffer(GL_ARRAY_BUFFER, VBR);
    glBufferData(GL_ARRAY_BUFFER, 3 * room_components[0]->vertex_number * sizeof(GLfloat), room_components[0]->vertex_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &IBR);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBR);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * room_components[0]->triangle_number * sizeof(GLfloat), room_components[0]->index_buffer_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &CBR);
    glBindBuffer(GL_ARRAY_BUFFER, CBR);
    glBufferData(GL_ARRAY_BUFFER, 3 * room_components[0]->vertex_number * sizeof(GLfloat), room_components[0]->color_buffer_data, GL_STATIC_DRAW);
    
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
        exit(-1);
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
    VertexShaderString = LoadShader("shaders/vertexshader.vs");
    FragmentShaderString = LoadShader("shaders/fragmentshader.fs");

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
    
    /* Set background (clear) color to black */
    glClearColor(0, 0, 0, 0);

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
    float aspect = 1.0;
    float nearPlane = 1.0; 
    float farPlane = 50.0;
    float temp[16];
    SetPerspectiveMatrix(fovy, aspect, nearPlane, farPlane, temp);
	ProjectionMatrix.set_transformation(temp);

    ViewMatrix.translate(0,0,-10);

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
	/* initialize objects */
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
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(KeyboardSpecialKeys);
    glutMainLoop();

    return EXIT_SUCCESS;
}
