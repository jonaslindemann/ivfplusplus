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

#pragma once

#include <ivf3dui/UIButtonBase.h>

#include <ivfmath/Vec3d.h>

#include <ivf/Transform.h>

namespace ivf {

IvfSmartPointer(UIButton);

class IVF3DUI_API UIButton : public UIButtonBase {
public:
	enum TVisualAction {
		VA_SCALED,
		VA_MOVING,
		VA_SHAPES
	};
private:
	TVisualAction m_visualAction;
	Vec3d m_movementDirection;
	Vec3d m_originalPosition;
	double m_movementDistance;

	ShapePtr m_upShape;
	ShapePtr m_downShape;
	ShapePtr m_shape;

	TransformPtr m_movementXfm;

	void visualUp();
	void visualDown();
	void visualRestore();

public:
	UIButton();
	virtual ~UIButton();

	IvfClassInfo("UIButton",UIButtonBase);
	IvfStdFactory(UIButton);

	void setDownShape(Shape* shape);
	void setUpShape(Shape* shape);
	void setShape(Shape* shape);
	void setMovementDirection(Vec3d vec);
	void setVisualAction(TVisualAction action);
	double getMovementDistance();
	void setMovementDistance(double dist);
	void setMovementDirection(double vx, double vy, double vz);

	virtual void doControlLeave(Vec3d vec);
	virtual void doControlOver(Vec3d vec);
	virtual void doControlDown(Vec3d vec, int button);
	virtual void doControlUp(Vec3d vec);

	virtual void doStateChange();
};

}