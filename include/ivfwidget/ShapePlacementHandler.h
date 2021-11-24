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

#include <ivfwidget/CoordinateInputHandler.h>

#include <ivf/ShapeSelection.h>
#include <ivf/NodeCursor.h>
#include <ivf/Ruler.h>

namespace ivf {

/** Create shape event class */
class IVFWIDGET_API CreateShapeEvent : public EventBase {
public:
	virtual void onCreateShapeEvent(double x, double y, double z, ShapePtr& shape) {};
};

/** Move selection event class */
class IVFWIDGET_API MoveSelectionEvent : public EventBase {
public:
	virtual bool onMoveSelection() {return true;};
};

/** Finish move event class */
class IVFWIDGET_API FinishMoveEvent : public EventBase {
public:
	virtual void onFinishMove() {};
};

/** Copy shape event class */
class IVFWIDGET_API CopyShapeEvent : public EventBase {
public:
	virtual void onCopyShape(double x, double y, double z, ShapePtr& shape, ShapePtr& newShape) {};
};

/** Finish copy event class */
class IVFWIDGET_API FinishCopyEvent : public EventBase {
public:
	virtual void onFinishCopy() {};
};

/** Cursor update event class */
class IVFWIDGET_API CursorUpdateEvent : public EventBase {
public:
	virtual void onCursorUpdate() {};
};

IvfSmartPointer(ShapePlacementHandler);

/**
 * Experimental shape placement handler
 *
 * Not yet supported....
 */
class IVFWIDGET_API ShapePlacementHandler : public CoordinateInputHandler,
	MouseDown3dEvent,
	MouseMove3dEvent,
	MouseUp3dEvent
{
public:
	enum TInputMethod {
		IM_TWO_CLICK,
		IM_CLICK_MODIFIER
	};
	enum TOperatingMode {
		OM_CREATE_SHAPE,
		OM_MOVE_SHAPE,
		OM_COPY_SHAPE
	};
private:
	NodeCursorPtr				m_cursor;
	RulerPtr					m_ruler;
	ScenePtr					m_scene;
	CompositePtr				m_composite;

	ShapePtr					m_shapeRepresentation;
	ShapePtr					m_moveShape;

	TInputMethod					m_inputMethod;
	TOperatingMode					m_operatingMode;

	int								m_clickCount;

	CreateShapeEvent*			m_createShapeEvent;
	MoveSelectionEvent*			m_moveSelectionEvent;
	CopyShapeEvent*				m_copyShapeEvent;
	FinishCopyEvent*			m_finishCopyEvent;
	FinishMoveEvent*			m_finishMoveEvent;
	CursorUpdateEvent*			m_cursorUpdateEvent;

	bool							m_lastButtonLeft;

	Vec3d						m_startPoint;
	Vec3d						m_currentPoint;
	Vec3d						m_endPoint;
	Vec3d						m_moveDelta;

	ShapeSelectionPtr			m_shapeSelection;

	bool							m_moving;
	bool							m_moveDone;

public:
	void reset();
	ShapePlacementHandler(WidgetBase* widget, Camera* camera, Scene* scene, Composite* composite);
	virtual ~ShapePlacementHandler();

	static ShapePlacementHandlerPtr create(WidgetBase* widget, Camera* camera, Scene* scene, Composite* composite)
	{
		return ShapePlacementHandlerPtr(new ShapePlacementHandler(widget, camera, scene, composite));
	}

	IvfClassInfo("ShapePlacementHandler",CoordinateInputHandler);

	void setShapeRepresentation(Shape* shape);

	void setInputMethod(TInputMethod method);
	TInputMethod getInputMethod();

	NodeCursor* getCursor();

	void setCreateShapeEvent(CreateShapeEvent* event);
	void setMoveSelectionEvent(MoveSelectionEvent* event);
	void setCursorUpdateEvent(CursorUpdateEvent* event);
	void setFinishMoveEvent(FinishMoveEvent* event);
	void setFinishCopyEvent(FinishCopyEvent* event);
	void setCopyShapeEvent(CopyShapeEvent* event);

	void setMoveShape(Shape* shape);

	TOperatingMode getOperatingMode();
	void setOperatingMode(TOperatingMode mode);

	void initiateMove(Vec3d& vec);
	void initiateMove(double x, double y, double z);
	void setShapeSelection(ShapeSelection* shapeSelection);
	bool isMoving();
	void finalizeMove();

	virtual void doLockXZ(bool flag);
	virtual void doDeactivate();
	virtual void doActivate();

	virtual void onMouseDown3d(double x, double y, double z);
	virtual void onMouseMove3d(double x, double y, double z);
	virtual void onMouseUp3d(double x, double y, double z);
};

}