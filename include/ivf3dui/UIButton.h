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

#pragma once

#include <ivf3dui/UIButtonBase.h>

#include <ivfmath/Vec3d.h>

#include <ivf/Transform.h>

namespace ivf {

IvfSmartPointer(CUIButton);

class IVF3DUI_API CUIButton : public CUIButtonBase {
public:
	enum TVisualAction {
		VA_SCALED,
		VA_MOVING,
		VA_SHAPES
	};
private:
	TVisualAction m_visualAction;
	CVec3d m_movementDirection;
	CVec3d m_originalPosition;
	double m_movementDistance;

	CShapePtr m_upShape;
	CShapePtr m_downShape;
	CShapePtr m_shape;

	CTransformPtr m_movementXfm;

	void visualUp();
	void visualDown();
	void visualRestore();

public:
	CUIButton();
	virtual ~CUIButton();

	IvfClassInfo("CUIButton",CUIButtonBase);

	void setDownShape(CShape* shape);
	void setUpShape(CShape* shape);
	void setShape(CShape* shape);
	void setMovementDirection(CVec3d vec);
	void setVisualAction(TVisualAction action);
	double getMovementDistance();
	void setMovementDistance(double dist);
	void setMovementDirection(double vx, double vy, double vz);

	virtual void doControlLeave(CVec3d vec);
	virtual void doControlOver(CVec3d vec);
	virtual void doControlDown(CVec3d vec, int button);
	virtual void doControlUp(CVec3d vec);

	virtual void doStateChange();
};

}