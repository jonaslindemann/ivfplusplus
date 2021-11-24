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

#pragma once

#include <ivfctl/Controller.h>
#include <ivfmath/Functions.h>

namespace ivf {

IvfSmartPointer(ColorController);

/**
 * Color controller class
 *
 * This controller controls the color component of the
 * controlled object
 */
class IVFCTL_API ColorController : public Controller {
public:
	enum TColorMode {
		CM_DIFFUSE,
		CM_SPECULAR,
		CM_AMBIENT,
		CM_EMISSION
	};
private:
	Function2dPtr m_redFunc;
	Function2dPtr m_greenFunc;
	Function2dPtr m_blueFunc;
	Function2dPtr m_alphaFunc;
	double m_time;
	TColorMode m_colorMode;
public:
	/** ColorController constructor */
	ColorController();

	/** ColorController */
	virtual ~ColorController();

	IvfClassInfo("ColorController",Controller);
	IvfStdFactory(ColorController);

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
	 * \param func Function2d derived class controlling the 
	 * color value over time. Max value of the function should be
	 * 1.0.
	 */
	void setRedFunction(Function2d* func);
	void setGreenFunction(Function2d* func);
	void setBlueFunction(Function2d* func);
	void setAlphaFunction(Function2d* func);

	virtual void doUpdate(double dt);
	virtual void doReset();
};

}