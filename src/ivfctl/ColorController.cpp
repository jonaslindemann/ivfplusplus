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

#include <ivfctl/ColorController.h>

using namespace ivf;

CColorController::CColorController()
{
	m_redFunc = nullptr;
	m_greenFunc = nullptr;
	m_blueFunc = nullptr;
	m_alphaFunc = nullptr;
	m_time = 0.0;
	m_colorMode = CM_DIFFUSE;
}

CColorController::~CColorController()
{
}

void CColorController::setRedFunction(CFunction2d *func)
{
	m_redFunc = func;
}

void CColorController::setGreenFunction(CFunction2d *func)
{
	m_greenFunc = func;
}

void CColorController::setBlueFunction(CFunction2d *func)
{
	m_blueFunc = func;
}

void CColorController::doUpdate(double dt)
{
	float red, green, blue, alpha;

	CShape* shape = this->getShape();

	if (shape==nullptr)
		return;

	CMaterial* material = shape->getMaterial();

	if (material==nullptr)
		return;

	switch (m_colorMode) {
	case CM_DIFFUSE:
		material->getDiffuseColor(red, green, blue, alpha);
		break;
	case CM_SPECULAR:
		material->getSpecularColor(red,  green, blue, alpha);
		break;
	case CM_AMBIENT:
		material->getAmbientColor(red, green, blue, alpha);
		break;
	case CM_EMISSION:
		material->getEmissionColor(red, green, blue, alpha);
		break;
	default:
		material->getDiffuseColor(red, green, blue, alpha);
		break;
	}

	if (m_redFunc!=nullptr)
		red = (float)m_redFunc->f(m_time);

	if (m_greenFunc!=nullptr)
		green = (float)m_redFunc->f(m_time);

	if (m_blueFunc!=nullptr)
		blue = (float)m_blueFunc->f(m_time);

	if (m_alphaFunc!=nullptr)
		alpha = (float)m_alphaFunc->f(m_time);

	switch (m_colorMode) {
	case CM_DIFFUSE:
		material->setDiffuseColor(red, green, blue, alpha);
		break;
	case CM_SPECULAR:
		material->setSpecularColor(red,  green, blue, alpha);
		break;
	case CM_AMBIENT:
		material->setAmbientColor(red, green, blue, alpha);
		break;
	case CM_EMISSION:
		material->setEmissionColor(red, green, blue, alpha);
		break;
	default:
		material->setDiffuseColor(red, green, blue, alpha);
		break;
	}

	m_time += dt;
}

void CColorController::setAlphaFunction(CFunction2d *func)
{
	m_alphaFunc = func;
}

void CColorController::setColorMode(TColorMode mode)
{
	m_colorMode = mode;
}

CColorController::TColorMode CColorController::getColorMode()
{
	return m_colorMode;
}

void CColorController::doReset()
{
	m_time = 0.0;
}
