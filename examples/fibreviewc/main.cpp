#include <ivf/ivfconfig.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfCylinder.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfGrid.h>
#include <ivf/IvfGlobalState.h>
#include <ivf/IvfScene.h>

#include <ivf/IvfLineSet.h>
#include <ivf/IvfFog.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfExtrusion.h>

#include <ivf/IvfGlobalState.h>
#include <ivf/IvfRasterization.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>
#include <ivfwidget/IvfFlyHandler.h>

#include <ivfwin32/IvfWin32Application.h>
#include <ivfwin32/IvfWin32Window.h>

#include <ivf/IvfLighting.h>
#include <ivf/IvfFog.h>
#include <ivf/IvfPixelOps.h>
#include <ivf/IvfBlending.h>

IvfSmartPointer(CViewWindow);

#include "IvfFibre.h"
#include "IvfVectorCoordList.h"
#include "IvfDiscreteTime.h"
#include "IvfUserSettings.h"
#include "IvfConnectionPoint.h"
#include "IvfColorMap.h"
#include "IvfFibreComposite.h"

#include "glfont.h"

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CViewWindow);

class CViewWindow: public CIvfWin32Window,
	CIvfInitEvent,
	CIvfKeyboardEvent,
	CIvfOverlayEvent,
	CIvfInitContextEvent,
	CIvfResizeEvent,
	CIvfAppLoopEvent
{
private:
	CIvfCameraPtr			m_camera;
	CIvfLightPtr			m_light;
	CIvfScenePtr			m_scene;

	CIvfMouseViewHandlerPtr m_mouseViewHandler;
	CIvfSceneHandlerPtr		m_sceneHandler;
	CIvfFlyHandlerPtr		m_flyHandler;

	CIvfLightingPtr					m_lighting;
	CIvfFogPtr						m_fog;
	CIvfPixelOpsPtr					m_pixelOps;
	CIvfBlendingPtr					m_blending;

	// Fibre related

	int								m_representation;
	CIvfFibreCompositePtr			m_fibres;
	CIvfDiscreteTimePtr				m_discreteTime;
	CIvfUserSettingsPtr				m_userSettings;
	CIvfLineSetPtr					m_boundingVolume;
	CIvfColorMapPtr					m_colorMap;
	std::vector<CIvfMaterialPtr>	m_meshMaterials;

	std::string						m_filename;

	bool m_rotating;

	bool m_stereo;
	bool m_redGreenStereo;
	bool m_redCyanStereo;

	bool m_useDisplayLists;
	bool m_useFog;
	bool m_useBlending;
	bool m_useHelp;
	bool m_useBoundingBox;
	bool m_useSmoothing;
	bool m_useFlyHandler;

	int m_width;
	int m_height;

	int m_currentColor;
	int m_fibreSides;
	
	double m_lastTime;

	void readMeshNEF(const char *name);
public:
	void enableFlyHandler();
	CViewWindow(int X, int Y, int W, int H);

	void enableRedCyanStereo();
	void enableRedGreenStereo();
	void enableStereo();
	void setFilename(const char* filename);
	
	virtual void onInit(int width, int height);
	virtual void onInitContext();
	virtual void onKeyboard(int key, int x, int y);
	virtual void onOverlay();
	virtual void onResize(int width, int height);
	virtual bool onAppLoop();
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CViewWindow::CViewWindow(int X, int Y, int W, int H)
:CIvfWin32Window(X, Y, W, H) 
{
	m_stereo = false;
	m_redGreenStereo = false;
	m_redCyanStereo = false;
	m_useFlyHandler = false;

	addInitEvent(this);
	addKeyboardEvent(this);
	addOverlayEvent(this);
	addInitContextEvent(this);
	addResizeEvent(this);
	assignAppLoopEvent(this);
}

void CViewWindow::onInit(int width, int height)
{
	cout << "Initializing..." << endl;
	
	// Initialize variables
	
	m_rotating = false;
	m_useDisplayLists = false;
	m_useFog = false;
	m_useBlending = false;
	m_useHelp = false;
	m_useBoundingBox = true;
	m_useSmoothing = false;
	m_currentColor = 4;
	m_fibreSides = 6;

	m_width = width;
	m_height = height;

	// Initialize Ivf++ camera
	
	m_camera = new CIvfCamera();
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);
	m_camera->setEyeSeparation(0.1);
	m_camera->setPerspective(45.0, 1.0, 100.0);
	m_camera->setViewPort(width, height);
	
	if (m_stereo||m_redGreenStereo||m_redCyanStereo)
		m_camera->setStereo(true);
	else
		m_camera->setStereo(false);
	
	// Create scene
	
	m_scene = new CIvfScene();
	m_scene->setCamera(m_camera);
	if (m_stereo)
		m_scene->setStereoMode(CIvfSceneBase::SM_QUAD_BUFFER);

	if (m_redGreenStereo)
	{
		m_scene->setStereoMode(CIvfSceneBase::SM_ANAGLYPH);
		m_scene->setAnaglyphColorPair(CIvfSceneBase::CP_RED_GREEN);
	}

	if (m_redCyanStereo)
	{
		m_scene->setStereoMode(CIvfSceneBase::SM_ANAGLYPH);
		m_scene->setAnaglyphColorPair(CIvfSceneBase::CP_RED_CYAN);
	}
			
	m_scene->setLightMode(CIvfSceneBase::LM_LOCAL);

	// Create a light
	
	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();
	
	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	//////////////////////////////////////////////////////////////
	// Setup fibre visualisation

	m_representation = FIBRE_EXTRUSION;

	// Create a materials
	
	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));
	
	material = new CIvfMaterial();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.3f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));
	
	material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));
	
	material = new CIvfMaterial();
	material->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.0f, 0.3f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));
	
	material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));
	
	material = new CIvfMaterial();
	material->setDiffuseColor(0.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.3f, 0.3f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));

	material = new CIvfMaterial();
	material->setDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	material->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));

	material = new CIvfMaterial();
	material->setDiffuseColor(0.5f, 0.1f, 0.0f, 1.0f);
	material->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	material->setSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));

	material = new CIvfMaterial();
	material->setDiffuseColor(0.1f, 0.0f, 0.5f, 1.0f);
	material->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	material->setSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_meshMaterials.push_back(CIvfMaterialPtr(material));

	m_discreteTime = new CIvfDiscreteTime();

	// Create user settings object 

	m_userSettings = new CIvfUserSettings();
	m_userSettings->setConnectionMaterial(m_meshMaterials[4]);
	m_userSettings->setColorMap(m_colorMap);

	m_lastTime = elapsedTime();
	
	//enableTimeout(0.01, 0);
	//enableIdleProcessing();

	gleSetJoinStyle(TUBE_JN_CAP|TUBE_NORM_EDGE|TUBE_JN_ANGLE);

	m_lighting = CIvfLighting::getInstance();
	m_fog = CIvfFog::getInstance();
	m_pixelOps = CIvfPixelOps::getInstance();
	m_blending = CIvfBlending::getInstance();

	// Initialize font 

	glBuildFont(this->getDC());

	// Initialize handlers

	m_sceneHandler = new CIvfSceneHandler(this, m_scene);
	m_mouseViewHandler = new CIvfMouseViewHandler(this, m_camera);
	
	m_flyHandler = new CIvfFlyHandler(this, m_camera);
	m_flyHandler->setRedraw(false);
	m_flyHandler->setMouseSensitivity(0.2);
	m_flyHandler->setMaxSpeed(200.0);
	m_flyHandler->setSpeedStep(4.0);

	if (m_useFlyHandler)
	{
		m_flyHandler->activate();
		m_mouseViewHandler->deactivate();
	}
	else
	{
		m_flyHandler->deactivate();
		m_mouseViewHandler->activate();
	}

	if (m_filename!="")
		readMeshNEF(m_filename.c_str());

	setUseOverlay(true);

	redraw();
}

