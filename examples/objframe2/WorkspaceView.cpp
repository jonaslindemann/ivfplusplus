#include "WorkspaceView.h"

#include <ivf/DepthBufferState.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

WorkspaceView::WorkspaceView(int X, int Y, int W, int H, const char* L)
:FltkBase(X, Y, W, H, L) 
{
	m_viewModeChangeEvent = nullptr;
	m_coordinateEvent = nullptr;
	m_selectionChangedEvent = nullptr;
}

// ------------------------------------------------------------
void WorkspaceView::onInit(int width, int height)
{
	// Initialize variables
	
	m_viewMode = VM_CREATE_SHAPES;
	m_nodeCount = 0;
	m_oldShape = nullptr;
	m_moveShape = nullptr;
	
	ivfSetGLEJoinStyle(TUBE_JN_CAP|TUBE_JN_ANGLE|TUBE_NORM_PATH_EDGE);
	
	// Initialize Ivf++ camera
	
	m_camera = Camera::create();
	m_camera->setPosition(0.0, 4.0, 8.0);
	m_camera->setTarget(0.0, 0.0, 0.0);
	m_camera->setEyeSeparation(0.1);
	m_camera->setPerspective(45.0, 1.0, 100.0);
	
	// Create a materials
	
	m_nodeMaterial = Material::create();
	m_nodeMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_nodeMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_nodeMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);
	
	m_lineMaterial = Material::create();
	m_lineMaterial->setDiffuseColor(0.3f, 0.8f, 0.0f, 1.0f);
	m_lineMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lineMaterial->setAmbientColor(0.0f, 0.2f, 0.0f, 1.0f);
	
	m_vectorMaterial = Material::create();
	m_vectorMaterial->setDiffuseColor(0.0f, 0.5f, 1.0f, 1.0f);
	m_vectorMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_vectorMaterial->setAmbientColor(0.0f, 0.1f, 0.2f, 1.0f);

	m_movementMaterial = Material::create();
	m_movementMaterial->setDiffuseColor(0.8f, 0.8f, 0.0f, 0.5f);
	m_movementMaterial->setSpecularColor(1.0f, 1.0f, 0.0f, 0.8f);
	m_movementMaterial->setAmbientColor(0.3f, 0.3f, 0.0f, 0.5f);
	
	auto lineSelectionMaterial = Material::create();
	lineSelectionMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	lineSelectionMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	lineSelectionMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);
	
	// Create scene
	
	m_scene = Scene::create();
	m_scene->setCamera(m_camera);
	m_scene->setLightMode(SceneBase::LM_LOCAL);
	
	auto grid = Grid::create();
	grid->setSize(10.0, 10.0);
	grid->setGridSpacing(0.5);
	grid->refresh();
	m_scene->addChild(grid);
	
	auto cube = Cube::create();
	cube->setSize(0.1);
	
	// Make vector arrows blend over existing
	// geometry
	
	auto blendState = BlendState::create();
	blendState->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create cursors
	
	m_selectionCursor = NodeCursor::create();
	m_selectionCursor->setType(NodeCursor::CT_Y_TOP);
	m_selectionCursor->disable();
	m_selectionCursor->setDirection(NodeCursor::CD_INWARDS);
	m_selectionCursor->setArrowType(NodeCursor::AT_SIMPLE);
	m_selectionCursor->setSize(0.5);
	m_selectionCursor->setShapeSize(0.1);
	
	m_movementCursor = NodeCursor::create();
	m_movementCursor->setType(NodeCursor::CT_XZ);
	m_movementCursor->disable();
	m_movementCursor->setSize(0.5);
	m_movementCursor->setShapeSize(0.1);
	m_movementCursor->setMaterial(m_movementMaterial);
	m_movementCursor->setRenderState(blendState);
	
	double arrowSize = 0.5;
	
	m_vectorCursor = VectorCursor::create();
	m_vectorCursor->setType(VectorCursor::CT_VECTOR);
	m_vectorCursor->disable();

	// Shape selection for handling line creation
	
	m_selection = ShapeSelection::create();
	m_lineSelection = ShapeSelection::create();
	m_lineSelection->setHighlightMaterial(lineSelectionMaterial);
	
	// Add all cursor and selection to scene
	
	m_scene->addChild(m_selectionCursor);

	// Blended objects are added to the post composite to be sure
	// they are rendered last, as Ivf++ does not do any depth sorting.

	m_scene->getPostComposite()->addChild(m_selection);
	m_scene->getPostComposite()->addChild(m_lineSelection);
	m_scene->getPostComposite()->addChild(m_movementCursor);
	m_scene->getPostComposite()->addChild(m_vectorCursor);
	m_scene->getPostComposite()->addChild(m_vectorCursor);
	
	// Create special composites for the created objects
	
	m_nodes = Composite::create();
	m_scene->addChild(m_nodes);
	
	m_lines = Composite::create();
	m_scene->addChild(m_lines);
	
	m_vectors = Composite::create();
	m_scene->addChild(m_vectors);
	
	// Setup lighting
	
	auto lighting = Lighting::getInstance();
	lighting->enable();
	
	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
	
	// Initialize event handlers
	
	m_sceneHandler = SceneHandler::create(this, m_scene);
	
	m_mouseViewHandler = MouseViewHandler::create(this, m_camera);
	m_mouseViewHandler->setKeyHandling(MouseViewHandler::KH_NORMAL);
	m_mouseViewHandler->activate();
	
	m_selectionHandler = SelectionHandler::create(this, m_scene);
	m_selectionHandler->setSingleSelectionEvent(this);
	m_selectionHandler->setHighlightEvent(this);
	m_selectionHandler->deactivate();
	
	m_shapePlacementHandler = ShapePlacementHandler::create(this, m_camera, m_scene, m_nodes);
	m_shapePlacementHandler->setInputMethod(ShapePlacementHandler::IM_TWO_CLICK);
	m_shapePlacementHandler->setShapeRepresentation(cube);
	m_shapePlacementHandler->getCursor()->setShapeSize(0.2);
	m_shapePlacementHandler->getCursor()->setDirection(NodeCursor::CD_OUTWARDS);
	m_shapePlacementHandler->setCreateShapeEvent(this);
	m_shapePlacementHandler->setMoveSelectionEvent(this);
	m_shapePlacementHandler->addMouseMove3dEvent(this);
	m_shapePlacementHandler->setCopyShapeEvent(this);
	m_shapePlacementHandler->setFinishCopyEvent(this);
	m_shapePlacementHandler->setFinishMoveEvent(this);
	m_shapePlacementHandler->setCursorUpdateEvent(this);
	m_shapePlacementHandler->activate();
	
	// Tell Ivf++ what events we want....
	
	addKeyboardEvent(this);
	addMouseDownEvent(this);
	addMouseMoveEvent(this);
	addMouseUpEvent(this);
	addMenuItemEvent(this);
}

