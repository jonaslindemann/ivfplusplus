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

#ifndef _CIvfShapePlacementHandler_h_
#define _CIvfShapePlacementHandler_h_

#include <ivfwidget/IvfCoordinateInputHandler.h>

#include <ivf/IvfShapeSelection.h>
#include <ivf/IvfNodeCursor.h>
#include <ivf/IvfRuler.h>

/** Create shape event class */
class IVFWIDGET_API CIvfCreateShapeEvent : public CIvfEventBase {
public:
	virtual void onCreateShapeEvent(double x, double y, double z, CIvfShapePtr& shape) {};
};

/** Move selection event class */
class IVFWIDGET_API CIvfMoveSelectionEvent : public CIvfEventBase {
public:
	virtual bool onMoveSelection() {return true;};
};

/** Finish move event class */
class IVFWIDGET_API CIvfFinishMoveEvent : public CIvfEventBase {
public:
	virtual void onFinishMove() {};
};

/** Copy shape event class */
class IVFWIDGET_API CIvfCopyShapeEvent : public CIvfEventBase {
public:
	virtual void onCopyShape(double x, double y, double z, CIvfShapePtr& shape, CIvfShapePtr& newShape) {};
};

/** Finish copy event class */
class IVFWIDGET_API CIvfFinishCopyEvent : public CIvfEventBase {
public:
	virtual void onFinishCopy() {};
};

/** Cursor update event class */
class IVFWIDGET_API CIvfCursorUpdateEvent : public CIvfEventBase {
public:
	virtual void onCursorUpdate() {};
};

IvfSmartPointer(CIvfShapePlacementHandler);

/**
 * Experimental shape placement handler
 *
 * Not yet supported....
 */
class IVFWIDGET_API CIvfShapePlacementHandler : public CIvfCoordinateInputHandler,
	CIvfMouseDown3dEvent,
	CIvfMouseMove3dEvent,
	CIvfMouseUp3dEvent
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
	CIvfNodeCursorPtr				m_cursor;
	CIvfRulerPtr					m_ruler;
	CIvfScenePtr					m_scene;
	CIvfCompositePtr				m_composite;

	CIvfShapePtr					m_shapeRepresentation;
	CIvfShapePtr					m_moveShape;

	TInputMethod					m_inputMethod;
	TOperatingMode					m_operatingMode;

	int								m_clickCount;

	CIvfCreateShapeEvent*			m_createShapeEvent;
	CIvfMoveSelectionEvent*			m_moveSelectionEvent;
	CIvfCopyShapeEvent*				m_copyShapeEvent;
	CIvfFinishCopyEvent*			m_finishCopyEvent;
	CIvfFinishMoveEvent*			m_finishMoveEvent;
	CIvfCursorUpdateEvent*			m_cursorUpdateEvent;

	bool							m_lastButtonLeft;

	CIvfVec3d						m_startPoint;
	CIvfVec3d						m_currentPoint;
	CIvfVec3d						m_endPoint;
	CIvfVec3d						m_moveDelta;

	CIvfShapeSelectionPtr			m_shapeSelection;

	bool							m_moving;
	bool							m_moveDone;

public:
	void reset();
	CIvfShapePlacementHandler(CIvfWidgetBase* widget, CIvfCamera* camera, CIvfScene* scene, CIvfComposite* composite);
	virtual ~CIvfShapePlacementHandler();

	IvfClassInfo("CIvfShapePlacementHandler",CIvfCoordinateInputHandler);

	void setShapeRepresentation(CIvfShape* shape);

	void setInputMethod(TInputMethod method);
	TInputMethod getInputMethod();

	CIvfNodeCursor* getCursor();

	void setCreateShapeEvent(CIvfCreateShapeEvent* event);
	void setMoveSelectionEvent(CIvfMoveSelectionEvent* event);
	void setCursorUpdateEvent(CIvfCursorUpdateEvent* event);
	void setFinishMoveEvent(CIvfFinishMoveEvent* event);
	void setFinishCopyEvent(CIvfFinishCopyEvent* event);
	void setCopyShapeEvent(CIvfCopyShapeEvent* event);

	void setMoveShape(CIvfShape* shape);

	TOperatingMode getOperatingMode();
	void setOperatingMode(TOperatingMode mode);

	void initiateMove(CIvfVec3d& vec);
	void initiateMove(double x, double y, double z);
	void setShapeSelection(CIvfShapeSelection* shapeSelection);
	bool isMoving();
	void finalizeMove();

	virtual void doLockXZ(bool flag);
	virtual void doDeactivate();
	virtual void doActivate();

	virtual void onMouseDown3d(double x, double y, double z);
	virtual void onMouseMove3d(double x, double y, double z);
	virtual void onMouseUp3d(double x, double y, double z);
};

#endif 
