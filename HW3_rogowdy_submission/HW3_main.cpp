//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "make_sphere.h"

#include "Sphere3D.h"
#include "GLAppearance.h"

#include <cmath>

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;

static const string vs_string_GLSphere_410 =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrixBox;                                    \n"
"uniform mat4 viewMatrixBox;                                           \n"
"uniform mat4 inverseViewMatrix;                                        \n"
"uniform mat4 modelMatrixBox;                                          \n"
"uniform vec3 diffuse_color;                                          \n"
"uniform vec3 ambient_color;                                          \n"
"uniform vec3 specular_color;                                          \n"
"uniform vec4 light_position;                                          \n"
"uniform float diffuse_intensity;                                          \n"
"uniform float ambient_intensity;                                          \n"
"uniform float specular_intensity;                                          \n"
"uniform float shininess;                                          \n"
"in vec3 in_Position;                                               \n"
"in vec3 in_Normal;                                                  \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"                                                                 \n"
"                                                                 \n"
"                                                                 \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    vec3 normal = normalize(in_Normal);                                                                        \n"
"    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));             \n"
"    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);                             \n"
"                                                                                                               \n"
"    vec4 surface_to_light =   normalize( light_position -  surfacePostion );                         \n"
"                                                                                                               \n"
"    // Diffuse color                                                                                           \n"
"    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);                           \n"
"    vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;                         \n"
"                                                                                                               \n"
"    // Ambient color                                                                                           \n"
"    vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;                                          \n"
"                                                                                                               \n"
"    // Specular color                                                                                          \n"
"    vec3 incidenceVector = -surface_to_light.xyz;                                                              \n"
"    vec3 reflectionVector = reflect(incidenceVector, normal);                                   \n"
"    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);             \n"
"    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);                                     \n"
"    float cosAngle = max( dot(surfaceToCamera, reflectionVector), 0.0);                                        \n"
"    float specular_coefficient = pow(cosAngle, shininess);                                                     \n"
"    vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;                      \n"
"                                                                                                               \n"
"    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);               \n"
"                                                                                                               \n"
"    pass_Color = vec3(out_diffuse_color + out_ambient_color + out_specular_color);                             \n"
"}                                                                                                              \n";


static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Normal;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Normal;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string_GLSphere_410 =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";


/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix

// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;

//NUMBER OF VERTICES FOR A SPHERE
int number_vertices_red = -1;
int number_vertices_blue = -1;
int number_vertices_green = -1;
int number_vertices_yellow = -1;


// USE THESE vertex array objects to define your objects
unsigned int vaoID[4];

unsigned int vboID[12];


/*!
 Code for the Triangle strip models
 */
