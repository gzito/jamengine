/**********************************************************************************
* 
* Shader.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito.
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
*
* Based on tdogl::Shader class by Thomas Dalling - https://github.com/tomdalling/opengl-series
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

#ifndef __JAM_SHADER_H__
#define __JAM_SHADER_H__

#include <jam/ShaderFile.h>
#include <jam/Singleton.h>
#include <jam/core/geom.h>

#include <vector>
#include <unordered_map>


#define JAM_PROGRAM_ATTRIB_POSITION						"in_Position"
#define JAM_PROGRAM_ATTRIB_NORMAL						"in_Normal"
#define JAM_PROGRAM_ATTRIB_COLOR						"in_Color"
#define JAM_PROGRAM_ATTRIB_TEXCOORDS					"in_TexCoords"
#define JAM_PROGRAM_ATTRIB_TANGENT						"in_Tangent"
#define JAM_PROGRAM_ATTRIB_BITANGENT					"in_Bitangent"
#define JAM_PROGRAM_ATTRIB_BONESID						"in_BonesId"
#define JAM_PROGRAM_ATTRIB_WEIGHTS						"in_Weights"

#define JAM_PROGRAM_UNIFORM_MATERIAL_SHININESS			"material_shininess"
#define JAM_PROGRAM_UNIFORM_MATERIAL_DIFFUSE			"material_diffuse"
#define JAM_PROGRAM_UNIFORM_MATERIAL_SPECULAR			"material_specular"
#define JAM_PROGRAM_UNIFORM_MATERIAL_NORMAL				"material_normal"

#define JAM_PROGRAM_UNIFORM_LIGHT_TYPE					"lights[%d].type"
#define JAM_PROGRAM_UNIFORM_LIGHT_POSITION				"lights[%d].position"
#define JAM_PROGRAM_UNIFORM_LIGHT_DIRECTION				"lights[%d].direction"
#define JAM_PROGRAM_UNIFORM_LIGHT_CONSTANT				"lights[%d].constant"
#define JAM_PROGRAM_UNIFORM_LIGHT_LINEAR				"lights[%d].linear"
#define JAM_PROGRAM_UNIFORM_LIGHT_QUADRATIC				"lights[%d].quadratic"
#define JAM_PROGRAM_UNIFORM_LIGHT_AMBIENT				"lights[%d].ambient"
#define JAM_PROGRAM_UNIFORM_LIGHT_DIFFUSE				"lights[%d].diffuse"
#define JAM_PROGRAM_UNIFORM_LIGHT_SPECULAR				"lights[%d].specular"

#define JAM_PROGRAM_UNIFORM_BONES						"bones[%d]"

#define JAM_PROGRAM_UNIFORM_VIEW_POS					"viewPos"

#define JAM_PROGRAM_UNIFORM_MODEL_MATRIX				"modelMatrix"
#define JAM_PROGRAM_UNIFORM_VIEW_MATRIX					"viewMatrix"
#define JAM_PROGRAM_UNIFORM_PROJ_MATRIX					"projMatrix"
#define JAM_PROGRAM_UNIFORM_NORMAL_MATRIX				"normalMatrix"

namespace jam
{

/**
    Represents an OpenGL program made by linking shaders.
*/
class JAM_API Shader : public RefCountedObject
{ 
public:
	/**
		Creates a program by linking a list of ShaderFile objects
		@param shaders  The shaders to link together to make the program
		@throws std::exception if an error occurs.
	*/
							Shader() ;
	virtual					~Shader() ;
        
	void					setShaderFiles( const std::vector<Ref<ShaderFile>>& shaderFiles ) ;

	void					compile() ;

	void					setModelMatrix( const Matrix4& mat ) ;
	void					setViewMatrix( const Matrix4& mat ) ;
	void					setProjectionMatrix( const Matrix4& mat ) ;
	void					setNormalMatrix( const Matrix3& mat ) ;
	void					setViewPosition( const Vector3& pos ) ;

    /**
        @result The program's object ID, as returned from glCreateProgram
        */
    GLuint					objectID() const ;

    void					use() const ;

    bool					isInUse() const ;

    void					stopUsing() const ;

