/**********************************************************************************
* 
* PSys_Particle.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2020 Giovanni Zito, Gianluca Sclano
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

#ifndef PSysParticle_h__
#define PSysParticle_h__

#include <jam/Application.h>

class Particle3D;
class PSysEmitter;
// ******************************************************************************
// *** PARTICLE COMPONENTS
// ******************************************************************************

// ******************************************************************************
class PSysIComponent
{
public:
	PSysIComponent():status(0), handle(0){ }
	virtual ~PSysIComponent(){};
	virtual void update()=0;
	int status;
	bool handle;
};

// ******************************************************************************
class PSysPosition: public PSysIComponent
{
public:
	PSysPosition(): x(0),y(0),z(0),dx(0),dy(0),dz(0),xMin(0),yMin(0),zMin(0),endX(0),endY(0),endZ(0){  }

	virtual ~PSysPosition() {}
	void setPositionX(float _x,float _dx=0, float _xMax=PSYS_NULL)
	{
		x = _x; 
		xMin=_x;
		endX=(_xMax!=PSYS_NULL) ?_xMax : _x;
		dx= _dx;
	}
	void setPositionY(float _y,float _dy=0, float _yMax=PSYS_NULL)
	{
		y = _y; 
		yMin=_y;
		endY=(_yMax!=PSYS_NULL) ?_yMax : _y;
		dy= _dy;
	}
	void setPositionZ(float _z,float _dz=0, float _zMax=PSYS_NULL)
	{
		z = _z; 
		zMin=_z;
		endZ=(_zMax!=PSYS_NULL) ?_zMax : _z;
		dz= _dz;
	}
	
	void setPosition(float _x, float _y, float _z)
	{
		x=_x; y=_y; z=_z;
	}

	void setSpeed(float _dx=0, float _dy=0, float _dz=0)
	{
		dx=_dx; dy=_dy; dz=_dz;
	}
	virtual void update()
	{
		float timeSinceLastFrame = jam::Application::getSingleton().getElapsed() ;
		if (timeSinceLastFrame)
			x+=dx*timeSinceLastFrame; y+=dy*timeSinceLastFrame; z+=dz*timeSinceLastFrame;
		/*x=LIMIT(x+dx,xMin,xMax);
		y=LIMIT(y+dy,yMin,yMax);
		z=LIMIT(z+dz,zMin,zMax);*/
	}
	float x,y,z;
	float dx,dy,dz;
	float xMin,yMin,zMin;
	float endX,endY,endZ;
	
};
// ******************************************************************************
class PSysSize: public PSysIComponent
{
public:
	PSysSize():	Scale(1),dScale(0),minScale(1),maxScale(1){  }
	virtual ~PSysSize(){};
	
	void setZoom(float _scale,float _dScale=0, float _endScale=PSYS_NULL)
	{
		Scale = _scale; 
		minScale=_scale;
		maxScale =(_endScale!=PSYS_NULL) ?_endScale : _scale;
		dScale = _dScale;
	}

	virtual void update()
	{
		float timeSinceLastFrame   = jam::Application::getSingleton().getElapsed() ;
		Scale= Scale + dScale*timeSinceLastFrame;
		/*Scale=LIMIT(Scale+dScale,minScale,maxScale);*/
	}

	float Scale,dScale;
	float minScale,maxScale;
};
// ******************************************************************************
class PSysAlpha: public PSysIComponent
{
public:
	PSysAlpha():	alpha(1),stepAlpha(0),endAlpha(1){  }
	virtual ~PSysAlpha(){};

	void setTransparency(float _alpha,float _dAlpha=0, float _alphaMax=PSYS_NULL)
	{
		alpha = _alpha; 
		startAlpha=_alpha;
		endAlpha =(_alphaMax!=PSYS_NULL) ?_alphaMax : alpha;
		stepAlpha = _dAlpha;
	}

	virtual void update()
	{
		float timeSinceLastFrame  = jam::Application::getSingleton().getElapsed() ;
		alpha +=stepAlpha*timeSinceLastFrame;
		/*alpha=LIMIT(alpha+stepAlpha,startAlpha,endAlpha);*/
	}

