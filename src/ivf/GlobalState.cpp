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

#include <ivf/GlobalState.h>

using namespace ivf;

GlobalState* GlobalState::m_instance = 0;
SingletonDestroyer<GlobalState> GlobalState::m_destroyer;

GlobalState* GlobalState::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new GlobalState();
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

GlobalState::GlobalState()
{
	m_renderMaterial = true;
	m_renderTexture = true;
	m_renderGreyscale = false;
    m_colorOutput = true;
}

GlobalState::~GlobalState()
{

}

void GlobalState::enableMaterialRendering()
{
	m_renderMaterial = true;
}

void GlobalState::disableMaterialRendering()
{
	m_renderMaterial = false;
}

bool GlobalState::isMaterialRenderingEnabled()
{
	return m_renderMaterial;
}

void GlobalState::enableTextureRendering()
{
	m_renderTexture = true;
}

void GlobalState::disableTextureRendering()
{
	m_renderTexture = false;
}

bool GlobalState::isTextureRenderingEnabled()
{
	return m_renderTexture;
}

void GlobalState::enableGreyscaleRendering()
{
	m_renderGreyscale = true;
}

void GlobalState::disableGreyscaleRendering()
{
	m_renderGreyscale = false;
}

bool GlobalState::isGreyscaleRenderingEnabled()
{
	return m_renderGreyscale;
}

void GlobalState::enableColorOutput()
{
    m_colorOutput = true;
}

void GlobalState::disableColorOutput()
{
    m_colorOutput = false;
}

bool GlobalState::isColorOutputEnabled()
{
    return m_colorOutput;
}


