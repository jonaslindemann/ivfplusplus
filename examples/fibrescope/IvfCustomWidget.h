//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
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

#ifndef _CCustomWidget_h_
#define _CCustomWidget_h_

#include <ivf/ivfconfig.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfFlyHandler.h>
#include <ivfwidget/IvfSceneHandler.h>

#include <ivffltk/IvfFltkBase.h>

#include <ivf/IvfShape.h>
#include <ivf/IvfGLPrimitive.h>
#include <ivf/IvfQuadSet.h>
#include <ivf/IvfTriSet.h>
#include <ivf/IvfWireBrick.h>
#include <ivf/IvfLineSet.h>
#include <ivf/IvfMultiply.h>

#include <ivf/IvfLighting.h>
#include <ivf/IvfFog.h>
#include <ivf/IvfPixelOps.h>
#include <ivf/IvfBlending.h>

#include "IvfFibre.h"
#include "IvfVectorCoordList.h"
#include "IvfDiscreteTime.h"
#include "IvfUserSettings.h"
#include "IvfConnectionPoint.h"
#include "IvfColorMap.h"
#include "IvfFibreComposite.h"
#include "IvfHighlightRenderer.h"

#include <FL/Fl_Slider.H>
#include <FL/Fl_Output.H>

class CCustomWidget : public CFltkBase,
	CInitEvent,
	CKeyboardEvent,
	CTimeoutEvent,
	CClearEvent {
public:
	enum TStereoMode {
		SM_NONE,
		SM_ANAGLYPH,
		SM_QUAD_BUFFER
	};
	enum TViewMode {
		VM_EXAMINER,
		VM_FLY
	};
private:
	int								m_timeStep;
	double							m_scaleFactor;
	double							m_fibreRadius;
	int								m_fibreSides;
	double							m_fogNear;
	double							m_fogFar;
	int								m_timeSteps;
	char							m_fileName[255];
	int								m_representation;
	int								m_startTime;
	int								m_endTime;
	int								m_count;
	int								m_renderingInterval;

	CCompositePtr				m_connections;
	CMaterialPtr					m_currentMaterial;
	CWireBrickPtr				m_origBox;
	CTexturePtr					m_fibreTexture;
	CLineSetPtr					m_boundingVolume;

	CCameraPtr					m_camera;
	CLightPtr					m_light;
	CScenePtr					m_scene;
	CMouseViewHandlerPtr			m_mouseViewHandler;
	CFlyHandlerPtr				m_flyHandler;
	CSceneHandlerPtr				m_sceneHandler;

	CLightingPtr					m_lighting;
	CFogPtr						m_fog;
	CPixelOpsPtr					m_pixelOps;
	CBlendingPtr					m_blending;

	CFibreCompositePtr			m_fibres;
	CMultiplyPtr					m_multiply;

	CDiscreteTimePtr				m_discreteTime;
	CUserSettingsPtr				m_userSettings;

	CHighlightRendererPtr		m_highlightRenderer;
	
	std::vector<CMaterialPtr>	m_meshMaterials;
	
	CColorMapPtr					m_colorMap;

	std::vector<double>				m_sigX;
	std::vector<double>				m_epsX;
	double							m_maxSig;
	double							m_maxEps;

	Fl_Output*						outMessage;
	Fl_Slider*						sldProgress;
	Fl_Window*						wndMain;

	TStereoMode						m_stereoMode;

	int								m_windowPos[2];
	int								m_windowSize[2];
	int								m_mainWindowPos[2];
	int								m_mainWindowSize[2];

	TViewMode						m_viewMode;

	bool							m_useDisplayLists;
	bool							m_vertexFollowPath;

	void readMesh(char* name);
	void readMeshCSV(char* name);

public:
		void setExtrusionTextureMode(int mode);
	CCustomWidget(int X, int Y, int W, int H, const char *L=0);
	
	// Implemented widget methods

	virtual void onInit(int width, int height);
	virtual void onDestroy();
	virtual void onOverlay();
	virtual void onInitContext(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual bool onTimeout();
	virtual void onClear();

	// Methods

	void nextStep();
	void open();
	void last();
	void rewind();
	void clearAll();
	void loadCamera();
	void storeCamera();
	void openCSV();
	void openNEF();
	void readMeshNEF(char * name);

	void disableFullscreen();
	void enableFullscreen();
	void setStereoMode(TStereoMode mode);

	// Get/set methods

	void setWidgets(Fl_Output* output, Fl_Slider* slider, Fl_Window* window);
	void setShowStressDiagram(bool flag);
	void setWorkspace(double size);
	void setShowUsage(bool flag);
	void setShowConnections(bool flag);
	void setReverse(bool flag);
	void setLoop(bool flag);
	void setColor(int color);

	void setScaleFactor(double factor);
	double getScaleFactor();
	
	void setTimeStep(int step);
	int getTimeStep();
	
	void setScaling(double scaling);
	double getScaling();
	
	void setFogFar(double fogFar);
	double getFogFar();
	
	void setFogNear(double fogNear);
	double getFogNear();
	
	int getSteps();

	void setTextureScale(float scale);
	void setRenderingInterval(int interval);
	void setSectionSides(int sides);
	void setRepresentation(int representation);
	void setTexture(int texture);
	
	void setEnlargeUsage(bool flag);
	bool getEnlargeUsage();
	
	void setConnectionSize(double size);
	double getConnectionSize();

	void setColorMap(int map);

	bool getVertexNormalFollowPath();
	void setVertexNormalFollowPath(bool flag);
	bool getUseDisplayLists();
	void setUseDisplayLists(bool flag);
	void setViewMode(TViewMode mode);

	void setBlendFibres(bool flag);
	bool getBlendFibres();

	void setBreakageLimit(double limit);
	double getBreakageLimit();

	void setFibreLighting(bool flag);
	bool getFibreLighting();

};

#endif
