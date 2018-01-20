//
//  Lighting.h
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up Phong illumination/shading.
//
//  This file should not be modified by students.
//

#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

void setUpLight( GLuint program, float colorR, float colorG, float colorB,
								float posX, float posY, float posZ,
								float ambR, float ambG, float ambB);

///
// These functions set up the material, and shading parameters
// for the objects shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpCheese( GLuint program );
void setUpSlab( GLuint program );
void setUpGrapes( GLuint program );
void setUpGlass( GLuint program );
void setUpBottle( GLuint program );
void setUpMug( GLuint program );
void setUpBottom( GLuint program );
void setUpRoom( GLuint program );

#endif
