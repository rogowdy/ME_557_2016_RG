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


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;




static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
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







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];


unsigned int vboID[4];

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
unsigned int createTriangleStripModel(void)
{

	// use the vertex array object vaoID[0] for this model representation
	float* vertices = new float[126];
	float *colors = new float[126];

	vertices[0] = 0; vertices[1] = 2; vertices[2] = 2;
	colors[0] = 0.5; colors[1] = 0.5; colors[2] = 0.5;

	vertices[3] = 0; vertices[4] = 2; vertices[5] = 1;
	colors[3] = 0; colors[4] = 1; colors[5] = 0.5;

	vertices[6] = 1; vertices[7] = 2; vertices[8] = 2;
	colors[6] = 0.5; colors[7] = 0; colors[8] = 1;

	vertices[9] = 1; vertices[10] = 2; vertices[11] = 1;
	colors[9] = 0; colors[10] = 0.5; colors[11] = 0.5;

	vertices[12] = 3; vertices[13] = 2; vertices[14] = 1;
	colors[12] = 0.5; colors[13] = 1; colors[14] = 0;

	vertices[15] = 1; vertices[16] = 2; vertices[17] = 0;
	colors[15] = 1; colors[16] = 0.5; colors[17] = 0;

	vertices[18] = 3; vertices[19] = 2; vertices[20] = 0;
	colors[18] = 0.5; colors[19] = 1; colors[20] = 0.5;

	vertices[21] = 1; vertices[22] = 2; vertices[23] = 0;
	colors[21] = 0; colors[22] = 0.5; colors[23] = 0.5;

	vertices[24] = 1; vertices[25] = 2; vertices[26] = 1;
	colors[24] = 0; colors[25] = 0.25; colors[26] = 0.75;

	vertices[27] = 1; vertices[28] = 0; vertices[29] = 0;
	colors[27] = 1; colors[28] = 0.25; colors[29] = 0.5;

	vertices[30] = 1; vertices[31] = 0; vertices[32] = 1;
	colors[30] = 1; colors[31] = 0.5; colors[32] = 0.25;

	vertices[33] = 0; vertices[34] = 0; vertices[35] = 0;
	colors[33] = 0.5; colors[34] = 1; colors[35] = 0;

	vertices[36] = 0; vertices[37] = 0; vertices[38] = 1;
	colors[36] = 0.75; colors[37] = 0.5; colors[38] = 0;

	vertices[39] = 0; vertices[40] = 2; vertices[41] = 0;
	colors[39] = 0.1; colors[40] = 0.8; colors[41] = 0.5;

	vertices[42] = 0; vertices[43] = 2; vertices[44] = 1;
	colors[42] = 0.5; colors[43] = 1; colors[44] = 0.25;

	vertices[45] = 0; vertices[46] = 2; vertices[47] = 1;
	colors[45] = 1; colors[46] = 0.5; colors[47] = 0.25;

	vertices[48] = 0; vertices[49] = 3; vertices[50] = 1;
	colors[48] = 0.6; colors[49] = 0.3; colors[50] = 0.7;

	vertices[51] = 0; vertices[52] = 2; vertices[53] = 2;
	colors[51] = 0; colors[52] = 1; colors[53] = 0.8;

	vertices[54] = 0; vertices[55] = 3; vertices[56] = 2;
	colors[54] = 0.8; colors[55] = 0.5; colors[56] = 0.75;

	vertices[57] = 1; vertices[58] = 2; vertices[59] = 2;
	colors[57] = 1; colors[58] = 0.5; colors[59] = 0.5;

	vertices[60] = 1; vertices[61] = 3; vertices[62] = 2;
	colors[60] = 0; colors[61] = 0.25; colors[62] = .75;

	vertices[63] = 3; vertices[64] = 2; vertices[65] = 1;
	colors[63] = 0.6; colors[64] = 0.5; colors[65] = .9;

	vertices[66] = 3; vertices[67] = 3; vertices[68] = 1;
	colors[66] = 1; colors[67] = 0; colors[68] = 0.5;

	vertices[69] = 3; vertices[70] = 2; vertices[71] = 0;
	colors[69] = 0.25; colors[70] = 0.5; colors[71] = 0.25;

	vertices[72] = 3; vertices[73] = 3; vertices[74] = 0;
	colors[72] = 0; colors[73] = 0; colors[74] = 0.4;

	vertices[75] = 0; vertices[76] = 2; vertices[77] = 0;
	colors[75] = 1; colors[76] = 0.6; colors[77] = 0.9;

	vertices[78] = 0; vertices[79] = 3; vertices[80] = 0;
	colors[78] = 0; colors[79] = 0.5; colors[80] = 1;

	vertices[81] = 0; vertices[82] = 2; vertices[83] = 1;
	colors[81] = 1; colors[82] = 0.25; colors[83] = 0.5;

	vertices[84] = 0; vertices[85] = 3; vertices[86] = 1;
	colors[84] = 0; colors[85] = 0.25; colors[86] = 0.5;

	vertices[87] = 0; vertices[88] = 3; vertices[89] = 2;
	colors[87] = 0.25; colors[88] = 1; colors[89] = 0.5;

	vertices[90] = 0; vertices[91] = 3; vertices[92] = 0;
	colors[90] = 0.75; colors[91] = 1; colors[92] = 0.5;

	vertices[93] = 1; vertices[94] = 3; vertices[95] = 2;
	colors[93] = 0; colors[94] = 0.5; colors[95] = .25;

	vertices[96] = 3; vertices[97] = 3; vertices[98] = 0;
	colors[96] = 0.5; colors[97] = 0.5; colors[98] = 0.5;

	vertices[99] = 3; vertices[100] = 3; vertices[101] = 1;
	colors[99] = 1; colors[100] = 0.7; colors[101] = 0.5;

	vertices[102] = 0; vertices[103] = 0; vertices[104] = 1;
	colors[102] = 0.9; colors[103] = 0.5; colors[104] =0.6;

	vertices[105] = 1; vertices[106] = 0; vertices[107] = 1;
	colors[105] = 1; colors[106] = 0.25; colors[107] = 0.5;

	vertices[108] = 0; vertices[109] = 2; vertices[110] = 1;
	colors[108] = 0.25; colors[109] = 0.5; colors[110] = 1;

	vertices[111] = 1; vertices[112] = 2; vertices[113] = 1;
	colors[111] = 0.5; colors[112] = 0.5; colors[113] = 0.5;

	vertices[114] = 0; vertices[115] = 0; vertices[116] = 0;
	colors[114] = 0; colors[115] = 0.5; colors[116] = 0.25;

	vertices[117] = 1; vertices[118] = 0; vertices[119] = 0;
	colors[117] = 1; colors[118] = 1; colors[119] = 0.5;

	vertices[120] =0; vertices[121] = 2; vertices[122] = 0;
	colors[120] = 0.5; colors[121] = 0.25; colors[122] = 0.6;

	vertices[123] = 0; vertices[124] = 0; vertices[125] = 1;
	colors[123] = 0.5; colors[124] = 0.4; colors[125] = 0.9;

	glGenVertexArrays(1, &vaoID[0]);
	glBindVertexArray(vaoID[0]);

	glGenBuffers(2, vboID);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 126 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0);

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, 126 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes point
	glEnableVertexAttribArray(1);
    return 1;

}

