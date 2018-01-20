//
//  Lighting
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Dhaval Chauhan (dmc8686)
//
//  Simple class for setting up Texture and Phong illumination/shading.
//
//	Colors from http://avatar.se/molscript/doc/colour_names.html
//
//  This code can be compiled as either C or C++.
//

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#endif
#include "Lighting.h"
// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL.h>

// Add any global definitions and/or variables you need here.

float lightSourceColor[4] = { 0.0, 0.0, 0.0, 1.0 };
float lightSourcePosition[4] = { 0.0, 0.0, 0.0, 1.0 };
float sceneAmbLightColor[4] = { 0.0, 0.0, 0.0, 1.0 };

///
// This function sets up the light parameters.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpLight( GLuint program, float colorR, float colorG, float colorB,
								float posX, float posY, float posZ,
								float ambR, float ambG, float ambB)
{
	lightSourceColor[0] = colorR;
	lightSourceColor[1] = colorG;
	lightSourceColor[2] = colorB;
	lightSourcePosition[0] = posX;
	lightSourcePosition[1] = posY;
	lightSourcePosition[2] = posZ;
	sceneAmbLightColor[0] = ambR;
	sceneAmbLightColor[1] = ambG;
	sceneAmbLightColor[2] = ambB;
	glUniform4fv(glGetUniformLocation(program, "lightSourceColor"), 1, lightSourceColor);
	glUniform4fv(glGetUniformLocation(program, "lightSourcePosition"), 1, lightSourcePosition);
	glUniform4fv(glGetUniformLocation(program, "sceneAmbLightColor"), 1, sceneAmbLightColor);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Cheese object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpCheese( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 0.855, 0.650, 0.125, 1.0 };
	float diffMatColor[] = { 1.000, 0.871, 0.650, 1.0 };
	float specMatColor[] = {  1, 1, 1, 1.0 };
	
	//coefficients for lighting
	float ambRefCoeff = 0.5;
	float diffRefCoeff = 0.7;
	float specRefCoeff = 0.1;
	float specExponent = 1.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Slab object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpSlab( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 0.9, 0.6, 0.22, 0.1 };
	float diffMatColor[] = { 0.9, 0.6, 0.22, 1.0 };
	float specMatColor[] = { 1.0, 1.0, 1.0, 1.0 };
	
	//coefficients for lighting
	float ambRefCoeff = 0.9;
	float diffRefCoeff = 0.8;
	float specRefCoeff = 0.2;
	float specExponent = 1.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Grapes object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpGrapes( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 0.596078, 0.603922, 0.196078, 1.0 };
	float diffMatColor[] = { 0.603922, 0.503922, 0.196078, 1.0 };
	float specMatColor[] = { 1.0, 1.0, 1.0, 1.0 };
	
	//coefficients for lighting
	float ambRefCoeff = 0.7;
	float diffRefCoeff = 0.9;
	float specRefCoeff = 0.12;
	float specExponent = 9.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Glass object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpGlass( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 1, 0.980392, 0.980392, 1.0 };
	float diffMatColor[] = { 1, 0.980392, 0.980392, 1.0 };
	float specMatColor[] = { 1.000, 1.000, 1.000, 1.0 };
	
	//coefficients for lighting
	float ambRefCoeff = 0.2;
	float diffRefCoeff = 0.8;
	float specRefCoeff = 0.9;
	float specExponent = 7.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Bottle object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpBottle( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 0.2, 0.0, 0.0, 1.0 };
	float diffMatColor[] = { 0.2, 0.0, 0.0, 1.0 };
	float specMatColor[] = { 1.0, 1.0, 1.0, 1.0 };
	
	//coefficients for lighting
	float ambRefCoeff = 0.4;
	float diffRefCoeff = 0.2;
	float specRefCoeff = 5.0;
	float specExponent = 20.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Mug object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent.
///
void setUpMug( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 0.496, 0.884, 0.996, 1.0 };
	float diffMatColor[] = { 0.796, 0.784, 0.696, 1.0 };
	float specMatColor[] = { 1.0, 1.0, 1.0, 1.0 };
	
	//coefficients for lighting
	float ambRefCoeff = 0.30;
	float diffRefCoeff = 1.0;
	float specRefCoeff = 1.0;
	float specExponent = 15.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Bottom table object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpBottom( GLuint program )
{	
	//coefficients for lighting
	float ambRefCoeff = 0.5;
	float diffRefCoeff = 0.2;
	float specRefCoeff = 0.5;
	float specExponent = 9.0;
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
	
	GLuint cloth = SOIL_load_OGL_texture (
        "newred.jpg", 
         SOIL_LOAD_AUTO, 
         SOIL_CREATE_NEW_ID, 
         SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
         SOIL_FLAG_TEXTURE_REPEATS); 
    
    //Use and Bind cloth image    
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cloth);
	
	glUniform1i(glGetUniformLocation(program, "clothTexture"), 0);
}

///
// This function sets up the lighting, material, and shading parameters
// for the Room object.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpRoom( GLuint program )
{
	//Object material colors for lighting
	float ambMatColor[] = { 0.855, 0.647, 0.125, 1.0 };
	float diffMatColor[] = { 0.055, 0.047, 0.025, 1.0 };
	float specMatColor[] = { 1.0, 1.0, 1.0, 1.0};
	
	//coefficients for lighting
	float ambRefCoeff = 0.2;
	float diffRefCoeff = 0.3;
	float specRefCoeff = 0.8;
	float specExponent = 16.0;
	
	//Send values to Shaders
	glUniform4fv(glGetUniformLocation(program, "ambMatColor"), 1, ambMatColor);
	glUniform4fv(glGetUniformLocation(program, "diffMatColor"), 1, diffMatColor);
	glUniform4fv(glGetUniformLocation(program, "specMatColor"), 1, specMatColor);
	
	glUniform1f(glGetUniformLocation(program, "ambRefCoeff"), ambRefCoeff);
	glUniform1f(glGetUniformLocation(program, "diffRefCoeff"), diffRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specRefCoeff"), specRefCoeff);
	glUniform1f(glGetUniformLocation(program, "specExponent"), specExponent);
}
