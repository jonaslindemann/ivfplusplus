//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//

// Implementation of: public class CIvfCamera

#include <ivf/View.h>

using namespace ivf;

// ------------------------------------------------------------
View::View()
{
	//setViewPort(0, 0, 640,480);
	m_selectTransform = true;

	m_viewport = new Viewport();
	m_viewport->setPosition(0,0);
	m_viewport->setSize(640,480);
}

// ------------------------------------------------------------
View::~View()
{

}

// ------------------------------------------------------------
void View::doBeginTransform()
{
	this->viewTransform();
}

// ------------------------------------------------------------
void View::initialize()
{
	m_viewport->apply();
	//glViewport(m_viewX, m_viewY, m_viewWidth, m_viewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	this->projectionTransform();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// ------------------------------------------------------------
void View::setViewPort(int width, int height)
{
	m_viewport->setSize(width, height);

	/*
	m_viewWidth = width;
	m_viewHeight = height;

	if (height==0)
		m_viewAspect = (GLdouble)(m_viewWidth);
	else
		m_viewAspect = (GLdouble)(m_viewWidth)/(GLdouble)(m_viewHeight);
	*/
}

// ------------------------------------------------------------
void View::setViewPort(int tx, int ty, int tw, int th)
{
	m_viewport->setPosition(tx, ty);
	m_viewport->setSize(tw, th);

	/*
	m_viewX = tx;
	m_viewY = ty;
	m_viewHeight = th;
	m_viewWidth = tw;

	if (th==0)
		m_viewAspect = (GLdouble)(m_viewWidth-m_viewX);
	else
		m_viewAspect = (GLdouble)(m_viewWidth)/(GLdouble)(m_viewHeight);
	*/
}

// ------------------------------------------------------------
double View::getAspect()
{
	return m_viewport->getAspect();
}

// ------------------------------------------------------------
void View::initializeSelect(int x, int y, int w, int h)
{
	GLint viewport[4];
	int viewX, viewY, viewWidth, viewHeight;

	m_viewport->getPosition(viewX, viewY);
	m_viewport->getSize(viewWidth, viewHeight);

	viewport[0] = viewX;
	viewport[1] = viewY;
	viewport[2] = viewWidth;
	viewport[3] = viewHeight;

	gluPickMatrix((GLdouble)(x), (GLdouble)viewHeight - (GLdouble)(y), (GLdouble)w, (GLdouble)h, viewport);
	if (m_selectTransform)
		this->projectionTransform();
}

// ------------------------------------------------------------
void View::getViewPort(int & w, int & h)
{
	m_viewport->getSize(w, h);
	/*
	w = m_viewHeight;
	h = m_viewWidth;
	*/
}

// ------------------------------------------------------------
void View::getViewPort(int &tx, int &ty, int &tw, int &th)
{
	m_viewport->getPosition(tx, ty);
	m_viewport->getSize(tw, th);

	/*
	tx = m_viewX;
	ty = m_viewY;
	th = m_viewHeight;
	tw = m_viewWidth;

	if (th==0)
		m_viewAspect = (GLdouble)(m_viewWidth-m_viewX);
	else
		m_viewAspect = (GLdouble)(m_viewWidth)/(GLdouble)(m_viewHeight);
	*/
}

// ------------------------------------------------------------
void View::projectionTransform()
{

}

// ------------------------------------------------------------
void View::viewTransform()
{

}

// ------------------------------------------------------------
void View::getViewFrustum(ViewFrustum *frustum)
{

}

// ------------------------------------------------------------
void View::setSelectTransform(bool flag)
{
	m_selectTransform = flag;
}

// ------------------------------------------------------------
Viewport* View::getViewport()
{
	return m_viewport;
}

void View::setViewport(Viewport *viewport)
{
	m_viewport = viewport;
}