// ------------------------------------------------------------
void WorkspaceView::setViewMode(TViewMode mode)
{
	m_viewMode = mode;
	
	switch (m_viewMode) {
	case VM_CREATE_SHAPES:
		m_shapePlacementHandler->finalizeMove();
		m_shapePlacementHandler->activate();
		m_shapePlacementHandler->setOperatingMode(ShapePlacementHandler::OM_CREATE_SHAPE);
		m_selectionHandler->deactivate();
		m_selectionCursor->disable();
		m_movementCursor->disable();
		break;
	case VM_CREATE_LINES:
		m_nodeCount=0;
		m_shapePlacementHandler->finalizeMove();
		m_shapePlacementHandler->deactivate();
		m_selectionHandler->activate();
		m_selectionCursor->disable();
		m_movementCursor->disable();
		break;
	case VM_CREATE_VECTORS:
		if (!m_vectorCursor->isEnabled())
		{
			m_shapePlacementHandler->finalizeMove();
			m_shapePlacementHandler->deactivate();
			m_selectionHandler->activate();
			m_selectionCursor->disable();
			m_movementCursor->disable();
			m_vectorClickCount=0;
			std::cout << "VCC: " << m_vectorClickCount << std::endl;
		}
		break;
	case VM_SELECT_SHAPES:
		m_shapePlacementHandler->finalizeMove();
		m_selection->setOffset(0.0, 0.0, 0.0);
		m_shapePlacementHandler->deactivate();
		m_selectionHandler->activate();
		m_selectionCursor->disable();
		m_movementCursor->disable();
		break;
	case VM_MOVE_SHAPES:
		if (m_shapePlacementHandler->isMoving())
			m_shapePlacementHandler->activate();
		else
		{
			m_shapePlacementHandler->setOperatingMode(ShapePlacementHandler::OM_MOVE_SHAPE);
			//m_shapePlacementHandler->setOperatingMode(ShapePlacementHandler::OM_MOVE_SHAPE);
			m_selectionHandler->activate();
			m_selectionCursor->disable();
			m_movementCursor->enable();
		}
		redraw();
		break;
	case VM_COPY_SHAPES:
		if (m_shapePlacementHandler->isMoving())
			m_shapePlacementHandler->activate();
		else
		{
			m_shapePlacementHandler->setOperatingMode(ShapePlacementHandler::OM_COPY_SHAPE);
			//m_shapePlacementHandler->setOperatingMode(ShapePlacementHandler::OM_MOVE_SHAPE);
			m_selectionHandler->activate();
			m_selectionCursor->disable();
			m_movementCursor->enable();
		}
		redraw();
		break;
	case VM_NONE:
		m_shapePlacementHandler->deactivate();
		m_selectionHandler->deactivate();
		break;
	default:
		
		break;
	};
}

