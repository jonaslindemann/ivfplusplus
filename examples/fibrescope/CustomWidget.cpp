//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2005 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include "CustomWidget.h"

#include <ivf/Scene.h>
#include <ivf/LineSet.h>
#include <ivf/Fog.h>
#include <ivf/Axis.h>
#include <ivf/Extrusion.h>

#include <ivf/GlobalState.h>
#include <ivf/Rasterization.h>

#include <ivf/Multiply.h>

#include <ivfimage/SgiImage.h>

#include <FL/fl_file_chooser.H>
#include <FL/fl_ask.H>

using namespace std;
using namespace ivf;

CCustomWidget::CCustomWidget(int X, int Y, int W, int H, const char *L)
:CFltkBase(X, Y, W, H)
{
	sldProgress = nullptr;
	outMessage = nullptr;
}

void CCustomWidget::onInit(int width, int height)
{
	// Set up widget / workspace

	// Initial state variables
	
	m_timeStep = 0;
	m_scaleFactor = 1.0;
	m_timeSteps = 0;
	m_fibreRadius = 0.1;
	m_fibreSides = 6;
	m_fogNear = 150.0;
	m_fogFar = 300.0;
	m_fibres = nullptr;
	m_connections = nullptr;
	m_boundingVolume = nullptr;
	m_representation = -1;
	m_stereoMode = SM_NONE;
	m_viewMode = VM_EXAMINER;

	// Initialize Ivf++ camera
	
	m_camera = CCamera::create();
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(0.0, 0.0, 0.0);
	m_camera->setEyeSeparation(0.01);
	m_camera->setPerspective(45.0, 1.0, 100.0);
	m_camera->setStereo(true);
	m_camera->setType(CCamera::CT_FLY);

	// Create scene
	
	m_scene = CScene::create();
	m_scene->setView(m_camera);
	m_scene->setStereoMode(CSceneBase::SM_NONE);
	m_scene->setLightMode(CSceneBase::LM_LOCAL);
	m_scene->setAnaglyphColorPair(CScene::CP_RED_CYAN);

	// Initialize mouse handler
	
	m_mouseViewHandler = new CMouseViewHandler(this, m_camera);
	m_mouseViewHandler->activate();

	// Initialize fly handler
	
	m_flyHandler = new CFlyHandler(this, m_camera);
	m_flyHandler->deactivate();

	// Initialize scene handler

	m_sceneHandler = new CSceneHandler(this, m_scene);

	// Create a light
	
	m_lighting = CLighting::getInstance();
	m_lighting->enable();
	
	m_light = m_lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
	
	// Define material palette

	auto material = CMaterial::create();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);
	
	material = CMaterial::create();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.3f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);

	material = CMaterial::create();
	material->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);
	
	material = CMaterial::create();
	material->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.0f, 0.3f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);
	
	material = CMaterial::create();
	material->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);
	
	material = CMaterial::create();
	material->setDiffuseColor(0.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.3f, 0.3f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);
	
	material = CMaterial::create();
	material->setDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	material->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);

	material = CMaterial::create();
	material->setDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	material->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_meshMaterials.push_back(material);

	material = CMaterial::create();
	material->setDiffuseColor(0.2f, 0.2f, 0.2f, 1.0f);
	material->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	material->setSpecularColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_meshMaterials.push_back(material);

	m_discreteTime = CDiscreteTime::create();

	// Create a color map object

	m_colorMap = CColorMap::create();
	m_colorMap->open("colormaps/colormap1.map");

	// Create user settings object 

	m_userSettings = CUserSettings::getInstance();
	m_userSettings->setConnectionMaterial(m_meshMaterials[4]);
	m_userSettings->setColorMap(m_colorMap);
	m_userSettings->setFibreLighting(true);

	// Highlight renderer

	m_highlightRenderer = CHighlightRenderer::create();
	
	// Load default fibre texture

	auto image = CSgiImage::create();
	image->setFileName("textures/tex01.rgb");
	image->read();
	
	m_fibreTexture = CTexture::create();
	m_fibreTexture->setImage(image);
	m_fibreTexture->setGenerateMipmaps(true);
	m_fibreTexture->setTextureMode(GL_MODULATE);
	m_fibreTexture->setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

	glLineWidth(2.0);
	
	//ivfSetGLEJoinStyle(TUBE_JN_CAP|TUBE_JN_ANGLE|TUBE_NORM_PATH_EDGE);

	gleSetJoinStyle(TUBE_JN_CAP|TUBE_NORM_EDGE|TUBE_JN_ANGLE);

	m_count = 0;
	m_renderingInterval = 1;

	auto axis = CAxis::create();
	m_scene->addChild(axis);

	m_lighting = CLighting::getInstance();
	m_fog = CFog::getInstance();
	m_pixelOps = CPixelOps::getInstance();
	m_blending = CBlending::getInstance();

	this->setUseOverlay(false);

	this->addKeyboardEvent(this);
	this->assignTimeoutEvent(0, this);
}

// ----------------------------------------------------------------------
void CCustomWidget::onDestroy()
{
}

// ----------------------------------------------------------------------
void CCustomWidget::setWorkspace(double size)
{
}

