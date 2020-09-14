/**********************************************************************************
*
* PSys_Globals.h
*
* This file is part of Jam
*
* Copyright (c) 2014-2020 Giovanni Zito, Gianluca Sclano
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
#pragma once

#if !defined(IW_PLATFORM_S3E) && !defined(IW_PLATFORM_S3E_LIB)
#pragma warning (disable:4244)
#pragma warning (disable:4018)
#pragma warning (disable:4305)
#pragma warning (disable:4800)
#endif

#include <algorithm>
#include <string>


#define DbgPrintf	//
#define dbgprintf


#define PSYS_INFINITE_LOOPS		0
#define PSYS_NULL			 -99999

#define PARTICLE	Particle3D

//#define PARENT_PSYS		emitter->PSysParent_ref

#define EMITTERS_POOL	100
#define MAX_EMITTERS_POOL	2000
#define MAX_PARTICLES_POOL	2000
#define PARTICLES_POOL  100
#define MIN_ALPHA_THRESHOLD	48.0f
#define MIN_SIZE_THRESHOLD	0.2f
#define MAX_PARTICLES_EMITTED 80000
#define MIN_PARTICLES_EMITTED 256
