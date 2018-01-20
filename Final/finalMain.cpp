//
//  finalMain.cpp
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Edited by Dhaval Chauhan (dmc8686) 2017/05/17.
//
//	This is a 3D reproduction of a still life image by Rick Harris. I have
//	used CSCI 610 lab6 supplied files as a framework to render this image.
//		
//	Most modifications were done to Shapes.cpp, textingMain.cpp, and 
//	Lighting.cpp files. No significant changes were made to other files.
//	
//	GUIDE TO OPERATE:
//	keyboard 'a' : animate;
//		Animation includes rotation in all directions and light source
//		with different Red, Green and Blue color intensities (Amateur
//  	implementation of disco lights). 
//	keyboard 's' : stop animation;
//	keyboard 'r' : reset;
//	keyboard '1' : rotate objects clockwise along x axis;
//	keyboard '2' : rotate objects clockwise along y axis;
//	keyboard '3' : rotate objects clockwise along z axis;
//	keyboard '4' : rotate objects counter-clockwise along x axis;
//	keyboard '5' : rotate objects counter-clockwise along y axis;
//	keyboard '6' : rotate objects counter-clockwise along z axis;
//	
//	CREDITS and REFERENCES:
//	Prof. Warren R. Carithers for guidance.
//
//	Blender - 3D modelling tool used to model objects in the image.	
//	
//	RedFoc Tutorials (Youtube) - Blender tutorials from this channel  
//	were referred to model the cup in this project.
//
//	http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//	This guide was used to implement loader for 3D meshes from .obj files. 
//	Object file has vertices, normals and their indices which is then used
//	to make the objects.
//
//	RGB values for objects were obtained from the following two websites.
//	http://prideout.net/archive/colors.php
//	http://avatar.se/molscript/doc/colour_names.html
//
//	cloth texture object was obtained from https://www.textures.com/
//

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "Buffers.h"
#include "ShaderSetup.h"
#include "Canvas.h"
#include "Shapes.h"
#include "Viewing.h"
#include "Lighting.h"

using namespace std;

// do we need to do a display() call?
bool updateDisplay = true;

bool stopR = false;
bool stopG = false;
bool stopB = false;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 800;
int w_height = 800;

//
// We need eight vertex buffers and 8 element buffers:
// one set for each object in the scene.
//
BufferSet slabBuffers;
BufferSet cheeseBuffers;
BufferSet grapesBuffers;
BufferSet glassBuffers;
BufferSet bottleBuffers;
BufferSet mugBuffers;
BufferSet bottomBuffers;
BufferSet roomBuffers;

// Animation flag
bool animating = false;

// Initial animation rotation angles for all the objects
// format: 	object1-x, object1-y, object1-z, 
//			object2-x, object2-y, object2-z, .....
float angles[24] = { 	0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 0.0f	};

float sceneLightColor[3] = {1.0, 1.0, 1.0};
float lightPosition[3] = {-1.2, 2.5, 0.1};
float sceneAmbColor[3] = {1.0, 1.0, 0.0};

// program IDs...for shader programs
// bottomShader for textured objects
// meshShader for normal objects
GLuint textureShader, phongShader;