// ----------------------------------------------------------------------
void CCustomWidget::readMesh(char *name)
{
	// Loads a fibre network 

	int i,j,k;
	
	double xf, yf, zf;
	double xs, ys, zs;
	double xLock, yLock, zLock;
	double fibreRadius;
	double connectionRadius;
	double x, y, z, usage;
	double rx, ry, rz;
	double dx, dy, dz;
	double A1, Iz1, Iy1;
	double Kv1, E1, G1;
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	double maxDispl;
	double xm, ym, zm;
	double width, height, depth;
	double maxSize;
	double sigX, sigY, sigZ;
	double tauX, tauY, tauZ;
	double epsX, epsY, epsZ;
	double gammaXY, gammaXZ, gammaYZ;
	double x1, y1, z1;
	double x2, y2, z2;
	
	int nTimesteps;
	int nFibres;
	int nCoords;
	int nNodes;
	
	vector<int> nFibreCoords;
	
	CFibrePtr fibre;
	CCoordHistPtr coordHist;

	CCoordListPtr coordList;
	CCoordListPtr undeformedList;
	
	CConnectionPointPtr connectionPoint;

	fstream f;
	f.open(name, ios::in);
	
	maxX = maxY = maxZ = -1e300;
	minX = minY = minZ = 1e300;
	maxDispl = -1e300;
	
	if (m_fibres!=nullptr)
		m_scene->removeShape(m_fibres);

	if (m_boundingVolume!=nullptr)
		m_scene->removeShape(m_boundingVolume);
	
	m_fibres = CFibreComposite::create();
	m_fibres->setTexture(m_fibreTexture);
	
	if (m_connections!=nullptr)
	{
		m_scene->removeShape(m_connections);
		delete m_connections;
	}

	m_sigX.clear();
	m_epsX.clear();
	
	m_connections = CComposite::create();
	
	// Wire cube coordinates
	
	f >> xf >> yf >> zf;
	f >> xs >> ys >> zs;
	
	// Lock point
	
	f >> xLock >> yLock >> zLock;
	
	// Number of time steps
	
	f >> nTimesteps;
	
	m_timeSteps = nTimesteps;
	
	// Number of fibres
	
	f >> nFibres;
	
	// Read fibres

	sldProgress->show();
	outMessage->show();
	Fl::flush();

	outMessage->value("Reading initial fibre geometry.");
	sldProgress->maximum(nFibres);
	Fl::flush();

	m_userSettings->setCamera(m_camera);
	m_userSettings->setRepresentation(m_representation);
	m_userSettings->setExtrusionSides(m_fibreSides);
	
	for (i=0; i<nFibres; i++)
	{
		sldProgress->value(i);
		Fl::flush();

		f >> nCoords;
		f >> fibreRadius;

		cout << nCoords << endl;
		
		nFibreCoords.push_back(nCoords);
		
		// Create fibre object

		
		fibre = CFibre::create();
		fibre->setDiscreteTime(m_discreteTime);
		coordHist = fibre->getCoordHist();

		if ((m_representation==FIBRE_BAND1)||(m_representation==FIBRE_BAND2))
			coordList = (CCoordList*) new CVectorCoordList();
		else
			coordList = (CCoordList*) new CArrayCoordList();

		coordHist->addList(coordList);
		
		// Read fibre coordinates

		if (m_representation==FIBRE_EXTRUSION)
			coordList->addCoord(0.0, 0.0, 0.0);
				
		for (j=0; j<nCoords; j++)
		{
			f >> x >> y >> z;
			coordList->addCoord(x, y, z);
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
		}

		if (m_representation==FIBRE_EXTRUSION)
			coordList->addCoord(0.0, 0.0, 0.0);

		// Modify first and last point if FIBRE_EXTRUSION

		if (m_representation==FIBRE_EXTRUSION)
		{
			coordList->getCoord(1, x1, y1, z1);
			coordList->getCoord(2, x2, y2, z2);
			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;
			coordList->setCoord(0, x1 - dx, y1 - dy, z1 - dz);

			coordList->getCoord(nCoords-1, x1, y1, z1);
			coordList->getCoord(nCoords, x2, y2, z2);
			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;
			coordList->setCoord(nCoords+1, x2 + dx, y2 + dy, z2 + dz);
		}

		// Read fibre material properties;
		
		f >> A1 >> Iz1 >> Iy1;
		f >> Kv1 >> E1 >> G1;
		
		// Add Fibre
		
		m_fibres->addChild(fibre);
		
	}

	// Create a bounding volume

	m_boundingVolume = CLineSet::create();
	m_boundingVolume->addCoord(minX, minY, minZ);
	m_boundingVolume->addCoord(maxX, minY, minZ);
	m_boundingVolume->addCoord(minX, minY, maxZ);
	m_boundingVolume->addCoord(maxX, minY, maxZ);
	m_boundingVolume->addCoord(minX, maxY, minZ);
	m_boundingVolume->addCoord(maxX, maxY, minZ);
	m_boundingVolume->addCoord(minX, maxY, maxZ);
	m_boundingVolume->addCoord(maxX, maxY, maxZ);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);

	CIndex* coordIdx = new CIndex();
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

	CIndex* colorIdx = new CIndex();
	colorIdx->assignFrom(coordIdx);

	m_boundingVolume->addCoordIndex(coordIdx);
	m_boundingVolume->addColorIndex(colorIdx);

	m_scene->addChild(m_boundingVolume);
	
	// Read displacements

	sldProgress->maximum(nTimesteps);
	outMessage->value("Reading fibre displacements.");
	Fl::flush();

	for (i=0; i<nTimesteps;i++)
	{
		sldProgress->value(i);
		Fl::flush();

		for (j=0; j<nFibres; j++)
		{
			fibre = (CFibre*)m_fibres->getChild(j);
			coordHist = fibre->getCoordHist();
			undeformedList = (CCoordList*)coordHist->getList(0);

			if ((m_representation==FIBRE_BAND1)||(m_representation==FIBRE_BAND2))
				coordList = (CCoordList*) new CVectorCoordList();
			else
				coordList = (CCoordList*) new CArrayCoordList();

			if (m_representation==FIBRE_EXTRUSION)
				coordList->addCoord(0.0, 0.0, 0.0);

			for (k=0;k<nFibreCoords[j];k++)
			{
				f >> dx >> dy >> dz;
				f >> rx >> ry >> rz;
				
				if (abs(dx)>maxDispl)
					maxDispl = abs(dx);
				if (abs(dy)>maxDispl)
					maxDispl = abs(dy);
				if (abs(dz)>maxDispl)
					maxDispl = abs(dz);
				
				if (k==0)
					coordList->setCoord(0, dx, dy, dz);

				coordList->addCoord(dx, dy, dz);
			}

			if (m_representation==FIBRE_EXTRUSION)
			{
				coordList->addCoord(0.0, 0.0, 0.0);
				coordList->setCoord(coordList->getSize()-1, dx, dy, dz);
			}

			coordHist->addList(coordList);
		}
	}
	
	//
	// Read connection points
	//
	
	f >> nNodes;
	
	if (!f.eof())
	{
		
		f >> connectionRadius;

		outMessage->value("Reading initial connection point positions.");
		sldProgress->maximum(nNodes);
		Fl::flush();
		
		for (i=0; i<nNodes; i++)
		{
			sldProgress->value(i);
			Fl::flush();
			// Create bPoint object
			
			connectionPoint = CConnectionPoint::create();
			connectionPoint->setDiscreteTime(m_discreteTime);
			connectionPoint->setRadius(connectionRadius);
			coordHist = connectionPoint->getCoordHist();
			coordList = new CVectorCoordList();
			coordHist->addList(coordList);
			
			// Read initial connection point coordinates
			
			f >> x >> y >> z;
			coordList->addCoord(x, y, z, 0.0f, 0.0f, 0.0f);
			
			// Add connection point
			
			m_connections->addChild(connectionPoint);
			
		}
		
		// Read connection point displacements

		for (i=0; i<nTimesteps;i++)
		{
			for (j=0; j<nNodes; j++)
			{
				connectionPoint = (CConnectionPoint*)m_connections->getChild(j);
				coordHist = connectionPoint->getCoordHist();
				coordList = CVectorCoordList::create();
				f >> dx >> dy >> dz >> usage;
				coordList->addCoord(dx, dy, dz, usage, usage, usage);
				coordHist->addList(coordList);
			}
		}
	}

	// Read stress information

	m_maxSig = -1e300;
	m_maxEps = -1e300;

	outMessage->value("Reading stress history.");
	sldProgress->maximum(nTimesteps);
	Fl::flush();

	for (i=0; i<nTimesteps; i++)
	{
		sldProgress->value(i);
		Fl::flush();
		f >> sigX >> sigY >> sigZ;
		f >> tauX >> tauY >> tauZ;
		f >> epsX >> epsY >> epsZ;
		f >> gammaXY >> gammaXZ >> gammaYZ;
		m_sigX.push_back(sigX);
		m_epsX.push_back(epsX);
		if (sigX>m_maxSig)
			m_maxSig = sigX;
		if (i<nTimesteps-3)
			if (epsX>m_maxEps)
				m_maxEps = epsX;

	}

	// Close file
	
	f.close();

	// Remove progress window

	sldProgress->hide();
	outMessage->hide();
	
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
	
	m_userSettings->setScaleFactor(maxSize*3/maxDispl);
	m_scaleFactor = maxSize*3/maxDispl;
	
	m_fibres->setPosition(-xm, -ym, -zm);
	m_connections->setPosition(-xm, -ym, -zm);
	m_boundingVolume->setPosition(-xm, -ym, -zm);
	m_highlightRenderer->setPosition(-xm, -ym, -zm);
	m_camera->setPerspective(45.0, 5.0, width*4.0);
	m_camera->setPosition(0.0, 0.0, width*2);
	m_camera->initialize();

	//camera->setEyeSeparation(width*0.0);
	m_camera->setEyeSeparation(width*0.01);

	m_mouseViewHandler->setPanScalefactor(width/w());
	m_mouseViewHandler->setZoomScalefactor(width/w());

	this->setFogNear(50.0);
	this->setFogFar(width*3);
	this->setScaleFactor(2.0);
	
	m_discreteTime->setInterval(0,nTimesteps-1);
	
	m_fibres->setMaterial(m_meshMaterials[1]);

	// Add our fibres to the scene
	
	this->setColor(4);
	
	m_scene->addChild(m_fibres);
	m_scene->addChild(m_connections);
	m_scene->addChild(m_highlightRenderer);
	m_scene->getComposite()->refresh();
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::open()
{
	// Open a fibre network 

	char* fname = fl_file_chooser("Open file", "*.fib", "");
	
	// If we have a filename we try to open.
	
	if (fname!=nullptr)
	{
		// Ask for representation

		int rep = fl_choice("Choose representation.", "Band I", "Band II", "Extruded", nullptr);

		strcpy(m_fileName, fname);
		this->clearAll();

		switch (rep) {
		case 0:
			this->setRepresentation(FIBRE_BAND1);
			break;
		case 1:
			this->setRepresentation(FIBRE_BAND2);
			break;
		case 2:
			this->setRepresentation(FIBRE_EXTRUSION);
			break;
		default:
			this->setRepresentation(FIBRE_EXTRUSION);
		}

		this->readMesh(fname);
		this->setTimeStep(0);
	}
}

