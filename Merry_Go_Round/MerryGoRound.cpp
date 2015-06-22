/******************************************************************
*
* MerryGoRound.cpp
*
* Computer Graphics Proseminar SS 2015
* 
* Hartl Martin, Cvjetkovic Bojan and Ebster Johannes
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
#include "source/LoadShader.h"   /* Provides loading function for shader code */
#include "source/Transformation.h"
#include "source/Shape.h"
#include "source/Cylinder.h"
#include "source/Cube.h"
#include "source/Block.h"
#include "source/Model.h"
#include "source/OBJParser.h"
#include "source/Camera.h"
#include "source/CameraMode.h"
#include "source/Lightsource.h"
#include "source/Texture.h"

/* necessary because GLUT_KEY_something int codes overlap with wasd */
#define mARROW_LEFT -2
#define mARROW_UP -3
#define mARROW_RIGHT -4
#define mARROW_DOWN -5


using namespace std;

/*----------------------------------------------------------------*/
/* 0: whole housing
 * 1-4: independently moving objects
 * 5-6: walls */

/* Flag for starting/stopping animation */
GLboolean anim = GL_TRUE;

/* handlers to vertex buffer objects */
GLuint VBO[7];

/* handlers to color buffer objects */
GLuint CBO[7];

/* handlers to index buffer objects */
GLuint IBO[7];

/* handlers for normal buffers */
GLuint NBO[7];

/* handlers for texture coordinate buffers */
GLuint TCBO[7];

/* handlers to vertex buffer room_components*/
GLuint VBR[2];

/* handlers to color buffer room_components*/
GLuint CBR[2];

/* handlers to index buffer room_components*/
GLuint IBR[2];

/* handlers to normal buffer room_components*/
GLuint NBR[2];

/* handlers to texture coordinates for room components */
GLuint TCBR[2];

/* Indices to vertex attributes; in this case positon and color */ 
enum DataID {vPosition = 0, vColor = 1, vNormal = 2, vUV = 3};

/* Strings for loading and storing shader code */
static const char* VertexShaderString;
static const char* FragmentShaderString;

GLuint ShaderProgram;

Transformation ProjectionMatrix; /* Perspective projection matrix */
Transformation ModelMatrix[7]; /* Model matrix */
Transformation RoomMatrix[1];
Transformation IdentityMatrix;
Transformation InitialTransform;
Camera camera(glm::vec3(0,0,10));
enum CameraMode camera_mode = MANUAL;
int currentKey = -1;
int lastX = 0;
int lastY = 0;
long t = glutGet(GLUT_ELAPSED_TIME);
/* camera mode auto stuff */
double auto_speed;

/* on/off for ligthing */
int ky = 1;
int kx = 1;
int kc = 1;

/* displayable objects */
Shape **objects = new Shape*[6];

/* walls and floor */
Shape **room_components = new Shape*[1];

/* light sources **/
Lightsource **lights = new Lightsource*[2];
glm::vec3 hsv_light1;

/*variables for hsv manipulation*/
float h,s,v;
glm::vec3 newRgb;

