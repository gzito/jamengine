/**********************************************************************************
* 
* Material.h
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

#ifndef __JAM_MATERIAL_H__
#define __JAM_MATERIAL_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/Color.h>
#include <jam/Texture2D.h>
#include <jam/Shader.h>
#include <jam/Object.h>

namespace jam
{
class JAM_API BlendMode
{
public:
	BlendMode() ;
	BlendMode( GLenum srcFactor, GLenum destFactor, GLenum blendEquation = GL_FUNC_ADD ) ;

	void					setBlendFunc( GLenum srcFactor, GLenum destFactor ) ;
	void					setBlendEquation(GLenum val) ;

	GLenum					getBlendFuncSrcFactor() const ;
	GLenum					getBlendFuncDstFactor() const ;
	GLenum					getBlendEquation() const ;

	static const BlendMode  Normal ;
	static const BlendMode  AlphaBlend ;
	static const BlendMode	Additive ;
	static const BlendMode	Multiplicative ;

private:
	GLenum					m_blendFuncSrcFactor ;
	GLenum					m_blendFuncDstFactor ;
	GLenum					m_blendEquation ;
};

/*!
	\class Material
		
	Materials describe how polygons reflect light or appear to emit light in a 3-D scene.
	Essentially, a material is a set of properties that tell OpenGL the	following things
	about the polygons it is rendering. 

	1) How they reflect ambient and diffuse light 
	2) What their specular highlights look like 
	3) Whether the polygons appear to emit light
*/
class JAM_API Material : public NamedObject
{
public:
	Material() ;
		
	void					setAmbientColor( const Color& color ) ;
	void					setDiffuseColor( const Color& color ) ;
	void					setAmbientAndDiffuseColor( const Color& color ) ;
	void					setSpecularColor( const Color& color ) ;
	void					setEmissiveColor( const Color& color ) ;
	void					setShininess( float shininess ) ;
	void					setDiffuseTexture( Texture2D* tex ) ;
	void					setSpecularTexture( Texture2D* tex ) ;
	void					setNormalTexture( Texture2D* tex ) ;

	void					setBlendEnabled(bool fval) ;
	void					setBlendMode( const BlendMode& blendMode ) ;
	void					setShader(Shader* val) ;

	Color					getAmbientColor() const ;
	Color					getDiffuseColor() const ;
	Color					getSpecularColor() const ;
	float					getShininess() const ;
	Texture2D*				getDiffuseTexture() ;
	Texture2D*				getSpecularTexture() ;
	Texture2D*				getNormalTexture() ;

	bool					getBlendEnabled() const ;
	BlendMode				getBlendMode() const ;
	Shader*					getShader() const ;

	void					bind() ;
	void					unbind() ;
	
private:
	Color					m_ambient ;
	Color					m_diffuse ;
	Color					m_specular ;
	float					m_shininess ;
	Color					m_emissive ;

	bool					m_blendEnabled ;
	BlendMode				m_blendMode ;

	// texture stages
	Texture2D*				m_diffuseTexture ;		// stage 0
	Texture2D*				m_specularTexture ;		// stage 1
	Texture2D*				m_normalTexture ;		// stage 2

	Shader*					m_pShader ;
};

JAM_INLINE Color			Material::getAmbientColor() const { return m_ambient; }
JAM_INLINE Color			Material::getDiffuseColor() const { return m_diffuse; }
JAM_INLINE Color			Material::getSpecularColor() const { return m_specular; }
JAM_INLINE float			Material::getShininess() const { return m_shininess; }
JAM_INLINE bool				Material::getBlendEnabled() const { return m_blendEnabled; }
JAM_INLINE BlendMode		Material::getBlendMode() const { return m_blendMode; }
JAM_INLINE void				Material::setBlendEnabled(bool fval) { m_blendEnabled = fval; }

JAM_INLINE GLenum			BlendMode::getBlendFuncSrcFactor() const { return m_blendFuncSrcFactor; }
JAM_INLINE GLenum			BlendMode::getBlendFuncDstFactor() const { return m_blendFuncDstFactor; }

JAM_INLINE GLenum			BlendMode::getBlendEquation() const { return m_blendEquation; }
JAM_INLINE void				BlendMode::setBlendEquation(GLenum val) { m_blendEquation = val; }


class JAM_API MaterialManager : public NamedObjectManager<Material>, public jam::Singleton<MaterialManager>
{
	friend class jam::Singleton<MaterialManager> ;

public:

protected:
	MaterialManager() = default ;
private:
};

JAM_INLINE  MaterialManager& GetMaterialMgr() { return (MaterialManager&) MaterialManager::getSingleton(); }

}

#endif

