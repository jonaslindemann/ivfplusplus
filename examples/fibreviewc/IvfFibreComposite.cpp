//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include "IvfFibreComposite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIvfFibreComposite::CIvfFibreComposite()
{
	m_useLight = true;
	m_lighting = CIvfLighting::getInstance();
}

CIvfFibreComposite::~CIvfFibreComposite()
{

}

void CIvfFibreComposite::beginTransform()
{
	if (m_useLight)
		m_lighting->enable();
	else
		m_lighting->disable();

	CIvfComposite::beginTransform();
}

void CIvfFibreComposite::endTransform()
{
	CIvfComposite::endTransform();
}

void CIvfFibreComposite::setUseLighting(bool flag)
{
	m_useLight = flag;
}
