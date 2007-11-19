#ifndef _CWorkspaceView_h_
#define _CWorkspaceView_h_

#include <ivf/IvfCamera.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>
#include <ivf/IvfQuadPlane.h>
#include <ivf/IvfGrid.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfGrid.h>
#include <ivf/IvfGlobalState.h>
#include <ivf/IvfScene.h>
#include <ivf/IvfBlendState.h>
#include <ivf/IvfNode.h>
#include <ivf/IvfSolidLine.h>
#include <ivf/IvfExtrArrow.h>
#include <ivf/IvfNodeCursor.h>
#include <ivf/IvfRuler.h>
#include <ivf/IvfShapeSelection.h>
#include <ivf/IvfPixelOps.h>
#include <ivf/IvfCompositeState.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>
#include <ivfwidget/IvfSelectionHandler.h>
#include <ivfwidget/IvfCoordinateInputHandler.h>
#include <ivfwidget/IvfShapePlacementHandler.h>

#include <ivffltk/IvfFltkBase.h>

#include "IvfVectorCursor.h"

enum TViewModeEvent {
	VM_CREATE_SHAPES,
	VM_CREATE_LINES,
	VM_CREATE_VECTORS,
	VM_MOVE_SHAPES,
	VM_COPY_SHAPES,
	VM_SELECT_SHAPES,
	VM_NONE
};

enum TScenarioType {
	ST_INITIAL,
	ST_MOVE_SINGLE,
	ST_COPY_SINGLE,
	ST_MOVE_MULTIPLE,
	ST_COPY_MULTIPLE,
	ST_DELETE_SINGLE,
	ST_DELETE_MULTIPLE,
	ST_CREATE_LINES,
	ST_CREATE_LOADS
};

class CViewModeChangeEvent {
public:
	virtual void onViewModeChange(TViewModeEvent newMode) {};
};

class CCoordinateEvent {
public:
	virtual void onCoordinate(double x, double y, double z) {};
};

class CSelectionChangedEvent {
public:
	virtual void onSelectionChanged() {};
};

IvfSmartPointer(CWorkspaceView);

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

class CWorkspaceView: public CIvfFltkBase,
	CIvfKeyboardEvent,
	CIvfCreateShapeEvent,
	CIvfSingleSelectionEvent,
	CIvfMouseDownEvent,
	CIvfMouseUpEvent,
	CIvfModifierDownEvent,
	CIvfModifierUpEvent,
	CIvfHighlightEvent,
	CIvfMenuItemEvent,
	CIvfMoveSelectionEvent,
	CIvfMouseMove3dEvent,
	CIvfCopyShapeEvent,
	CIvfFinishCopyEvent,
	CIvfFinishMoveEvent,
	CIvfCursorUpdateEvent,
	CIvfMouseMoveEvent
{
public:
	enum TViewMode {
		VM_CREATE_SHAPES,
		VM_CREATE_LINES,
		VM_CREATE_VECTORS,
		VM_MOVE_SHAPES,
		VM_COPY_SHAPES,
		VM_SELECT_SHAPES,
		VM_NONE
	};

	enum TScenarioType {
		ST_INITIAL,
		ST_MOVE_SINGLE,
		ST_COPY_SINGLE,
		ST_MOVE_MULTIPLE,
		ST_COPY_MULTIPLE,
		ST_DELETE_SINGLE,
		ST_DELETE_MULTIPLE,
		ST_CREATE_LINES,
		ST_CREATE_LOADS
	};
private:
	CIvfCameraPtr					m_camera;
	CIvfLightPtr					m_light;
	CIvfScenePtr					m_scene;
	CIvfMaterialPtr					m_nodeMaterial;
	CIvfMaterialPtr					m_lineMaterial;
	CIvfMaterialPtr					m_vectorMaterial;
	CIvfMaterialPtr					m_movementMaterial;

	CIvfShapeStdPtr					m_oldShape;
	CIvfShapeSelectionPtr			m_selection;
	CIvfShapeSelectionPtr			m_lineSelection;

	CIvfNodeCursorPtr				m_selectionCursor;
	CIvfNodeCursorPtr				m_movementCursor;
	CIvfExtrArrowPtr				m_loadCursor;
	CIvfVectorCursorPtr				m_vectorCursor;

	CIvfShapeStdPtr					m_moveShape;

	CIvfCompositePtr				m_nodes;
	CIvfCompositePtr				m_lines;
	CIvfCompositePtr				m_vectors;

	CIvfCompositePtr				m_movementShapes;

	CIvfMouseViewHandlerPtr			m_mouseViewHandler;
	CIvfSceneHandlerPtr				m_sceneHandler;
	CIvfShapePlacementHandlerPtr	m_shapePlacementHandler;
	CIvfSelectionHandlerPtr			m_selectionHandler;

	TViewMode						m_viewMode;
	TViewMode						m_savedViewMode;

	CViewModeChangeEvent*			m_viewModeChangeEvent;			
	CCoordinateEvent*				m_coordinateEvent;
	CSelectionChangedEvent*			m_selectionChangedEvent;

	int								m_nodeCount;

	int								m_mouseDownPos[2];
	int								m_vectorClickCount;

public:
	CWorkspaceView(int X, int Y, int W, int H, const char* L);

	void setViewMode(TViewMode mode);
	void restoreViewMode();
	void saveViewMode();

	void deleteSelectedNodes();
	void selectAll();
	void deleteNodes();

	void open(const char* filename);
	void save();

	CIvfShapeSelection* getSelection();

	void createScenario(TScenarioType scenarioType);
	void createSimpleStructure(int rows, int cols, int stacks);


	void setViewModeChangeEvent(CViewModeChangeEvent* event);
	void setCoordinateEvent(CCoordinateEvent* event);
	void setSelectionChangedEvent(CSelectionChangedEvent* event);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual void onFunctionKey(CIvfWidgetBase::TFunctionKey key, int x, int y);

	virtual void onCreateShapeEvent(double x, double y, double z, CIvfShapePtr& shape);

	virtual void onSelect(CIvfShape* shape);
	virtual void onHighlight(CIvfShape* shape);

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);

	virtual void onModifierDown();
	virtual void onModifierUp();

	virtual bool onMoveSelection();
	virtual void onCopyShape(double x, double y, double z, CIvfShapePtr& shape, CIvfShapePtr& newShape);
	virtual void onFinishCopy();
	virtual void onFinishMove();

	virtual void onMouseMove3d(double x, double y, double z);

	virtual void onCursorUpdate();
};

#endif