// ------------------------------------------------------------
void WorkspaceView::saveViewMode()
{
	m_savedViewMode = m_viewMode;
}

// ------------------------------------------------------------
void WorkspaceView::restoreViewMode()
{
	setViewMode(m_savedViewMode);
}

// ------------------------------------------------------------
void WorkspaceView::deleteSelectedNodes()
{
	ShapeSelectionVector vec;
	ShapeSelectionVectorIterator it;
	
	m_selection->getSelection(vec);
	m_selection->clear();
	
	m_moveShape = nullptr;
	m_oldShape = nullptr;
	
	for (it=vec.begin(); it!=vec.end(); it++)
	{
		auto shape = *it;
		
		if (shape->getRefCount()<4)
			m_nodes->removeShape(shape);
		if (shape->isClass("SolidLine"))
			m_lines->removeShape(shape);
	}
	
	vec.clear();

	if (m_selectionChangedEvent)
		m_selectionChangedEvent->onSelectionChanged();
	
	redraw();
}

// ------------------------------------------------------------
void WorkspaceView::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'n':
		setViewMode(VM_CREATE_SHAPES);
		if (m_viewModeChangeEvent!=nullptr)
			m_viewModeChangeEvent->onViewModeChange((TViewModeEvent)m_viewMode);
		redraw();
		break;
	case 's':
		setViewMode(VM_SELECT_SHAPES);
		if (m_viewModeChangeEvent!=nullptr)
			m_viewModeChangeEvent->onViewModeChange((TViewModeEvent)m_viewMode);
		redraw();
		break;
	case 'm':
		setViewMode(VM_MOVE_SHAPES);
		if (m_viewModeChangeEvent!=nullptr)
			m_viewModeChangeEvent->onViewModeChange((TViewModeEvent)m_viewMode);
		redraw();
		break;
	case 'c':
		setViewMode(VM_COPY_SHAPES);
		if (m_viewModeChangeEvent!=nullptr)
			m_viewModeChangeEvent->onViewModeChange((TViewModeEvent)m_viewMode);
		redraw();
		break;
	case 'd':
		deleteSelectedNodes();
		redraw();
		break;
	case 'r':
		deleteNodes();
		redraw();
		break;
	case 'a':
		if (m_viewMode==VM_SELECT_SHAPES)
			selectAll();
		redraw();
		break;
	case 'z':
		save();
		break;
	case '1':
		createScenario(ST_INITIAL);
		break;
	case '2':
		createScenario(ST_MOVE_SINGLE);
		break;
	case '3':
		createScenario(ST_COPY_SINGLE);
		break;
	case '4':
		createScenario(ST_MOVE_MULTIPLE);
		break;
	case '5':
		createScenario(ST_COPY_MULTIPLE);
		break;
	case '6':
		createScenario(ST_DELETE_SINGLE);
		break;
	case '7':
		createScenario(ST_DELETE_MULTIPLE);
		break;
	case '8':
		createScenario(ST_CREATE_LINES);
		break;
	case '9':
		createScenario(ST_CREATE_LOADS);
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
void WorkspaceView::onFunctionKey(WidgetBase::TFunctionKey key, int x, int y)
{
	switch (key) {
	case WidgetBase::FK_ESCAPE:
		m_lineSelection->clear();
		m_selection->clear();
		m_nodeCount=0;
		m_vectorCursor->reset();
		m_vectorCursor->disable();
		m_vectorClickCount = 0;
		setViewMode(m_viewMode);
		m_shapePlacementHandler->reset();
		redraw();
		break;
	case WidgetBase::FK_DELETE:
		deleteSelectedNodes();
		redraw();
		break;
	default:
		
		break;
	}
}

