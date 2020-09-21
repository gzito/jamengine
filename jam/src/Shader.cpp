/**********************************************************************************
* 
* Shader.cpp
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

#include "stdafx.h"

#include "jam/Shader.h"
//#include "jam/Utilities.h"
#include "jam/ResourceManager.h"
#include "jam/Application.h"

#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

namespace jam
{

//
// GPUProgram
//

Shader::Shader() :
    m_object(0),
	m_shaderFiles()
{
}

Shader::~Shader() {
    //might be 0 if ctor fails by throwing exception
	glUseProgram(0) ;
    if( m_object != 0 ) glDeleteProgram(m_object);
}

//void Shader::load(const String& filename)
//{
//	std::vector<Ref<ShaderFile>> myShaders ;
//
//	Ref<ShaderFile> shaderFile ;
//	
//	shaderFile = ShaderFile::shaderFromFile(appendPath(DEFAULT_SHADERS_PATH, filename+".vert"),GL_VERTEX_SHADER) ;
//	myShaders.push_back(shaderFile) ;
//
//	shaderFile = ShaderFile::shaderFromFile(appendPath(DEFAULT_SHADERS_PATH, filename+".frag"),GL_FRAGMENT_SHADER) ;
//	myShaders.push_back(shaderFile) ;
//
//	setShaderFiles( myShaders ) ;
//	setFullPathName( appendPath(DEFAULT_SHADERS_PATH, filename) ) ;
//	compile() ;
//}

void Shader::setShaderFiles( const std::vector<Ref<ShaderFile>>& shaderFiles )
{
	m_shaderFiles = shaderFiles ;
}

void Shader::compile()
{
    if( m_shaderFiles.size() == 0 )
        JAM_ERROR("No shaders were provided to create the program");
    
    //create the program object
    m_object = glCreateProgram();
    if(m_object == 0)
        JAM_ERROR("glCreateProgram failed");
    
    //attach all the shaders
    for(unsigned i = 0; i < m_shaderFiles.size(); ++i)
        glAttachShader(m_object, m_shaderFiles[i]->objectID());

	JAM_TRACE( "Linking program \"%s\"\n", getName().c_str() ) ;

    //link the shaders together
    glLinkProgram(m_object);
    
    //detach all the shaders
    for(unsigned i = 0; i < m_shaderFiles.size(); ++i)
        glDetachShader(m_object, m_shaderFiles[i]->objectID());

	m_shaderFiles.clear() ;
    
    //throw exception if linking failed
    GLint status;
    glGetProgramiv(m_object, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(m_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteProgram(m_object); m_object = 0;
        JAM_ERROR(msg.c_str());
    }
}

void Shader::setModelMatrix(const Matrix4& mat)
{
	GLint modelMatrix_locIdx = uniformLocation(JAM_PROGRAM_UNIFORM_MODEL_MATRIX) ;
	if( modelMatrix_locIdx != -1 ) {
		glUniformMatrix4fv( modelMatrix_locIdx, 1, GL_FALSE, glm::value_ptr(mat) ) ;
	}

	GLint normalMatrix_locIdx = uniformLocation(JAM_PROGRAM_UNIFORM_NORMAL_MATRIX) ;
	if( normalMatrix_locIdx != -1 ) {
		Matrix3 normalMatrix = Matrix3( glm::transpose( glm::inverse(mat) ) ) ;
		glUniformMatrix3fv( normalMatrix_locIdx, 1, GL_FALSE, glm::value_ptr(normalMatrix) ) ;
	}
}
	
void Shader::setViewMatrix( const Matrix4& mat )
{
	setUniformSafe( JAM_PROGRAM_UNIFORM_VIEW_MATRIX, mat ) ;
}
	
void Shader::setProjectionMatrix( const Matrix4& mat )
{
	setUniformSafe(JAM_PROGRAM_UNIFORM_PROJ_MATRIX,mat) ;
}

void Shader::setNormalMatrix(const Matrix3 & mat)
{
	setUniformSafe(JAM_PROGRAM_UNIFORM_NORMAL_MATRIX,mat) ;
}

void Shader::setViewPosition(const Vector3 & pos)
{
	setUniformSafe( JAM_PROGRAM_UNIFORM_VIEW_POS, pos ) ;
}


GLuint Shader::objectID() const {
    return m_object;
}

void Shader::use() const {
	JAM_ASSERT_MSG( m_object != 0, "GPU program is null" ) ;
	if( !isInUse() ) {
		glUseProgram(m_object);
		GetShaderMgr().setCurrent( const_cast<Shader*>(this) ) ;
	}
}

bool Shader::isInUse() const {
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return ( currentProgram == (GLint)m_object ) && ( GetShaderMgr().getCurrent() == this );
}

void Shader::stopUsing() const {
    glUseProgram(0);
	GetShaderMgr().setCurrent( nullptr ) ;
}

GLint Shader::attrib(const GLchar* attribName) const {
    if(!attribName)
        JAM_ERROR("attribName was NULL");
    
    GLint attrib = glGetAttribLocation(m_object, attribName);
    if(attrib == -1)
        JAM_ERROR("Program attribute not found: %s", attribName);
    
    return attrib;
}

GLint Shader::uniform(const GLchar* uniformName) const {
    if(!uniformName)
        JAM_ERROR("uniformName was NULL");
    
    GLint uniform = glGetUniformLocation(m_object, uniformName);
    if(uniform == -1)
        JAM_ERROR("Program uniform not found: %s", uniformName);
    
    return uniform;
}

GLint Shader::attribLocation(const GLchar* attribName) const {
    if(!attribName)
        JAM_ERROR("attribName was NULL");
    
    return glGetAttribLocation(m_object, attribName);
}

GLint Shader::uniformLocation(const GLchar* uniformName) const {
    if(!uniformName)
        JAM_ERROR("uniformName was NULL");
    
    return glGetUniformLocation(m_object, uniformName);
}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(name), v0); } \
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(name), v0, v1); } \
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(name), v0, v1, v2); } \
    void Shader::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(name), v0, v1, v2, v3); } \
\
    void Shader::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Shader::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Shader::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(name), v); } \
    void Shader::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
        { assert(isInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(name), v); } \
\
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0) \
        { assert(isInUse()); glUniform1 ## TYPE_SUFFIX (uniform(name), v0); } \
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(isInUse()); glUniform2 ## TYPE_SUFFIX (uniform(name), v0, v1); } \
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(isInUse()); glUniform3 ## TYPE_SUFFIX (uniform(name), v0, v1, v2); } \
    void Shader::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(isInUse()); glUniform4 ## TYPE_SUFFIX (uniform(name), v0, v1, v2, v3); } \
\
    void Shader::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform1 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Shader::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform2 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Shader::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform3 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
    void Shader::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(isInUse()); glUniform4 ## TYPE_SUFFIX ## v (uniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void Shader::setUniformSafe(const GLchar* uniformName, GLint v0)
{
    JAM_ASSERT(isInUse());
    if(!uniformName) {
        JAM_ERROR("uniformName was NULL");
	}
    
    GLint loc = glGetUniformLocation(m_object, uniformName);
	if( loc >= 0 ) {
		glUniform1i( loc, v0 ) ;
	}
#ifdef JAM_TRACE_SHADERS
	else {
		JAM_TRACE( "Info: undeclared uniform \"%s\" in shader \"%s\"\n", uniformName, getName().c_str() ) ;
	}
#endif
}

void Shader::setUniformSafe(const GLchar* uniformName, const Matrix4& m, GLboolean transpose)
{
    JAM_ASSERT(isInUse());
    if(!uniformName) {
        JAM_ERROR("uniformName was NULL");
	}
    
    GLint loc = glGetUniformLocation(m_object, uniformName);
	if( loc >= 0 ) {
		glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(m));
	}
#ifdef JAM_TRACE_SHADERS
	else {
		JAM_TRACE( "Info: undeclared uniform \"%s\" in shader \"%s\"\n", uniformName, getName().c_str() ) ;
	}
#endif
}

void Shader::setUniformSafe(const GLchar* uniformName, const Vector3& v)
{
    JAM_ASSERT(isInUse());
    if(!uniformName) {
        JAM_ERROR("uniformName was NULL");
	}
    
    GLint loc = glGetUniformLocation(m_object, uniformName);
	if( loc >= 0 ) {
		glUniform3fv( loc, 1, glm::value_ptr(v) ) ;
	}
#ifdef JAM_TRACE_SHADERS
	else {
		JAM_TRACE( "Info: undeclared uniform \"%s\" in shader \"%s\"\n", uniformName, getName().c_str() ) ;
	}
#endif
}

void Shader::setUniformMatrix2(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    JAM_ASSERT(isInUse());
    glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void Shader::setUniformMatrix3(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    JAM_ASSERT(isInUse());
    glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void Shader::setUniformMatrix4(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) {
    JAM_ASSERT(isInUse());
    glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void Shader::setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose) {
    JAM_ASSERT(isInUse());
    glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* name, const Matrix3& m, GLboolean transpose) {
    JAM_ASSERT(isInUse());
    glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* name, const Matrix4& m, GLboolean transpose) {
    JAM_ASSERT(isInUse());
    glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Shader::setUniform(const GLchar* uniformName, const Vector3& v) {
    setUniform3v(uniformName, glm::value_ptr(v));
}

void Shader::setUniform(const GLchar* uniformName, const glm::vec4& v) {
    setUniform4v(uniformName, glm::value_ptr(v));
}

void Shader::setVertexAttribPointer (const GLchar* attribName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
	glVertexAttribPointer( attrib(attribName), size, type, normalized, stride, pointer ) ;
}

void Shader::setVertexAttribIntegerPointer(const GLchar* attribName, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	glVertexAttribIPointer( attrib(attribName), size, type, stride, pointer ) ;
}

void Shader::setVertexAttribDoublePointer(const GLchar* attribName, GLint size, GLsizei stride, const GLvoid * pointer)
{
	glVertexAttribLPointer( attrib(attribName), size, GL_DOUBLE, stride, pointer ) ;
}

void Shader::enableVertexAttribArray(const GLchar* attribName)
{
	glEnableVertexAttribArray( attrib(attribName) ) ;
}

void Shader::disableVertexAttribArray(const GLchar* attribName)
{
	glDisableVertexAttribArray( attrib(attribName) ) ;
}

void Shader::dump()
{
	GLuint programme = objectID();

	printf("--------------------\nshader programme %i info:\n",programme);
	int params = -1;
	glGetProgramiv(programme,GL_LINK_STATUS,&params);
	printf("GL_LINK_STATUS = %i\n",params);

	glGetProgramiv(programme,GL_ATTACHED_SHADERS,&params);
	printf("GL_ATTACHED_SHADERS = %i\n",params);

	glGetProgramiv(programme,GL_ACTIVE_ATTRIBUTES,&params);
	printf("GL_ACTIVE_ATTRIBUTES = %i\n",params);
	for(int i = 0; i < params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib(
			programme,
			i,
			max_length,
			&actual_length,
			&size,
			&type,
			name
		);
		if(size > 1) {
			for(int j = 0; j < size; j++) {
				char long_name[64];
				sprintf(long_name,"%s[%i]",name,j);
				int location = glGetAttribLocation(programme,long_name);
				printf("  %i) type:%s name:%s location:%i\n",
					   i,GL_type_to_string(type),long_name,location);
			}
		} else {
			int location = glGetAttribLocation(programme,name);
			printf("  %i) type:%s name:%s location:%i\n",
				   i,GL_type_to_string(type),name,location);
		}
	}

	glGetProgramiv(programme,GL_ACTIVE_UNIFORMS,&params);
	printf("GL_ACTIVE_UNIFORMS = %i\n",params);
	for(int i = 0; i < params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform(
			programme,
			i,
			max_length,
			&actual_length,
			&size,
			&type,
			name
		);
		if(size > 1) {
			for(int j = 0; j < size; j++) {
				char long_name[64];
				sprintf(long_name,"%s[%i]",name,j);
				int location = glGetUniformLocation(programme,long_name);
				printf("  %i) type:%s name:%s location:%i\n",
					   i,GL_type_to_string(type),long_name,location);
			}
		} else {
			int location = glGetUniformLocation(programme,name);
			printf("  %i) type:%s name:%s location:%i\n",
				   i,GL_type_to_string(type),name,location);
		}
	}

	dumpInfoLog();
}

void Shader::dumpInfoLog()
{
	int max_length = 2048;
	int actual_length = 0;
	char program_log[2048];
	glGetProgramInfoLog(objectID(),max_length,&actual_length,program_log);
	printf("program info log for GL index %u:\n%s",objectID(),program_log);
}

const char* Shader::GL_type_to_string(GLenum type)
{
	switch(type) {
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
	}
	return "other";
}

//
// ShaderManager
//

const String ShaderManager::DEFAULT_PROGRAM_UNLIT_NAME = "default_shader_unlit" ;
const String ShaderManager::DEFAULT_PROGRAM_LIT_NAME = "default_shader_lit" ;
const String ShaderManager::SKINNIG_PROGRAM_LIT_NAME = "skinning_shader_lit" ;
const String ShaderManager::SKYBOX_PROGRAM_NAME = "skybox_shader" ;
const String ShaderManager::NORMAL_MAPPING_PROGRAM_NAME = "normal_mapping_shader" ;
const String ShaderManager::SCREEN_PROGRAM_NAME = "screen_shader" ;
// TODO: FIXIT !!!
//const String ShaderManager::DEFAULT_SHADERS_PATH	= "../../../../jam/shaders" ;
const String ShaderManager::DEFAULT_SHADERS_PATH	= "../../jam/shaders" ;

ShaderManager::ShaderManager() : 
	m_pCurrentShader(nullptr)
{
}

void ShaderManager::createDefaultUnlit()
{
	loadAndCreateProgram(DEFAULT_PROGRAM_UNLIT_NAME) ;
}

void ShaderManager::createDefaultLit()
{
	loadAndCreateProgram(DEFAULT_PROGRAM_LIT_NAME) ;
}

void ShaderManager::createSkinningLit()
{
	loadAndCreateProgram(SKINNIG_PROGRAM_LIT_NAME) ;
}

void ShaderManager::createSkyBox()
{
	loadAndCreateProgram(SKYBOX_PROGRAM_NAME) ;
}

void ShaderManager::createNormalMapping()
{
	loadAndCreateProgram(NORMAL_MAPPING_PROGRAM_NAME) ;
}

void ShaderManager::createScreen()
{
	loadAndCreateProgram(SCREEN_PROGRAM_NAME) ;
}

Shader*	ShaderManager::getDefaultUnlit()
{
	return getShader(DEFAULT_PROGRAM_UNLIT_NAME) ;
}

Shader*	ShaderManager::getDefaultLit()
{
	return getShader(DEFAULT_PROGRAM_LIT_NAME) ;
}

Shader*	ShaderManager::getSkinningLit()
{
	return getShader(SKINNIG_PROGRAM_LIT_NAME) ;
}

Shader* ShaderManager::getSkyBox()
{
	return getShader(SKYBOX_PROGRAM_NAME) ;
}

Shader* ShaderManager::getNormalMapping()
{
	return getShader(NORMAL_MAPPING_PROGRAM_NAME) ;
}

Shader* ShaderManager::getScreen()
{
	return getShader(SCREEN_PROGRAM_NAME) ;
}

Shader* ShaderManager::getShader(const String& name)
{
	try {
		return getObject(name) ;
	}
	catch( std::out_of_range& e ) {
		JAM_ERROR("Shader \"%s\" not found", name.c_str()) ;
	}
}

void ShaderManager::loadAndCreateProgram( const String& shaderName )
{
	std::vector<Ref<ShaderFile>> shadersFiles ;

	Resource vsResource( shaderName + ".vert" ) ;
	ResHandle* vsHandle( Application::getSingleton().getResourceManager().getHandle(&vsResource) ) ;
	Ref<ShaderFile> rVertexShader( ShaderFile::shaderFromFile(vsHandle,GL_VERTEX_SHADER), true ) ;
	shadersFiles.push_back(rVertexShader) ;

	Resource psResource( shaderName + ".frag" ) ;
	ResHandle* psHandle( Application::getSingleton().getResourceManager().getHandle(&psResource) ) ;
	Ref<ShaderFile> rPixelShader( ShaderFile::shaderFromFile(psHandle,GL_FRAGMENT_SHADER), true ) ;
	shadersFiles.push_back(rPixelShader) ;

	Shader* program( new Shader() ) ;
	program->setName(shaderName) ;
	program->setShaderFiles( shadersFiles ) ;
	program->compile() ;

	addObject( program ) ;
}

Shader*	ShaderManager::getCurrent()
{
	JAM_ASSERT_MSG( (m_pCurrentShader != nullptr), "No current shader" ) ;
	return m_pCurrentShader ;
}

void ShaderManager::setCurrent( Shader* pShader)
{
	m_pCurrentShader = pShader ;
}

}
