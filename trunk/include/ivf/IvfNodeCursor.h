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

#ifndef _CIvfNodeCursor_h_
#define _CIvfNodeCursor_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfArrow.h>
#include <ivf/IvfQuadPlane.h>

IvfSmartPointer(CIvfNodeCursor);

/**
 * Node cursor class
 *
 * Implements a special cursor for manipulating nodes
 */
class IVF_API CIvfNodeCursor : public CIvfShape {
public:
	enum TCursorType {
		CT_XZ,
		CT_Y,
		CT_XYZ,
		CT_Y_TOP,
		CT_NONE
	};
	enum TCursorDirection {
		CD_OUTWARDS,
		CD_INWARDS
	};
	enum TCursorArrowType {
		AT_NORMAL,
		AT_SIMPLE
	};
private:
	double m_size;
	double m_shapeSize;
	TCursorType m_cursorType;
	TCursorDirection m_cursorDirection;
	TCursorArrowType m_cursorArrowType;

	CIvfArrowPtr m_arrow;
	
	CIvfTransformPtr m_arrowXZ1;
	CIvfTransformPtr m_arrowXZ2;
	CIvfTransformPtr m_arrowXZ3;
	CIvfTransformPtr m_arrowXZ4;

	CIvfTransformPtr m_arrowY1;
	CIvfTransformPtr m_arrowY2;

	CIvfCompositePtr m_cursorShape;

	CIvfShapePtr	 m_nodeShape;

	CIvfQuadPlanePtr	m_plane;

	bool			m_usePlane;

	void initCursor();
public:
	/** Class constructor */
	CIvfNodeCursor();

	/** Class destructor */
	virtual ~CIvfNodeCursor();

	IvfClassInfo("CIvfNodeCursor",CIvfShape);

	/** Set size of cursor */
	void setSize(double size);

	/** Return cursor size */
	double getSize();

	/** Set cursor type */
	void setType(TCursorType type);

	/** Set shape used at center of cursor */
	void setShape(CIvfShape* shape);

	/** Return shape at center of cursor */
	CIvfShape* getShape();

	/** Enable/disable special plane helper object */
	void setUsePlaneHelper(bool flag);

	/** Return state of helper object */
	bool getUsePlaneHelper();

	/** Set arrow types see TCursorType */
	void setArrowType(TCursorArrowType type);

	/** Return arrow type */
	TCursorArrowType getArrowType();

	/** Set direction of cursor arrow see TCursorDirection. */
	void setDirection(TCursorDirection direction);

	/** Return cursor direction */
	TCursorDirection getDirection();

	/** Sets the size of the shape */
	void setShapeSize(double size);
protected:
	virtual void createGeometry();
};

#endif 
