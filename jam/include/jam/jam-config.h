/**********************************************************************************
* 
* jam-config.h
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

#ifndef __JAM_JAM_CONFIG_H__
#define __JAM_JAM_CONFIG_H__

#define JAM_CM_MAX_COLL_TYPES		32
#define JAM_WINDOWS_WIDTH			960
#define JAM_WINDOWS_HEIGHT			540


//#define IW_USE_PROFILE
//#define JAM_DEBUG_MENU_ENABLED

//#define JAM_CM_QUADTREE_DISABLED
//#define JAM_DRAW3DBATCH_DISABLED
//#define JAM_TEXT3D_DISABLED
//#define JAM_IMAGELINE3D_DISABLED
//#define JAM_LINE3D_DISABLED
//#define JAM_SPRITES_DISABLED
//#define JAM_COLLISIONS_DISABLED
//#define JAM_PHYSIC_ENABLED
//#define JAM_FORCE_BLENDING_MODE
//#define JAM_USE_QUAD_LIST
//#define JAM_MULTITHREADING_ENABLED

#ifdef _DEBUG

// must enable this to use JAM_TRACE_XXX macros
#define JAM_DEBUG

//#define JAM_CHECK_SINGLE_UPDATE_CALL
//#define JAM_TRACE_AUTORELEASEPOOL
//#define JAM_TRACE_AUTORELEASEPOOL_DETAILS
//#define JAM_TRACE_AUTORELEASEPOOL_DETAILS_SPRITES
//#define JAM_TRACE_ACTIVE_NODES
//#define JAM_TRACE_BATCH
//#define JAM_TRACE_INVIEW_SPRITES
//#define JAM_TRACE_COLLISIONS
//#define JAM_TRACE_SHADERS
#endif

#endif