unsigned int createRedSphere(void)
{
    // use the vertex array object vaoID[0] for this model representation
	// Set the center and the radius
	float center[3] = { 0.0, 0.0, 0.0 };
	float radius = 5.0;


	// Set the number of segment elements and rows of the sphere. 
	// This will affect the number of triangles in the model
	int segments = 10;
	int rows = 10;


	// Compute the number of array elements necessary to store all coordinates.
	int  N = NumVec3ArrayElements(rows, segments);


	// create memory for the vertices and the normals. 
	float* vertices = new float[N];
	float* normals = new float[N];
	float* colors = new float[N];

	// copy the data to the vectors
	// Create the points and the normal vectors
	number_vertices_red = Make_Sphere(rows, segments, center, radius, vertices, normals);
    
    glGenVertexArrays(1, &vaoID[0]); // Create our Vertex Array Object
    glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
	int locPos = glGetAttribLocation(program, "in_Position");
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); // Disable our Vertex Array Object
    
	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locNorm = glGetAttribLocation(program, "in_Normal");
	glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locNorm); // Enable the second vertex attribute array
    
    
	//Color
    glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
	int logColor = glGetAttribLocation(program, "in_Color");
    glVertexAttribPointer((GLuint)logColor, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(logColor); // Enable the second vertex attribute array
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
     
    return 1;
}
unsigned int createBlueSphere(void)
{
    // use the vertex array object vaoID[1] for this model representation    
	// Set the center and the radius
	float center[3] = { 0.0, 0.0, 0.0 };
	float radius = 5.0;


	// Set the number of segment elements and rows of the sphere. 
	// This will affect the number of triangles in the model
	int segments = 10;
	int rows = 10;


	// Compute the number of array elements necessary to store all coordinates.
	int  N = NumVec3ArrayElements(rows, segments);


	// create memory for the vertices and the normals. 
	float* vertices = new float[N];
	float* normals = new float[N];
	float* colors = new float[N];


	// Create the points and the normal vectors
	number_vertices_blue = Make_Sphere(rows, segments, center, radius, vertices, normals);

    glGenVertexArrays(1, &vaoID[1]); // Create our Vertex Array Object
    glBindVertexArray(vaoID[1]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, &vboID[3]); // Generate our Vertex Buffer Object
    
	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[3]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locPos = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locPos); // Disable our Vertex Array Object

								  //Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[4]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locNorm = glGetAttribLocation(program, "in_Normal");
	glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locNorm); // Enable the second vertex attribute array


								  //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[5]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int logColor = glGetAttribLocation(program, "in_Color");
	glVertexAttribPointer((GLuint)logColor, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(logColor); // Enable the second vertex attribute array
    glBindVertexArray(0);

    return 1;
}
unsigned int createGreenSphere(void)
{
	// use the vertex array object vaoID[2] for this model representation
	// Set the center and the radius
	float center[3] = { 0.0, 0.0, 0.0 };
	float radius = 5.0;


	// Set the number of segment elements and rows of the sphere. 
	// This will affect the number of triangles in the model
	int segments = 10;
	int rows = 10;


	// Compute the number of array elements necessary to store all coordinates.
	int  N = NumVec3ArrayElements(rows, segments);


	// create memory for the vertices and the normals. 
	float* vertices = new float[N];
	float* normals = new float[N];
	float* colors = new float[N];


	// Create the points and the normal vectors
	number_vertices_green = Make_Sphere(rows, segments, center, radius, vertices, normals);

	glGenVertexArrays(1, &vaoID[2]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[2]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, &vboID[6]); // Generate our Vertex Buffer Object

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[6]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locPos = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locPos); // Disable our Vertex Array Object

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[7]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locNorm = glGetAttribLocation(program, "in_Normal");
	glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locNorm); // Enable the second vertex attribute array


	//Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[8]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int logColor = glGetAttribLocation(program, "in_Color");
	glVertexAttribPointer((GLuint)logColor, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(logColor); // Enable the second vertex attribute array
	glBindVertexArray(0);

	return 1;
}
unsigned int createYellowSphere(void)
{
	// use the vertex array object vaoID[3] for this model representation
	// Set the center and the radius
	float center[3] = { 0.0, 0.0, 0.0 };
	float radius = 5.0;


	// Set the number of segment elements and rows of the sphere. 
	// This will affect the number of triangles in the model
	int segments = 10;
	int rows = 10;


	// Compute the number of array elements necessary to store all coordinates.
	int  N = NumVec3ArrayElements(rows, segments);


	// create memory for the vertices and the normals. 
	float* vertices = new float[N];
	float* normals = new float[N];
	float* colors = new float[N];


	// Create the points and the normal vectors
	number_vertices_yellow = Make_Sphere(rows, segments, center, radius, vertices, normals);

	glGenVertexArrays(1, &vaoID[3]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[3]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, &vboID[9]); // Generate our Vertex Buffer Object
								// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[9]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locPos = glGetAttribLocation(program, "in_Position");
	glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locPos); // Disable our Vertex Array Object

								  //Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[10]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int locNorm = glGetAttribLocation(program, "in_Normal");
	glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(locNorm); // Enable the second vertex attribute array


								  //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[11]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	int logColor = glGetAttribLocation(program, "in_Color");
	glVertexAttribPointer((GLuint)logColor, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(logColor); // Enable the second vertex attribute array
	
	glBindVertexArray(0);




	return 1;
}

/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderRedSphere(void)
{
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[0]);
        
    glDrawArrays(GL_TRIANGLE_STRIP, 0, number_vertices_red );

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}
void renderBlueSphere(void)
{

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[1]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, number_vertices_blue);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}
void renderGreenSphere(void)
{

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[2]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, number_vertices_green);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}
void renderYellowSphere(void)
{

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[3]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, number_vertices_yellow);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the four spheres
 */
void setupScene(void) {
    
    createRedSphere();
    createBlueSphere();
	createGreenSphere();
	createYellowSphere();
    
}


