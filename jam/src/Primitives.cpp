/**********************************************************************************
* 
* Primitives.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito.
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* 
**********************************************************************************/

#include "stdafx.h"

#include "jam/Primitives.h"

using namespace std ;


namespace jam
{
static Vector3 cube_vertices[] = {
	// front
	{-1.0,  1.0,  1.0 },
	{ 1.0,  1.0,  1.0 },
	{ 1.0, -1.0,  1.0 },
	{-1.0, -1.0,  1.0 },
	// top
	{-1.0,  1.0, -1.0 },
	{ 1.0,  1.0, -1.0 },
	{ 1.0,  1.0,  1.0 },
	{-1.0,  1.0,  1.0 },
	// right
	{ 1.0,  1.0, -1.0 },
	{ 1.0,  1.0,  1.0 },
	{ 1.0, -1.0,  1.0 },
	{ 1.0, -1.0, -1.0 },
	// back
	{-1.0,  1.0, -1.0 },
	{ 1.0,  1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
	{-1.0, -1.0, -1.0 },
	// bottom
	{-1.0, -1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
	{ 1.0, -1.0,  1.0 },
	{-1.0, -1.0,  1.0 },
	// left
	{-1.0,  1.0, -1.0 },
	{-1.0,  1.0,  1.0 },
	{-1.0, -1.0,  1.0 },
	{-1.0, -1.0, -1.0 },
};

static Vector3 cube_normals[] = {
	// front
	{ 0.0,  0.0,  1.0 },
	{ 0.0,  0.0,  1.0 },
	{ 0.0,  0.0,  1.0 },
	{ 0.0,  0.0,  1.0 },
	// top
	{ 0.0,  1.0,  0.0 },
	{ 0.0,  1.0,  0.0 },
	{ 0.0,  1.0,  0.0 },
	{ 0.0,  1.0,  0.0 },
	// right
	{ 1.0,  0.0,  0.0 },
	{ 1.0,  0.0,  0.0 },
	{ 1.0,  0.0,  0.0 },
	{ 1.0,  0.0,  0.0 },
	// back
	{ 0.0,  0.0, -1.0 },
	{ 0.0,  0.0, -1.0 },
	{ 0.0,  0.0, -1.0 },
	{ 0.0,  0.0, -1.0 },
	// bottom
	{ 0.0, -1.0,  0.0 },
	{ 0.0, -1.0,  0.0 },
	{ 0.0, -1.0,  0.0 },
	{ 0.0, -1.0,  0.0 },
	// left
	{-1.0,  0.0,  0.0 },
	{-1.0,  0.0,  0.0 },
	{-1.0,  0.0,  0.0 },
	{-1.0,  0.0,  0.0 },
};

static Vector2 cube_tex_coords[] = {
	// front
	{ 0.0,  1.0 },
	{ 1.0,  1.0 },
	{ 1.0,  0.0 },
	{ 0.0,  0.0 },
	// top
	{ 0.0,  1.0 },
	{ 1.0,  1.0 },
	{ 1.0,  0.0 },
	{ 0.0,  0.0 },
	// right
	{ 0.0,  1.0 },
	{ 1.0,  1.0 },
	{ 1.0,  0.0 },
	{ 0.0,  0.0 },
	// back
	{ 0.0,  1.0 },
	{ 1.0,  1.0 },
	{ 1.0,  0.0 },
	{ 0.0,  0.0 },
	// bottom
	{ 0.0,  1.0 },
	{ 1.0,  1.0 },
	{ 1.0,  0.0 },
	{ 0.0,  0.0 },
	// left
	{ 0.0,  1.0 },
	{ 1.0,  1.0 },
	{ 1.0,  0.0 },
	{ 0.0,  0.0 },
};

static GLushort cube_elements[] = {
		// front
		0, 1, 2,
		0, 2, 3,
		// top
		4, 5, 6,
		4, 6, 7,
		// right
		8, 10, 9,
		8, 11,10,
		// back
		12, 14, 13,
		12, 15, 14,
		// bottom
		16, 18, 17,
		16, 19, 18,
		// left
		20, 21, 22,
		20, 22, 23,
};

static Vector3 quad_vertices[] = {
	{ -0.5f, 0.5f, 0.0f },
	{  0.5f, 0.5f, 0.0f },
	{  0.5f,-0.5f, 0.0f },
	{ -0.5f,-0.5f, 0.0f },
};

static Vector3 quad_normals[] = {
	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },
};

static Vector2 quad_tex_coords[] = {
	// front
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 0.0f },
};

static GLushort quad_elements[] = {
		0, 1, 2,
		0, 2, 3,
};

void Primitives::createCube( TightVertexBuffer*& vb )
{
	vb = new TightVertexBuffer( 24, 36 ) ;
	vb->useColors(false) ;

	for( int16_t i=0; i<sizeof(cube_vertices)/sizeof(Vector3); i++ ) {
		vb->addVertex( cube_vertices[i].x, cube_vertices[i].y, cube_vertices[i].z, 
					  cube_normals[i].x, cube_normals[i].y ,cube_normals[i].z,
					  Color::WHITE.getRgba(), cube_tex_coords[i].x, cube_tex_coords[i].y ) ;
	}
	for( int16_t i=0; i<sizeof(cube_elements)/sizeof(GLushort); i=i+3 ) {
		vb->addTriIndices( cube_elements[i], cube_elements[i+1], cube_elements[i+2] ) ;
	}
}

void Primitives::createQuad( TightVertexBuffer*& vb )
{
	vb = new TightVertexBuffer( 4, 6 ) ;
	vb->useColors(false) ;

	for( int16_t i=0; i<sizeof(quad_vertices)/sizeof(Vector3); i++ ) {
		vb->addVertex( quad_vertices[i].x, quad_vertices[i].y, quad_vertices[i].z, 
					  quad_normals[i].x, quad_normals[i].y ,quad_normals[i].z,
					  Color::WHITE.getRgba(), quad_tex_coords[i].x, quad_tex_coords[i].y ) ;
	}
	for( int16_t i=0; i<sizeof(quad_elements)/sizeof(GLushort); i=i+3 ) {
		vb->addTriIndices( quad_elements[i], quad_elements[i+1], quad_elements[i+2] ) ;
	}
}

Mesh* Primitives::createCubeMesh()
{
	Mesh* pMesh = new Mesh() ;
	pMesh->create(24,36) ;

	pMesh->getVerticesArray().copyFrom( cube_vertices ) ;
	pMesh->getNormalsArray().copyFrom( cube_normals ) ;
	pMesh->getTexCoordsArray().copyFrom( cube_tex_coords ) ;
	pMesh->getElementsArray().copyFrom( cube_elements ) ;

	return pMesh ;
}

Mesh* Primitives::createQuadMesh()
{
	Mesh* pMesh = new Mesh() ;
	pMesh->create(4,6) ;

	pMesh->getVerticesArray().copyFrom( quad_vertices ) ;
	pMesh->getNormalsArray().copyFrom( quad_normals ) ;
	pMesh->getTexCoordsArray().copyFrom( quad_tex_coords ) ;
	pMesh->getElementsArray().copyFrom( quad_elements ) ;

	return pMesh ;
}

}