	float alpha,stepAlpha;
	float startAlpha,endAlpha;
//	float oldA;
	//float fadeNext;  float fadeFar;
};
// ******************************************************************************
class PSysAngle: public PSysIComponent
{
public:
	PSysAngle():alfa(1),rotation(0),minAlfa(0),maxAlfa(0){  }
	virtual ~PSysAngle(){};

	void setRotation(float _alfa,float _dAlfa=0, float _alfaMax=PSYS_NULL)
	{
		alfa = _alfa; 
		minAlfa=_alfa;
		maxAlfa =(_alfaMax!=PSYS_NULL) ?_alfaMax : _alfa;
		rotation = _dAlfa;
	}
	virtual void update()
	{
		float timeSinceLastFrame   = jam::Application::getSingleton().getElapsed() ;
		alfa=alfa+rotation*timeSinceLastFrame;
		/*alfa=WRAPf(alfa+rotation,minAlfa,maxAlfa);*/
	}

	float alfa,rotation,minAlfa,maxAlfa;
};
// ******************************************************************************
class PSysColor: public PSysIComponent
{
public:
	PSysColor():	R(1),stepR(0),endR(1),
					G(1),stepG(0),endG(1),
					B(1),stepB(0),endB(1)
	{  }
	virtual ~PSysColor(){};

	void setColor(float _R, float _G, float _B, float _dR=0,float _dG=0, float _dB=0, float _eR=1, float _eG=1, float _eB=1)
	{
		R = _R; 
		endR=_eR;
		stepR = _dR;

		G = _G; 
		endG=_eG;
		stepG = _dG;

		B = _B; 
		endB=_eB;
		stepB = _dB;
		
	}

	virtual void update()
	{
		float timeSinceLastFrame   = jam::Application::getSingleton().getElapsed() ;
		R+=stepR*timeSinceLastFrame;
		G+=stepG*timeSinceLastFrame;
		B+=stepB*timeSinceLastFrame;
		/*alpha=LIMIT(alpha+stepAlpha,startAlpha,endAlpha);*/
	}

	float R,stepR,endR;
	float G,stepG,endG;
	float B,stepB,endB;
	
};
// ******************************************************************************
// *** PARTICLE INTERFACE
// ******************************************************************************
class PSysIParticle
{
public:
	PSysIParticle():ic(0),status(1), duration(0),durationRange(0),counter(0),startCounter(0),loops(0),emitted(0),emitter(0)	{};
	virtual ~PSysIParticle();
	virtual void update()=0;
	virtual void emit()=0;
	virtual void destroy()=0;
	virtual void set()=0;

	PSysEmitter* setEmitter() const { return emitter; }
	void getEmitter(PSysEmitter* val) { emitter = val; }

	int status;
	float duration ; 
	float durationRange;
	int counter ;
	int startCounter;				// dead or alive status
	int loops;						// Regenerations loop [0=infinite,n]

	bool emitted;		
	PSysEmitter* emitter; 	

private:
	size_t ic;
};

// ******************************************************************************
// ***
// ******************************************************************************
// ******************************************************************************
// ******************************************************************************
// ******************************************************************************
class PSysSimpleParticle : public PSysIParticle
{
public:
	PSysSimpleParticle():ic(0),status(1){};
	virtual ~PSysSimpleParticle()
	{
		destroy();
	}


	virtual void update()
	{
		if (status)
		{
			for(ic=0; ic<component.size();ic++)
			{
				component[ic]->update();
			}
		}
	}
	virtual void set()
	{
		//todo
	}

	virtual void emit()
	{
		//todo
	}

	virtual void destroy()
	{
		//todo
		for(ic=0; ic<component.size();ic++)
		{
			JAM_DELETE(component[ic]);
		}
		component.clear();
		//emitter->RemoveParticle(*this);
	}

	int status;
	std::vector<PSysIComponent*> component;
	int duration ; int durationRange;
	int counter ;
	int startCounter;				// dead or alive status
	int loops;						// Regenerations loop [0=infinite,n]
	bool emitted;		
	PSysEmitter* emitter; 	

private:
	size_t ic;
};


#endif // PSysParticle_h__