/**********************************************************************************
* 
* Layer.h
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

#ifndef __JAM_LAYER_H__
#define __JAM_LAYER_H__

#include <jam/jam.h>
#include <jam/Node.h>
#include <jam/Color.h>

namespace jam
{

class Draw3DImageManager ;

/*!
	This class represents a 2D layer

	When adding a layer to the scene, specify z-order to set a corresponding drawing slot.
	If a z-order isn't specified, the default will be 0, i.e. the same slot of the scene
*/
class JAM_API Layer : public Node
{
public:
							Layer() ;
	virtual					~Layer() {}
	virtual const Color&	getColor() const override { return Color::WHITE; }
	virtual void			setColor( const Color& c ) override {}

protected:
	void					init() ;

							// prevents the use
							Layer( Layer& ) = delete ;
							Layer& operator=(const Layer&) = delete ;
};

class JAM_API ColorLayer : public Layer
{
public:
							ColorLayer() ;
							ColorLayer(const Color& color) ;
							ColorLayer(const Color& color, float width, float height) ;

	/// set width
	void					setWidth(float w);
	
	/// set height
	void					setHeight(float h);

	/// set width and height
	void					setWidthAndHeight(float w, float h);

	virtual void			setColor( const Color& c ) override;
	virtual const Color&	getColor() const override { return m_color ; }

	void					render();

protected:
	void					init( float w, float h ) ;

private:
							// prevents the use
							ColorLayer( ColorLayer& ) = delete ;
							ColorLayer& operator=(const ColorLayer&) = delete ;

private:
	Color					m_color ;
};

// TODO : TO BE IMPLEMENTED
class JAM_API MultiplexLayer : public Layer
{
public:
							MultiplexLayer() ;

private:
							// prevents the use
							MultiplexLayer( MultiplexLayer& ) = delete ;
							MultiplexLayer& operator=(const MultiplexLayer&) = delete ;

};


}	

#endif // __JAM_LAYER_H__
