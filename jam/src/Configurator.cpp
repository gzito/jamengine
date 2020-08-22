/**********************************************************************************
* 
* Configurator.cpp
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

#include "jam/Configurator.h"


namespace jam
{
	Configurator::Configurator()
	{
		m_ini.SetUnicode() ;
	}

	void Configurator::loadFile(const String& filename)
	{
		m_filename = filename ;
		if( m_ini.LoadFile(filename.c_str()) < 0 ) 
		{
			JAM_ERROR( "Cannot open '%s' for reading", filename.c_str() ) ;
		}
	}

	int Configurator::getInt( const String& section, const String& name )
	{
		int v = 0 ;
		String s = getString(section,name) ;
		if( !s.empty() ) {
			v = std::stoi(s) ;
		}
		return v ;
	}

	float Configurator::getFloat( const String& section, const String& name )
	{
		float v = 0.0f ;
		String s = getString(section,name) ;
		if( !s.empty() ) {
			v = std::stof(s) ;
		}
		return v ;
	}

	String Configurator::getString( const String& section, const String& name )
	{
		const char* v = m_ini.GetValue(section.c_str(),name.c_str()) ;

/*
		if( v == nullptr ) {
			JAM_ERROR( "Cannot find key '%s' in section [%s] in file '%s'", name, section, m_filename ) ;
		}
*/
		return (v != nullptr) ? v : "" ;
	}

}
