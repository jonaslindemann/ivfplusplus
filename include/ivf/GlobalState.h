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

#include <ivf/Base.h>

#include <ivf/Base.h>
#include <ivf/Light.h>

#include <ivfdef/SingletonDestroyer.h>

namespace ivf {

IvfStdPointer(GlobalState);

/**
 * Ivf state singleton class
 *
 * An instance of the CIvfGlobalState class is retrieved using the
 * getInstance() method. See the following code:
 *
 * @author Jonas Lindemann
 */
class IVF_API GlobalState : public Base {
private:
	static GlobalState* m_instance;
	static SingletonDestroyer<GlobalState> m_destroyer;
	bool m_renderMaterial;
	bool m_renderTexture;
	bool m_renderGreyscale;
    bool m_colorOutput;
public:
	/** Return instance of CIvfGlobalState */
	static GlobalState* getInstance();

	IvfClassInfo("CIvfGlobalState",Base);

	virtual ~GlobalState();

	void enableMaterialRendering();
	void disableMaterialRendering();
	bool isMaterialRenderingEnabled();

	void enableTextureRendering();
	void disableTextureRendering();
	bool isTextureRenderingEnabled();

	void enableGreyscaleRendering();
	void disableGreyscaleRendering();
	bool isGreyscaleRenderingEnabled();
    
    void enableColorOutput();
    void disableColorOutput();
    bool isColorOutputEnabled();

protected:
	GlobalState();
	friend class SingletonDestroyer<GlobalState>;
};

}
