/**********************************************************************************
* 
* Light.h
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

#ifndef __JAM_LIGHT_H__
#define __JAM_LIGHT_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/Color.h>
#include <jam/Bank.h>
#include <jam/Singleton.h>
#include <jam/core/geom.h>

namespace jam
{

/*!
	\class Light
*/
class JAM_API Light : public BankItem
{
public:
	enum class Type {
		DIRECTIONAL = 1,
		POINT,
		SPOT
	};

	Light() ;

	void				update( int16_t idx ) ;
	void				dump() ;

	void				setType(Type type) ;
	void				setPosition( const Vector3& position ) ;
	void				setDirection( const Vector3& direction ) ;
	void				setConstantAttenuationFactor( float constantFactor ) ;	
	void				setLinearAttenuationFactor( float linearFactor ) ;	
	void				setQuadraticAttenuationFactor( float quadraticFactor ) ;
	void				setAmbientColor( const Color& ambient ) ;
	void				setDiffuseColor( const Color& diffuse ) ;
	void				setSpecularColor( const Color& specular ) ;

	Type				getType() const ;
	Vector3			getPosition() const ;
	Vector3			getDirection() const ;
	float				getConstantAttenuationFactor() const ;
	float				getLinearAttenuationFactor() const ;
	float				getQuadraticAttenuationFactor() const ;
	Color				getAmbientColor() const ;
	Color				getDiffuseColor() const ;
	Color				getSpecularColor() const ;

private:
	Type				m_type ;
	Vector3			m_position ;
	Vector3			m_direction ;

	// attenuation triplet for point lights
	float				m_constant ;
	float				m_linear ;
	float				m_quadratic ;  

	Color				m_ambient ;
	Color				m_diffuse ;
	Color				m_specular ; 
};			

JAM_INLINE Light::Type	Light::getType() const { return m_type; }
JAM_INLINE Vector3	Light::getPosition() const { return m_position; }
JAM_INLINE Vector3	Light::getDirection() const { return m_direction; }
JAM_INLINE float		Light::getConstantAttenuationFactor() const { return m_constant; }
JAM_INLINE float		Light::getLinearAttenuationFactor() const { return m_linear; }
JAM_INLINE float		Light::getQuadraticAttenuationFactor() const { return m_quadratic; }
JAM_INLINE Color		Light::getAmbientColor() const { return m_ambient; }
JAM_INLINE Color		Light::getDiffuseColor() const { return m_diffuse; }
JAM_INLINE Color		Light::getSpecularColor() const { return m_specular; }

class JAM_API LightManager : public Bank<Light>, public jam::Singleton<LightManager>
{
	friend class jam::Singleton<LightManager> ;

public:

protected:
	LightManager() : Bank<Light>() {}
private:
};

JAM_INLINE LightManager& GetLightMgr() { return (LightManager&) LightManager::getSingleton(); }

}

#endif

