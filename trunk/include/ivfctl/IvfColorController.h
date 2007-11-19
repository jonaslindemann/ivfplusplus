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

#ifndef _CIvfColorController_h_
#define _CIvfColorController_h_

#include <ivfctl/IvfController.h>
#include <ivfmath/IvfFunctions.h>

IvfSmartPointer(CIvfColorController);

/**
 * Color controller class
 *
 * This controller controls the color component of the
 * controlled object
 */
class IVFCTL_API CIvfColorController : public CIvfController {
public:
	enum TColorMode {
		CM_DIFFUSE,
		CM_SPECULAR,
		CM_AMBIENT,
		CM_EMISSION
	};
private:
	CIvfFunction2dPtr m_redFunc;
	CIvfFunction2dPtr m_greenFunc;
	CIvfFunction2dPtr m_blueFunc;
	CIvfFunction2dPtr m_alphaFunc;
	double m_time;
	TColorMode m_colorMode;
public:
	/** CIvfColorController constructor */
	CIvfColorController();

	/** CIvfColorController */
	virtual ~CIvfColorController();

	IvfClassInfo("CIvfColorController",CIvfController);

	/** 
	 * Set color mode
	 *
	 * \param mode defines which color component should be 
	 * affected.
	 */
	void setColorMode(TColorMode mode);

	/** Returns color mode */
	TColorMode getColorMode();

	/** 
	 * Set the function controlling the red color
	 *
	 * \param func CIvfFunction2d derived class controlling the 
	 * color value over time. Max value of the function should be
	 * 1.0.
	 */
	void setRedFunction(CIvfFunction2d* func);
	void setGreenFunction(CIvfFunction2d* func);
	void setBlueFunction(CIvfFunction2d* func);
	void setAlphaFunction(CIvfFunction2d* func);

	virtual void doUpdate(double dt);
	virtual void doReset();
};

#endif 
