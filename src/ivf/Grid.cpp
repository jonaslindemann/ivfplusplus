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

#include <ivf/Grid.h>

using namespace ivf;

CGrid::CGrid()
{
	m_p1.setComponents(-1.0,  0.0, -1.0);
	m_p2.setComponents( 1.0,  0.0,  1.0);

	m_corners = new CLineSet();
	m_outline = new CLineSet();
	m_gridLines = new CLineSet();
	m_surface = new CQuadSet();
	m_axis = new CAxis();
    m_axis->setState(CShape::OS_OFF);
    m_corners->setState(CShape::OS_OFF);
    m_outline->setState(CShape::OS_OFF);

	m_gridSpacing = 0.1;

	m_useGrid = true;
	m_useAxis = true;
	m_useOutline = true;
	m_useCorners = true;
	m_useSurface = false;

	m_gridInterval = 1;

	m_surfaceMaterial = new CMaterial();
	m_surfaceMaterial->setDiffuseColor(0.3f, 0.3f, 0.3f, 0.2f);

	m_cornerColor[0] = 0.8f; m_cornerColor[1] = 0.8f; m_cornerColor[2] = 0.8f; m_cornerColor[3] = 1.0f;
	m_outlineColor[0] = 0.5f; m_outlineColor[1] = 0.5f; m_outlineColor[2] = 0.5f; m_outlineColor[3] = 1.0f;
	m_majorColor[0] = 0.5f; m_majorColor[1] = 0.5f; m_majorColor[2] = 0.5f; m_majorColor[3] = 1.0f; 
	m_minorColor[0] = 0.3f; m_minorColor[1] = 0.3f; m_minorColor[2] = 0.3f; m_minorColor[3] = 1.0f; 

	initGrid();
}

CGrid::~CGrid()
{
	delete m_corners;
	delete m_outline;
	delete m_gridLines;
	delete m_surface;
	delete m_axis;
}

void CGrid::initGrid()
{
	double x1, y1, z1;
	double x2, y2, z2;

	// Define corners

	m_corners->clear();

	m_p1.getComponents(x1, y1, z1);
	m_p2.getComponents(x2, y2, z2);

	double cs = ((x2-x1)+(z2-z1))*0.5*0.1;

	m_corners->addCoord(x1, y1, z1);        // 0
	m_corners->addCoord(x1+cs, y1, z1);		// 1
	m_corners->addCoord(x1, y1, z1+cs);		// 2

	m_corners->addCoord(x2, y1, z1);		// 3
	m_corners->addCoord(x2-cs, y1, z1);		// 4
	m_corners->addCoord(x2, y1, z1+cs);		// 5

	m_corners->addCoord(x2, y1, z2);		// 6
	m_corners->addCoord(x2-cs, y1, z2);		// 7
	m_corners->addCoord(x2, y1, z2-cs);		// 8

	m_corners->addCoord(x1, y1, z2);		// 9
	m_corners->addCoord(x1+cs, y1, z2);		// 10
	m_corners->addCoord(x1, y1, z2-cs);		// 11

	m_corners->setLineWidth(2);

	CIndex* idx = new CIndex();
	idx->add(0,1);
	idx->add(0,2);
	idx->add(3,4);
	idx->add(3,5);
	idx->add(6,7);
	idx->add(6,8);
	idx->add(9,10);
	idx->add(9,11);

	m_corners->addCoordIndex(idx);

	m_corners->addColor(m_cornerColor[0], m_cornerColor[1], m_cornerColor[2], m_cornerColor[3]);

	CIndex* colorIdx = new CIndex();
	colorIdx->createConstant(0, idx->getSize());

	m_corners->addColorIndex(colorIdx);
	m_corners->setUseColor(true);

	// Define outline

	m_outline->clear();
	m_outline->setLineWidth(1);

	m_outline->addCoord(x1+cs, y1, z1);
	m_outline->addCoord(x2-cs, y1, z1);

	m_outline->addCoord(x2, y1, z1+cs);
	m_outline->addCoord(x2, y1, z2-cs);

	m_outline->addCoord(x1+cs, y1, z2);
	m_outline->addCoord(x2-cs, y1, z2);

	m_outline->addCoord(x1, y1, z1+cs);
	m_outline->addCoord(x1, y1, z2-cs);

	idx = new CIndex();

	idx->add(0,1);
	idx->add(2,3);
	idx->add(4,5);
	idx->add(6,7);

	m_outline->addCoordIndex(idx);

	m_outline->addColor(m_outlineColor[0], m_outlineColor[1], m_outlineColor[2], m_outlineColor[3]);

	colorIdx = new CIndex();
	colorIdx->createConstant(0, idx->getSize());

	m_outline->addColorIndex(colorIdx);
	m_outline->setUseColor(true);

	// Create surface

	m_surface->clear();

	m_surface->addCoord(x1, y1, z2);
	m_surface->addCoord(x2, y1, z2);
	m_surface->addCoord(x2, y1, z1);
	m_surface->addCoord(x1, y1, z1);

	idx = new CIndex();
	idx->createLinear(0,4);

	m_surface->addCoordIndex(idx);

	m_surface->setMaterial(m_surfaceMaterial);

	// Create axis

	m_axis->setSize(cs);

	// Create grid lines

	m_gridLines->clear();
	m_gridLines->setLineWidth(1);

	m_gridLines->addCoord(0, y1, z1);
	m_gridLines->addCoord(0, y1, z2);

	m_gridLines->addCoord(x1, y1, 0);
	m_gridLines->addCoord(x2, y1, 0);

	idx = new CIndex();
	idx->add(0,1);
	idx->add(2,3);

	m_gridLines->addColor(m_majorColor[0], m_majorColor[1], m_majorColor[2], m_majorColor[3]);
	m_gridLines->addColor(m_minorColor[0], m_minorColor[1], m_minorColor[2], m_minorColor[3]);

	colorIdx = new CIndex();
	colorIdx->add(0,0);
	colorIdx->add(0,0);

	double x, z;
	int i = 4 ;
	int j = 0;

	for (x=x1; x<x2; x+=m_gridSpacing)
	{
		if (!isRoughly(x,0.0))
		{
			if (j%m_gridInterval==0)
			{
				//xx = (double)((long)((x + (m_gridSpacing/2)) / m_gridSpacing) * m_gridSpacing);
				m_gridLines->addCoord(x, y1, z1);
				m_gridLines->addCoord(x, y1, z2);
				idx->add(i,i+1);
				colorIdx->add(1,1);
				i+=2;
			}
		}
		j++;
	}

	j = 0;

	for (z=z1; z<z2; z+=m_gridSpacing)
	{
		if (!isRoughly(z,0.0))
		{
			if (j%m_gridInterval==0)
			{
				//zz = (double)((long)((z + (m_gridSpacing/2)) / m_gridSpacing) * m_gridSpacing);
				m_gridLines->addCoord(x1, y1, z);
				m_gridLines->addCoord(x2, y1, z);
				idx->add(i,i+1);
				colorIdx->add(1,1);
				i+=2;
			}
		}
		j++;
	}

	m_gridLines->addCoordIndex(idx);
	m_gridLines->addColorIndex(colorIdx);
	m_gridLines->setUseColor(true);
}

