//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
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
#include <glm/gtx/transform.hpp>

// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Plane3D.h"
#include "Texture.h"

#include "GLObjectObj.h"
#include "RayIntersectionTest.h"
#include "GLAppearance.h"
#include "Sphere3D.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;

GLObjectObj* loadedModel1 = new GLObjectObj("../data/teapot_t.obj");
GLObjectObj* mario = new GLObjectObj("../data/mario_64.obj");
GLObjectObj* mario_result = new GLObjectObj("../data/mario_64.obj");

//// INITIAL LOCATION OF MARIO
glm::mat4 mario_init_sizeup = glm::scale(glm::vec3(0.1, 0.1f, 0.1f));
glm::mat4 mario_init_translate = glm::translate(glm::vec3(0.0, 0.0, 0.0f));
glm::mat4 mario_init_rotate = glm::rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f));

//// NAVIGATION OF MARIO

// to transform mario
glm::mat4 g_tranform_mario = mario_init_sizeup * mario_init_translate * mario_init_rotate;
glm::mat4 g_tranform_mario_result;
glm::mat4 g_view_result;

// to swap between navigation with interpolation and without interpolation.

int    g_with_interpolation = 1;

int    g_swap_trackball_navigation = 1;

float g_delta = 20.0;

// this is a helper variable to allow us to change the texture blend model
extern int g_change_texture_blend;


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool move = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == 49 && action == GLFW_PRESS) // 1
	{
		g_with_interpolation++;
		g_with_interpolation = g_with_interpolation % 2;
		mario->enableInterpolation((bool)g_with_interpolation);
		mario_result->enableInterpolation((bool)g_with_interpolation);
	}
	else if (key == 50 && action == GLFW_PRESS) // 2
	{
		g_swap_trackball_navigation++;
		g_swap_trackball_navigation = g_swap_trackball_navigation % 2;
	}


	////////////////////////////////////////////////////////////////////////////////////////////
	// Translation
	if ((key == 87 && action == GLFW_REPEAT) || (key == 87 && action == GLFW_PRESS)) // key w
	{
		//cout <<  "key w pressed" << endl;
		g_tranform_mario = g_tranform_mario * glm::translate(glm::vec3(0.0, 0.0, g_delta));
		mario->setMatrix(g_tranform_mario);
		move = true;
	}
	else if ((key == 83 && action == GLFW_REPEAT) || (key == 83 && action == GLFW_PRESS)) // key s
	{
		//cout <<  "key s pressed" << endl;
		g_tranform_mario = g_tranform_mario * glm::translate(glm::vec3(0.0, 0.0, -g_delta));
		mario->setMatrix(g_tranform_mario);
		move = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Rotation
	if ((key == 65 && action == GLFW_REPEAT) || (key == 65 && action == GLFW_PRESS)) // key a
	{
		//cout <<  "key a pressed" << endl;
		g_tranform_mario = g_tranform_mario * glm::rotate(0.08f, glm::vec3(0.0f, 1.0f, 0.0f));
		mario->setMatrix(g_tranform_mario);
		move = true;
	}

	else if ((key == 68 && action == GLFW_REPEAT) || (key == 68 && action == GLFW_PRESS)) // key d
	{
		//cout <<  "key d pressed" << endl;
		g_tranform_mario = g_tranform_mario * glm::rotate(-0.08f, glm::vec3(0.0f, 1.0f, 0.0f));
		mario->setMatrix(g_tranform_mario);
		move = true;
	}


	if (move)
	{
		glm::vec3 s(g_tranform_mario[3][0], g_tranform_mario[3][1], g_tranform_mario[3][2]);
		glm::vec3 e(g_tranform_mario[3][0], g_tranform_mario[3][1], g_tranform_mario[3][2] - 20);
		vector<glm::vec3> res;

		// perform the ray intersectiont test.
		RayIntersectionTest::intersect(s, e, *mario, res);

		// pick the first result if one is available
		if (res.size() > 0) {
			glm::vec3 position = res[0];

			g_tranform_mario_result = g_tranform_mario;
			g_tranform_mario_result[3][0] = position[0];
			g_tranform_mario_result[3][1] = position[1];
			g_tranform_mario_result[3][2] = position[2];
			mario_result->setMatrix(g_tranform_mario_result);
		}


	}

	//cout << key << endl;
}

