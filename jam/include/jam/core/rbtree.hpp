/**********************************************************************************
* 
* rbtree.hpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito.
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
* Original author for C implementation: Prakhar Jain
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

#ifndef __JAM_RBTREE_H__
#define __JAM_RBTREE_H__

#include <memory>

namespace jam
{
	
/*!
*
* \class rbtree
*
* Red/Black tree C++ implementation
*
*/
template <typename T>
class rbtree
{
public:
	enum class nodecolor
	{
		RED = 1,
		BLACK = 2
	};

	struct node
	{
	public:
		T			key;
		node*		left ;
		node*		right ;
		node*		p ;
		nodecolor	color ;
	};

	typedef	node*	NODEPTR	;

public:
					rbtree() ;
					~rbtree() ;

	void			insert( T z ) ;
	bool			erase( T z ) ;
	NODEPTR			search( T k ) ;
	void			inorder( void (*pFunc)(T k) ) ;
	NODEPTR			minimum() ;
	NODEPTR			maximum() ;
	NODEPTR			successor( T x ) ;
	NODEPTR			predecessor( T x ) ;

private:
	void			rdelete(NODEPTR x) ;
	void			insertfixup( NODEPTR z ) ;
	void			leftrotate( NODEPTR x ) ;
	void			rightrotate( NODEPTR y ) ;
	void			transplant(NODEPTR u, NODEPTR v ) ;
	void			erasefixup(NODEPTR x ) ;
	NODEPTR			search( NODEPTR pNode, T k ) ;
	void			inorder( NODEPTR x, void (*pFunc)(T k) ) ;
	NODEPTR			minimum( NODEPTR pNode ) ;
	NODEPTR			maximum( NODEPTR pNode ) ;

public:
	static void		initNIL() ;
	static NODEPTR	NILPTR ;

private:
	NODEPTR			m_root ;

	static node		NIL ;
};

template <typename T> typename rbtree<T>::node rbtree<T>::NIL ;
template <typename T> typename rbtree<T>::NODEPTR rbtree<T>::NILPTR = &rbtree<T>::NIL;

template <class T> 
rbtree<T>::rbtree() : m_root(rbtree::NILPTR)
{
}

template<class T>
rbtree<T>::~rbtree()
{
	rdelete( m_root ) ;
}