	const String&			getName() const ;
	void					setName( const String& name ) ;

public:
    /**
        @result The attribute index for the given name, as returned from glGetAttribLocation.
		@remark This method will throw an exception if the given attribute doesn't exist
        */
    GLint					attrib(const GLchar* attribName) const ;
        
        
    /**
        @result The uniform index for the given name, as returned from glGetUniformLocation.
		@remark This method will throw an exception if the given uniform doesn't exist
        */
    GLint					uniform(const GLchar* uniformName) const ;

	/**
        @result The attribute index if the program has the given attribute name, otherwise returns -1
        */
	GLint					attribLocation(const GLchar* attribName) const ;

	/**
        @result The uniform location index if the program has the given uniform name, otherwise returns -1
        */
    GLint					uniformLocation(const GLchar* uniformName) const ;

	/**
        Setters for attribute and uniform variables.
        These are convenience methods for the glVertexAttrib* and glUniform* functions.
        */
#define _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
    void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
\
    void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
    void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

    _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
    _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
    _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
    _TDOGL_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

    void					setUniformSafe(const GLchar* uniformName, GLint v0);
    void					setUniformSafe(const GLchar* uniformName, const Matrix4& m, GLboolean transpose=GL_FALSE);
    void					setUniformSafe(const GLchar* uniformName, const Vector3& v);

    void					setUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void					setUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void					setUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
    void					setUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose=GL_FALSE);
    void					setUniform(const GLchar* uniformName, const Matrix3& m, GLboolean transpose=GL_FALSE);
    void					setUniform(const GLchar* uniformName, const Matrix4& m, GLboolean transpose=GL_FALSE);
    void					setUniform(const GLchar* uniformName, const Vector3& v);
    void					setUniform(const GLchar* uniformName, const glm::vec4& v);

	void					setVertexAttribPointer			(const GLchar* attribName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) ;
	void					setVertexAttribIntegerPointer	(const GLchar* attribName, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) ;
	void					setVertexAttribDoublePointer	(const GLchar* attribName, GLint size, GLsizei stride, const GLvoid* pointer) ;

	void					enableVertexAttribArray(const GLchar* attribName) ;
	void					disableVertexAttribArray(const GLchar* attribName) ;

	void					dump();
	void					dumpInfoLog();

	static const char*		GL_type_to_string(GLenum type);

private:
    GLuint					m_object;
	String					m_name ;
	std::vector<Ref<ShaderFile>>	m_shaderFiles ;

    //copying disabled
							Shader(const Shader&) = delete ;
    const Shader&			operator=(const Shader&) = delete ;
};


//*****************************************************************************
// ShaderManager
//
class JAM_API ShaderManager : public Singleton<ShaderManager>
{
using ShadersMap = std::unordered_map<String,Shader*> ;

public:
							ShaderManager() ;

	static const String		DEFAULT_PROGRAM_UNLIT_NAME ;
	static const String		DEFAULT_PROGRAM_LIT_NAME ;
	static const String		SKINNIG_PROGRAM_LIT_NAME ;
	static const String		SKYBOX_PROGRAM_NAME ;
	static const String		NORMAL_MAPPING_PROGRAM_NAME ;
	static const String		SCREEN_PROGRAM_NAME ;

	static const String		DEFAULT_SHADERS_PATH ;

	void					createDefaultUnlit() ;
	void					createDefaultLit() ;
	void					createSkinningLit() ;
	void					createSkyBox() ;
	void					createNormalMapping() ;
	void					createScreen() ;

	Ref<Shader>				getDefaultUnlit() ;
	Ref<Shader>				getDefaultLit() ;
	Ref<Shader>				getSkinningLit() ;
	Ref<Shader>				getSkyBox() ;
	Ref<Shader>				getNormalMapping() ;
	Ref<Shader>				getScreen() ;

	Ref<Shader>				getShader( const String& name ) ;

	void					loadAndCreateProgram( const String& shaderName ) ;

	Shader*					getCurrent() ;
	void					setCurrent( Shader* pShader ) ;

private:
	ShadersMap				m_shadersMap ;
	Shader*					m_pCurrentShader ;
};

}

#endif // ifndef __JAM_SHADER_H__
