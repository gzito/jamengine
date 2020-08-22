/**********************************************************************************
* 
* Scene.h
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

#ifndef __JAM_SCENE_H__
#define __JAM_SCENE_H__

#include <jam/jam.h>
#include <jam/Node.h>
#include <jam/InputManager.h>
#include <jam/Ring2f.h>


namespace jam
{

class Draw3DBatch ;

/*!
	This class represents a 2D Scene

	Default scene's z-order is 0, i.e. sprites added directly to scene will be drawn using the front slot nearest to the main 3D frustrum
*/
class JAM_API Scene : public Node
{
	friend class Node ;
	friend class Application ;

public:
							Scene() ;
	virtual					~Scene() {} ;

	void					init();

	void					resetFogOfView( );

	virtual void			setColor( const Color& c) {}
	virtual const Color&	getColor() const { return Color::WHITE; }

	void					visitGraph( Draw3DBatch* pBatch ) ;

	virtual void			destroy() ;

	/** Enable fog of view (default is not enabled) */
	void					enableFogOfView(bool enabled=true) { m_fogOfViewEnabled = enabled ; resetFogOfView() ;}
	bool					isFogOfViewEnabled() const { return m_fogOfViewEnabled; }
	
	void					setFogOfViewInnerRadius(float inRadius) { m_fogOfViewInnerRadius = inRadius; }
	void					setFogOfViewOuterRadius(float outRadius) { m_fogOfViewOuterRadius = outRadius; }

	void					setFogOfViewRing(const jam::Ring2f& val) { m_fogOfViewRing = val; }
	const jam::Ring2f&		getFogOfViewRing() const { return m_fogOfViewRing; }

	/// Returns the topmost Node under the given touch (or 0 if there isn't a touchable node under the touch)
	Node*					getTouchedNode(size_t idx) { return m_touchedNodes[idx] ; } ; 

private:
	void					clearTouchableNodes() ;
	void					addTouchableNode( Node* pNode );
	void					updateTouchableNodes() ;
	bool					getTouches(size_t idx) const { return m_touches[idx]; }
	void					setTouches(size_t idx, bool val) { m_touches[idx] = val; }
	void					setTouchedNode(size_t idx, Node* n) { m_touchedNodes[idx] = n ; } ; 

private:
	// it's the list of touchable (potential) nodes to be checked for touches
	// the list is polulated during the visitGraph()
	NodesList				m_touchableNodes ;

	// in the updateTochableNodes(), for each touch id, we set the corresponding array item to true
	// if at least a (touchable) node is under the touch
	bool					m_touches[JAM_MAX_TOUCHES] ;

	// for each touch id, we set the corresponding array item with the foremost Node pointer 
	Node*					m_touchedNodes[JAM_MAX_TOUCHES] ;

	// fog of view
	Ring2f					m_fogOfViewRing ;
	float					m_fogOfViewInnerRadius ;
	float					m_fogOfViewOuterRadius ;
	bool					m_fogOfViewEnabled ;
};


}	

#endif // __JAM_SCENE_H__