// ------------------------------------------------------------
bool CViewWindow::onAppLoop()
{
	m_flyHandler->update();
	redraw();
	return true;
}

// ------------------------------------------------------------
void CViewWindow::onOverlay()
{
	glColor3f(1.0f, 1.0f, 0.5f);
	glRasterPos2i(m_width-172, m_height-8);
 	glPrint("FibreVIEW Version 1.0");	

	if (m_useHelp)
	{
		glRasterPos2i(5, 15);
 		glPrint("FibreVIEW keys");	

		int rowHeight = 15;

		glRasterPos2i(5, 25+1*rowHeight);
 		glPrint("[l] - Toggle display lists on/off");	
		glRasterPos2i(5, 25+2*rowHeight);
 		glPrint("[f] - Toggle fog on/off");	
		glRasterPos2i(5, 25+3*rowHeight);
 		glPrint("[a] - Increase eye separation");	
		glRasterPos2i(5, 25+4*rowHeight);
 		glPrint("[z] - Decrease eye separation");	
		glRasterPos2i(5, 25+5*rowHeight);
 		glPrint("[+] - Next material");	
		glRasterPos2i(5, 25+6*rowHeight);
 		glPrint("[-] - Previous material");	
		glRasterPos2i(5, 25+7*rowHeight);
 		glPrint("[b] - Toggle blending on/off");	
		glRasterPos2i(5, 25+8*rowHeight);
 		glPrint("[e] - Toggle bounding box on/off");	
		glRasterPos2i(5, 25+9*rowHeight);
 		glPrint("[h] - This help screen");	
		glRasterPos2i(5, 25+10*rowHeight);
 		glPrint("[t/g] - Fog near limit");	
		glRasterPos2i(5, 25+11*rowHeight);
 		glPrint("[u/j] - Fog far limit");	
		glRasterPos2i(5, 25+12*rowHeight);
 		glPrint("[i/k] - Add/Decrease extrusion sides");	
		glRasterPos2i(5, 25+13*rowHeight);
 		glPrint("[q] - Toggle extrusion smoothing on/off");	
	}
}