template <class T> 
void rbtree<T>::insert( T z )
{
	NODEPTR Z = (NODEPTR) malloc(sizeof(struct node));
	Z->key = z;
	NODEPTR y = NILPTR;
	NODEPTR x = m_root;
	while (x != NILPTR) {
		y = x;
		if (Z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	Z->p = y;
	if (y == NILPTR)
		m_root = Z;
	else if (Z->key < y->key)
		y->left = Z;
	else
		y->right = Z;
	Z->left = NILPTR;
	Z->right = NILPTR;
	Z->color = RED;
	insertfixup(Z);
}

template <class T> 
bool rbtree<T>::erase( T z )
{
	NODEPTR Z = search(m_root, z);
	if (Z == NILPTR) {
		return false ;
	}
	NODEPTR y = Z;
	int yoc = y->color;
	NODEPTR x;
	if (Z->left == NILPTR) {
		x = Z->right;
		transplant(Z,Z->right);
	}
	else if (Z->right == NILPTR) {
		x = Z->left;
		transplant(Z,Z->left);
	}
	else {
		y = minimum(Z->right);
		yoc = y->color;
		x = y->right;
		if (y->p == Z)
			x->p = y;
		else {
			transplant(y,y->right);
			y->right = Z->right;
			y->right->p = y;
		}
		transplant(Z,y);
		y->left = Z->left;
		y->left->p = y;
		y->color = Z->color;
	}
	if (yoc == BLACK)
		erasefixup(x);

	delete Z ;

	return true ;
}

template <class T> 
typename rbtree<T>::NODEPTR	rbtree<T>::search( T k )
{
	return search(m_root,k) ;
}

template <class T> 
void rbtree<T>::inorder( void (*pFunc)(T k) )
{
	inorder( m_root, pFunc ) ;
}

template <class T> 
typename rbtree<T>::NODEPTR	rbtree<T>::minimum()
{
	return minimum(m_root) ;
}

template <class T> 
typename rbtree<T>::NODEPTR	rbtree<T>::maximum()
{
	return maximum(m_root) ;
}

template <class T> 
typename rbtree<T>::NODEPTR rbtree<T>::successor( T x )
{
	NODEPTR temp = search(m_root, x);
	if (temp == NILPTR) {
		return temp;
	}
	if (temp->right != NILPTR)
		return minimum(temp->right);
	NODEPTR y = temp->p;
	while (y != NILPTR && temp == y->right) {
		temp = y;
		y = y->p;
	}
	return y;
}

template <class T> 
typename rbtree<T>::NODEPTR rbtree<T>::predecessor( T x )
{
	NODEPTR temp = search(m_root, x);
	if (temp == NILPTR) {
		return temp;
	}
	if (temp->left != NILPTR)
		return maximum(temp->left);
	NODEPTR y = temp->p;
	while (y != NILPTR && temp == y->left) {
		temp = y;
		y = y->p;
	}
	return y;
}

template<class T>
void rbtree<T>::rdelete(NODEPTR x)
{
	if (x != NILPTR) {
		rdelete(x->left);
		rdelete(x->right);
		delete x ;
	}
}

template <class T> 
void rbtree<T>::insertfixup( NODEPTR z )
{
	while (z->p->color == RED) {
		if (z->p == z->p->p->left) {
			NODEPTR y = z->p->p->right;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->right) {
					z = z->p;
					leftrotate(z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				rightrotate(z->p->p);
			}
		}
		else {
			NODEPTR y = z->p->p->left;
			if (y->color == RED) {
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if (z == z->p->left) {
					z = z->p;
					rightrotate(z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				leftrotate(z->p->p);
			}
		}
	}
	m_root->color = BLACK;
}

template <class T> 
void rbtree<T>::leftrotate( NODEPTR x )
{
	NODEPTR y = x->right;
	x->right = y->left;
	if (y->left != NILPTR)
		y->left->p = x;
	y->p = x->p;
	if (x->p == NILPTR)
		m_root = y;
	else if (x->p->left == x)
		x->p->left = y;
	else
		x->p->right = y;
	y->left = x;
	x->p = y;
}

template <class T> 
void rbtree<T>::rightrotate( NODEPTR y )
{
	NODEPTR x = y->left;
	y->left = x->right;
	if (x->right != NILPTR)
		x->right->p = y;
	x->p = y->p;
	if (y->p == NILPTR)
		m_root = x;
	else if (y->p->left == y)
		y->p->left = x;
	else
		y->p->right = x;
	x->right = y;
	y->p = x;
}


/*
* replace subtree u as child of root node with subtree v
* node u's parent becomes node v's parent and u's parent ends up having v as its appropriate child
*/
template <class T> 
void rbtree<T>::transplant( NODEPTR u, NODEPTR v )
{
	if (u->p == NILPTR)
		m_root = v;
	else if (u == u->p->left)
		u->p->left = v;
	else
		u->p->right = v;
	v->p = u->p;
}

template <class T> 
void rbtree<T>::erasefixup( NODEPTR x  )
{
	while (x != m_root && x->color == BLACK)
	{
		if (x == x->p->left) {
			NODEPTR w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				leftrotate(x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
			 	if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rightrotate(w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				leftrotate(x->p);
				x = m_root;
			}
		}
		else {
			NODEPTR w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				rightrotate(x->p);
				w = x->p->left;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					leftrotate(w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				rightrotate(x->p);
				x = m_root;
			}
		}
	}
	x->color = BLACK;
}

template <class T> 
typename rbtree<T>::NODEPTR rbtree<T>::search( NODEPTR pNode, T k )
{
	if (pNode == NILPTR || pNode->key == k)
		return pNode;
	if (k < pNode->key)
		return search(pNode->left, k);
	else
		return search(pNode->right, k);
}

template <class T> 
void rbtree<T>::inorder( NODEPTR x, void (*pFunc)(T k) )
{
	if (x != NILPTR) {
		inorder(x->left,pFunc);
		pFunc(x->key);
		inorder(x->right,pFunc);
	}
}

template <class T> 
typename rbtree<T>::NODEPTR rbtree<T>::minimum( NODEPTR pNode )
{
	while (pNode->left != NILPTR)
		pNode = pNode->left;
	return pNode;
}

template <class T> 
typename rbtree<T>::NODEPTR rbtree<T>::maximum( NODEPTR pNode )
{
	while (pNode->right != NILPTR)
		pNode = pNode->right;
	return pNode;
}

template <class T> 
void rbtree<T>::initNIL()
{
	NIL.left = NIL.right = NIL.p = NILPTR;
	NIL.color = BLACK ;
}

} // end namespace

#endif // __JAM_RBTREE_H__
