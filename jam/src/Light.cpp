/**********************************************************************************
* 
* Light.cpp
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

#include <jam/Light.h>
#include <jam/Shader.h>

namespace jam
{

	//*******************
	//
	// Class Light
	//
	//*******************


	Light::Light() :
		NamedObject(),
		m_type(Type::DIRECTIONAL),
		m_position(0.0f,0.0f,0.0f),
		m_direction(0.0f,0.0f,-1.0f),
		m_constant(1.0f),
		m_linear(0.09f),
		m_quadratic(0.032f),
		m_ambient(Color::WHITE),
		m_diffuse(Color::WHITE),
		m_specular(Color::WHITE)
	{
	}

	void Light::update( int16_t idx )
	{
		static char uniformName[64] = { 0 } ;
		Shader* pShader = GetShaderMgr().getCurrent() ;

		sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_TYPE, idx ) ;
		if( pShader->uniformLocation(uniformName) >= 0 ) {
			pShader->setUniform(uniformName, (int32_t)m_type) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_POSITION, idx ) ;
			pShader->setUniform(uniformName, m_position) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_DIRECTION, idx ) ;
			pShader->setUniform(uniformName, m_direction ) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_CONSTANT, idx ) ;
			pShader->setUniform(uniformName, m_constant) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_LINEAR, idx ) ;
			pShader->setUniform(uniformName, m_linear) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_QUADRATIC, idx ) ;
			pShader->setUniform(uniformName, m_quadratic) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_AMBIENT, idx ) ;
			Vector3 ambient = Vector3(m_ambient.getFloatingComponents()) ;
			//JAM_TRACE( "%d %d %d\n", m_ambient.r, m_ambient.g, m_ambient.b ) ;
			pShader->setUniform(uniformName, ambient ) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_DIFFUSE, idx ) ;
			pShader->setUniform(uniformName, Vector3(m_diffuse.getFloatingComponents()) ) ;

			sprintf( uniformName, JAM_PROGRAM_UNIFORM_LIGHT_SPECULAR, idx ) ;
			pShader->setUniform(uniformName, Vector3(m_specular.getFloatingComponents()) ) ;
		}
	}

	void Light::dump()
	{
		JAM_TRACE( "type: %ld\n", m_type ) ;

		JAM_TRACE( "m_position: (%f,%f,%f)\n", m_position.x, m_position.y, m_position.z ) ;
		
		JAM_TRACE( "m_direction: (%f,%f,%f)\n", m_direction.x, m_direction.y, m_direction.z) ;
		
		JAM_TRACE( "m_constant: %f\n", m_constant ) ;
		
		JAM_TRACE( "m_linear: %f\n", m_linear ) ;
		
		JAM_TRACE( "m_quadratic: %f\n", m_quadratic ) ;
		
		JAM_TRACE( "m_ambient: (%d,%d,%d,%d)\n", m_ambient.r, m_ambient.g, m_ambient.b, m_ambient.a ) ;
		
		JAM_TRACE( "m_diffuse: (%d,%d,%d,%d)\n", m_diffuse.r, m_diffuse.g, m_diffuse.b, m_diffuse.a ) ;
		
		JAM_TRACE( "m_specular: (%d,%d,%d,%d)\n", m_specular.r, m_specular.g, m_specular.b, m_specular.a ) ;
		
	}

	void Light::setType(Type type)
	{
		m_type = type ;
	}

	void Light::setPosition(const Vector3 & position)
	{
		m_position = position ;
	}

	void Light::setDirection(const Vector3 & direction)
	{
		m_direction = direction ;
	}

	void Light::setConstantAttenuationFactor(float constantFactor)
	{
		m_constant = constantFactor ;
	}

	void Light::setLinearAttenuationFactor(float linearFactor)
	{
		m_linear = linearFactor ;
	}

	void Light::setQuadraticAttenuationFactor(float quadraticFactor)
	{
		m_quadratic = quadraticFactor ;
	}

	void Light::setAmbientColor(const Color & ambient)
	{
		m_ambient = ambient ;
	}

	void Light::setDiffuseColor(const Color & diffuse)
	{
		m_diffuse = diffuse ;
	}

	void Light::setSpecularColor(const Color & specular)
	{
		m_specular = specular ;
	}

}