// ------------------------------------------------------------
void CViewWindow::onResize(int width, int height)
{
	m_width = width;
	m_height = height;
}

// ------------------------------------------------------------
void CViewWindow::onInitContext()
{

}

// ------------------------------------------------------------
void CViewWindow::onKeyboard(int key, int x, int y)
{
	double eyeSep;
	double fogNear, fogFar;
	
	switch (key) {
	case 'l':
		m_useDisplayLists=!m_useDisplayLists;
		m_scene->getComposite()->setUselist(m_useDisplayLists);
		redraw();
		break;
	case 'f':
		m_useFog=!m_useFog;

		if (m_useFog)
		{
			m_fog->setType(CIvfFog::FT_LINEAR);
			m_fog->setColor(0.0f, 0.0f, 0.0f, 1.0f);
			m_fog->enable();
		}
		else
			m_fog->disable();

		redraw();
		break;
	case 'a':
		eyeSep = m_camera->getEyeSeparation();
		eyeSep += 0.1;
		m_camera->setEyeSeparation(eyeSep);
		MessageBeep(-1);
		redraw();
		break;
	case 'z':
		eyeSep = m_camera->getEyeSeparation();
		eyeSep -= 0.1;
		m_camera->setEyeSeparation(eyeSep);
		MessageBeep(-1);
		redraw();
		break;
	case '+':
		m_currentColor++;

		if (m_currentColor==9)
			m_currentColor = 0;

		m_fibres->setMaterial(m_meshMaterials[m_currentColor]);

		redraw();
		break;
	case '-':
		m_currentColor--;

		if (m_currentColor<0)
			m_currentColor = 8;

		m_fibres->setMaterial(m_meshMaterials[m_currentColor]);

		redraw();
		break;
	case 'b':
		m_useBlending=!m_useBlending;

		if (m_useBlending)
		{
			glBlendFunc(GL_ONE, GL_ONE);
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			glBlendFunc(GL_ONE, GL_ZERO);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
		redraw();
		break;
	case 'h':
		m_useHelp=!m_useHelp;
		redraw();
		break;
	case 'e':
		m_useBoundingBox=!m_useBoundingBox;

		if (m_useBoundingBox)
			m_boundingVolume->enable();
		else
			m_boundingVolume->disable();

		redraw();
		break;
	case 't':
		m_fog->getLimits(fogNear, fogFar);
		fogNear+=20.0;
		m_fog->setLimits(fogNear, fogFar);
		redraw();
		break;
	case 'g':
		m_fog->getLimits(fogNear, fogFar);
		fogNear-=20.0;
		m_fog->setLimits(fogNear, fogFar);
		redraw();
		break;
	case 'u':
		m_fog->getLimits(fogNear, fogFar);
		fogFar+=20.0;
		m_fog->setLimits(fogNear, fogFar);
		redraw();
		break;
	case 'j':
		m_fog->getLimits(fogNear, fogFar);
		fogFar-=20.0;
		m_fog->setLimits(fogNear, fogFar);
		redraw();
		break;
	case 'i':
		m_fibreSides+=1;
		m_userSettings->setExtrusionSides(m_fibreSides);
		m_scene->getComposite()->refresh();
		redraw();
		break;
	case 'k':
		m_fibreSides-=1;
		m_userSettings->setExtrusionSides(m_fibreSides);
		m_scene->getComposite()->refresh();
		redraw();
		break;
	case 'q':
		m_useSmoothing=!m_useSmoothing;
		if (m_useSmoothing)
			ivfSetGLEJoinStyle(TUBE_JN_CAP|TUBE_JN_ANGLE|TUBE_NORM_PATH_EDGE);
		else
			ivfSetGLEJoinStyle(TUBE_JN_CAP|TUBE_JN_ANGLE|TUBE_NORM_EDGE);
		redraw();
		break;
	default:
		break;
	}
}

void CViewWindow::readMeshNEF(const char *name)
{
	// Loads a fibre network Niklas Edlind Format (NEF) 

	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	double xm, ym, zm;
	double width, height, depth;
	double maxSize;
	double x1, y1, z1;
	double x2, y2, z2;
	double x, y, z, dx, dy, dz;

	cout << "Reading NEF file: " << name << endl;

	vector<int> nFibreCoords;
	
	CIvfFibre* fibre;
	CIvfCoordHist* coordHist;

	CIvfCoordList* coordList;
	
	fstream f;
	f.open(name, ios::in);
	
	m_userSettings->setCamera(m_camera);
	m_userSettings->setRepresentation(m_representation);
	//m_userSettings->setFibreRadius(m_fibreRadius);
	
	maxX = maxY = maxZ = -1e300;
	minX = minY = minZ = 1e300;
	
	if (m_fibres!=NULL)
	{
		m_scene->removeShape(m_fibres);
		m_fibres = NULL;
	}

	if (m_boundingVolume!=NULL)
	{
		m_scene->removeShape(m_boundingVolume);
		m_boundingVolume = NULL;
	}
	
	m_fibres = new CIvfFibreComposite();

	if ((m_representation==FIBRE_BAND1)||(m_representation==FIBRE_BAND2))
		m_fibres->setUseLighting(false);
	else
		m_fibres->setUseLighting(true);


	// Read fibres

	cout << "Reading fibre geometry.";

	double fibreRadius;

	f >> fibreRadius;

	m_userSettings->setFibreRadius(fibreRadius);

	int j;

	while (!f.eof())
	{
		double value;
		int nPointsPerSegment;

		f >> value;
		
		nPointsPerSegment = (int)value;

		fibre = new CIvfFibre();
		fibre->setDiscreteTime(m_discreteTime);
		fibre->setUserSettings(m_userSettings);

		coordHist = fibre->getCoordHist();

		if ((m_representation==FIBRE_BAND1)||(m_representation==FIBRE_BAND2))
			coordList = (CIvfCoordList*) new CIvfVectorCoordList();
		else
			coordList = (CIvfCoordList*) new CIvfArrayCoordList();

		coordHist->addList(coordList);

		if (m_representation==FIBRE_EXTRUSION)
			coordList->addCoord(0.0, 0.0, 0.0);

		for (j=0; j<nPointsPerSegment; j++)
		{
			f >> x >> y >> z;

			if (x>maxX)
				maxX = x;
			if (y>maxY)
				maxY = y;
			if (z>maxZ)
				maxZ = z;
			if (x<minX)
				minX = x;
			if (y<minY)
				minY = y;
			if (z<minZ)
				minZ = z;

			coordList->addCoord(x, y, z);
		}
		
		// Modify first and last point if FIBRE_EXTRUSION

		if (m_representation==FIBRE_EXTRUSION)
		{
			coordList->addCoord(0.0, 0.0, 0.0);

			coordList->getCoord(1, x1, y1, z1);
			coordList->getCoord(2, x2, y2, z2);
			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;
			coordList->setCoord(0, x1 - dx, y1 - dy, z1 - dz);

			coordList->getCoord(nPointsPerSegment-1, x1, y1, z1);
			coordList->getCoord(nPointsPerSegment, x2, y2, z2);
			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;
			coordList->setCoord(nPointsPerSegment+1, x2 + dx, y2 + dy, z2 + dz);
		}

		// Add Fibre

		m_fibres->addChild(fibre);

		cout << ".";
	}

	cout << endl;

	// Create a bounding volume

	cout << "Min x = " << minX << " Max x = " << maxX << endl;
	cout << "Min y = " << minY << " Max y = " << maxY << endl;
	cout << "Min z = " << minZ << " Max z = " << maxZ << endl;

	m_boundingVolume = new CIvfLineSet();
	m_boundingVolume->addCoord(minX, minY, minZ);
	m_boundingVolume->addCoord(maxX, minY, minZ);
	m_boundingVolume->addCoord(minX, maxY, minZ);
	m_boundingVolume->addCoord(maxX, maxY, minZ);
	m_boundingVolume->addCoord(minX, minY, maxZ);
	m_boundingVolume->addCoord(maxX, minY, maxZ);
	m_boundingVolume->addCoord(minX, maxY, maxZ);
	m_boundingVolume->addCoord(maxX, maxY, maxZ);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 0.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 0.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 0.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 0.0f);

	CIvfIndexPtr coordIdx = new CIvfIndex();
	int zero = 0;
	coordIdx->add(zero,1);
	coordIdx->add(1,3);
	coordIdx->add(3,2);
	coordIdx->add(2,zero);
	coordIdx->add(zero,4);
	coordIdx->add(1,5);
	coordIdx->add(3,7);
	coordIdx->add(2,6);
	coordIdx->add(4,5);
	coordIdx->add(5,7);
	coordIdx->add(7,6);
	coordIdx->add(6,4);

	CIvfIndexPtr colorIdx = new CIvfIndex();
	colorIdx->assignFrom(coordIdx);

	m_boundingVolume->addCoordIndex(coordIdx);
	m_boundingVolume->addColorIndex(colorIdx);

	m_scene->addChild(m_boundingVolume);

	// Close file
	
	f.close();

	// Move and scale workspace
	
	width = maxX-minX;
	height = maxY-minY;
	depth = maxZ-minZ;

	maxSize = 0.0;
	if (width>maxSize)
		maxSize = width;
	if (height>maxSize)
		maxSize = height;
	if (depth>maxSize)
		maxSize = depth;
	
	xm = (width)/2.0;
	ym = (height)/2.0;
	zm = (depth)/2.0;
	
	m_userSettings->setScaleFactor(1.0);
	
	m_fibres->setPosition(-(minX+xm), -(minY+ym), -(minZ+zm));
	m_boundingVolume->setPosition(-(minX+xm), -(minY+ym), -(minZ+zm));

	m_camera->setPerspective(45.0, 5.0, width*10.0);
	m_camera->setPosition(0.0, 0.0, width*2);

	CIvfVec3d forward;
	forward.setComponents(0.0, 0.0, -1.0);
	m_camera->setForwardVector(forward);
	m_camera->initialize();
	m_camera->setEyeSeparation(width*0.02);

	//m_mouseViewHandler->setPanScalefactor(width/w());
	//m_mouseViewHandler->setZoomScalefactor(width/w());

	//m_flyHandler->setMaxSpeed(200.0);

	m_fog->setStart(50.0);
	m_fog->setEnd(width*3);

	//this->setFogNear(50.0);
	//this->setFogFar(width*3);
	//this->setScaleFactor(2.0);
	
	m_discreteTime->setInterval(0,0);
	
	m_fibres->setMaterial(m_meshMaterials[4]);

	// Add our fibres to the scene
	
	//this->setColor(4);
	
	m_scene->addChild(m_fibres);
	m_scene->getComposite()->refresh();
}

