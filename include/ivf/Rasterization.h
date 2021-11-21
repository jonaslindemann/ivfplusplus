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

#include <ivfdef/SingletonDestroyer.h>

namespace ivf {

IvfStdPointer(CRasterization);

/**
 * Rasterisation singleton class
 *
 * This class controls OpenGL rasterisation options.
 *
 * The class is accesssed in the following way:
 *
 * \code
 * CIvfRasterizationPtr rasterOps = CRasterization::getInstance();
 * \endcode
 */
class IVF_API CRasterization : public CBase {
public:
	enum TCullFace {
		CF_FRONT,
		CF_BACK,
		CF_FRONT_AND_BACK
	};
	enum TTargetFace {
		TF_FRONT,
		TF_BACK,
		TF_FRONT_AND_BACK
	};
	enum TPolygonMode {
		PM_POINT,
		PM_LINE,
		PM_FILL
	};
private:
	static CRasterization* m_instance;
	static CSingletonDestroyer<CRasterization> m_destroyer;
public:
	/** Singleton access method */
	static CRasterization* getInstance();

	IvfClassInfo("CRasterization",CBase);
	IvfStdFactory(CRasterization);

	/** Sets OpenGL polygon mode see TPolygonMode */
	void setPolygonMode(TTargetFace face, TPolygonMode mode);

	/** Sets up smooth shading (default in OpenGL) */
	void defineSmoothShadeMode();

	/** Sets up wire frame rendering */
	void defineWireFrameMode();

	/** Enables culling of back faces */
	void enableCullFace();

	/** Disables culling of back faces */
	void disableCullFace();

	/** Return back face culling state */
	bool isCullFaceEnabled();

	/** Enables smoothing of polygons */
	void enablePolygonSmooth();

	/** Disables polygon smoothing */
	void disablePolygonSmooth();

	/** Enables line smoothing */
	void enableLineSmooth();

	/** Disable line smoothing */
	void disableLineSmooth();

	/** Sets line width */
	void setLineWidth(float width);

	/** Return cull face state */
	TCullFace getCullFace();

	/** Set the face to be culled, see TCullFace */
	void setCullFace(TCullFace face);
protected:
	CRasterization();
	friend class CSingletonDestroyer<CRasterization>;

};

}