Texture* crackles;

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
    glUniformMatrix4fv(ViewUniform, 1, GL_FALSE, glm::value_ptr(camera.viewMatrix()));

	/* light */
	GLint LightPos1Uniform = glGetUniformLocation(ShaderProgram, "lightPos1");
	if (LightPos1Uniform == -1){
		cerr << "Could not bind uniform lightPos1" << endl;
		exit(-1);
	}
	glUniform3fv(LightPos1Uniform, 1, glm::value_ptr(camera.viewMatrix() * glm::vec4(lights[0]->pos, 1.0)));
	
	GLint LightColor1Uniform = glGetUniformLocation(ShaderProgram, "lightColor1");
	if (LightColor1Uniform == -1){
		cerr << "Could not bind uniform lightColor1" << endl;
		exit(-1);
	}
	glUniform3fv(LightColor1Uniform, 1, glm::value_ptr(lights[0]->rgb));

	/* green moving light */
	GLint LightPos2Uniform = glGetUniformLocation(ShaderProgram, "lightPos2");
	if (LightPos2Uniform == -1){
		cerr << "Could not bind uniform lightPos2" << endl;
		exit(-1);
	}
	glUniform3fv(LightPos2Uniform, 1, glm::value_ptr(camera.viewMatrix() * glm::transpose(glm::make_mat4(ModelMatrix[6].matrix)) * glm::vec4(lights[1]->pos, 1)));
	GLint LightColor2Uniform = glGetUniformLocation(ShaderProgram, "lightColor2");
	if (LightColor2Uniform == -1){
		cerr << "Could not bind uniform lightColor2" << endl;
		exit(-1);
	}
	glUniform3fv(LightColor2Uniform, 1, glm::value_ptr(lights[1]->rgb));

    GLint kA = glGetUniformLocation(ShaderProgram, "kA");
    GLint kD = glGetUniformLocation(ShaderProgram, "kD");
    GLint kS = glGetUniformLocation(ShaderProgram, "kS");
    if (kA == -1 || kD == -1 || kS == -1)
    {
        cout << kA << kD << kS << endl;
        cerr << "Could not bind uniform light constants" << endl;
        exit(-1);
    }

    /* Activate first (and only) texture unit */
    glActiveTexture(GL_TEXTURE0);

    /* Bind current texture  */
    glBindTexture(GL_TEXTURE_2D, crackles->TextureID);

    /* Get texture uniform handle from fragment shader */
    GLuint TextureUniform  = glGetUniformLocation(ShaderProgram, "myTextureSampler");

    /* Set location of uniform sampler variable */
    glUniform1i(TextureUniform, 0);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vUV);

    for (int i = 0; i < 7; i++) {

        glUniform1f(kA, objects[i]->kA * ky);
        glUniform1f(kD, objects[i]->kD * kx);
        glUniform1f(kS, objects[i]->kS * kc);


        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, CBO[i]);
        glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, TCBO[i]);
        glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, NBO[i]);
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);

        GLint size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

        GLint RotationUniform = glGetUniformLocation(ShaderProgram, "ModelMatrix");
        if (RotationUniform == -1) {
            cerr << "Could not bind uniform ModelMatrix" << endl;
            exit(-1);
        }
        glUniformMatrix4fv(RotationUniform, 1, GL_TRUE, ModelMatrix[i].matrix);

        glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    }

    for (int i = 0; i < 2; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, VBR[i]);
        glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, CBR[i]);
        glVertexAttribPointer(vColor, 3, GL_FLOAT,GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, NBR[i]);
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, TCBR[i]);
        glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBR[i]);

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
    }
    /*RoomMatrix*/


    /* Disable attributes */
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
    glDisableVertexAttribArray(vNormal);
    glDisableVertexAttribArray(vUV);

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

void PassiveMouse(int x, int y) {
    static int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    static int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    static bool just_warped = false;

    /* needed because glutWarpPointer creates a callback to this function, will hang the event loop */
    if (just_warped) {
        just_warped = false;
        return;
    }

    int xOff = x - centerX;
    int yOff = y - centerY;
    camera.SetViewByMouse(xOff,yOff,camera_mode);
    glutWarpPointer(centerX, centerY);
    just_warped = true;
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
            cout << "SEMI-AUTO (fixed center)" << endl;
            camera.focusOnCenter();
            return;
        case '3':
            camera_mode = MANUAL;
            cout << "MANUAL (first person)" << endl;
            return;
    }

   if (camera_mode == AUTO) {
            switch(key){
				case 'w': auto_speed += 0.0001;
						  break;
				case 's': auto_speed -= 0.0001;
						  break;
			}
    }

    currentKey = key;
}

void KeyboardSpecialKeys(int key, int xp, int yp) {
    switch(key) {
        case GLUT_KEY_UP:
            currentKey = mARROW_UP;
            return;
        case GLUT_KEY_LEFT:
            currentKey = mARROW_LEFT;
            return;
        case GLUT_KEY_RIGHT:
            currentKey = mARROW_RIGHT;
            return;
        case GLUT_KEY_DOWN:
            currentKey = mARROW_DOWN;
            return;
    }
}

void KeyboardUp(unsigned char key, int x, int y) {
	switch(key){
		case 'y':
			ky = !ky;
			break;
		case 'x':
			kx = !kx;
			break;
		case 'c':
			kc = !kc;
			break;
	}
    currentKey = -1;
}

