//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//

// Implementation of: public class CIvfCursor

#include <ivf/IvfCursor.h>

// ------------------------------------------------------------
CIvfCursor::CIvfCursor ()
		:CIvfComposite()
{
	m_cursorType = CT_SOLID_CURSOR;
	m_size = 1.0;

	m_xMaterial = new CIvfMaterial();
	m_xMaterial->setDiffuseColor(1.0, 1.0, 0.0, 1.0);
	m_xMaterial->setAmbientColor(0.5, 0.5, 0.0, 1.0);

	m_yMaterial = new CIvfMaterial();
	m_yMaterial->setDiffuseColor(1.0, 1.0, 0.0, 1.0);
	m_yMaterial->setAmbientColor(0.5, 0.5, 0.0, 1.0);

	m_zMaterial = new CIvfMaterial();
	m_zMaterial->setDiffuseColor(1.0, 1.0, 0.0, 1.0);
	m_zMaterial->setAmbientColor(0.5, 0.5, 0.0, 1.0);

	m_xBrick = new CIvfBrick();
	m_xBrick->setSize(m_size, m_size*0.05, m_size*0.05);
	m_xBrick->setMaterial(m_xMaterial);

	m_yBrick = new CIvfBrick();
	m_yBrick->setSize(m_size*0.05, m_size, m_size*0.05);
	m_yBrick->setMaterial(m_yMaterial);

	m_zBrick = new CIvfBrick();
	m_zBrick->setSize(m_size*0.05, m_size*0.05, m_size);
	m_zBrick->setMaterial(m_zMaterial);

	addChild(m_xBrick);
	addChild(m_yBrick);
	addChild(m_zBrick);
}

// ------------------------------------------------------------
CIvfCursor::~CIvfCursor ()
{
}

// ------------------------------------------------------------
void CIvfCursor::setSize(double size)
{
	m_size = size;
	m_xBrick->setSize(m_size, m_size*0.05, m_size*0.05);
	m_yBrick->setSize(m_size*0.05, m_size, m_size*0.05);
	m_zBrick->setSize(m_size*0.05, m_size*0.05, m_size);
}

// ------------------------------------------------------------
void CIvfCursor::setCursorType(TCursorType type)
{
	m_cursorType = type;
}

// ------------------------------------------------------------
void CIvfCursor::doCreateGeometry()
{
	if (this->getCursorType() == CT_SOLID_CURSOR)
		CIvfComposite::doCreateGeometry();
	else
	{
		glPushAttrib(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
			glColor3d(1.0, 1.0, 0.0);
			glVertex3d(-m_size/2.0,0.0,0.0);
			glVertex3d(m_size/2.0,0.0,0.0);
			glColor3d(1.0, 1.0, 0.0);
			glVertex3d(0.0,-m_size/2.0,0.0);
			glVertex3d(0.0, m_size/2.0,0.0);
			glColor3d(1.0, 1.0, 0.0);
			glVertex3d(0.0, 0.0, -m_size/2);
			glVertex3d(0.0, 0.0,  m_size/2);
		glEnd();
		glPopAttrib();
	}
}

// ------------------------------------------------------------
int CIvfCursor::getCursorType()
{
	return m_cursorType;
}

