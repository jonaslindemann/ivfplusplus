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

#ifndef _CIvfUIButton_h_
#define _CIvfUIButton_h_

#include <ivf3dui/IvfUIButtonBase.h>

#include <ivfmath/IvfVec3d.h>

#include <ivf/IvfTransform.h>

IvfSmartPointer(CIvfUIButton);

class IVF3DUI_API CIvfUIButton : public CIvfUIButtonBase {
public:
	enum TVisualAction {
		VA_SCALED,
		VA_MOVING,
		VA_SHAPES
	};
private:
	TVisualAction m_visualAction;
	CIvfVec3d m_movementDirection;
	CIvfVec3d m_originalPosition;
	double m_movementDistance;

	CIvfShapePtr m_upShape;
	CIvfShapePtr m_downShape;
	CIvfShapePtr m_shape;

	CIvfTransformPtr m_movementXfm;

	void visualUp();
	void visualDown();
	void visualRestore();

public:
	CIvfUIButton();
	virtual ~CIvfUIButton();

	IvfClassInfo("CIvfUIButton",CIvfUIButtonBase);

	void setDownShape(CIvfShape* shape);
	void setUpShape(CIvfShape* shape);
	void setShape(CIvfShape* shape);
	void setMovementDirection(CIvfVec3d vec);
	void setVisualAction(TVisualAction action);
	double getMovementDistance();
	void setMovementDistance(double dist);
	void setMovementDirection(double vx, double vy, double vz);

	virtual void doControlLeave(CIvfVec3d vec);
	virtual void doControlOver(CIvfVec3d vec);
	virtual void doControlDown(CIvfVec3d vec, int button);
	virtual void doControlUp(CIvfVec3d vec);

	virtual void doStateChange();
};

#endif 
