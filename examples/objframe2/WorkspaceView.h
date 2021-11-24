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

class ViewModeChangeEvent {
public:
	virtual void onViewModeChange(TViewModeEvent newMode) {};
};

class CoordinateEvent {
public:
	virtual void onCoordinate(double x, double y, double z) {};
};

class SelectionChangedEvent {
public:
	virtual void onSelectionChanged() {};
};

IvfSmartPointer(WorkspaceView);

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

class WorkspaceView: public ivf::FltkBase,
	ivf::KeyboardEvent,
	ivf::CreateShapeEvent,
	ivf::SingleSelectionEvent,
	ivf::MouseDownEvent,
	ivf::MouseUpEvent,
	ivf::ModifierDownEvent,
	ivf::ModifierUpEvent,
	ivf::HighlightEvent,
	ivf::MenuItemEvent,
	ivf::MoveSelectionEvent,
	ivf::MouseMove3dEvent,
	ivf::CopyShapeEvent,
	ivf::FinishCopyEvent,
	ivf::FinishMoveEvent,
	ivf::CursorUpdateEvent,
	ivf::MouseMoveEvent
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
	ivf::CameraPtr					m_camera;
	ivf::LightPtr					m_light;
	ivf::ScenePtr					m_scene;
	ivf::MaterialPtr				m_nodeMaterial;
	ivf::MaterialPtr				m_lineMaterial;
	ivf::MaterialPtr				m_vectorMaterial;
	ivf::MaterialPtr				m_movementMaterial;

	ivf::ShapeStdPtr				m_oldShape;
	ivf::ShapeSelectionPtr			m_selection;
	ivf::ShapeSelectionPtr			m_lineSelection;

	ivf::NodeCursorPtr				m_selectionCursor;
	ivf::NodeCursorPtr				m_movementCursor;
	ivf::ExtrArrowPtr				m_loadCursor;
	VectorCursorPtr					m_vectorCursor;

	ivf::ShapeStdPtr				m_moveShape;

	ivf::CompositePtr				m_nodes;
	ivf::CompositePtr				m_lines;
	ivf::CompositePtr				m_vectors;

	ivf::CompositePtr				m_movementShapes;

	ivf::MouseViewHandlerPtr		m_mouseViewHandler;
	ivf::SceneHandlerPtr			m_sceneHandler;
	ivf::ShapePlacementHandlerPtr	m_shapePlacementHandler;
	ivf::SelectionHandlerPtr		m_selectionHandler;

	TViewMode						m_viewMode;
	TViewMode						m_savedViewMode;

	ViewModeChangeEvent*			m_viewModeChangeEvent;			
	CoordinateEvent*				m_coordinateEvent;
	SelectionChangedEvent*			m_selectionChangedEvent;

	int								m_nodeCount;

	int								m_mouseDownPos[2];
	int								m_vectorClickCount;

public:
	WorkspaceView(int X, int Y, int W, int H, const char* L);

	void setViewMode(TViewMode mode);
	void restoreViewMode();
	void saveViewMode();

	void deleteSelectedNodes();
	void selectAll();
	void deleteNodes();

	void open(const char* filename);
	void save();

	ivf::ShapeSelection* getSelection();

	void createScenario(TScenarioType scenarioType);
	void createSimpleStructure(int rows, int cols, int stacks);


	void setViewModeChangeEvent(ViewModeChangeEvent* event);
	void setCoordinateEvent(CoordinateEvent* event);
	void setSelectionChangedEvent(SelectionChangedEvent* event);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual void onFunctionKey(WidgetBase::TFunctionKey key, int x, int y);

	virtual void onCreateShapeEvent(double x, double y, double z, ivf::ShapePtr& shape);

	virtual void onSelect(ivf::Shape* shape);
	virtual void onHighlight(ivf::Shape* shape);

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);

	virtual void onModifierDown();
	virtual void onModifierUp();

	virtual bool onMoveSelection();
	virtual void onCopyShape(double x, double y, double z, ivf::ShapePtr& shape, ivf::ShapePtr& newShape);
	virtual void onFinishCopy();
	virtual void onFinishMove();

	virtual void onMouseMove3d(double x, double y, double z);

	virtual void onCursorUpdate();
};

#endif
