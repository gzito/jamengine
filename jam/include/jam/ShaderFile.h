/**********************************************************************************
* 
* ShaderFile.h
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

#ifndef __JAM_SHADERFILE_H__
#define __JAM_SHADERFILE_H__

#include <GL/glew.h>
#include <jam/jam.h>
#include <jam/ResourceManager.h>

namespace jam
{

/**
    Represents a compiled OpenGL shader.
*/
class JAM_API ShaderFile : public RefCountedObject
{ 
public:
	/**
    Creates a shader of the given type
         
    @param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
                    or GL_FRAGMENT_SHADER.
         
    @throws std::exception if an error occurs.
	*/

							ShaderFile( GLenum shaderType ) ;

    /**
        Creates a shader from a string of shader source code.
         
        @param shaderCode  The source code for the shader.
        @param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
                        or GL_FRAGMENT_SHADER.
         
        @throws std::exception if an error occurs.
    */
							ShaderFile( ResHandle* resHandle, GLenum shaderType );

	virtual					~ShaderFile() ;

	void					setSource( const String& shaderCode ) ;
	void					compile() ;

    /**
        Creates a shader from a text file.
         
        @param filePath    The path to the text file containing the shader source.
        @param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
                           or GL_FRAGMENT_SHADER.
         
        @throws std::exception if an error occurs.
        */
    static ShaderFile* shaderFromFile( ResHandle* resHandle, GLenum shaderType );
        
    /**
        @result The shader's object ID, as returned from glCreateShader
        */
    GLuint					objectID() const ;

	bool					isCompiled() const ;

	ResHandle*              getResHandle() const ;

private:
							ShaderFile( const ShaderFile& other ) = delete ;
	ShaderFile&				operator=( const ShaderFile& other ) = delete ;

private:
    GLuint					m_objectID;
	String					m_sourceCode ;
	GLenum					m_shaderType ;
	bool					m_compiled ;
	ResHandle*      		m_resHandle ;	
};
JAM_INLINE bool				ShaderFile::isCompiled() const { return m_compiled; }
JAM_INLINE ResHandle*       ShaderFile::getResHandle() const { return m_resHandle; }


class JAM_API ShaderFileResourceLoader : public IResourceLoader
{
public:
							ShaderFileResourceLoader() ;
	bool					useRawFile() const override ;
	size_t					getLoadedResourceSize( char* rawBuffer, size_t rawSize ) override ;
	bool					loadResource( char* rawBuffer, size_t rawSize, ResHandle& handle ) override ;
	bool					discardRawBufferAfterLoad() const override ;
	bool					addNullZero() const override ;
};

}


#endif // __JAM_SHADERFILE_H__