void KeyboardSpecialUp(int key, int x, int y) {
    currentKey = -1;
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
    int degrees_per_sec = 25;
    int units_per_sec = 6;// for translation
    float dt = (glutGet(GLUT_ELAPSED_TIME) -t)/1000.; // elapsed time in seconds
    t = glutGet(GLUT_ELAPSED_TIME);
    /* camera transformation */
    switch(currentKey) {
        case 'q':
        case 'Q':
            exit(0);
        case 'i':
			h += 10.0;
			if(h > 360.0){
				h -= 360.0;
			}
			newRgb = lights[1]->hsvToRgb(glm::vec3(h,s,v));
			lights[1]->rgb = newRgb;
			break;
		case 'k':
			h -= 10.0;
			if(h < 0.0){
				h += 360.0;
			}
			newRgb = lights[1]->hsvToRgb(glm::vec3(h,s,v));
			lights[1]->rgb = newRgb;
			break;
		case 'o':
			v += 0.05;
			newRgb = lights[1]->hsvToRgb(glm::vec3(h,s,v));
			lights[1]->rgb = newRgb;
			break;
		case 'l':
			v -= 0.05;
			newRgb = lights[1]->hsvToRgb(glm::vec3(h,s,v));
			lights[1]->rgb = newRgb;
			break;
    }
    switch(camera_mode) {
        case AUTO:
            break;
        case SEMI:
            switch (currentKey) {
                case 'w':
                    camera.forward(dt*units_per_sec);
                    break;

                case 'a':
                    camera.left(dt*units_per_sec);
                    camera.focusOnCenter();
                    break;

                case 's':
                    camera.back(dt*units_per_sec);
                    break;

                case 'd':
                    camera.right(dt*units_per_sec);
                    camera.focusOnCenter();
                    break;
                case mARROW_UP :
                    camera.rotateAroundCenter(-degrees_per_sec*dt,glm::vec3(1,0,0));
                    break;
                case mARROW_DOWN :
                    camera.rotateAroundCenter(degrees_per_sec*dt,glm::vec3(1,0,0));
                    break;
                case mARROW_LEFT :
                    camera.rotateAroundCenter(-degrees_per_sec*dt,glm::vec3(0,1,0));
                    break;
                case mARROW_RIGHT:
                    camera.rotateAroundCenter(degrees_per_sec*dt,glm::vec3(0,1,0));
                    break;
            }
            break;
        case MANUAL:
            switch (currentKey) {
                case 'w':
                    camera.forward(dt*units_per_sec);
                    break;

                case 'a':
                    camera.left(dt*units_per_sec);
                    break;

                case 's':
                    camera.back(dt*units_per_sec);
                    break;

                case 'd':
                    camera.right(dt*units_per_sec);
                    break;

                case mARROW_UP :
                    camera.rotate(degrees_per_sec*dt, camera.getU());
                    break;
                case mARROW_DOWN :
                    camera.rotate(-degrees_per_sec*dt, camera.getU());
                    break;
                case mARROW_LEFT :
                    camera.rotate(degrees_per_sec*dt,glm::vec3(0,1,0));
                    break;
                case mARROW_RIGHT:
                    camera.rotate(-degrees_per_sec*dt,glm::vec3(0,1,0));
                    break;
            }
    }

    /* rotation */
    if(anim){

		float angle = (glutGet(GLUT_ELAPSED_TIME) / 1000.0) * (180.0/M_PI);
		Transformation RotationMatrixAnim;
		Transformation TranslationMatrixAnim1;
		Transformation TranslationMatrixAnim2;
		Transformation TranslationMatrixAnim3;
		Transformation TranslationMatrixAnim4;
		Transformation LightRotationMatrix;
		
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
		
		LightRotationMatrix.rotateY(angle/2);

		/* Apply model rotation; finally move cube down */
		ModelMatrix[0].set_transformation(RotationMatrixAnim.matrix);
		ModelMatrix[1].set_transformation(TranslationMatrixAnim1.matrix);
		ModelMatrix[2].set_transformation(TranslationMatrixAnim2.matrix);
		ModelMatrix[3].set_transformation(TranslationMatrixAnim3.matrix);
		ModelMatrix[4].set_transformation(TranslationMatrixAnim4.matrix);
        ModelMatrix[5].set_transformation(glm::value_ptr(glm::mat4(1)));
        ModelMatrix[6].set_transformation(LightRotationMatrix.matrix);
	}

    /* Request redrawing of window content */
    glutPostRedisplay();
}

/******************************************************************
*
* initObjects
*
* initialize shapes and set light sources
*
*******************************************************************/

