/**********************************************************************************
* 
* types.h
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

#ifndef __JAM_TYPES_H__
#define __JAM_TYPES_H__

#include <cstdint>
#include <memory>
#include <gc_cpp.h>

namespace jam
{

// simple types shorcuts
using 		U8 = uint8_t;
using 		U16 = uint16_t;
using 		U32 = uint32_t;
using 		U64 = uint64_t;
using 		I8 = int8_t;
using 		I16 = int16_t;
using 		I32 = int32_t;
using 		I64 = int64_t;
using 		F32 = float;
using 		F64 = double;

// smart pointers
//template<class T> using sptr = std::shared_ptr<T> ;
//template<class T> using wptr = std::weak_ptr<T> ;
//template<class T> using uptr = std::unique_ptr<T> ;
//using Collectible = gc ;
using Collectible = gc_cleanup ;

// jam::time
using time = float ;
using GUID = size_t ;

}

#endif // __JAM_TYPES_H__