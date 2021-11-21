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
class IVFWIDGET_API CCreateShapeEvent : public CEventBase {
public:
	virtual void onCreateShapeEvent(double x, double y, double z, CShapePtr& shape) {};
};

/** Move selection event class */
class IVFWIDGET_API CMoveSelectionEvent : public CEventBase {
public:
	virtual bool onMoveSelection() {return true;};
};

/** Finish move event class */
class IVFWIDGET_API CFinishMoveEvent : public CEventBase {
public:
	virtual void onFinishMove() {};
};

/** Copy shape event class */
class IVFWIDGET_API CCopyShapeEvent : public CEventBase {
public:
	virtual void onCopyShape(double x, double y, double z, CShapePtr& shape, CShapePtr& newShape) {};
};

/** Finish copy event class */
class IVFWIDGET_API CFinishCopyEvent : public CEventBase {
public:
	virtual void onFinishCopy() {};
};

/** Cursor update event class */
class IVFWIDGET_API CCursorUpdateEvent : public CEventBase {
public:
	virtual void onCursorUpdate() {};
};

IvfSmartPointer(CShapePlacementHandler);

/**
 * Experimental shape placement handler
 *
 * Not yet supported....
 */
class IVFWIDGET_API CShapePlacementHandler : public CCoordinateInputHandler,
	CMouseDown3dEvent,
	CMouseMove3dEvent,
	CMouseUp3dEvent
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
	CNodeCursorPtr				m_cursor;
	CRulerPtr					m_ruler;
	CScenePtr					m_scene;
	CCompositePtr				m_composite;

	CShapePtr					m_shapeRepresentation;
	CShapePtr					m_moveShape;

	TInputMethod					m_inputMethod;
	TOperatingMode					m_operatingMode;

	int								m_clickCount;

	CCreateShapeEvent*			m_createShapeEvent;
	CMoveSelectionEvent*			m_moveSelectionEvent;
	CCopyShapeEvent*				m_copyShapeEvent;
	CFinishCopyEvent*			m_finishCopyEvent;
	CFinishMoveEvent*			m_finishMoveEvent;
	CCursorUpdateEvent*			m_cursorUpdateEvent;

	bool							m_lastButtonLeft;

	CVec3d						m_startPoint;
	CVec3d						m_currentPoint;
	CVec3d						m_endPoint;
	CVec3d						m_moveDelta;

	CShapeSelectionPtr			m_shapeSelection;

	bool							m_moving;
	bool							m_moveDone;

public:
	void reset();
	CShapePlacementHandler(CWidgetBase* widget, CCamera* camera, CScene* scene, CComposite* composite);
	virtual ~CShapePlacementHandler();

	IvfClassInfo("CShapePlacementHandler",CCoordinateInputHandler);

	void setShapeRepresentation(CShape* shape);

	void setInputMethod(TInputMethod method);
	TInputMethod getInputMethod();

	CNodeCursor* getCursor();

	void setCreateShapeEvent(CCreateShapeEvent* event);
	void setMoveSelectionEvent(CMoveSelectionEvent* event);
	void setCursorUpdateEvent(CCursorUpdateEvent* event);
	void setFinishMoveEvent(CFinishMoveEvent* event);
	void setFinishCopyEvent(CFinishCopyEvent* event);
	void setCopyShapeEvent(CCopyShapeEvent* event);

	void setMoveShape(CShape* shape);

	TOperatingMode getOperatingMode();
	void setOperatingMode(TOperatingMode mode);

	void initiateMove(CVec3d& vec);
	void initiateMove(double x, double y, double z);
	void setShapeSelection(CShapeSelection* shapeSelection);
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