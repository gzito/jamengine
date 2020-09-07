/**********************************************************************************
* 
* ShaderFile.cpp
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

#include <stdafx.h>

#include "jam/ShaderFile.h"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

namespace jam
{
ShaderFile::ShaderFile( GLenum shaderType ) :
    m_objectID(0), m_shaderType(shaderType), m_compiled(false), m_sourceCode(), m_resHandle()
{
}

ShaderFile::ShaderFile( ResHandle* resHandle, GLenum shaderType ) :
     ShaderFile( shaderType )
{
	m_resHandle = resHandle ;
	if( m_resHandle ) {
		setSource( m_resHandle->getBuffer() ) ;
	}
}

ShaderFile::~ShaderFile()
{
	if( m_objectID != 0 ) {
		glDeleteShader(m_objectID); m_objectID = 0;
	}
}

void ShaderFile::compile()
{
	if( !m_compiled ) {
		//compile
		JAM_TRACE( "Compiling shader \"%s\"\n", m_resHandle->getResource().getName().c_str() ) ;

		glCompileShader(m_objectID);
    
		//throw exception if compile error occurred
		GLint status;
		glGetShaderiv(m_objectID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			String msg("Compile failure in shader: \"" + m_resHandle->getResource().getName() + "\"\n");
        
			GLint infoLogLength;
			glGetShaderiv(m_objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* strInfoLog = new char[infoLogLength + 1];
			glGetShaderInfoLog(m_objectID, infoLogLength, NULL, strInfoLog);
			msg += strInfoLog;
			delete[] strInfoLog;
        
			glDeleteShader(m_objectID); m_objectID = 0;
			JAM_ERROR(msg.c_str());
		}

		m_compiled = true ;
	}
}

GLuint ShaderFile::objectID() const {
    return m_objectID;
}

ShaderFile* ShaderFile::shaderFromFile( ResHandle* resHandle, GLenum shaderType ) {
    ShaderFile* sh = new (GC) ShaderFile(resHandle, shaderType) ;
	sh->compile() ;
    return sh;	
}

void ShaderFile::setSource( const String& shaderCode )
{
	if( m_objectID == 0) {
		//create the shader object
		m_objectID = glCreateShader(m_shaderType);
		if(m_objectID == 0) {
			JAM_ERROR("glCreateShader failed");
		}
	}

	m_sourceCode = shaderCode ;
    const char* code = m_sourceCode.c_str();
    glShaderSource(m_objectID, 1, (const GLchar**)&code, NULL);
	m_compiled = false ;
}

//
// ShaderFileResourceLoader
//
ShaderFileResourceLoader::ShaderFileResourceLoader()
{
	m_patterns.clear() ;
	m_patterns.emplace_back("*.vert");
	m_patterns.emplace_back("*.frag");
}

bool ShaderFileResourceLoader::useRawFile() const
{
	return true;
}

size_t ShaderFileResourceLoader::getLoadedResourceSize(char* rawBuffer,size_t rawSize)
{
	return rawSize;
}

bool ShaderFileResourceLoader::loadResource(char* rawBuffer,size_t rawSize,ResHandle& handle)
{
	return true;
}

bool ShaderFileResourceLoader::discardRawBufferAfterLoad() const
{
	return false;
}

bool ShaderFileResourceLoader::addNullZero() const
{
	return true;
}

}