void CViewWindow::enableStereo()
{
	m_stereo = true;
}

void CViewWindow::enableRedGreenStereo()
{
	m_redGreenStereo = true;
}

void CViewWindow::enableRedCyanStereo()
{
	m_redCyanStereo = true;
}

void CViewWindow::setFilename(const char *filename)
{
	m_filename = filename;
}


void CViewWindow::enableFlyHandler()
{
	m_useFlyHandler = true;
}

// ------------------------------------------------------------
int main(int argc, char* argv[])
{
	cout << "FibreScope Viewer Application Version 1.0" << endl;
	cout << "  Copyright (C) 2003 Division of Structural Mechanics" << endl;
	cout << "  Lund University" << endl << endl;

	// Process command line

	bool fullscreen = false;
	bool stereo = false;
	bool rg_stereo = false;
	bool rc_stereo = false;
	bool fly = false;

	if (argc==1)
	{
		cout << "Usage: fibreviewc filename [options]" << endl;
		return 0;
	}

	std::string filename = argv[1];
	std::string option;
	
	int i = 1;

	while (i<argc)
	{
		option = argv[i];

		if (option=="--fullscreen")
			fullscreen = true;

		if (option=="--stereo")
			stereo = true;

		if (option=="--rg_stereo")
			rg_stereo = true;

		if (option=="--rc_stereo")
			rc_stereo = true;

		if (option=="--fly")
			fly = true;
		i++;
	}

	if (stereo)
	{
		rg_stereo = false;
		rc_stereo = false;
	}
	else
	{
		if (rg_stereo)
			rc_stereo = false;
	}

	CIvfWin32ApplicationPtr application = CIvfWin32Application::getInstance();
	CViewWindowPtr window = new CViewWindow(0,0,1024,768);

	if (stereo)
		window->enableStereo();

	if (rg_stereo)
		window->enableRedGreenStereo();

	if (rc_stereo)
		window->enableRedCyanStereo();

	if (fly)
		window->enableFlyHandler();

	window->setFilename(filename.c_str());
	window->setFullscreen(fullscreen);
	window->setCaption("FibreVIEW 1.0");
	window->show();

	if (fly)
		application->runAppLoop(window);
	else
		application->run();
	
	return 0;
}