// ------------------------------------------------------------
void WorkspaceView::onCreateShapeEvent(double x, double y, double z, ShapePtr& shape)
{
	auto node = Node::create();
	node->setSize(0.1);
	node->setPosition(x, y, z);
	node->setMaterial(m_nodeMaterial);
	shape = node;
}

// ------------------------------------------------------------
void WorkspaceView::onSelect(Shape* shape)
{
	if (isLeftButtonDown())
	{
		if (shape!=nullptr)
		{
			switch (m_viewMode) {
			case VM_CREATE_LINES:
				if (shape->isClass("Node"))
				{
					m_nodeCount++;
					
					if (m_nodeCount==2)
					{
						m_lineSelection->add(shape);
						
						m_nodeCount = 0;
						
						auto line = SolidLine::create();
						line->setRadius(0.03);
						line->setMaterial(m_lineMaterial);
						
						NodePtr node1 = (Node*)m_lineSelection->getShape(0);
						NodePtr node2 = (Node*)m_lineSelection->getShape(1);
						
						line->setNodes(node1, node2);
						m_lines->addChild(line);
						
						m_lineSelection->clear();
					}
					else
						m_lineSelection->add(shape);
				}
				break;
			case VM_SELECT_SHAPES:
				if (m_selection->contains(shape))
					m_selection->remove(shape);
				else
					m_selection->add(shape);

				if (m_selectionChangedEvent)
					m_selectionChangedEvent->onSelectionChanged();

				break;
			case VM_CREATE_VECTORS:
				if (shape->isClass("Node"))
				{
					m_vectorCursor->setPosition(shape->getPosition());
					m_vectorCursor->setType(VectorCursor::CT_BETA);
					m_vectorCursor->enable();
					m_selectionHandler->deactivate();
					std::cout << "VCC: " << m_vectorClickCount << std::endl;
				}
				break;
			default:
				m_selection->add(shape);
				break;
			}
		}
		else
		{
			m_selection->clear();
			if (m_selectionChangedEvent)
				m_selectionChangedEvent->onSelectionChanged();
		}
	
		redraw();
	}
}

