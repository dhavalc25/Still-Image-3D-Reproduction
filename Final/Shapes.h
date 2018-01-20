//
// Shapes.h
//
// Prototypes for shape-drawing functions.
//
// This file should not be modified by students.
//

#ifndef _SHAPES_H_
#define _SHAPES_H_

#include "Canvas.h"
#include <string>
#include <iostream>

// Macros for object and shading selection
#define OBJ_SLAB	0
#define OBJ_CHEESE	3
#define OBJ_GRAPES	6
#define OBJ_GLASS	9
#define OBJ_BOTTLE	12
#define OBJ_MUG		15
#define OBJ_BOTTOM	18
#define OBJ_ROOM	21

///
// Make objects 
//
// @param choice - 	0 for slab
//					3 for cheese
//					6 for grapes
//					9 for glass
//					12 for bottle
//					15 for mug
//					18 for bottom table
//					21 for room
//
// @param C      - the Canvas we'll use
///
void makeShape( int choice, Canvas &C );

///
// loadMesh() - Read .obj files and format the data to
// load them into our buffers.
//
// @param path - Path of the Object file
// @param C - Canvas object
// @param choice - Object ID
///
void loadMesh( char* path, Canvas &C );

#endif