// ----------------------------------------------------------------------
void CCustomWidget::setTimeStep(int step)
{
	// Set current timestep

	m_discreteTime->setTime(step);
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setScaleFactor(double factor)
{
	// Set fibre scale factor 

	m_fibreRadius = factor;
	m_userSettings->setFibreRadius(factor);
	m_scene->getComposite()->refresh();
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::nextStep()
{
	// Go to next step

	m_discreteTime->next();
	
	this->redraw();

	m_count++;
	
	if (m_count%20==0)
	{
		/*
		m_endTime = glutGet(GLUT_ELAPSED_TIME);
		cout << 20.0*1000.0/((double)m_endTime - (double)m_startTime) << endl;
		m_startTime = glutGet(GLUT_ELAPSED_TIME);
		*/
	}
}

// ----------------------------------------------------------------------
int CCustomWidget::getTimeStep()
{
	// Return time step

	return m_discreteTime->getCurrentTime();
}

// ----------------------------------------------------------------------
void CCustomWidget::setColor(int color)
{
	// Set fibre color

	if ((color>=0)&&(color<9))
	{
		m_fibres->setMaterial(m_meshMaterials[color]);
	}
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::clearAll()
{
	// Remove fibres from scene

	if (m_fibres!=nullptr)
	{
		m_scene->removeShape(m_fibres);
		m_fibres = nullptr;
	}

	if (m_connections!=nullptr)
	{
		m_scene->getComposite()->removeShape(m_connections);
		m_connections = nullptr;
	}

	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setFogNear(double fogNear)
{
	// Set fog near limit
	m_fogNear = fogNear;

	CFog* fog = CFog::getInstance();
	fog->setStart(m_fogNear);

	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setFogFar(double fogFar)
{
	// Set far fog limit

	m_fogFar = fogFar;

	CFog* fog = CFog::getInstance();
	fog->setEnd(m_fogFar);

	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::onInitContext(int width, int height)
{
	// Initialize OpenGL

	CFltkBase::onInitContext(width, height);

	m_lighting->setTwoSide(true);
	m_lighting->enable();

	if (m_representation==FIBRE_EXTRUSION)
		CRasterization::getInstance()->enableCullFace();
	else
		CRasterization::getInstance()->disableCullFace();


	CLight* light = m_lighting->getLight(0);
	light->setType(CLight::LT_DIRECTIONAL);
	light->setDirection(0.0, 0.0, 1.0);
	light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f); 
	light->enable();

	m_fog->setType(CFog::FT_LINEAR);
	m_fog->setLimits(m_fogNear, m_fogFar);
	m_fog->setColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_fog->enable();
	
	m_pixelOps->enableDepthTest();
}

// ----------------------------------------------------------------------
void CCustomWidget::setScaling(double scaling)
{
	// Set fibre displacement scale

	m_scaleFactor = scaling;
	m_userSettings->setScaleFactor(m_scaleFactor);
	this->redraw();
}

// ----------------------------------------------------------------------
double CCustomWidget::getScaling()
{
	// Return scale factor

	return m_scaleFactor;
}

// ----------------------------------------------------------------------
int CCustomWidget::getSteps()
{
	// Return number of time steps

	return m_timeSteps;
}

// ----------------------------------------------------------------------
void CCustomWidget::rewind()
{
	// Goto first timestep

	m_discreteTime->rewind();
	this->redraw();	
}

// ----------------------------------------------------------------------
void CCustomWidget::last()
{
	// Goto last timestep 

	m_discreteTime->last();
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setLoop(bool flag)
{
	// Set loop flag

	m_discreteTime->setLoop(flag);
}

// ----------------------------------------------------------------------
void CCustomWidget::setReverse(bool flag)
{
	// Set reverse flag

	if (flag)
		m_discreteTime->setEndAction(IVF_DT_REVERSE);
	else
		m_discreteTime->setEndAction(IVF_DT_STOP);
}

// ----------------------------------------------------------------------
void CCustomWidget::setShowConnections(bool flag)
{
	// Turn on/off connections

	if ((flag)&&(m_connections!=nullptr))
		m_connections->setState(CGLBase::OS_ON);
	else
		m_connections->setState(CGLBase::OS_OFF);		
	this->redraw();
}		

// ----------------------------------------------------------------------
void CCustomWidget::setShowUsage(bool flag)
{
	// Turn on/off usage information

	m_userSettings->setShowUsage(flag);
	this->redraw();
}


// ----------------------------------------------------------------------
double CCustomWidget::getFogFar()
{
	// Return far fog limit

	return m_fogFar;
}

// ----------------------------------------------------------------------
double CCustomWidget::getFogNear()
{
	// Return fog near limit

	return m_fogNear;
}

// ----------------------------------------------------------------------
double CCustomWidget::getScaleFactor()
{
	// return fibre scale factor

	return m_fibreRadius;
}

// ----------------------------------------------------------------------
void CCustomWidget::onOverlay()
{
	// Draw stress diagram in an overlay

	int i;
	double diagramWidth = (double)h()*0.25;
	double diagramHeight = (double)h()*0.25;
	double margin = (double)h()*0.05;

	// Draw axes

	glLineWidth(2.0);

	glBegin(GL_LINE_STRIP);

	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex2d((double)w()-margin-diagramWidth, (double)h() - margin - diagramHeight);
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex2d((double)w()-margin-diagramWidth, (double)h() - margin);
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex2d((double)w()-margin, (double)h() - margin);

	glEnd();

	// Draw curve

	if (m_sigX.size()>0)
	{
		glBegin(GL_LINE_STRIP);
		for (i=0; i<(int)m_sigX.size(); i++)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2d((double)w()-margin-diagramWidth + m_epsX[i]*diagramWidth/m_maxEps,(double)h() - margin - m_sigX[i]*diagramHeight/m_maxSig);
		}
		glEnd();
		
		// Draw a point at the current timestep
		
		glPointSize(8.0);
		
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f);	
		i = getTimeStep();
		glVertex2d((double)w()-margin-diagramWidth + m_epsX[i]*diagramWidth/m_maxEps,(double)h() - margin - m_sigX[i]*diagramHeight/m_maxSig);
		glEnd();
	}

	glPointSize(1.0);
}

// ----------------------------------------------------------------------
void CCustomWidget::setShowStressDiagram(bool flag)
{
	// Turn on/off stress diagram

	this->setUseOverlay(flag);
}

// ----------------------------------------------------------------------
void CCustomWidget::setWidgets(Fl_Output *output, Fl_Slider *slider, Fl_Window* window)
{
	// Assign progress and message widgets

	sldProgress = slider;
	outMessage = output;
	wndMain = window;
}

// ----------------------------------------------------------------------
void CCustomWidget::setColorMap(int map)
{
	char filename[40];
	sprintf(filename,"colormaps/colormap%d.map",map+1);
	m_colorMap->open(filename);
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setConnectionSize(double size)
{
	m_userSettings->setConnectionSize(size);
	this->redraw();
}

// ----------------------------------------------------------------------
double CCustomWidget::getConnectionSize()
{
	return m_userSettings->getConnectionSize();
}

// ----------------------------------------------------------------------
void CCustomWidget::setEnlargeUsage(bool flag)
{
	m_userSettings->setEnlargeUsage(flag);
}

// ----------------------------------------------------------------------
bool CCustomWidget::getEnlargeUsage()
{
	return m_userSettings->getEnlargeUsage();
}

// ----------------------------------------------------------------------
void CCustomWidget::setTexture(int texture)
{
	char filename[40];
	sprintf(filename,"textures/tex%d.rgb",texture+1);

	CSgiImage* image = new CSgiImage();
	image->setFileName(filename);
	image->read();

	CTexture* newTexture = new CTexture();
	newTexture->addReference();
	//newTexture->load(filename);
	newTexture->setImage(image);
	newTexture->setGenerateMipmaps(true);
	newTexture->setTextureMode(GL_MODULATE);
	newTexture->setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	m_fibres->setTexture(newTexture);
	m_fibreTexture = newTexture;
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setRepresentation(int representation)
{
	m_representation = representation;
}

// ----------------------------------------------------------------------
void CCustomWidget::setSectionSides(int sides)
{
	m_fibreSides = sides;
	m_userSettings->setExtrusionSides(sides);
	m_scene->getComposite()->refresh();
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setRenderingInterval(int interval)
{
	m_renderingInterval = interval;
	m_fibres->setRenderInterval(m_renderingInterval);
	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::storeCamera()
{
	fstream f;
	double xp, yp, zp;
	double xt, yt, zt;

	f.open("camera.dat", ios::out);
	
	m_camera->getPosition(xp, yp, zp);
	m_camera->getTarget(xt, yt, zt);

	f << xp << " " << yp << " " << zp << endl;
	f << xt << " " << yt << " " << zt << endl;

	f.close();

	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::loadCamera()
{
	fstream f;
	double xp, yp, zp;
	double xt, yt, zt;

	f.open("camera.dat", ios::in);
	f >> xp >> yp >> zp;
	f >> xt >> yt >> zt;
	m_camera->setPosition(xp, yp, zp);
	m_camera->setTarget(xt, yt, zt);

	f.close();

	this->redraw();
}

// ----------------------------------------------------------------------
void CCustomWidget::setTextureScale(float scale)
{
	m_userSettings->setTextureScale(scale);
	this->redraw();	
}

// ----------------------------------------------------------------------
void CCustomWidget::openCSV()
{
	// Open a fibre network 

	char* fname = fl_file_chooser("Open file", "*.csv", "");
	
	// If we have a filename we try to open.
	
	if (fname!=nullptr)
	{
		// Ask for representation

		int rep = fl_choice("Choose representation.", "Band I", "Band II", "Extruded", nullptr);

		strcpy(m_fileName, fname);
		this->clearAll();

		switch (rep) {
		case 0:
			this->setRepresentation(FIBRE_BAND1);
			break;
		case 1:
			this->setRepresentation(FIBRE_BAND2);
			break;
		case 2:
			this->setRepresentation(FIBRE_EXTRUSION);
			break;
		default:
			this->setRepresentation(FIBRE_EXTRUSION);
		}

		this->readMeshCSV(fname);
		this->setTimeStep(0);
	}
}

// ----------------------------------------------------------------------
void CCustomWidget::readMeshCSV(char *name)
{
	// Loads a fibre network 

	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	double xm, ym, zm;
	double width, height, depth;
	double maxSize;
	double x1, y1, z1;
	double x2, y2, z2;
	double len;
	double x, y, z, dx, dy, dz;

	char buff[255];
	
	int nCoords;
	
	vector<int> nFibreCoords;
	
	CFibre* fibre;
	CCoordHist* coordHist;

	CCoordList* coordList;
	
	fstream f;
	f.open(name, ios::in);
	
	m_userSettings->setCamera(m_camera);
	m_userSettings->setRepresentation(m_representation);
	m_userSettings->setFibreRadius(m_fibreRadius);
	
	maxX = maxY = maxZ = -1e300;
	minX = minY = minZ = 1e300;
	
	if (m_fibres!=nullptr)
	{
		m_scene->removeShape(m_fibres);
		m_fibres = nullptr;
	}

	if (m_boundingVolume!=nullptr)
	{
		m_scene->removeShape(m_boundingVolume);
		m_boundingVolume = nullptr;
	}
	
	m_fibres = new CFibreComposite();
	m_fibres->setTexture(m_fibreTexture);
	
	if (m_connections!=nullptr)
	{
		m_scene->removeShape(m_connections);
		m_connections = nullptr;
	}

	m_sigX.clear();
	m_epsX.clear();
	
	//m_connections = new CComposite();

	m_timeSteps = 0;
	
	// Read fibres

	sldProgress->show();
	outMessage->show();
	Fl::flush();

	outMessage->value("Reading initial fibre geometry.");
	sldProgress->maximum(0);
	Fl::flush();

	// Skip first row

	f.getline(buff, 255);

	double scl = 100.0;
	x = 0.0;
	y = 0.0;
	z = 0.0;
	len = 0.0;

	while (!f.eof())
	{
		x = x/scl;
		y = y/scl;
		z = z/scl;
		len = len/scl;

		f >> x;
		f.read(buff,1);
		f >> y;
		f.read(buff,1);
		f >> z;
		f.read(buff,1);
		f >> dx;
		f.read(buff,1);
		f >> dy;
		f.read(buff,1);
		f >> dz;
		f.read(buff,1);
		f >> len;
		f.read(buff,1);

		if (f.eof())
			break;

		x = x*scl;
		y = y*scl;
		z = z*scl;
		len = len*scl;

		//cout << x << " " << y << " " << z << " " << len << endl;

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

		// Create fibre object
		
		fibre = new CFibre();
		fibre->setDiscreteTime(m_discreteTime);
		coordHist = fibre->getCoordHist();

		if ((m_representation==FIBRE_BAND1)||(m_representation==FIBRE_BAND2))
			coordList = (CCoordList*) new CVectorCoordList();
		else
			coordList = (CCoordList*) new CArrayCoordList();

		coordHist->addList(coordList);
		
		// Read fibre coordinates

		nCoords = 2;

		if (m_representation==FIBRE_EXTRUSION)
			coordList->addCoord(0.0, 0.0, 0.0);

		coordList->addCoord(x - 0.5*dx*len, z - 0.5*dz*len, y - 0.5*dy*len);
		coordList->addCoord(x + 0.5*dx*len, z + 0.5*dz*len, y + 0.5*dy*len);
				
		if (m_representation==FIBRE_EXTRUSION)
			coordList->addCoord(0.0, 0.0, 0.0);

		// Modify first and last point if FIBRE_EXTRUSION

		if (m_representation==FIBRE_EXTRUSION)
		{
			coordList->getCoord(1, x1, y1, z1);
			coordList->getCoord(2, x2, y2, z2);
			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;
			coordList->setCoord(0, x1 - dx, y1 - dy, z1 - dz);

			coordList->getCoord(nCoords-1, x1, y1, z1);
			coordList->getCoord(nCoords, x2, y2, z2);
			dx = x2 - x1;
			dy = y2 - y1;
			dz = z2 - z1;
			coordList->setCoord(nCoords+1, x2 + dx, y2 + dy, z2 + dz);
		}

		// Add Fibre
		
		m_fibres->addChild(fibre);
	}
	

	// Create a bounding volume

	m_boundingVolume = new CLineSet();
	m_boundingVolume->addCoord(minX, minZ, minY);
	m_boundingVolume->addCoord(maxX, minZ, minY);
	m_boundingVolume->addCoord(minX, maxZ, minY);
	m_boundingVolume->addCoord(maxX, maxZ, minY);
	m_boundingVolume->addCoord(minX, minZ, maxY);
	m_boundingVolume->addCoord(maxX, minZ, maxY);
	m_boundingVolume->addCoord(minX, maxZ, maxY);
	m_boundingVolume->addCoord(maxX, maxZ, maxY);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);
	m_boundingVolume->addColor(1.0f, 1.0f, 1.0f);

	CIndex* coordIdx = new CIndex();
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

	CIndex* colorIdx = new CIndex();
	colorIdx->assignFrom(coordIdx);

	m_boundingVolume->addCoordIndex(coordIdx);
	m_boundingVolume->addColorIndex(colorIdx);

	m_scene->addChild(m_boundingVolume);

	// Close file
	
	f.close();

	// Remove progress window

	sldProgress->hide();
	outMessage->hide();
	
	// Move and scale workspace
	
	width = maxX-minX;
	height = maxY-minY;
	depth = maxZ-minZ;

	cout << width << " " << height << " " << depth << endl;
	
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
	m_scaleFactor = 1.0;
	
	this->setWorkspace(width);
	m_fibres->setPosition(-(minX+xm), -(minZ+zm), -(minY+ym));
	m_boundingVolume->setPosition(-(minX+xm), -(minZ+zm), -(minY+ym));

	m_camera->setPerspective(45.0, 5.0, width*10.0);
	m_camera->setPosition(0.0, 0.0, width*2);
	//m_camera->setPosition(0.0, 0.0, 0.0);
	CVec3d forward;
	forward.setComponents(0.0, 0.0, -1.0);
	m_camera->setForwardVector(forward);


	m_camera->initialize();

	m_camera->setEyeSeparation(width*0.002);

	m_mouseViewHandler->setPanScalefactor(width/w());
	m_mouseViewHandler->setZoomScalefactor(width/w());

	m_flyHandler->setMaxSpeed(200.0);

	this->setFogNear(50.0);
	this->setFogFar(width*3);
	this->setScaleFactor(2.0);
	
	m_discreteTime->setInterval(0,0);
	
	m_fibres->setMaterial(m_meshMaterials[1]);

	// Add our fibres to the scene
	
	this->setColor(4);
	
	m_scene->addChild(m_fibres);
	m_scene->getComposite()->refresh();
	this->redraw();
}

void CCustomWidget::setStereoMode(TStereoMode mode)
{
	m_stereoMode = mode;
	switch (mode) {
	case SM_NONE:
		m_scene->setStereoMode(CSceneBase::SM_NONE);
		break;
	case SM_ANAGLYPH:
		m_scene->setStereoMode(CSceneBase::SM_ANAGLYPH);
		break;
	case SM_QUAD_BUFFER:
		m_scene->setStereoMode(CSceneBase::SM_QUAD_BUFFER);
		break;
	default:
		m_scene->setStereoMode(CSceneBase::SM_NONE);
		break;
	}
	redraw();
}

void CCustomWidget::enableFullscreen()
{
	if (wndMain!=nullptr)
	{
		m_mainWindowPos[0] = wndMain->x();
		m_mainWindowPos[1] = wndMain->y();
		m_mainWindowSize[0] = wndMain->w();
		m_mainWindowSize[1] = wndMain->h();

		m_windowPos[0] = x();
		m_windowPos[1] = y();
		m_windowSize[0] = w();
		m_windowSize[1] = h();
		
		wndMain->fullscreen();
		resize(wndMain->x(), wndMain->y(), wndMain->w(), wndMain->h());
	}	
}

void CCustomWidget::disableFullscreen()
{
	if (wndMain!=nullptr)
	{
		resize(m_windowPos[0], m_windowPos[1], m_windowSize[0], m_windowSize[1]);
		wndMain->fullscreen_off(m_mainWindowPos[0], m_mainWindowPos[1], m_mainWindowSize[0], m_mainWindowSize[1]);
	}
}

void CCustomWidget::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'q':
		disableFullscreen();
		break;
	default:

		break;
	}
}

bool CCustomWidget::onTimeout()
{
	m_flyHandler->update();
	return true;
}

void CCustomWidget::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CCustomWidget::readMeshNEF(char *name)
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
	
	CFibre* fibre;
	CCoordHist* coordHist;

	CCoordList* coordList;
	
	fstream f;
	f.open(name, ios::in);
	
	m_userSettings->setCamera(m_camera);
	m_userSettings->setRepresentation(m_representation);
	//m_userSettings->setFibreRadius(m_fibreRadius);
	
	maxX = maxY = maxZ = -1e300;
	minX = minY = minZ = 1e300;
	
	if (m_multiply!=nullptr)
	{
		m_scene->removeShape(m_multiply);
		m_multiply = nullptr;
	}

	if (m_boundingVolume!=nullptr)
	{
		m_scene->removeShape(m_boundingVolume);
		m_boundingVolume = nullptr;
	}
	
	m_fibres = new CFibreComposite();

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

		fibre = new CFibre();
		fibre->setDiscreteTime(m_discreteTime);

		coordHist = fibre->getCoordHist();

		if ((m_representation==FIBRE_BAND1)||(m_representation==FIBRE_BAND2))
			coordList = (CCoordList*) new CVectorCoordList();
		else
			coordList = (CCoordList*) new CArrayCoordList();

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

	m_boundingVolume = new CLineSet();
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

	CIndexPtr coordIdx = new CIndex();
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

	CIndexPtr colorIdx = new CIndex();
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

	CVec3d forward;
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

	m_multiply = new CMultiply();
	m_multiply->setShape(m_fibres);
	m_multiply->setRepeat(3, 1, 3);
	m_multiply->setOffsets(width, height, depth);
	m_multiply->setStartIndices(-1, 0, -1);

	m_scene->addChild(m_multiply);
	m_scene->getComposite()->refresh();

	this->redraw();
}

void CCustomWidget::openNEF()
{
	// Open a fibre network 

	char* fname = fl_file_chooser("Open file", "*.nef", "");
	
	// If we have a filename we try to open.
	
	if (fname!=nullptr)
	{
		// Ask for representation

		int rep = fl_choice("Choose representation.", "Band I", "Band II", "Extruded", nullptr);

		strcpy(m_fileName, fname);
		this->clearAll();

		switch (rep) {
		case 0:
			this->setRepresentation(FIBRE_BAND1);
			break;
		case 1:
			this->setRepresentation(FIBRE_BAND2);
			break;
		case 2:
			this->setRepresentation(FIBRE_EXTRUSION);
			break;
		default:
			this->setRepresentation(FIBRE_EXTRUSION);
		}

		this->readMeshNEF(fname);
		this->setTimeStep(0);
	}
}

void CCustomWidget::setViewMode(TViewMode mode)
{
	m_viewMode = mode;

	switch (m_viewMode) {
	case VM_EXAMINER:
		disableTimeout(0);
		m_mouseViewHandler->activate();
		m_flyHandler->deactivate();
		break;
	case VM_FLY:
		enableTimeout(0.01, 0);
		m_mouseViewHandler->deactivate();
		m_flyHandler->activate();
		break;
	default:
		m_mouseViewHandler->activate();
		m_flyHandler->deactivate();
		break;
	}

	redraw();
}

void CCustomWidget::setUseDisplayLists(bool flag)
{
	m_useDisplayLists = flag;
	//m_scene->getComposite()->setUselist(flag);
	if (m_fibres!=nullptr)
		m_fibres->setUselist(flag);
	redraw();
}

bool CCustomWidget::getUseDisplayLists()
{
	return m_useDisplayLists;
}

void CCustomWidget::setVertexNormalFollowPath(bool flag)
{
	m_vertexFollowPath = flag;
	if (m_vertexFollowPath)
		ivfSetGLEJoinStyle(TUBE_JN_CAP|TUBE_JN_ANGLE|TUBE_NORM_PATH_EDGE);
	else
		ivfSetGLEJoinStyle(TUBE_JN_CAP|TUBE_JN_ANGLE|TUBE_NORM_EDGE);

	redraw();
}

bool CCustomWidget::getVertexNormalFollowPath()
{
	return m_vertexFollowPath;
}

void CCustomWidget::setExtrusionTextureMode(int mode)
{
	switch (mode) {
	case 0:
		gleTextureMode(0);
		break;
	case 1:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_VERTEX_FLAT);
		break;
	case 3:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_NORMAL_FLAT);
		break;
	case 4:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_VERTEX_CYL);
		break;
	case 5:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_NORMAL_CYL);
		break;
	case 6:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_VERTEX_SPH);
		break;
	case 7:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_NORMAL_SPH);
		break;
	case 8:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_VERTEX_MODEL_FLAT);
		break;
	case 9:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_NORMAL_MODEL_FLAT);
		break;
	case 10:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_VERTEX_MODEL_CYL);
		break;
	case 11:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_NORMAL_MODEL_CYL);
		break;
	case 12:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_VERTEX_MODEL_SPH);
		break;
	case 13:
		gleTextureMode(GLE_TEXTURE_ENABLE|GLE_TEXTURE_NORMAL_MODEL_SPH); 
		break;
	default:
		gleTextureMode(0);
		break;
	}
}

void CCustomWidget::setBlendFibres(bool flag)
{
	m_userSettings->setBlendFibres(flag);	
	redraw();
}

bool CCustomWidget::getBlendFibres()
{
	return m_userSettings->getBlendFibres();
}

void CCustomWidget::setBreakageLimit(double limit)
{
	m_userSettings->setBreakageLimit(limit);
	redraw();
}

double CCustomWidget::getBreakageLimit()
{
	return m_userSettings->getBreakageLimit();
}

void CCustomWidget::setFibreLighting(bool flag)
{
	m_userSettings->setFibreLighting(flag);
	redraw();
}

bool CCustomWidget::getFibreLighting()
{
	return m_userSettings->getFibreLighting();
}

