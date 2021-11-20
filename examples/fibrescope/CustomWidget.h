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

#include <ivfwidget/MouseViewHandler.h>
#include <ivfwidget/FlyHandler.h>
#include <ivfwidget/SceneHandler.h>

#include <ivffltk/FltkBase.h>

#include <ivf/Shape.h>
#include <ivf/GLPrimitive.h>
#include <ivf/QuadSet.h>
#include <ivf/TriSet.h>
#include <ivf/WireBrick.h>
#include <ivf/LineSet.h>
#include <ivf/Multiply.h>

#include <ivf/Lighting.h>
#include <ivf/Fog.h>
#include <ivf/PixelOps.h>
#include <ivf/Blending.h>

#include "Fibre.h"
#include "VectorCoordList.h"
#include "DiscreteTime.h"
#include "UserSettings.h"
#include "ConnectionPoint.h"
#include "ColorMap.h"
#include "FibreComposite.h"
#include "HighlightRenderer.h"

#include <FL/Fl_Slider.H>
#include <FL/Fl_Output.H>

class CCustomWidget : public ivf::CFltkBase,
	ivf::CInitEvent,
	ivf::CKeyboardEvent,
	ivf::CTimeoutEvent,
	ivf::CClearEvent {
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

	ivf::CCompositePtr				m_connections;
	ivf::CMaterialPtr				m_currentMaterial;
	ivf::CWireBrickPtr				m_origBox;
	ivf::CTexturePtr				m_fibreTexture;
	ivf::CLineSetPtr				m_boundingVolume;

	ivf::CCameraPtr					m_camera;
	ivf::CLightPtr					m_light;
	ivf::CScenePtr					m_scene;
	ivf::CMouseViewHandlerPtr		m_mouseViewHandler;
	ivf::CFlyHandlerPtr				m_flyHandler;
	ivf::CSceneHandlerPtr			m_sceneHandler;

	ivf::CLightingPtr				m_lighting;
	ivf::CFogPtr					m_fog;
	ivf::CPixelOpsPtr				m_pixelOps;
	ivf::CBlendingPtr				m_blending;

	CFibreCompositePtr				m_fibres;

	ivf::CMultiplyPtr				m_multiply;

	CDiscreteTimePtr				m_discreteTime;
	CUserSettingsPtr				m_userSettings;

	CHighlightRendererPtr			m_highlightRenderer;
	
	std::vector<ivf::CMaterialPtr>	m_meshMaterials;
	
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