/*!
 ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
 */
unsigned int createPolygonModel(void)
{

	// use the vertex array object vaoID[1] for this model representation
	
	float* vertices = new float[147];
	float *colors = new float[147];

	vertices[0] = 0; vertices[1] = 2; vertices[2] = 1;
	colors[0] = 0.5; colors[1] = 1.0; colors[2] = 0.25;

	vertices[3] = 0; vertices[4] = 2; vertices[5] = 2;
	colors[3] = 0.25; colors[4] = 0.75; colors[5] = 1.0;

	vertices[6] = 1; vertices[7] = 2; vertices[8] = 2;
	colors[6] = 0.9; colors[7] = 0.25; colors[8] = 0.6;

	vertices[9] = 3; vertices[10] = 2; vertices[11] = 1;
	colors[9] = 0.1; colors[10] = 1.0; colors[11] = 0.5;

	vertices[12] = 1; vertices[13] = 2; vertices[14] = 0;
	colors[12] = 0.5; colors[13] = 1.0; colors[14] = 0.25;

	vertices[15] = 1; vertices[16] = 2; vertices[17] = 1;
	colors[15] = 0.6; colors[16] = 0.8; colors[17] = 0;

	vertices[18] = 3; vertices[19] = 2; vertices[20] = 1;
	colors[18] = 0.4; colors[19] = 0.3; colors[20] = 1;

	vertices[21] = 3; vertices[22] = 2; vertices[23] = 0;
	colors[21] = 0; colors[22] = 0.5; colors[23] = 1.0;

	vertices[24] = 0; vertices[25] = 3; vertices[26] = 2;
	colors[24] = 0.25; colors[25] = 0; colors[26] = 1.0;

	vertices[27] = 0; vertices[28] = 3; vertices[29] = 0;
	colors[27] = 0.75; colors[28] = .25; colors[29] = 0.9;

	vertices[30] = 3; vertices[31] = 3; vertices[32] = 0;
	colors[30] = 1; colors[31] = 0; colors[32] = 0.6;

	vertices[33] = 3; vertices[34] = 3; vertices[35] = 1;
	colors[33] = 0.25; colors[34] = 0.6; colors[35] = 1;

	vertices[36] = 1; vertices[37] = 3; vertices[38] = 2;
	colors[36] = 0.4; colors[37] = 0.8; colors[38] = 0.6;

	vertices[39] = 0; vertices[40] = 3; vertices[41] = 2;
	colors[39] = 0.75; colors[40] = 1.0; colors[41] = 0;

	vertices[42] = 0; vertices[43] = 3; vertices[44] = 0;
	colors[42] = 0.4; colors[43] = 1.0; colors[44] = 0;

	vertices[45] = 0; vertices[46] = 0; vertices[47] = 0;
	colors[45] = 0.5; colors[46] = 1.0; colors[47] = 0.75;

	vertices[48] = 0; vertices[49] = 0; vertices[50] = 1;
	colors[48] = 0; colors[49] = 0.2; colors[50] = 0.8;

	vertices[51] = 0; vertices[52] = 2; vertices[53] = 1;
	colors[51] = 0.5; colors[52] = 1.0; colors[53] = 0.25;

	vertices[54] = 0; vertices[55] = 2; vertices[56] = 2;
	colors[54] = 0.4; colors[55] = 0.8; colors[56] = 0;

	vertices[57] = 0; vertices[58] = 0; vertices[59] = 0;
	colors[57] = 0.8; colors[58] = 0.5; colors[59] = 1.0;

	vertices[60] = 1; vertices[61] = 0; vertices[62] = 0;
	colors[60] = 0; colors[61] = 0.5; colors[62] = 0.25;

	vertices[63] = 1; vertices[64] = 2; vertices[65] = 0;
	colors[63] = 0.75; colors[64] = 0.4; colors[65] = 0.6;

	vertices[66] = 3; vertices[67] = 2; vertices[68] = 0;
	colors[66] = 0; colors[67] = 1.0; colors[68] = 0.25;

	vertices[69] = 3; vertices[70] = 3; vertices[71] = 0;
	colors[69] = 1; colors[70] = 0.25; colors[71] = 0.5;

	vertices[72] = 0; vertices[73] = 3; vertices[74] = 0;
	colors[72] = 1.0; colors[73] = 0.75; colors[74] = 0.6;

	vertices[75] = 3; vertices[76] = 2; vertices[77] = 0;
	colors[75] = 0; colors[76] = 0.8; colors[77] = 0.25;

	vertices[78] = 3; vertices[79] = 2; vertices[80] = 1;
	colors[78] = 0.1; colors[79] = 0.8; colors[80] = 0.5;

	vertices[81] = 3; vertices[82] = 3; vertices[83] = 1;
	colors[81] = 0; colors[82] = 0.5; colors[83] = 1.0;

	vertices[84] = 3; vertices[85] = 3; vertices[86] = 0;
	colors[84] = 0.6; colors[85] = 0.4; colors[86] = 0.25;

	vertices[87] = 3; vertices[88] = 2; vertices[89] = 1;
	colors[87] = 1.0; colors[88] = 0.5; colors[89] = 0.25;

	vertices[90] = 3; vertices[91] = 3; vertices[92] = 1;
	colors[90] = 0; colors[91] = 0.25; colors[92] = 0.8;

	vertices[93] = 1; vertices[94] = 3; vertices[95] = 2;
	colors[93] = 0.25; colors[94] = 0.4; colors[95] = 0.6;

	vertices[96] = 1; vertices[97] = 2; vertices[98] = 2;
	colors[96] = 0; colors[97] = 1.0; colors[98] = 1.0;

	vertices[99] = 0; vertices[100] = 2; vertices[101] = 2;
	colors[99] = 0.5; colors[100] = 0.25; colors[101] = 0.9;

	vertices[102] = 0; vertices[103] = 3; vertices[104] = 2;
	colors[102] = 0.1; colors[103] = 0.5; colors[104] = 0.4;

	vertices[105] = 1; vertices[106] = 3; vertices[107] = 2;
	colors[105] = 1.0; colors[106] = 0; colors[107] = 0.8;

	vertices[108] = 1; vertices[109] = 2; vertices[110] = 2;
	colors[108] = 0.6; colors[109] = 0.4; colors[110] = 0.25;

	vertices[111] = 0; vertices[112] = 0; vertices[113] = 1;
	colors[111] = 1.0; colors[112] = 0; colors[113] = 0.5;

	vertices[114] = 1; vertices[115] = 0; vertices[116] = 1;
	colors[114] = 0.4; colors[115] = 0.8; colors[116] = 1.0;

	vertices[117] = 1; vertices[118] = 2; vertices[119] = 1;
	colors[117] = 0; colors[118] = 0.4; colors[119] = 0.8;

	vertices[120] = 0; vertices[121] = 2; vertices[122] = 1;
	colors[120] = 0.5; colors[121] = 0.25; colors[122] = 0.75;

	vertices[123] = 1; vertices[124] = 0; vertices[125] = 1;
	colors[123] = 0.25; colors[124] = 0.4; colors[125] = 0.7;

	vertices[126] = 1; vertices[127] = 0; vertices[128] = 0;
	colors[126] = 0.6; colors[127] = 0.8; colors[128] = 1.0;

	vertices[129] = 1; vertices[130] = 2; vertices[131] = 0;
	colors[129] = 0; colors[130] = 0.3; colors[131] = 0.9;

	vertices[132] = 1; vertices[133] = 2; vertices[134] = 1;
	colors[132] = 1.0; colors[133] = 0; colors[134] = 0.5;

	vertices[135] = 0; vertices[136] = 0; vertices[137] = 0;
	colors[135] = 0.3; colors[136] = 0.7; colors[137] = 0.9;

	vertices[138] = 0; vertices[139] = 0; vertices[140] = 1;
	colors[138] = 0.8; colors[139] = 0.25; colors[140] = 0.5;

	vertices[141] = 1; vertices[142] = 0; vertices[143] = 1;
	colors[141] = 0; colors[142] = 0.1; colors[143] = 0.6;

	vertices[144] = 1; vertices[145] = 0; vertices[146] = 0;
	colors[144] = 0.8; colors[145] = 0.7; colors[146] = 0.25;

	glGenVertexArrays(1, &vaoID[1]);
	glBindVertexArray(vaoID[1]);

	glGenBuffers(2, &vboID[2]);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[3]);
	glBufferData(GL_ARRAY_BUFFER, 147 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes point
	glEnableVertexAttribArray(0); //Disable our Vertex Array Object

	//colors
	glBindBuffer(GL_ARRAY_BUFFER, vboID[4]);
	glBufferData(GL_ARRAY_BUFFER, 147 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes point
	glEnableVertexAttribArray(0); //Disable our Vertex Array Object

   
    return 1;
}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleStripModel(void)
{
	glUseProgram(program);
    // Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);


	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 21);
	glDrawArrays(GL_TRIANGLE_STRIP, 21, 24);
	glDrawArrays(GL_TRIANGLE_STRIP, 45, 42);
	glDrawArrays(GL_TRIANGLE_STRIP, 87, 15);
	glDrawArrays(GL_TRIANGLE_STRIP, 102, 12);
	glDrawArrays(GL_TRIANGLE_STRIP, 114, 12);
	


    // Unbind our Vertex Array Object
    glBindVertexArray(0);

}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderPolygonModel(void)
{

	glUseProgram(program);
	// Bind the buffer and switch it to an active buffer

	glBindVertexArray(vaoID[1]);
        
	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY POLYGONS YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles
    glDrawArrays(GL_POLYGON, 0 , 12);
	glDrawArrays(GL_POLYGON, 12, 12);
	glDrawArrays(GL_POLYGON, 24, 15);
	glDrawArrays(GL_POLYGON, 39, 18);
	glDrawArrays(GL_POLYGON, 57, 18);
	glDrawArrays(GL_POLYGON, 75, 12);
	glDrawArrays(GL_POLYGON, 87, 12);
	glDrawArrays(GL_POLYGON, 99, 12);
	glDrawArrays(GL_POLYGON, 111, 12);
	glDrawArrays(GL_POLYGON, 123, 12);
	glDrawArrays(GL_POLYGON, 135, 12);

	
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 This function creates the two models
 */
void setupScene(void) {
    
    createTriangleStripModel();
    renderTriangleStripModel();
	
	createPolygonModel();
	renderPolygonModel();
    
}




int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
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
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
	 //// The Shader Program ends here
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    


    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    

    
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
        

        // This moves the model to the right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        

		// This line renders your triangle strip model
        renderTriangleStripModel();
        

        // This moves the model to the left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
		// This line renders your Ppolygon model
        renderPolygonModel();
        
        
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

