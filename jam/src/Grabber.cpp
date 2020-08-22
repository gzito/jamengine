/**********************************************************************************
* 
* Grabber.cpp
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

#include "jam/Grabber.h"
//#include "jam/Utilities.h"
#include "jam/Draw3dManager.h"

namespace jam
{

/*
Grabber::Grabber() : m_pSurface(0)
{
	m_pSurface = new CIwGxSurface() ;
}

Grabber::~Grabber()
{
	JAM_DELETE(m_pSurface) ;
}
*/

void Grabber::grab( jam::DrawItem* pDrawitem )
{
/*
	bool succeded = m_pSurface->CreateSurface(
		pDrawitem->getTexture(),
		(int32)pDrawitem->getWidth(),(int32)pDrawitem->getHeight(),
		CIwGxSurface::ALPHA_F | CIwGxSurface::CREATE_8888_SURFACE_F | CIwGxSurface::PRESERVE_FRAME_BUFFER_F ) ;

	JAM_ASSERT_MSG(succeded==true, ("CreateSurface failed"));
*/
}

void Grabber::beforeRender()
{
/*
	m_clearColor = IwGxGetColClear() ;
	m_pSurface->MakeCurrent() ;
	IwGxSetColClear(0,0,0,0);
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
	int16 w = (int16)m_pSurface->GetWidth()/2 ;
	int16 h = (int16)m_pSurface->GetHeight()/2 ;
	CIwSVec2 org( w, h ); 
	IwGxSetScreenSpaceOrg(&org);
*/
}

void Grabber::afterRender()
{
/*
	IwGxFlush() ;
	CIwGxSurface::MakeDisplayCurrent() ;
	IwGxSetColClear( m_clearColor.r(), m_clearColor.g(), m_clearColor.b(), m_clearColor.a() ) ;
 	CIwSVec2 org( (int16)Draw3DManager::HalfVPWidth, (int16)Draw3DManager::HalfVPHeight );
 	IwGxSetScreenSpaceOrg(&org);
*/
}

Texture2D* Grabber::getTexture()
{
	Texture2D* pTexture = 0 ;
/*
	if( m_pSurface && m_pSurface->IsValid() ) {
		pTexture = m_pSurface->GetTexture() ;
	}

	JAM_ASSERT_MSG(pTexture, ("CIwGxSurface::GetTexture() failed"));
*/
	return pTexture ;
}

}
