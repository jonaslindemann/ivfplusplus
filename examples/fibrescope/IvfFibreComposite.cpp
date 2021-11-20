//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2005 Division of Structural Mecahnics, Lund University
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

#include "IvfUserSettings.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFibreComposite::CFibreComposite()
{

}

CFibreComposite::~CFibreComposite()
{

}

void CFibreComposite::doBeginTransform()
{
	CUserSettingsPtr userSettings = CUserSettings::getInstance();
	CLightingPtr lighting = CLighting::getInstance();

	if (userSettings->getFibreLighting())
		lighting->enable();
	else
		lighting->disable();

	CMaterialPtr material = this->getMaterial();
	if (material!=NULL)
	{
		if (userSettings->getRepresentation()==FIBRE_BAND2)
		{
			this->getMaterial()->getShininess(m_oldShininess);
			this->getMaterial()->setShininess(128.0f);
		}
	}

	CComposite::doBeginTransform();
}

void CFibreComposite::doEndTransform()
{
	CComposite::doEndTransform();

	CUserSettingsPtr userSettings = CUserSettings::getInstance();

	CMaterialPtr material = this->getMaterial();
	if (material!=NULL)
	{
		if (userSettings->getRepresentation()==FIBRE_BAND2)
		{
			this->getMaterial()->setShininess(m_oldShininess);
		}
	}
}