// ------------------------------------------------------------
void WorkspaceView::onHighlight(Shape* shape)
{
	m_moveShape = nullptr;
	
	if (m_oldShape!=nullptr)
		m_oldShape->disableHighlight();
	
	if (shape!=nullptr)
	{
		switch (m_viewMode) {
		case VM_SELECT_SHAPES:
			if (shape->isClass("Node"))
			{
				m_selectionCursor->enable();
				m_selectionCursor->setPosition(shape->getPosition());
			}
			else
				m_selectionCursor->disable();
			
			shape->enableHighlight();
			break;
			
		case VM_CREATE_LINES:
			if (shape->isClass("Node"))
			{
				m_selectionCursor->enable();
				m_selectionCursor->setPosition(shape->getPosition());
			}
			else
				m_selectionCursor->disable();
			
			if (shape->isClass("Node"))
				shape->enableHighlight();
			
			break;
			
		case VM_CREATE_VECTORS:
			
			if (shape->isClass("Node"))
			{
				m_vectorCursor->enable();
				m_vectorCursor->setPosition(shape->getPosition());
			}
			else
				m_vectorCursor->disable();
			
			if (shape->isClass("Node"))
				shape->enableHighlight();
			
			break;
		default:
			
			if (m_selection->contains(shape))
			{
				m_moveShape = shape;
				m_movementCursor->enable();
				m_movementCursor->setPosition(shape->getPosition());
				shape->enableHighlight();
				m_oldShape = shape;
				redraw();
			}
			break;
		}
		
		m_oldShape = shape;
		redraw();
	}
	else
	{
		m_selectionCursor->disable();
		m_movementCursor->disable();
		m_vectorCursor->disable();
		redraw();
	}
}

// ------------------------------------------------------------
void WorkspaceView::onMouseDown(int x, int y)
{
	m_mouseDownPos[0] = x;
	m_mouseDownPos[1] = y;
	
	if (isMiddleButtonDown())
	{
		saveViewMode();
		setViewMode(VM_NONE);
	}
	else
	{
		if (m_viewMode==VM_MOVE_SHAPES)
		{
			if (isLeftButtonDown()&&m_moveShape!=nullptr)
			{
				m_shapePlacementHandler->initiateMove(m_moveShape->getPosition());
				m_moveShape->disableHighlight();
				m_shapePlacementHandler->setShapeSelection(m_selection);
				m_shapePlacementHandler->activate();
				
				m_movementCursor->disable();
				m_selectionHandler->deactivate();
				redraw();
			}
		}
		
		if (m_viewMode==VM_COPY_SHAPES)
		{
			if (isLeftButtonDown()&&m_moveShape!=nullptr)
			{
				m_shapePlacementHandler->initiateMove(m_moveShape->getPosition());
				m_moveShape->disableHighlight();
				m_shapePlacementHandler->setShapeSelection(m_selection);
				m_shapePlacementHandler->activate();
				
				m_movementCursor->disable();
				m_selectionHandler->deactivate();
				redraw();
			}
		}
		
		if (m_viewMode==VM_CREATE_VECTORS)
		{
			m_vectorClickCount++;						
		}
	}
}

// ------------------------------------------------------------
void WorkspaceView::onMouseMove(int x, int y)
{
	if (m_viewMode==VM_CREATE_VECTORS)
	{
		if (m_vectorClickCount==2)
		{
			int dx = m_mouseDownPos[0] - x;
			int dy = m_mouseDownPos[1] - y;
			
			m_vectorCursor->setAlpha(-(dx)*0.5);
			m_vectorCursor->setType(VectorCursor::CT_ALPHA);
			//m_vectorCursor->setBeta(dy*0.5);
			
			redraw();
		}
		
		if (m_vectorClickCount==1)
		{
			int dx = m_mouseDownPos[0] - x;
			int dy = m_mouseDownPos[1] - y;
			
			//m_vectorCursor->setAlpha(dx*0.5);
			m_vectorCursor->setBeta(-90.0-(dy)*0.5);
			m_vectorCursor->setType(VectorCursor::CT_BETA);
			
			redraw();
		}
	}
}

