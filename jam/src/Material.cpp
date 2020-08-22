/**********************************************************************************
* 
* Material.cpp
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

#include <jam/Material.h>

namespace jam
{

	BlendMode::BlendMode() : 
		m_blendFuncSrcFactor(GL_SRC_ALPHA), m_blendFuncDstFactor(GL_ONE_MINUS_SRC_ALPHA), m_blendEquation(GL_FUNC_ADD)
	{
	}
	
	BlendMode::BlendMode(GLenum srcFactor,GLenum destFactor,GLenum blendEquation /* = GL_FUNC_ADD */) :
		m_blendFuncSrcFactor(srcFactor), m_blendFuncDstFactor(destFactor), m_blendEquation(blendEquation)
	{

	}

	void BlendMode::setBlendFunc(GLenum srcFactor,GLenum destFactor)
	{
		m_blendFuncSrcFactor = srcFactor ;
		m_blendFuncDstFactor = destFactor ;
	}

	//*******************
	//
	// Class Material
	//
	//*******************

	/*!
		\brief Default constructor
		
		Creates a new material
	*/
	Material::Material() :
		BankItem(),
		m_ambient(51,51,51),
		m_diffuse(204,204,204),
		m_specular(Color::GRAY),
		m_shininess(0.5f),
		m_emissive(Color::BLACK),
		m_blendEnabled(false),
		m_blendMode(),
		m_diffuseTexture(nullptr),
		m_specularTexture(nullptr),
		m_normalTexture(nullptr),
		m_pShader(nullptr)
	{
		setShader( ShaderManager::getSingleton().getDefaultUnlit() ) ;
	}


	/*!
		\brief Sets material's ambient color
		Ambient reflection, like ambient light, is nondirectional. Ambient reflection has a
		lesser impact on the apparent color of a rendered object, but it does affect the
		overall color and is most noticeable when little or no diffuse light reflects off
		the material.
		Diffuse and ambient reflection work together to determine the perceived color of an
		object, and are usually identical values. For example, to render a blue crystalline
		object, you create a material that reflects only the blue component of diffuse and
		ambient light. When placed in a room with a white light, the crystal appears to be
		blue. However, in a room that has only red light, the same crystal would appear to be
		black, because its material doesn't reflect red light.
	*/
	void Material::setAmbientColor( const Color& color )
	{
		m_ambient = color ;
	}
	

	/*!
		\brief Sets material's diffuse color

		The Diffuse and Ambient properties of a material describe how it reflects the ambient
		and diffuse light in a scene. Because most scenes contain much more diffuse light
		than ambient light, diffuse reflection plays the largest part in determining color.
		Additionally, because diffuse light is directional, the angle of incidence for diffuse
		light affects the overall intensity of the reflection. Diffuse reflection is greatest
		when the light strikes a vertex parallel to the vertex normal. As the angle increases,
		the effect of diffuse reflection diminishes. The amount of light reflected is the
		cosine of the angle between the incoming light and the vertex normal.

	*/
	void Material::setDiffuseColor( const Color& color )
	{
		m_diffuse = color ;
	}
	

	void Material::setAmbientAndDiffuseColor( const Color& color )
	{
		m_ambient = color ;
		m_diffuse = color ;
	}


	/*!
		\brief Sets material's specular color

		Specular reflection creates highlights on objects, making them appear shiny.
		The material object contains two attributes that describe the specular highlight
		color as well as the material's overall shininess. You establish the color of the
		specular highlights by setting the Specular member to the desired RGBA color
		the most common colors are white or light gray. The values you set in the Power
		member control how sharp the specular effects are.

		Specular highlights can create dramatic effects. Drawing again on the blue crystal
		analogy: a larger Power value creates sharper specular highlights, making the crystal
		appear to be quite shiny. Smaller values increase the area of the effect, creating
		a dull reflection that make the crystal look frosty. To make an object truly matte,
		set the Power member to zero and the color in Specular to black. Experiment with
		different levels of reflection to produce a realistic appearance for your needs.
		
		\sa setPower
	*/
	void Material::setSpecularColor( const Color& color )
	{
		m_specular = color ;
	}
	

	/*!
		\brief Sets material's emissive color

		Materials can be used to make a rendered object appear to be self-luminous.
		The Emissive property of a material is used to describe the color and
		transparency of the emitted light. Emission affects an object's color and can,
		for example, make a dark material brighter and take on part of the emitted color.

		You can use a material's emissive property to add the illusion that an object is
		emitting light, without incurring the computational overhead of adding a light to
		the scene. In the case of the blue crystal, the emissive property is useful if you
		want to make the crystal appear to light up, but not cast light on other objects in
		the scene. Remember, materials with emissive properties don't emit light that can be
		reflected by other objects in a scene. To achieve this reflected light, you need to
		place an additional light within the scene.
	*/
	void Material::setEmissiveColor( const Color& color )
	{
		m_emissive = color ;
	}
	

	/*!
		\brief Sets material's specular power [0,128]
		\sa setSpecularColor
	*/
	void Material::setShininess( float shininess )
	{
		m_shininess = shininess ;
	}

	void Material::setDiffuseTexture( const Ref<Texture2D>& tex )
	{
		m_diffuseTexture = tex ;
	}

	void Material::setSpecularTexture( const Ref<Texture2D>& tex )
	{
		m_specularTexture = tex ;
	}

	void Material::setNormalTexture( const Ref<Texture2D>& tex )
	{
		m_normalTexture = tex ;
	}

	void Material::setBlendMode(const BlendMode& blendMode)
	{
		m_blendMode = blendMode ;
	}

	void Material::setShader(const Ref<Shader>& val)
	{
		m_pShader = val;
	}

	Ref<Texture2D> Material::getDiffuseTexture()
	{
		return m_diffuseTexture ;
	}

	Ref<Texture2D> Material::getSpecularTexture()
	{
		return m_specularTexture ;
	}

	Ref<Texture2D> Material::getNormalTexture()
	{
		return m_normalTexture ;
	}

	Ref<Shader> Material::getShader() const
	{
		return m_pShader;
	}
	
	void Material::bind()
	{
		static char uniformName[64] = { 0 } ;

		// for sprites
		if( this->getBlendEnabled() ) {
			glEnable(GL_BLEND) ;
			BlendMode bm = this->getBlendMode() ;
			glBlendFunc( bm.getBlendFuncSrcFactor(), bm.getBlendFuncDstFactor());
			glBlendEquation( bm.getBlendEquation() ) ;
			glDisable(GL_DEPTH_TEST) ;
		}

		Ref<Shader> pProgram = getShader() ;
		pProgram->use();

		GLint shininessLocIdx = pProgram->uniformLocation(JAM_PROGRAM_UNIFORM_MATERIAL_SHININESS) ;
		if( shininessLocIdx >= 0 ) {
			glUniform1f( shininessLocIdx, getShininess() ) ;
		}

		// maximum 3 texture units : diffuse, specular and normal
		Texture2D* pTex = 0 ;
		for( uint8_t idx = 0; idx < 3; idx++ )
		{
			if( idx == 0 ) {
				pTex = getDiffuseTexture().get() ;
			}
			else if( idx == 1 ) {
				pTex = getSpecularTexture().get() ;
			}
			else {
				pTex = getNormalTexture().get() ;
			}

			if( pTex ) {
				glActiveTexture( GL_TEXTURE0 + idx );	// select active texture unit
				glBindTexture( GL_TEXTURE_2D, pTex->getId() );
				if( idx == 0 ) {
					sprintf( uniformName, JAM_PROGRAM_UNIFORM_MATERIAL_DIFFUSE ) ;
				}
				else if ( idx == 1 ) {
					sprintf( uniformName, JAM_PROGRAM_UNIFORM_MATERIAL_SPECULAR ) ;
				}
				else {	// idx == 2
					sprintf( uniformName, JAM_PROGRAM_UNIFORM_MATERIAL_NORMAL ) ;
				}
				pProgram->setUniformSafe( uniformName, idx ) ;	// <--- idx is textureUnit
			}
			else { 
				// no textures used for this texture unit
				glActiveTexture( GL_TEXTURE0 + idx );	// select active texture unit
				glBindTexture( GL_TEXTURE_2D, 0 ); // no texture
			}
		}
	}

	void Material::unbind()
	{
		for( uint8_t idx = 0; idx < 3; idx++ ) {
			glActiveTexture( GL_TEXTURE0 + idx );	// select active texture unit
			glBindTexture( GL_TEXTURE_2D, 0 );
		}

		glDisable(GL_BLEND) ;
		glEnable(GL_DEPTH_TEST) ;
	}

}