int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
  
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here

	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string_GLSphere_410;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string_GLSphere_410;
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
	 //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glBindAttribLocation(program, 0, "in_Position");
	glBindAttribLocation(program, 1, "in_Normal");
	glBindAttribLocation(program, 2, "in_Color");
       
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
                

		// Rendering the Red Sphere

		// The spotlight object
		GLSpotLightSource  light_redsphere;
		light_redsphere._lightPos = glm::vec4(0.0, 20.0, 0.0, 1.0);
		light_redsphere._ambient_intensity = 0.3;
		light_redsphere._specular_intensity = 8.0;
		light_redsphere._diffuse_intensity = 3.0;
		light_redsphere._attenuation_coeff = 0.02;
		light_redsphere._cone_angle = 12.0; // in degree
		light_redsphere._cone_direction = glm::vec3(-1.0, -1.0, 0.0); // this must be aligned with the object and light position.

		light_redsphere._ambientIdx = glGetUniformLocation(program, "ambient_intensity");
		light_redsphere._diffuseIdx = glGetUniformLocation(program, "diffuse_intensity");
		light_redsphere._specularIdx = glGetUniformLocation(program, "specular_intensity");

		glUniform1f(light_redsphere._ambientIdx, light_redsphere._ambient_intensity);
		glUniform1f(light_redsphere._diffuseIdx, light_redsphere._diffuse_intensity);
		glUniform1f(light_redsphere._specularIdx, light_redsphere._specular_intensity);

		glUniform4fv(light_redsphere._lightPosIdx, 1, &light_redsphere._lightPos[0]);

		// Create a material object
		GLMaterial material_redsphere;
		material_redsphere._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
		material_redsphere._ambient_material = glm::vec3(0.0, 0.0, 1.0);
		material_redsphere._specular_material = glm::vec3(1.0, 1.0, 1.0);
		material_redsphere._shininess = 12.0;

		material_redsphere._ambientColorPos = glGetUniformLocation(program, "ambient_Color");
		material_redsphere._diffuseColorPos = glGetUniformLocation(program, "diffuse_Color");
		material_redsphere._specularColorPos = glGetUniformLocation(program, "specular_Color");

		// Send the material to your shader program
		glUniform3fv(material_redsphere._ambientColorPos, 1, &material_redsphere._ambient_material[0]);
		glUniform3fv(material_redsphere._diffuseColorPos, 1, &material_redsphere._diffuse_material[0]);
		glUniform3fv(material_redsphere._specularColorPos, 1, &material_redsphere._specular_material[0]);
		glUniform3fv(material_redsphere._emissiveIdx, 1, &material_redsphere._emissive_material[0]);
		glUniform1f(material_redsphere._shininessIdx, material_redsphere._shininess);
		glUniform1f(material_redsphere._transparencyIdx, material_redsphere._transparency);
		
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		renderRedSphere();

		


		// Rendering the Blue Sphere

		// The spotlight object
		GLLightSource  light_bluesphere;
		light_bluesphere._lightPos = glm::vec4(20.0, 20.0, 0.0, 1.0);
		light_bluesphere._ambient_intensity = 0.3;
		light_bluesphere._specular_intensity = 8.0;
		light_bluesphere._diffuse_intensity = 3.0;

		light_bluesphere._ambientIdx = glGetUniformLocation(program, "ambient_intensity");
		light_bluesphere._diffuseIdx = glGetUniformLocation(program, "diffuse_intensity");


		glUniform1f(light_bluesphere._ambientIdx, light_bluesphere._ambient_intensity);
		glUniform1f(light_bluesphere._diffuseIdx, light_bluesphere._diffuse_intensity);
		glUniform1f(light_bluesphere._specularIdx, light_bluesphere._specular_intensity);

		glUniform4fv(light_bluesphere._lightPosIdx, 1, &light_bluesphere._lightPos[0]);

		// Create a material object
		GLMaterial material_bluesphere;
		material_bluesphere._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
		material_bluesphere._ambient_material = glm::vec3(0.0, 0.0, 1.0);
		material_bluesphere._shininess = 12.0;

		material_bluesphere._ambientColorPos = glGetUniformLocation(program, "ambient_Color");
		material_bluesphere._diffuseColorPos = glGetUniformLocation(program, "diffuse_Color");


		// Send the material to your shader program
		glUniform3fv(material_bluesphere._ambientColorPos, 1, &material_bluesphere._ambient_material[0]);
		glUniform3fv(material_bluesphere._diffuseColorPos, 1, &material_bluesphere._diffuse_material[0]);
		glUniform3fv(material_bluesphere._specularColorPos, 1, &material_bluesphere._specular_material[0]);
		glUniform3fv(material_bluesphere._emissiveIdx, 1, &material_bluesphere._emissive_material[0]);
		glUniform1f(material_bluesphere._shininessIdx, material_bluesphere._shininess);
		glUniform1f(material_bluesphere._transparencyIdx, material_bluesphere._transparency);
		
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		renderBlueSphere();

		
		
		// Rendering the Green Sphere

		// The spotlight object
		GLSpotLightSource  light_greensphere;
		light_greensphere._lightPos = glm::vec4(20.0, 20.0, 0.0, 1.0);
		light_greensphere._ambient_intensity = 0.3;
		light_greensphere._specular_intensity = 8.0;
		light_greensphere._diffuse_intensity = 3.0;
		light_greensphere._attenuation_coeff = 0.02;
		light_greensphere._cone_angle = 12.0; // in degree
		light_greensphere._cone_direction = glm::vec3(-1.0, -1.0, 0.0); // this must be aligned with the object and light position.

		light_greensphere._ambientIdx = glGetUniformLocation(program, "ambient_intensity");
		light_greensphere._diffuseIdx = glGetUniformLocation(program, "diffuse_intensity");
		light_greensphere._specularIdx = glGetUniformLocation(program, "specular_intensity");

		glUniform1f(light_greensphere._ambientIdx, light_greensphere._ambient_intensity);
		glUniform1f(light_greensphere._diffuseIdx, light_greensphere._diffuse_intensity);
		glUniform1f(light_greensphere._specularIdx, light_greensphere._specular_intensity);

		glUniform4fv(light_greensphere._lightPosIdx, 1, &light_greensphere._lightPos[0]);

		// Create a material object
		GLMaterial material_greensphere;
		material_greensphere._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
		material_greensphere._ambient_material = glm::vec3(0.0, 0.0, 1.0);
		material_greensphere._specular_material = glm::vec3(1.0, 1.0, 1.0);
		material_greensphere._shininess = 12.0;

		material_greensphere._ambientColorPos = glGetUniformLocation(program, "ambient_Color");
		material_greensphere._diffuseColorPos = glGetUniformLocation(program, "diffuse_Color");
		material_greensphere._specularColorPos = glGetUniformLocation(program, "specular_Color");

		// Send the material to your shader program
		glUniform3fv(material_greensphere._ambientColorPos, 1, &material_greensphere._ambient_material[0]);
		glUniform3fv(material_greensphere._diffuseColorPos, 1, &material_greensphere._diffuse_material[0]);
		glUniform3fv(material_greensphere._specularColorPos, 1, &material_greensphere._specular_material[0]);
		glUniform3fv(material_greensphere._emissiveIdx, 1, &material_greensphere._emissive_material[0]);
		glUniform1f(material_greensphere._shininessIdx, material_greensphere._shininess);
		glUniform1f(material_greensphere._transparencyIdx, material_greensphere._transparency);
		
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		renderGreenSphere();

		
		// Rendering the Yellow Sphere

		// The spotlight object
		GLSpotLightSource  light_yellowsphere;
		light_yellowsphere._lightPos = glm::vec4(20.0, 20.0, 0.0, 1.0);
		light_yellowsphere._ambient_intensity = 0.3;
		light_yellowsphere._specular_intensity = 8.0;
		light_yellowsphere._diffuse_intensity = 3.0;
		light_yellowsphere._attenuation_coeff = 0.02;
		light_yellowsphere._cone_angle = 12.0; // in degree
		light_yellowsphere._cone_direction = glm::vec3(-1.0, -1.0, 0.0); // this must be aligned with the object and light position.

		light_yellowsphere._ambientIdx = glGetUniformLocation(program, "ambient_intensity");
		light_yellowsphere._diffuseIdx = glGetUniformLocation(program, "diffuse_intensity");
		light_yellowsphere._specularIdx = glGetUniformLocation(program, "specular_intensity");

		glUniform1f(light_yellowsphere._ambientIdx, light_yellowsphere._ambient_intensity);
		glUniform1f(light_yellowsphere._diffuseIdx, light_yellowsphere._diffuse_intensity);
		glUniform1f(light_yellowsphere._specularIdx, light_yellowsphere._specular_intensity);

		glUniform4fv(light_yellowsphere._lightPosIdx, 1, &light_yellowsphere._lightPos[0]);

		// Create a material object
		GLMaterial material_yellowsphere;
		material_yellowsphere._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
		material_yellowsphere._ambient_material = glm::vec3(0.0, 0.0, 1.0);
		material_yellowsphere._specular_material = glm::vec3(1.0, 1.0, 1.0);
		material_yellowsphere._shininess = 12.0;

		material_yellowsphere._ambientColorPos = glGetUniformLocation(program, "ambient_Color");
		material_yellowsphere._diffuseColorPos = glGetUniformLocation(program, "diffuse_Color");
		material_yellowsphere._specularColorPos = glGetUniformLocation(program, "specular_Color");

		// Send the material to your shader program
		glUniform3fv(material_yellowsphere._ambientColorPos, 1, &material_yellowsphere._ambient_material[0]);
		glUniform3fv(material_yellowsphere._diffuseColorPos, 1, &material_yellowsphere._diffuse_material[0]);
		glUniform3fv(material_yellowsphere._specularColorPos, 1, &material_yellowsphere._specular_material[0]);
		glUniform3fv(material_yellowsphere._emissiveIdx, 1, &material_yellowsphere._emissive_material[0]);
		glUniform1f(material_yellowsphere._shininessIdx, material_yellowsphere._shininess);
		glUniform1f(material_yellowsphere._transparencyIdx, material_yellowsphere._transparency);
		
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		renderYellowSphere();
		


        // This moves the model to the left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, -0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
		// disable the shader program
        glUseProgram(0);


        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}