//
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param C   - the Canvas to use
//
void createShape( int obj, Canvas &C )
{
    // clear any previous shape
    C.clear();

    // make the shape
    makeShape( obj, C );
    
    // create the necessary buffers
    if( obj == OBJ_SLAB ) {
        slabBuffers.createBuffers( C );
	}
    else if( obj == OBJ_CHEESE ) {
        cheeseBuffers.createBuffers( C );
	}
    else if( obj == OBJ_GRAPES ) {
        grapesBuffers.createBuffers( C );
	}
    else if( obj == OBJ_GLASS ) {
        glassBuffers.createBuffers( C );
	}
    else if( obj == OBJ_BOTTLE ) {
        bottleBuffers.createBuffers( C );
	}
    else if( obj == OBJ_MUG ) {
        mugBuffers.createBuffers( C );
	}
    else if( obj == OBJ_BOTTOM ) {
        bottomBuffers.createBuffers( C );
	}
    else if( obj == OBJ_ROOM ) {
        roomBuffers.createBuffers( C );
	}
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Load shaders, verifying each
    // texture shader files for textured objects
    ShaderError error;
    textureShader = shaderSetup( "texture.vert", "texture.frag", &error );
    if( !textureShader ) {
        cerr << "Error setting up texture shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    // phong shader files for non-textured objects
    phongShader = shaderSetup( "phong.vert", "phong.frag", &error );
    if( !phongShader ) {
        cerr << "Error setting up phong shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }
	
    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // Create all our objects
    createShape( OBJ_SLAB, *canvas );
    createShape( OBJ_CHEESE, *canvas );
    createShape( OBJ_GRAPES, *canvas );
    createShape( OBJ_GLASS, *canvas );
    createShape( OBJ_BOTTLE, *canvas );
    createShape( OBJ_MUG, *canvas );
    createShape( OBJ_BOTTOM, *canvas );
    createShape( OBJ_ROOM, *canvas );
}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to use
///
void selectBuffers( GLuint program, BufferSet &B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B.ebuffer );

    // set up the vertex attribute variables
    GLint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    int offset = B.vSize;

    if( B.cSize ) {  // color data
        GLint vColor = glGetAttribLocation( program, "vColor" );
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.cSize;
    }

    if( B.nSize ) {  // normal data
        GLint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.nSize;
    }

    if( B.tSize ) {  // texture coordinate data
        GLint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.tSize;
    }
}

///
// Display callback
//
// Invoked whenever the image must be redrawn
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Set up lights
    glUseProgram( phongShader );
	setUpLight(	phongShader, sceneLightColor[0],	sceneLightColor[1],	sceneLightColor[2],
				lightPosition[0],	lightPosition[1],	lightPosition[2],
				sceneAmbColor[0],	sceneAmbColor[1],	sceneAmbColor[2]);
				
    glUseProgram( textureShader );
	setUpLight(	textureShader, sceneLightColor[0],	sceneLightColor[1],	sceneLightColor[2],
				lightPosition[0],	lightPosition[1],	lightPosition[2],
				sceneAmbColor[0],	sceneAmbColor[1],	sceneAmbColor[2]);
	
    // =========================draw the Cheese=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpSlab( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_SLAB], angles[OBJ_SLAB+1], angles[OBJ_SLAB+2],
        1.55f, 0.5f, -1.5f 	
    );
    // draw it
    selectBuffers( phongShader, slabBuffers );
    glDrawElements( GL_TRIANGLES, slabBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    // =========================draw the Cheese=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpCheese( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_CHEESE], angles[OBJ_CHEESE+1], angles[OBJ_CHEESE+2],
        1.55f, 0.5f, -1.5f 	
    );
    // draw it
    selectBuffers( phongShader, cheeseBuffers );
    glDrawElements( GL_TRIANGLES, cheeseBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    // =========================draw the Grapes=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpGrapes( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_GRAPES], angles[OBJ_GRAPES+1], angles[OBJ_GRAPES+2],
        1.55f, 0.5f, -1.5f 	
    );
    // draw it
    selectBuffers( phongShader, grapesBuffers );
    glDrawElements( GL_TRIANGLES, grapesBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    
    // =========================draw the Glass=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpGlass( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_GLASS], angles[OBJ_GLASS+1], angles[OBJ_GLASS+2],
        1.55f, 0.5f, -1.5f 	
    );
    // draw it
    selectBuffers( phongShader, glassBuffers );
    glDrawElements( GL_TRIANGLES, glassBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    
    // =========================draw the Bottle=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpBottle( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_BOTTLE], angles[OBJ_BOTTLE+1], angles[OBJ_BOTTLE+2],
        1.55f, 0.5f, -1.5f 
    );
    // draw it
    selectBuffers( phongShader, bottleBuffers );
    glDrawElements( GL_TRIANGLES, bottleBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    // =========================draw the Mug=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpMug( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_MUG], angles[OBJ_MUG+1], angles[OBJ_MUG+2],
        1.55f, 0.5f, -1.5f 	
    );
    // draw it
    selectBuffers( phongShader, mugBuffers );
    glDrawElements( GL_TRIANGLES, mugBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    // =========================draw the Table=====================
    
    glUseProgram( textureShader );
    // set up viewing and projection parameters
    setUpFrustum( textureShader );
    // set up the Phong shading information
    setUpBottom( textureShader );
    // set up the camera
    setUpCamera( textureShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f	
    );
    setUpTransforms( textureShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_BOTTOM], angles[OBJ_BOTTOM+1], angles[OBJ_BOTTOM+2],
        1.55f, 0.5f, -1.5f 	
    );
    // draw it
    selectBuffers( textureShader, bottomBuffers );
    glDrawElements( GL_TRIANGLES, bottomBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    // =========================draw the Room=====================
    
    glUseProgram( phongShader );
    // set up viewing and projection parameters
    setUpFrustum( phongShader );
    // set up the Phong shading information
    setUpRoom( phongShader );
    // set up the camera
    setUpCamera( phongShader,
        1.55f, 2.2f, 5.5f,
        1.55f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f
    );
    setUpTransforms( phongShader,
        5.0f, 5.0f, 5.0f,
        angles[OBJ_ROOM], angles[OBJ_ROOM+1], angles[OBJ_ROOM+2],
        1.55f, 0.5f, -1.5f 
    );
    // draw it
    selectBuffers( phongShader, roomBuffers );
    glDrawElements( GL_TRIANGLES, roomBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
}

///
// Rotate all the objects in the given direction.
//
// @param direction - GLSL program object.
///
void rotateAllObjects(int direction)
{
	for(int i = 0; i < (sizeof(angles)/sizeof(*angles)) - 3; i+=3)
	{
			if(direction == 1)
				angles[i+0] += 1.5f;
			if(direction == 2)
				angles[i+1] += 1.5f;
			if(direction == 3)
				angles[i+2] += 1.5f;
			if(direction == 4)
				angles[i+0] -= 1.5f;
			if(direction == 5)
				angles[i+1] -= 1.5f;
			if(direction == 6)
				angles[i+2] -= 1.5f;
	}
}

///
// reset all the objects in default direction and lighting.
///
void resetAllObjects( void )
{
	stopR = false;
	stopR = false;
	stopR = false;
	sceneLightColor[0] = 1.0;
	sceneLightColor[1] = 1.0;
	sceneLightColor[2] = 1.0;
	lightPosition[0] = -1.2;
	lightPosition[1] = 2.5;
	lightPosition[2] = 1.0;
	sceneAmbColor[0] = 1.0;
	sceneAmbColor[1] = 1.0;
	sceneAmbColor[2] = 0.0;
	for(int i = 0; i < sizeof(angles)/sizeof(*angles); i++)
	{
		angles[i] = 0.0f;
	}
}

///
// Keyboard callback
//
// We don't need to distinguish between (e.g.) 'a' and 'A', so
// we can use the simpler form of callback
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    switch( key & 0x7f ) {

	// termination
        case 033:  // Escape key
        case 'q': case 'Q':
            glfwSetWindowShouldClose( window, 1 );

	// automated animation
        case 'a': case 'A':
	    animating = true;
	    break;
	    
        case 's': case 'S':    // stop animating
        animating = false;
        break;

	// incremental rotation along the axes
        case '1': rotateAllObjects(1); 
        break;
        case '2': rotateAllObjects(2); 
        break;
        case '3': rotateAllObjects(3); 
        break;
        case '4': rotateAllObjects(4);  
        break;
        case '5': rotateAllObjects(5);
        break;
        case '6': rotateAllObjects(6);
        break;

	// reset
		case 'r': case 'R':    // reset rotations
			resetAllObjects();
			break;
    }

    updateDisplay = true;
}

