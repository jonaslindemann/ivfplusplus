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
// Modified by Carl-Johan Lejdfors
//

#ifndef _CIvfOldLightModel_h_
#define _CIvfOldLightModel_h_

#include <ivf/IvfGLBase.h>
#include <ivf/IvfOldLight.h>

IvfSmartPointer(COldLightModel);

/**
* Light model class
*
* Contains all scene lighting info 
* @author Carl-Johan Lejdfors
*/
class IVF_API COldLightModel : public CGLBase {
public:
	void deleteLight(COldLight* light);
	/** Constructor */
	COldLightModel();

	/** Destructor */
	~COldLightModel(); 

	IvfClassInfo("COldLightModel",CGLBase);

	/** Set environment ambient lighting components */
	void setAmbient( GLfloat r, GLfloat g, 
		GLfloat b, GLfloat a );

	/** Set local viewer flag */
	void setLocalViewer( GLboolean t );

	/** Set two sided flag */
	void setTwoSide( GLboolean t );

	/** Return environment ambient lighting components */
	void getAmbient( GLfloat &r, GLfloat &g, 
		GLfloat &b, GLfloat &a ) const;

	/** Return local viewer flag */
	bool getLocalViewer() const;

	/** Return two sided flag */
	bool getTwoSide( ) const;
	
	/** Render light model */
	void render(); 
	
	/** 
	 * Set the use reference flag
	 * 
	 * Determines if reference counting should be used on object.
	 */
	void setUseReference( bool flag );
    
	/** 
	 * Add a light to light model 
	 * 
	 * If addReference is not called before adding the light to 
	 * the light model, it is owned by the light model instance and
	 * is deleted if not needed anymore.
	 */
	void addLight( COldLight *light );

	/** Removes a light from the light model */
	COldLight* removeLight( int index );

	/** Deletes a light from the model */
	void deleteLight( int index );
	
	/** Returns the i:th light*/
	COldLight* getLight( int i );
	const COldLight* getLight( int i ) const;
	
	/** Returns the total number of lights */
	int getNoLights() const;

	/** Activate all lights */
	void activate();

	/** Deactivate all lights */
	void deactivate();

	/** Disable all OpenGL lights */
	void disableAll();

	void deleteAll();
protected:
	COldLightModel& operator = ( const COldLightModel& );
	COldLightModel( const COldLightModel& );
	
private:
	bool m_usereference;
	std::vector<COldLight*> m_lights;
	
	GLfloat m_ambient[4];
	GLint m_local[1], m_twoside[1];
};

#endif /* IvfOldLightModel_H */