int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
	SetCameraManipulator(CameraTypes::CAMERA_MANIPULATOR);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
       
    // create an apperance object.
	GLAppearance* apperance_floor = new GLAppearance("../data/shaders/multi_texture.vs", "../data/shaders/multi_texture.fs");
	// Three textures for flooring	
	GLTextureThree* floor_texture = new GLTextureThree();
	floor_texture->loadAndCreateTextures("../data/textures/texture_brick.bmp", "../data/textures/sunset.bmp", "../data/textures/mydogs.bmp");
	apperance_floor->setTexture(floor_texture);
	
    //************************************************************************************************
    // Finalize the appearance object
    apperance_floor->finalize();
    
    GLPlane3D* plane_0 = new GLPlane3D(0.0, 0.0, 0.0, 200.0, 200.0);
    plane_0->setApperance(*apperance_floor);
    plane_0->init();

	GLPlane3D* upper_plane= new GLPlane3D(0.0, 150.0, 20.0, 200.0, 200.0);
	upper_plane->setApperance(*apperance_floor);
	upper_plane->setMatrix(mario_init_rotate);
	upper_plane->init();

	GLSphere3D* sphere1 = new GLSphere3D(50.0, -50.0, 0.0, 10.0, 90, 50);
	GLSphere3D* sphere2 = new GLSphere3D(-50.0, 50.0, 0.0, 10.0, 90, 50);
	GLSphere3D* sphere3 = new GLSphere3D(-75.0, 0.0, 0.0, 10.0, 90, 50);
	GLSphere3D* sphere4 = new GLSphere3D(-50.0, -50.0, 0.0, 10.0, 90, 50);
	
	//************************************************************************************************
	// Creating and Setting Appearance	
	// create an apperance object.
	GLAppearance* apperance_mario = new GLAppearance("../data/shaders/multi_vertex_lights.vs", "../data/shaders/multi_vertex_lights.fs");
	GLAppearance* apperance_other = new GLAppearance("../data/shaders/multi_vertex_lights.vs", "../data/shaders/multi_vertex_lights.fs");
	GLAppearance* apperance_spheres = new GLAppearance("../data/shaders/multi_vertex_lights.vs", "../data/shaders/multi_vertex_lights.fs");

	GLDirectLightSource  light_source;
	light_source._lightPos = glm::vec4(-100.0, 100.0, 0.0, 0.0);
	light_source._ambient_intensity = 0.5;
	light_source._specular_intensity = 0.1;
	light_source._diffuse_intensity =  5.5;
	light_source._attenuation_coeff = 2.0;

	GLSpotLightSource  light_source_spot;
	light_source_spot._lightPos = glm::vec4(20.0, -20.0, 5.0, 0.0);
	light_source_spot._ambient_intensity = 0.01;
	light_source_spot._specular_intensity = 10.0;
	light_source_spot._diffuse_intensity = 0.01;
	light_source_spot._attenuation_coeff = 0.02;
	light_source_spot._cone_angle = 5.0; // in degree
	light_source_spot._cone_direction = glm::vec3(-1.0, 0.0, 0.0); // this must be aligned with the object and light position.

	// add the light to this apperance object
	apperance_mario->addLightSource(light_source);
	apperance_mario->addLightSource(light_source_spot);
	apperance_other->addLightSource(light_source);
	apperance_spheres->addLightSource(light_source);

	// Create a material object
	GLMaterial material_1;
	material_1._diffuse_material = glm::vec3(0.0, 0.5, 0.0);
	material_1._ambient_material = glm::vec3(1.0, 0.0, 0.0);
	material_1._specular_material = glm::vec3(0.0, 0.0, 1.0);
	material_1._shininess = 25.0;
	material_1._transparency = 1.0;

	GLMaterial material_2;
	material_2._diffuse_material = glm::vec3(0.0, 1.0, 0.0);
	material_2._ambient_material = glm::vec3(1.0, 0.0, 0.8);
	material_2._specular_material = glm::vec3(0.2, 1.0, 0.4);
	material_2._shininess = 25.0;
	material_2._transparency = 1.0;

	GLMaterial sphere_material;
	sphere_material._diffuse_material = glm::vec3(0.0, 1.0, 0.0);
	sphere_material._ambient_material = glm::vec3(1.0, 0.0, 0.8);
	sphere_material._specular_material = glm::vec3(0.2, 1.0, 0.4);
	sphere_material._shininess = 25.0;
	sphere_material._transparency = 1.0;

	// Add the material to the apperance object

	apperance_mario->setMaterial(material_1);
	apperance_mario->finalize();

	apperance_other->setMaterial(material_2);
	apperance_other->finalize();

	apperance_spheres->setMaterial(sphere_material);
	apperance_spheres->finalize();

	// If you want to change appearance parameters after you init the object, call the update function
	apperance_mario->updateLightSources();
	

	loadedModel1->setApperance(*apperance_other);
	loadedModel1->init();

	mario->setApperance(*apperance_mario);
	mario->init();

	sphere1->setApperance(*apperance_spheres);
	sphere1->init();

	sphere2->setApperance(*apperance_spheres);
	sphere2->init();

	sphere3->setApperance(*apperance_spheres);
	sphere3->init();

	sphere4->setApperance(*apperance_spheres);
	sphere4->init();



	glm::mat4 sizeup = glm::scale(glm::vec3(10.0, 10.0f, 10.0f));
	glm::mat4 translate = glm::translate(glm::vec3(3.0, 0.0, 0.0f));

	mario->setMatrix(mario_init_sizeup * mario_init_translate * mario_init_rotate);
	loadedModel1->setMatrix(sizeup * translate);

    // If you want to change appearance parameters after you init the object, call the update function
	// This is effectively calling a method on the object.  Look into the method definition to understand what this function does.
   
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    //SetViewAsLookAt(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));


    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Here we set a new keyboard callback

	// Set the keyboard callback so that when we press ESC, it knows what to do.
	glfwSetKeyCallback(window, keyboard_callback);


	glm::mat4 camera_transformation = glm::lookAt(glm::vec3(1.5f, 0.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 object_transformation = glm::mat4();
	glm::mat4 camera_matrix = glm::mat4();
	glm::mat4 camera_delta = glm::rotate(-0.4f, glm::vec3(1.0f, 0.0f, 0.0f));

    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());
        

        // draw the objects
        cs->draw();
        plane_0->draw();
		upper_plane->draw();
		loadedModel1->draw();
		mario->draw();
		mario_result->draw();
		sphere1->draw();
		sphere2->draw();
		sphere3->draw();
		sphere4->draw();
        // change the texture appearance blend mode
                
		bool ret = floor_texture->setTextureBlendMode(g_change_texture_blend);
		if (ret)apperance_floor->updateTextures();
		
		
		// Camera manipulation
		if (g_swap_trackball_navigation == 0)
		{
			// This code attaches the virtual camera just behind the object.
			// Read the location from the object on the ground
			object_transformation = mario_result->getModelMatrix();

			// Add the camera and a camera delta
			camera_matrix = camera_delta *  camera_transformation * glm::inverse(object_transformation);

			// set the view matrix
			SetViewAsMatrix(camera_matrix);
		}
		else
		{
			// Set the trackball location
			SetTrackballLocation(GetCurrentCameraMatrix(), glm::vec3(-100.0f, -100.0f, 60.0f));
		}
		
		
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;
    
    
}

