#ifndef _CWorkspaceView_h_
#define _CWorkspaceView_h_

#include <ivf/Camera.h>
#include <ivf/Cube.h>
#include <ivf/Sphere.h>
#include <ivf/Cylinder.h>
#include <ivf/QuadPlane.h>
#include <ivf/Grid.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Grid.h>
#include <ivf/GlobalState.h>
#include <ivf/Scene.h>
#include <ivf/BlendState.h>
#include <ivf/Node.h>
#include <ivf/SolidLine.h>
#include <ivf/ExtrArrow.h>
#include <ivf/NodeCursor.h>
#include <ivf/Ruler.h>
#include <ivf/ShapeSelection.h>
#include <ivf/PixelOps.h>
#include <ivf/CompositeState.h>

#include <ivfwidget/MouseViewHandler.h>
#include <ivfwidget/SceneHandler.h>
#include <ivfwidget/SelectionHandler.h>
#include <ivfwidget/CoordinateInputHandler.h>
#include <ivfwidget/ShapePlacementHandler.h>

#include <ivffltk/FltkBase.h>

#include "VectorCursor.h"

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

class CWorkspaceView: public ivf::CFltkBase,
	ivf::CKeyboardEvent,
	ivf::CCreateShapeEvent,
	ivf::CSingleSelectionEvent,
	ivf::CMouseDownEvent,
	ivf::CMouseUpEvent,
	ivf::CModifierDownEvent,
	ivf::CModifierUpEvent,
	ivf::CHighlightEvent,
	ivf::CMenuItemEvent,
	ivf::CMoveSelectionEvent,
	ivf::CMouseMove3dEvent,
	ivf::CCopyShapeEvent,
	ivf::CFinishCopyEvent,
	ivf::CFinishMoveEvent,
	ivf::CCursorUpdateEvent,
	ivf::CMouseMoveEvent
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
	ivf::CCameraPtr					m_camera;
	ivf::CLightPtr					m_light;
	ivf::CScenePtr					m_scene;
	ivf::CMaterialPtr					m_nodeMaterial;
	ivf::CMaterialPtr					m_lineMaterial;
	ivf::CMaterialPtr					m_vectorMaterial;
	ivf::CMaterialPtr					m_movementMaterial;

	ivf::CShapeStdPtr					m_oldShape;
	ivf::CShapeSelectionPtr			m_selection;
	ivf::CShapeSelectionPtr			m_lineSelection;

	ivf::CNodeCursorPtr				m_selectionCursor;
	ivf::CNodeCursorPtr				m_movementCursor;
	ivf::CExtrArrowPtr				m_loadCursor;
	CVectorCursorPtr				m_vectorCursor;

	ivf::CShapeStdPtr					m_moveShape;

	ivf::CCompositePtr				m_nodes;
	ivf::CCompositePtr				m_lines;
	ivf::CCompositePtr				m_vectors;

	ivf::CCompositePtr				m_movementShapes;

	ivf::CMouseViewHandlerPtr			m_mouseViewHandler;
	ivf::CSceneHandlerPtr				m_sceneHandler;
	ivf::CShapePlacementHandlerPtr	m_shapePlacementHandler;
	ivf::CSelectionHandlerPtr			m_selectionHandler;

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

	ivf::CShapeSelection* getSelection();

	void createScenario(TScenarioType scenarioType);
	void createSimpleStructure(int rows, int cols, int stacks);


	void setViewModeChangeEvent(CViewModeChangeEvent* event);
	void setCoordinateEvent(CCoordinateEvent* event);
	void setSelectionChangedEvent(CSelectionChangedEvent* event);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual void onFunctionKey(CWidgetBase::TFunctionKey key, int x, int y);

	virtual void onCreateShapeEvent(double x, double y, double z, ivf::CShapePtr& shape);

	virtual void onSelect(ivf::CShape* shape);
	virtual void onHighlight(ivf::CShape* shape);

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);

	virtual void onModifierDown();
	virtual void onModifierUp();

	virtual bool onMoveSelection();
	virtual void onCopyShape(double x, double y, double z, ivf::CShapePtr& shape, ivf::CShapePtr& newShape);
	virtual void onFinishCopy();
	virtual void onFinishMove();

	virtual void onMouseMove3d(double x, double y, double z);

	virtual void onCursorUpdate();
};

#endif
