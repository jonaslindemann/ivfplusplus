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

#include <ivfwidget/HandlerBase.h>
#include <ivfwidget/WidgetBase.h>

#include <ivf/Scene.h>

namespace ivf {

IvfSmartPointer(SceneHandler);

/**
 * Scene handler
 *
 * The scene handler is used enable quick creation of
 * application by automatically handling rendering
 * and resizing for a scene based Ivf++ application.
 */
class IVFWIDGET_API SceneHandler : public HandlerBase,
	ResizeEvent,
	RenderEvent
{
private:
	WidgetBase* m_widget;
	ScenePtr m_scene;
	bool m_multiPass;
	bool m_nPasses;
public:
	/**
	 * Class constructor
	 *
	 * @param widget Reference to a WidgetBase derived class.
	 * @param scene Reference to a Scene derived scene containing
	 * the scene graph setup.
	 */
	SceneHandler(WidgetBase* widget, Scene* scene);

	/** Class destructor */
	virtual ~SceneHandler();

	IvfClassInfo("SceneHandler",HandlerBase);

	static SceneHandlerPtr create(WidgetBase* widget, Scene* scene)
	{
		return SceneHandlerPtr(new SceneHandler(widget, scene));
	}

	void onResize(int width, int height);
	void onRender();
};

}