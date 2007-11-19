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

#include <ivf/IvfLighting.h>

#include <ivf/IvfGL.h>

CIvfLighting* CIvfLighting::m_instance = 0;
CIvfSingletonDestroyer<CIvfLighting> CIvfLighting::m_destroyer;

CIvfLighting* CIvfLighting::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CIvfLighting(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CIvfLighting::CIvfLighting() 
{ 
    int i;

	for (i=0; i<8; i++)
	{
		CIvfLight* light = new CIvfLight();
		light->addReference();
		light->setNumber(i);
		m_lights.push_back(light);
	}

	m_ambient[0] = 0.2f;
	m_ambient[1] = 0.2f;
	m_ambient[2] = 0.2f;
	m_ambient[3] = 1.0f;
}

CIvfLighting::~CIvfLighting() 
{ 
    int i;

	for (i=0; i<8; i++)
	{
		CIvfLight* light = m_lights[i];
		delete light;
	}
}

void CIvfLighting::enable()
{
	glEnable(GL_LIGHTING);
}

void CIvfLighting::disable()
{
	glDisable(GL_LIGHTING);
}

void CIvfLighting::setLocalViewer(bool flag)
{
	m_local[0] = ( flag ) ? 1 : 0;
	glLightModeliv( GL_LIGHT_MODEL_LOCAL_VIEWER, m_local );

}

bool CIvfLighting::getLocalViewer()
{
	return m_local;
}

void CIvfLighting::setTwoSide(bool flag)
{
	m_twoside[0] = ( flag ) ? 1 : 0;
	glLightModeliv( GL_LIGHT_MODEL_TWO_SIDE, m_twoside );
}

bool CIvfLighting::getTwoSide()
{
	return m_twoside;
}

void CIvfLighting::setAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambient[0] = red;
	m_ambient[1] = green;
	m_ambient[2] = blue;
	m_ambient[3] = alpha;
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, m_ambient );
}

int CIvfLighting::getSize()
{
	return 8;
}

CIvfLight* CIvfLighting::getLight(int idx)
{
	if ((idx>=0) && (idx<this->getSize()))
	{
		return m_lights[idx];
	}
	else
		return NULL;
}

void CIvfLighting::enableLights()
{
    int i;

	for (i=0; i<8; i++)
		m_lights[i]->enable();
}

void CIvfLighting::disableLights()
{
    int i;

	for (i=0; i<8; i++)
		m_lights[i]->disable();
}

void CIvfLighting::render()
{
	int i;

	for (i=0; i<8; i++)
		if (m_lights[i]->isEnabled())
			m_lights[i]->render();
}

bool CIvfLighting::isEnabled()
{
	GLboolean lightEnabled;
	glGetBooleanv(GL_LIGHTING, &lightEnabled);
	return (bool)lightEnabled;
}

void CIvfLighting::saveState()
{
	glPushAttrib(GL_LIGHTING);
}

void CIvfLighting::restoreState()
{
	glPopAttrib();
}

void CIvfLighting::saveEnabledState()
{
	m_enabled.clear();

	int i;

	for (i=0; i<8; i++)
	{
		CIvfLightPtr light = this->getLight(i);
		m_enabled.push_back(light->isEnabled());	
	}
}

void CIvfLighting::restoreEnabledState()
{
	if (m_enabled.size()>0)
	{
		int i;
	
		for (i=0; i<8; i++)
		{
			CIvfLightPtr light = this->getLight(i);
			if (m_enabled[i])
				light->enable();
			else
				light->disable();
		}
	}
}