void initObjects() {
	/* create the basic shape */
    objects[0] = new Cylinder(500, 3., 0.2, 0., 0., 0., 0., 1., 0., 0.);
    objects[0]->add_shape(new Cylinder(500, 3., 0.2, 0., 2., 0., 1., 1., 0., 0.));
    objects[0]->add_shape(new Cylinder(50, 0.3, 1.8, 0., 0.2, 0., 0., 1., 1., 0.));

    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, 2., 0.2, 0., 0., 0., 0., 1.));
    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, -2., 0.2, 0., 0., 0., 0., 1.));
    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, 0., 0.2, 2., 0., 0., 0., 1.));
    objects[0]->add_shape(new Cylinder(20, 0.1, 1.8, 0., 0.2, -2., 0., 0., 0., 1.));

    obj_scene_data horse;
    /* Load horse OBJ model */
    char filename[] = "models/horse.obj";
    if(!parse_obj_scene(&horse, filename)){
        cerr << "Could not load file. Exiting." << endl;
        exit(EXIT_FAILURE);
	}

    objects[1] = new Model(horse, 2., 0.4, 0., .6, 0.545, 0.271, 0.075);
    objects[2] = new Model(horse, -2., 0.4, 0., .6, 0.545, 0.271, 0.075);
    objects[3] = new Model(horse, 0., 0.4, 2., .6, 0.545, 0.271, 0.075);
    objects[4] = new Model(horse, 0., 0.4, -2., .6, 0.545, 0.271, 0.075);

    for(int i = 1; i < 5; i++){
        objects[i]->kA = 0.1;
        objects[i]->kD = 0.3;
        objects[i]->kS = 0.5;
    }

    obj_scene_data sphere;
    /* Load sphere OBJ model */
    char filename2[] = "models/Sphere.obj";
    if(!parse_obj_scene(&sphere, filename2)){
        cerr << "Could not load file. Exiting." << endl;
        exit(EXIT_FAILURE);
    }

    objects[5] = new Model(sphere, 0., 2, 5., .1, 1., 1., 1.);
    objects[6] = new Model(sphere, 0., 1., 5., .1, 1., 1., 1.);

    room_components[0] = new Block(0.0, -1.25, 3.0, 0.1, 12.0, 14.0);
    room_components[1] = new Block(0.0, 3.75, -4.0, 10.0, 12.0, 0.1);
	/* set light sources */
	lights[0] = new Lightsource(0., 2, 5.0, 1.0, 1.0, 1.0); //fixed light
	lights[1] = new Lightsource(0., 1., 5., 0., 1., 0.); //light moving with the merry go round
	hsv_light1 = lights[1]->rgbToHsv(lights[1]->rgb);
	h = hsv_light1[0];
	s = hsv_light1[1];
	v = hsv_light1[2];
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
    for (int i = 0; i < 7; i++) {

        glGenBuffers(1, &VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i]->vertex_number * sizeof(GLfloat),objects[i]->vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &IBO[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * objects[i]->triangle_number * sizeof(GLshort), objects[i]->index_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &CBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, CBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i]->vertex_number * sizeof(GLfloat), objects[i]->color_buffer_data, GL_STATIC_DRAW);
        
        glGenBuffers(1, &NBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, NBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * objects[i]->vertex_number * sizeof(GLfloat), objects[i]->normal_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &TCBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, TCBO[i]);
        glBufferData(GL_ARRAY_BUFFER, 2 * objects[i]->vertex_number * sizeof(GLfloat), objects[i]->uv_buffer_data,
                     GL_STATIC_DRAW);
    }
    for (int i = 0; i < 2; i++) {

        /* initialize buffers for room components */
        glGenBuffers(1, &VBR[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBR[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * room_components[i]->vertex_number * sizeof(GLfloat),
                     room_components[i]->vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &IBR[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBR[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * room_components[i]->triangle_number * sizeof(GLfloat),
                     room_components[i]->index_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &CBR[i]);
        glBindBuffer(GL_ARRAY_BUFFER, CBR[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * room_components[i]->vertex_number * sizeof(GLfloat),
                     room_components[i]->color_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &NBR[i]);
        glBindBuffer(GL_ARRAY_BUFFER, NBR[i]);
        glBufferData(GL_ARRAY_BUFFER, 3 * room_components[i]->vertex_number * sizeof(GLfloat),
                     room_components[i]->normal_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &TCBR[i]);
        glBindBuffer(GL_ARRAY_BUFFER, TCBR[i]);
        glBufferData(GL_ARRAY_BUFFER, 2 * room_components[i]->vertex_number * sizeof(GLfloat),
                     room_components[i]->uv_buffer_data,
                     GL_STATIC_DRAW);

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

    initObjects();
    crackles = new Texture("data/crackles.bmp");

    /* Setup scene and rendering parameters */
    Initialize();

    /* Specify callback functions;enter GLUT event processing loop,
     * handing control over to GLUT */
    glutIdleFunc(OnIdle);
    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(PassiveMouse);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(KeyboardSpecialKeys);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialUpFunc(KeyboardSpecialUp);
    glutMainLoop();

    return EXIT_SUCCESS;
}
