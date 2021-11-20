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
//
// Written by Jonas Lindemann
//

#include <ivfgle/GleScalarArray.h>

using namespace ivf;

CGleScalarArray::CGleScalarArray()
{
	m_scalars = NULL;
	m_size = 0;
}

CGleScalarArray::CGleScalarArray(int size)
{
	m_scalars = NULL;
	m_size = 0;

	setSize(size);
}

CGleScalarArray::~CGleScalarArray()
{
	if (m_scalars!=NULL)
		delete [] m_scalars;
}

void CGleScalarArray::setSize(int size)
{
	// Delete previous spine coords/colors if any
	
	if (m_scalars!=NULL)
		delete [] m_scalars;
		
	// Set new size
		
	m_size = size;
		
	// Allocate memory for new spine
		
	m_scalars = new gleDouble[m_size];
}

void CGleScalarArray::setValue(int idx, double value)
{
	if ((idx>=0)&&(idx<m_size))
		m_scalars[idx] = value;
}

double CGleScalarArray::getValue(int idx)
{
	if ((idx>=0)&&(idx<m_size))
		return m_scalars[idx];
	else 
		return 0.0;
}

int CGleScalarArray::getSize()
{
	return m_size;
}

gleDouble* CGleScalarArray::getData()
{
	if (m_scalars!=NULL)
		return m_scalars;
	else
		return NULL;
}
