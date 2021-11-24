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
// Modified by Carl-Johan Lejdfors
//

#include <ivf/OldLightModel.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
OldLightModel::OldLightModel()
: GLBase(),
m_usereference(true)
{
	setAmbient( 0.2, 0.2, 0.2, 1.0 );
	setLocalViewer(0);
	setTwoSide(0);
}

// ------------------------------------------------------------
OldLightModel::~OldLightModel()
{
	deactivate();

	for ( vector<OldLight*>::iterator it = m_lights.begin(); it != m_lights.end() ; ++it ) {
		(*it)->deleteReference();
		if ( !(*it)->referenced() )
			delete *it;
	}
}


// ------------------------------------------------------------
void OldLightModel::setAmbient( GLfloat r, GLfloat g,
								GLfloat b, GLfloat a )
{
	m_ambient[0] = r;
	m_ambient[1] = g;
	m_ambient[2] = b;
	m_ambient[3] = a;
}

// ------------------------------------------------------------
void OldLightModel::setLocalViewer( GLboolean t )
{
	m_local[0] = ( t ) ? 1 : 0;
}

// ------------------------------------------------------------
void OldLightModel::setTwoSide( GLboolean t )
{
	m_twoside[0] = ( t ) ? 1 : 0;
}

// ------------------------------------------------------------
void OldLightModel::getAmbient( GLfloat &r, GLfloat &g, 
								GLfloat &b, GLfloat &a ) const
{
	r = m_ambient[0];
	g = m_ambient[1];
	b = m_ambient[2];
	a = m_ambient[3];
}

// ------------------------------------------------------------
bool OldLightModel::getLocalViewer() const
{
	return m_local;
}

// ------------------------------------------------------------
bool OldLightModel::getTwoSide() const
{
	return m_twoside;
}

// ------------------------------------------------------------
void OldLightModel::render()
{
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, m_ambient );
	glLightModeliv( GL_LIGHT_MODEL_LOCAL_VIEWER, m_local );
	glLightModeliv( GL_LIGHT_MODEL_TWO_SIDE, m_twoside );
	
	vector<OldLight*>::const_iterator cit = m_lights.begin();
	while ( cit != m_lights.end() ) {
		(*cit)->render();
		++cit;
	}
	
	
	//glEnable( GL_LIGHTING );
	
}

// ------------------------------------------------------------
void OldLightModel::setUseReference( bool t )
{
	m_usereference = t;
}

// ------------------------------------------------------------
void OldLightModel::addLight( OldLight *light ) 
{
	if ( m_usereference )
		light->addReference();
	m_lights.push_back(light);
}

// ------------------------------------------------------------
OldLight* OldLightModel::removeLight( int index ) 
{
	vector<OldLight*>::iterator it = m_lights.begin();
	
	if ( index >= 0 && index < (int)m_lights.size() ) {
		OldLight *light = m_lights[index];
		
		if ( light != nullptr  && m_usereference ) {
			light->deleteReference();
			//      if ( !light->referenced() )
			//	delete light;
		}
		it += index;
		m_lights.erase(it); 
		return light;
	} else 
		return nullptr;
}

// ------------------------------------------------------------
void OldLightModel::deleteLight( int i )
{
	delete this->removeLight(i);
}

// ------------------------------------------------------------
OldLight* OldLightModel::getLight( int i )
{
	return m_lights[i];
}

// ------------------------------------------------------------
const OldLight* OldLightModel::getLight( int i ) const
{
	return m_lights[i];
}

// ------------------------------------------------------------
int OldLightModel::getNoLights() const
{
  return static_cast<int>(m_lights.size());
}

// ------------------------------------------------------------
void OldLightModel::activate()
{
	vector<OldLight*>::const_iterator cit = m_lights.begin();
	while ( cit != m_lights.end() ) {
		(*cit)->activate();
		++cit;
	}
}

// ------------------------------------------------------------
void OldLightModel::deactivate()
{
	disableAll();
	vector<OldLight*>::const_iterator cit = m_lights.begin();
	while ( cit != m_lights.end() ) {
		(*cit)->deactivate();
		++cit;
	}
}

// ------------------------------------------------------------
void OldLightModel::disableAll()
{
	GLenum light;
	
	for ( int i = 0 ; i < (int)IVF_MAX_LIGHTS ; ++i ) 
	{
		light = GL_LIGHT0 + i;
		glDisable( light );
	}
}

// ------------------------------------------------------------
void OldLightModel::deleteAll()
{
	for ( vector<OldLight*>::iterator it = m_lights.begin(); it != m_lights.end() ; ++it ) {
		(*it)->deleteReference();
		if ( !(*it)->referenced() )
			delete *it;
	}
}

// ------------------------------------------------------------
void OldLightModel::deleteLight(OldLight *light)
{
	for ( vector<OldLight*>::iterator it = m_lights.begin(); it != m_lights.end() ; ++it ) 
	{
		if (*it==light)
		{
			(*it)->deleteReference();
			if ( !(*it)->referenced() )
				delete *it;
		}
	}
}