///
// Animate (rotate in all directions) the objects.
// Try different colors as well along with that.
///
void animate( void ) {
    if( animating ) {
		
		if(!stopR)
		{
			stopG = true;
			stopB = true;
			sceneLightColor[0] += 0.1f;
			if(sceneLightColor[0] > 1.0)
			{
				stopR = true;
				stopG = false;
				sceneLightColor[0] = 0.05f;
			}
		}
		
		if(!stopG)
		{
			sceneLightColor[1] += 0.1f;
			if(sceneLightColor[1] > 1.0)
			{
				stopG = true;
				stopB = false;
				sceneLightColor[1] = 0.05f;
			}
		}
		
		if(!stopB)
		{
			sceneLightColor[2] += 0.1f;
			if(sceneLightColor[2] > 1.0)
			{
				stopB = true;
				sceneLightColor[2] = 0.05f;
			}
		}
		
		if(stopR && stopB && stopG)
		{
			stopR = false;
			stopB = false; 
			stopG = false; 
		}
		
		for(int i = 0; i < (sizeof(angles)/sizeof(*angles)) - 3; i++)
		{
			angles[i] += 0.5f;
		}
        updateDisplay = true;
    }
}

///
// Print error.
///
void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}

///
// main program for final project
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    // NOTE:  Mac users may need to uncomment the following four lines
    // in order to force an OpenGL 3.2 (or better) context

    // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Check README section in finalMain.cpp for controls", NULL, NULL );

    if( !window ) {
        cerr << "GLFW window create failed!" << endl;
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }
    if( !GLEW_VERSION_3_2 ) {
        cerr << "GLEW: OpenGL 3.2 not available" << endl;
        if( !GLEW_VERSION_2_1 ) {
            cerr << "GLEW: OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
    if( maj < 3 || (maj == 3 && min < 2) ) {
        // nope!
        cerr << "*** GLSL 1.50 shaders may not compile" << endl;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    while( !glfwWindowShouldClose(window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( window );
        }
        glfwPollEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();


    return 0;
}