// ------------------------------------------------------------
void WorkspaceView::onMouseUp(int x, int y)
{
	if (m_viewMode==VM_NONE)
	{
		restoreViewMode();
	}
	else
	{
		if (m_viewMode==VM_CREATE_VECTORS)
		{
			std::cout << "VCC: " << m_vectorClickCount << std::endl;
			
			if (m_vectorClickCount==3)
			{
				m_vectorClickCount=0;

				// Create new vector

				double arrowSize = 0.5;

				auto vectorArrow = ExtrArrow::create();
				vectorArrow->setSize(arrowSize*1.0, arrowSize*0.4);
				vectorArrow->setRadius(arrowSize*0.12, arrowSize*0.07);
				vectorArrow->setOffset(-0.7);
				vectorArrow->setDirection(0.0, -1.0, 0.0);
				vectorArrow->setMaterial(m_vectorMaterial);

				vectorArrow->setPosition(m_vectorCursor->getPosition());
				vectorArrow->setDirection(m_vectorCursor->getDirection());

				m_vectors->addChild(vectorArrow);
				
				m_vectorCursor->disable();
				m_vectorCursor->reset();
				m_selectionHandler->activate();
				
				redraw();
			}
		}
	}
}

// ------------------------------------------------------------
void WorkspaceView::onModifierDown()
{
	if (m_shapePlacementHandler->isActive())
	{
		if (getModifierKey()==MT_CTRL)
			m_shapePlacementHandler->setLockXZ(true);
		else
			m_shapePlacementHandler->setLockXZ(false);
		redraw();
	}
}

// ------------------------------------------------------------
void WorkspaceView::onModifierUp()
{
	if (m_shapePlacementHandler->isActive())
	{
		m_shapePlacementHandler->setLockXZ(false);
		redraw();
	}
}

// ------------------------------------------------------------
bool WorkspaceView::onMoveSelection()
{
	setViewMode(VM_SELECT_SHAPES);
	if (m_viewModeChangeEvent!=nullptr)
		m_viewModeChangeEvent->onViewModeChange((TViewModeEvent)m_viewMode);
	
	return true;	
}

// ------------------------------------------------------------
void WorkspaceView::deleteNodes()
{
	m_selection->clear();
	m_nodes->deleteAll();

	if (m_selectionChangedEvent)
		m_selectionChangedEvent->onSelectionChanged();

	redraw();
}

// ------------------------------------------------------------
void WorkspaceView::selectAll()
{
	int i;
	
	for (i=0; i<m_nodes->getSize(); i++)
	{
		ShapePtr shape = m_nodes->getChild(i);
		m_selection->add(shape);
	}

	if (m_selectionChangedEvent)
		m_selectionChangedEvent->onSelectionChanged();
	
	redraw();
}

// ------------------------------------------------------------
void WorkspaceView::setViewModeChangeEvent(ViewModeChangeEvent *event)
{
	m_viewModeChangeEvent = event;
}

// ------------------------------------------------------------
void WorkspaceView::setCoordinateEvent(CoordinateEvent *event)
{
	m_coordinateEvent = event;
}

// ------------------------------------------------------------
void WorkspaceView::onMouseMove3d(double x, double y, double z)
{
	if (m_coordinateEvent!=nullptr)
		m_coordinateEvent->onCoordinate(x, y, z);
}

// ------------------------------------------------------------
void WorkspaceView::onCopyShape(double x, double y, double z, ShapePtr& shape, ShapePtr& newShape)
{
	auto node = Node::create();
	node->setSize(0.1);
	node->setPosition(x, y, z);
	node->setMaterial(m_nodeMaterial);
	newShape = node;
}

// ------------------------------------------------------------
void WorkspaceView::onFinishCopy()
{
	setViewMode(VM_SELECT_SHAPES);
	if (m_viewModeChangeEvent!=nullptr)
		m_viewModeChangeEvent->onViewModeChange((TViewModeEvent)m_viewMode);
}

// ------------------------------------------------------------
void WorkspaceView::onFinishMove()
{
	m_lines->refresh();
}

// ------------------------------------------------------------
void WorkspaceView::onCursorUpdate()
{
}

// ------------------------------------------------------------
void WorkspaceView::setSelectionChangedEvent(SelectionChangedEvent *event)
{
	m_selectionChangedEvent = event;
}

// ------------------------------------------------------------
ShapeSelection* WorkspaceView::getSelection()
{
	return m_selection;
}