void CGrid::refresh()
{
	initGrid();
}

void CGrid::doCreateGeometry()
{
	if (m_useAxis) m_axis->render();
	if (m_useSurface) m_surface->render();
	if (m_useCorners) m_corners->render();
	if (m_useOutline) m_outline->render();
	if (m_useGrid) m_gridLines->render();
}

bool CGrid::isRoughly(double x, double value)
{
	return (x>(value-0.00001))&&(x<(value+0.00001));
}

void CGrid::setSize(double width, double height)
{
	m_p1.setComponents(-width/2.0, 0.0, -height/2.0);
	m_p2.setComponents( width/2.0, 0.0,  height/2.0);
}

void CGrid::setGridSpacing(double spacing)
{
	m_gridSpacing = spacing;
}

void CGrid::setUseCorners(bool flag)
{
	m_useCorners = flag;
}

void CGrid::setUseOutline(bool flag)
{
	m_useOutline = flag;
}

void CGrid::setUseGrid(bool flag)
{
	m_useGrid = flag;
}

void CGrid::setUseAxis(bool flag)
{
	m_useAxis = flag;
}

void CGrid::setUseSurface(bool flag)
{
	m_useSurface = flag;
}

void CGrid::doUpdateBoundingSphere()
{
	// This is very simple

	if (getBoundingSphere()!=NULL)
	{
		if (m_p1.length()>m_p2.length())
			getBoundingSphere()->setRadius(m_p1.length());
		else
			getBoundingSphere()->setRadius(m_p2.length());
	}
}

void CGrid::setAxisSize(double size)
{
	m_axis->setSize(size);
}

CAxis* CGrid::getAxisShape()
{
	return m_axis;
}


void CGrid::setGridInterval(int interval)
{
	m_gridInterval = interval;
	initGrid();
}

void CGrid::setCornerColor(float red, float green, float blue, float alpha)
{
	m_cornerColor[0] = red; m_cornerColor[1] = green; m_cornerColor[2] = blue; m_cornerColor[3] = alpha;
}

void CGrid::setOutlineColor(float red, float green, float blue, float alpha)
{
	m_outlineColor[0] = red; m_outlineColor[1] = green; m_outlineColor[2] = blue; m_outlineColor[3] = alpha;
}

void CGrid::setSurfaceMaterial(CMaterial* material)
{
	m_surfaceMaterial = material;
}

void CGrid::setMajorColor(float red, float green, float blue, float alpha)
{
	m_majorColor[0] = red; m_majorColor[1] = green; m_majorColor[2] = blue; m_majorColor[3] = alpha; 

}

void CGrid::setMinorColor(float red, float green, float blue, float alpha)
{
	m_minorColor[0] = red; m_minorColor[1] = green; m_minorColor[2] = blue; m_minorColor[3] = alpha; 
}


