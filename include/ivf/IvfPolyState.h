//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

#ifndef _CIvfPolyState_h_
#define _CIvfPolyState_h_

#include <ivf/IvfRenderState.h>

IvfSmartPointer(CIvfPolyState);

/**
 * Poly state class
 *
 * This class encapsulates the OpenGL glPolygonMode() 
 * state function.
 */
class IVF_API CIvfPolyState : public CIvfRenderState {
public:
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
	TTargetFace m_face;
	TPolygonMode m_mode;
	int m_oldFace;
	int m_oldMode;
public:
	/** CIvfPolyState constructor */
	CIvfPolyState();

	/** CIvfPolyState destructor */
	virtual ~CIvfPolyState();

	IvfClassInfo("CIvfPolyState",CIvfRenderState);

	/** 
	 * Set target face 
	 * 
	 * @param face can be TF_FRONT, TF_BACK and 
	 * TF_FRONT_AND_BACK.
	 */
	void setFace(TTargetFace face);

	/** Return target face */
	TTargetFace getFace();

	/** 
	 * Set face rendering mode 
	 *
	 * @param mode can be PM_POINT, PM_LINE, and PM_FILL.
	 */
	void setMode(TPolygonMode mode);

	/** Return face rendering mode */
	TPolygonMode getMode();

	virtual void doRestoreState();
	virtual void doSaveState();
	virtual void doState();
};

#endif 
