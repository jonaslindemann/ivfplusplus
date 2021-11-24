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
//
// Written by Jonas Lindemann
//

#include <ivf/Lighting.h>

#include <ivf/GL.h>

using namespace ivf;
Lighting* Lighting::m_instance = 0;
SingletonDestroyer<Lighting> Lighting::m_destroyer;

Lighting* Lighting::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new Lighting(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

Lighting::Lighting() 
{ 
    int i;

	for (i=0; i<8; i++)
	{
		Light* light = new Light();
		light->addReference();
		light->setNumber(i);
		m_lights.push_back(light);
	}

	m_ambient[0] = 0.2f;
	m_ambient[1] = 0.2f;
	m_ambient[2] = 0.2f;
	m_ambient[3] = 1.0f;
}

Lighting::~Lighting() 
{ 
    int i;

	for (i=0; i<8; i++)
	{
		Light* light = m_lights[i];
		delete light;
	}
}

void Lighting::enable()
{
	glEnable(GL_LIGHTING);
}

void Lighting::disable()
{
	glDisable(GL_LIGHTING);
}

void Lighting::setLocalViewer(bool flag)
{
	m_local[0] = ( flag ) ? 1 : 0;
	glLightModeliv( GL_LIGHT_MODEL_LOCAL_VIEWER, m_local );

}

bool Lighting::getLocalViewer()
{
	return m_local;
}

void Lighting::setTwoSide(bool flag)
{
	m_twoside[0] = ( flag ) ? 1 : 0;
	glLightModeliv( GL_LIGHT_MODEL_TWO_SIDE, m_twoside );
}

bool Lighting::getTwoSide()
{
	return m_twoside;
}

void Lighting::setAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambient[0] = red;
	m_ambient[1] = green;
	m_ambient[2] = blue;
	m_ambient[3] = alpha;
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, m_ambient );
}

int Lighting::getSize()
{
	return 8;
}

Light* Lighting::getLight(int idx)
{
	if ((idx>=0) && (idx<this->getSize()))
	{
		return m_lights[idx];
	}
	else
		return nullptr;
}

void Lighting::enableLights()
{
    int i;

	for (i=0; i<8; i++)
		m_lights[i]->enable();
}

void Lighting::disableLights()
{
    int i;

	for (i=0; i<8; i++)
		m_lights[i]->disable();
}

void Lighting::render()
{
	int i;

	for (i=0; i<8; i++)
		if (m_lights[i]->isEnabled())
			m_lights[i]->render();
}

bool Lighting::isEnabled()
{
	GLboolean lightEnabled;
	glGetBooleanv(GL_LIGHTING, &lightEnabled);
	return (bool)lightEnabled;
}

void Lighting::saveState()
{
	glPushAttrib(GL_LIGHTING);
}

void Lighting::restoreState()
{
	glPopAttrib();
}

void Lighting::saveEnabledState()
{
	m_enabled.clear();

	int i;

	for (i=0; i<8; i++)
	{
		LightPtr light = this->getLight(i);
		m_enabled.push_back(light->isEnabled());	
	}
}

void Lighting::restoreEnabledState()
{
	if (m_enabled.size()>0)
	{
		int i;
	
		for (i=0; i<8; i++)
		{
			LightPtr light = this->getLight(i);
			if (m_enabled[i])
				light->enable();
			else
				light->disable();
		}
	}
}
