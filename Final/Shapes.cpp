//
// Shapes.cpp
//
// Routines for shape-drawing functions.
//
// This file was modified for final project
//
// Edited by: Dhaval Chauhan (dmc8686)
//
//	http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//	This guide was used to implement loader of 3D meshes from .obj files. 
//	Object file has vertices, normals and their indices which is then used
//	to render the objects.
//

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring> 
#include <glm/glm.hpp>
#include <glm/common.hpp>
using namespace std;

#include "Canvas.h"
#include "Shapes.h"

vector< unsigned int > vertInds, normInds;
vector< Vertex > verts, norms;

///
// makeMesh() - bind the correct vertex and normal buffers
// and send them to draw triangle functions.
//
// @param C - Canvas object
// @param choice - Object ID
///
static void makeMesh( Canvas &C , int choice)
{
	int countVertices = vertInds.size() / 3;
    int i;

    for( i = 0; i < countVertices; i++ ) 
    {
        // Calculate the base indices of the three vertices
        int point1 = vertInds[3 * i];     
        int point2 = vertInds[3 * i + 1]; 
        int point3 = vertInds[3 * i + 2]; 

        Vertex p1 = { verts[point1].x,
                      verts[point1].y,
                      verts[point1].z };

        Vertex p2 = { verts[point2].x,
                      verts[point2].y,
                      verts[point2].z };

        Vertex p3 = { verts[point3].x,
                      verts[point3].y,
                      verts[point3].z };
		
        // Calculate the base indices of the three normals
        int normal1 = normInds[3 * i];     
        int normal2 = normInds[3 * i + 1]; 
        int normal3 = normInds[3 * i + 2]; 

        Normal n1 = { norms[normal1].x,
                      norms[normal1].y,
                      norms[normal1].z };

        Normal n2 = { norms[normal2].x,
                      norms[normal2].y,
                      norms[normal2].z };

        Normal n3 = { norms[normal3].x,
                      norms[normal3].y,
                      norms[normal3].z };
					  
		C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );
    }
}

///
// makeTexture() - bind the correct vertex and texture 
// vertex buffers and send them to draw triangle functions.
//
// @param C - Canvas object
// @param choice - Object ID
///
static void makeTexture( Canvas &C , int choice)
{
    int i;
	int countVertices = vertInds.size()/3;

    for( i = 0; i < countVertices; i++ ) 
    {

        // Calculate the base indices of the three vertices
        int point1 = vertInds[3 * i];     
        int point2 = vertInds[3 * i + 1]; 
        int point3 = vertInds[3 * i + 2]; 

        Vertex p1 = { verts[point1].x,
                      verts[point1].y,
                      verts[point1].z };

        Vertex p2 = { verts[point2].x,
                      verts[point2].y,
                      verts[point2].z };

        Vertex p3 = { verts[point3].x,
                      verts[point3].y,
                      verts[point3].z };
		
        // Calculate the base indices of the three texture coordinates
        int normal1 = vertInds[3 * i];     
        int normal2 = vertInds[3 * i + 1]; 
        int normal3 = vertInds[3 * i + 2]; 

        // UVcoord is really Vertex, so we need a 'z' component
        // Map z to the y of the texture image to align it horizontal.
        UVcoord uv1 = { verts[normal1].x, verts[normal1].z, 0.0f };
        UVcoord uv2 = { verts[normal2].x, verts[normal2].z, 0.0f };
        UVcoord uv3 = { verts[normal3].x, verts[normal3].z, 0.0f };
					  
		C.addTriangleWithUV( p1, uv1, p2, uv2, p3, uv3 );
    }
}

///
// loadMesh() - Read .obj files and format the data to
// load them into our buffers.
//
// @param path - Path of the Object file
// @param C - Canvas object
// @param choice - Object ID
///
void loadMesh( char const * path, Canvas &C , int choice)
{
	//clear the buffers
	verts.clear();
	norms.clear();
	vertInds.clear();
	normInds.clear();
	
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("File not found. Please check again !\n");
	}
	char lineHeader[128];
	while( 1 ){
		
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; 
		
		// Read vertices.
		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			Vertex vert;
			fscanf(file, "%f %f %f\n", &vert.x, &vert.y, &vert.z );
			verts.push_back(vert);
		}
		
		// Read normals.
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			Vertex norm;
			fscanf(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z );
			norms.push_back(norm);
		}
		
		// Read triangle vertex and normal indices.
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			unsigned int vertIndex[3], normIndex[3];
			fscanf(file, "%d//%d %d//%d %d//%d\n", &vertIndex[0], &normIndex[0], 
			&vertIndex[1], &normIndex[1], &vertIndex[2], &normIndex[2] );
			
			// Push vertex indices to buffers.
			vertInds.push_back(vertIndex[0] - 1);
			vertInds.push_back(vertIndex[1] - 1);
			vertInds.push_back(vertIndex[2] - 1);
			
			// Push normal indices to buffers.
			normInds.push_back(normIndex[0] - 1);
			normInds.push_back(normIndex[1] - 1);
			normInds.push_back(normIndex[2] - 1);
		}
	}
	
	if(choice != OBJ_BOTTOM)
		makeMesh( C , choice);
	else
		makeTexture ( C , choice);
}

///
// Make objects from .obj files of the objects.
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
void makeShape( int choice, Canvas &C )
{
	if(choice == OBJ_SLAB)
		loadMesh( "SlabScaled.obj" , C , OBJ_SLAB);
	if(choice == OBJ_CHEESE)
		loadMesh( "NewCheese.obj" , C , OBJ_CHEESE);
	if(choice == OBJ_GRAPES)
		loadMesh( "GrapesScaled.obj" , C , OBJ_GRAPES);
	if(choice == OBJ_GLASS)
		loadMesh( "GlassScaled.obj" , C , OBJ_GLASS);
	if(choice == OBJ_BOTTLE)
		loadMesh( "NewBottle.obj" , C , OBJ_BOTTLE);
	if(choice == OBJ_MUG)
		loadMesh( "MugScaled.obj" , C , OBJ_MUG);
	if(choice == OBJ_BOTTOM)
		loadMesh( "BottomScaled.obj" , C , OBJ_BOTTOM);
	if(choice == OBJ_ROOM)
		loadMesh( "NewRoom.obj" , C , OBJ_ROOM);
}
