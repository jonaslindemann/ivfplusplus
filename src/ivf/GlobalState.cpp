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

#include <ivf/GlobalState.h>

using namespace ivf;

CGlobalState* CGlobalState::m_instance = 0;
CSingletonDestroyer<CGlobalState> CGlobalState::m_destroyer;

CGlobalState* CGlobalState::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CGlobalState();
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CGlobalState::CGlobalState()
{
	m_renderMaterial = true;
	m_renderTexture = true;
	m_renderGreyscale = false;
    m_colorOutput = true;
}

CGlobalState::~CGlobalState()
{

}

void CGlobalState::enableMaterialRendering()
{
	m_renderMaterial = true;
}

void CGlobalState::disableMaterialRendering()
{
	m_renderMaterial = false;
}

bool CGlobalState::isMaterialRenderingEnabled()
{
	return m_renderMaterial;
}

void CGlobalState::enableTextureRendering()
{
	m_renderTexture = true;
}

void CGlobalState::disableTextureRendering()
{
	m_renderTexture = false;
}

bool CGlobalState::isTextureRenderingEnabled()
{
	return m_renderTexture;
}

void CGlobalState::enableGreyscaleRendering()
{
	m_renderGreyscale = true;
}

void CGlobalState::disableGreyscaleRendering()
{
	m_renderGreyscale = false;
}

bool CGlobalState::isGreyscaleRenderingEnabled()
{
	return m_renderGreyscale;
}

void CGlobalState::enableColorOutput()
{
    m_colorOutput = true;
}

void CGlobalState::disableColorOutput()
{
    m_colorOutput = false;
}

bool CGlobalState::isColorOutputEnabled()
{
    return m_colorOutput;
}


