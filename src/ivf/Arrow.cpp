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

// Implementation of: public class CIvfArrow

#include <ivf/config.h>
#include <ivf/Arrow.h>

using namespace ivf;

// ------------------------------------------------------------
Arrow::Arrow ()
		:Composite()
		//TODO: check and complete member initialisation list!
{
	m_head = new Cone();
	m_tail = new Cylinder();
	m_extraHead = new Cone();
	m_offset = 0.0;
	setArrowType(IVF_ARROW_STANDARD);
	setSize(1.0,  0.2);
	setRadius(0.05, 0.025);
	addChild(m_head);
	addChild(m_tail);
	addChild(m_extraHead);
}

// ------------------------------------------------------------
Arrow::~Arrow ()
{
}

// ------------------------------------------------------------
void Arrow::setSize(double length, double head)
{
	m_length = length;
	m_headSize = head;
	m_head->setHeight(m_headSize);
	m_extraHead->setHeight(m_headSize);
	m_head->setPosition(0.0, m_offset + m_length/2.0 - m_headSize/2.0, 0.0);
	if (this->getArrowType() == IVF_ARROW_DOUBLE)
	{
		m_extraHead->setPosition(0.0, m_offset + m_length/2.0 - m_headSize/2.0 - m_headSize, 0.0);
		m_tail->setHeight(m_length - m_headSize*2.0);
		m_tail->setPosition(0.0, m_offset + m_length/2.0 - (m_length - m_headSize*2.0)/2.0 - m_headSize*2.0, 0.0);
	}
	else
	{
		m_tail->setHeight(m_length - m_headSize);
		m_tail->setPosition(0.0, m_offset + m_length/2.0 - (m_length - m_headSize)/2.0 - m_headSize, 0.0);
	}
}

void Arrow::setRadius(double head, double tail)
{
	m_headRadius = head;
	m_tailRadius = tail;
	m_head->setTopRadius(0.0);
	m_head->setBottomRadius(m_headRadius);
	m_extraHead->setBottomRadius(m_headRadius);
	m_tail->setRadius(m_tailRadius);
}

void Arrow::setArrowType(int type)
{
	m_arrowType = type;
	setSize(m_length,m_headSize);
	if (m_arrowType == IVF_ARROW_DOUBLE)
		m_extraHead->setState(GLBase::OS_ON);
	else
		m_extraHead->setState(GLBase::OS_OFF);
}

int Arrow::getArrowType()
{
	return m_arrowType;
}

void Arrow::alignVector(Vec3d *vec)
{
	double ex, ey, ez;
	double l;
	double alfa, beta;
	Vec3d v;

	vec->getComponents(ex, ey, ez);
	v.setComponents(ex, ey, ez);
	v.normalize();
	v.getComponents(ex, ey, ez);

	l = sqrt(pow(ex,2) + pow(ey,2));

	if ( (ex==0.0)&&(ez==0.0) )
	{
		if ( ey>0 )
		{
			alfa = 0.0;
			beta = M_PI/2.0;
		}
		else
		{
			alfa = 0.0;
			beta = -M_PI/2.0;
		}
	}
	else
	{
		alfa = atan2(ex,ez);
		beta = atan2(l,ez);
	}

	

	if (alfa<=-M_PI*0.999999)
		alfa = 0.0;

	this->setRotation(0.0, -alfa*360.0/2.0/M_PI, -beta*360.0/2.0/M_PI);
}

void Arrow::getSize(double &length, double &head)
{
	length = m_length;
	head = m_headSize;
}

void Arrow::setOffset(double offset)
{
	m_offset = offset;
	this->setSize(m_length, m_headSize);
}

double Arrow::getOffset()
{
	return m_offset;
}

double Arrow::getLength()
{
	return m_length;
}