// ------------------------------------------------------------
void WorkspaceView::createScenario(TScenarioType scenarioType)
{
	m_camera->setPosition(0.0, 4.0, 8.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);
	m_camera->setEyeSeparation(0.1);
	m_camera->setPerspective(45.0, 1.0, 100.0);

	m_moveShape = nullptr;
	m_oldShape = nullptr;

	m_selection->clear();
	m_lineSelection->clear();

	if (m_selectionChangedEvent)
		m_selectionChangedEvent->onSelectionChanged();

	m_lines->deleteAll();
	m_nodes->deleteAll();
	m_vectors->deleteAll();

	switch (scenarioType) {
	case ST_INITIAL:

		break;
	case ST_MOVE_SINGLE:
		createSimpleStructure(2,2,3);
		break;
	case ST_COPY_SINGLE:
		createSimpleStructure(2,2,3);
		break;
	case ST_MOVE_MULTIPLE:
		createSimpleStructure(3,3,4);
		break;
	case ST_COPY_MULTIPLE:
		createSimpleStructure(3,3,4);
		break;
	case ST_DELETE_SINGLE:
		createSimpleStructure(2,2,3);
		break;
	case ST_DELETE_MULTIPLE:
		createSimpleStructure(3,3,4);
		break;
	case ST_CREATE_LINES:
		createSimpleStructure(3,3,4);
		break;
	case ST_CREATE_LOADS:
		//createSimpleStructure(3,3,4);
		open("structure.top");
		break;
	default:
		break;

	}

	redraw();
}

// ------------------------------------------------------------
void WorkspaceView::createSimpleStructure(int rows, int cols, int stacks)
{
	int i, j, k;

	double spacing = 1.0;

	double x, z;

	x = -(double)(rows-1)*spacing/2.0;
	z = -(double)(cols-1)*spacing/2.0;

	for (i=0; i<rows; i++)
	{
		for (j=0; j<cols; j++)
		{
			for (k=0; k<stacks; k++)
			{
				auto node = Node::create();
				node->setSize(0.1);
				node->setPosition(x+(double)i*spacing, (double)k*spacing, z+(double)j*spacing);
				node->setMaterial(m_nodeMaterial);

				m_nodes->addChild(node);
			}
		}
	}
}

void WorkspaceView::save()
{
	using namespace std;

	fstream f;

	// Enumerate nodes

	int i;
	double x, y, z;

	f.open("structure.top", std::ios::out);

	f << m_nodes->getSize() << endl;

	for (i=0; i<m_nodes->getSize(); i++)
	{
		ShapePtr shape = m_nodes->getChild(i);
		shape->setTag(i);
		shape->getPosition(x, y, z);

		f << x << " " << y << " " << z << endl;
	}

	f << m_lines->getSize() << endl;

	for (i=0; i<m_lines->getSize(); i++)
	{
		Shape* shape = m_lines->getChild(i);
		SolidLinePtr line = (SolidLine*) shape;

		f << line->getNode(0)->getTag() << " " << line->getNode(1)->getTag() << endl;
	}

	f.close();
}

void WorkspaceView::open(const char *filename)
{
	using namespace std;

	fstream f;

	// Enumerate nodes

	int i;
	double x, y, z;

	int nNodes, nVectors, nodeIdx1, nodeIdx2;

	f.open(filename, ios::in);

	f >> nNodes;

	for (i=0; i<nNodes; i++)
	{
		f >> x >> y >> z;

		auto node = Node::create();
		node->setSize(0.1);
		node->setPosition(x, y, z);
		node->setMaterial(m_nodeMaterial);

		m_nodes->addChild(node);
	}

	f >> nVectors;

	for (i=0; i<nVectors; i++)
	{
		f >> nodeIdx1 >> nodeIdx2;
		
		auto line = SolidLine::create();
		line->setRadius(0.03);
		line->setMaterial(m_lineMaterial);
		
		Node* node1 = (Node*)m_nodes->getChild(nodeIdx1);
		Node* node2 = (Node*)m_nodes->getChild(nodeIdx2);

		line->setNodes(node1, node2);

		m_lines->addChild(line);
	}

	f.close();
}
