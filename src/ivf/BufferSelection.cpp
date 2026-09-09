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

#include <ivf/BufferSelection.h>
#include <ivf/LegacyGL.h>
#include <ivf/rc.h>

#include <algorithm>
#include <vector>

using namespace ivf;

BufferSelection::BufferSelection()
{
	m_composite = nullptr;
	m_selectedShape = nullptr;
	m_camera = nullptr;
	m_pickFbo = 0;
	m_pickColorRb = 0;
	m_pickDepthRb = 0;
	m_pickWidth = 0;
	m_pickHeight = 0;
}

BufferSelection::~BufferSelection()
{
	releasePickBuffer();
}

int BufferSelection::pick(int x, int y)
{
	m_selectedShapes.clear();
	m_selectedShape = nullptr;

	// GL_SELECT does not exist in a core profile, and mixing it with a bound
	// program is meaningless anyway. Whenever the shader path is live, pick by
	// drawing ids instead.

	if (rcIsShaderActive())
		return pickColorId(x, y);

	if ((m_composite!=nullptr)&&(m_camera!=nullptr))
	{
		
		GLint hits;
		
		glSelectBuffer(512, m_selectBuf);
		
		glRenderMode(GL_SELECT);
		
		lgInitNames();
		lgPushName(IVF_NONAME);
		
		lgMatrixMode(GL_PROJECTION);
		lgPushMatrix();

		lgLoadIdentity();
		
		m_camera->initializeSelect(x, y, 4, 4);
		
		lgMatrixMode(GL_MODELVIEW);
		lgPushMatrix();
		m_camera->render();
		m_composite->render();
		lgPopMatrix();
		lgMatrixMode(GL_PROJECTION);
		lgPopMatrix();
		lgMatrixMode(GL_MODELVIEW);
		
		hits = glRenderMode(GL_RENDER);
		
		if (hits!=0)
			processHits(hits, m_selectBuf);
		
		return hits;
	}
	return -1;
}

void BufferSelection::setComposite(Composite *composite)
{
	m_composite = composite;
	update();
}

Composite* BufferSelection::getComposite()
{
	return m_composite;
}

void BufferSelection::update()
{
	m_allObjects.clear();
	nameChildren(m_composite);
}

void BufferSelection::nameChildren(Shape *shape)
{
	// Recursively name all children

	int i;

	if (shape->isClass("Composite"))
	{
		// Name all children of composite

		Composite* composite = (Composite*) shape;
		for (i = 0; i<composite->getSize(); i++)
		{
			Shape* child = composite->getChild(i);
			nameChildren(child);
		}

		// If the useName flag is set name the composite

		if (composite->getUseName()==TRUE)
		{
			m_allObjects.push_back(composite);
			composite->setObjectName(static_cast<int>(m_allObjects.size())-1);
		}
	}
	else
	{
		// Name shape

		m_allObjects.push_back(shape);
		shape->setObjectName(static_cast<int>(m_allObjects.size())-1);
	}
}

Shape* BufferSelection::getSelectedShape()
{
	/*
	if (m_selectedShapes.size()>0)
		return m_selectedShapes[m_selectedShapes.size()-1];
	else
		return nullptr;
	*/
	return m_selectedShape;
}

void BufferSelection::setView(View *view)
{
	m_camera = view;
}

View* BufferSelection::getView()
{
	return m_camera;
}

void BufferSelection::processHits(GLint hits, GLuint buffer[])
{
	GLuint depth = ~0;
	unsigned int i, getThisName, j;
	GLuint nNames, *ptr;
	GLuint objectName;
	GLuint nearZ, farZ;

	double minZ = 1e300;
	
	ptr = (GLuint *) buffer;

	m_selectedShape = nullptr;

	for (i = 0; i < (unsigned int)hits; i++) 
	{  
		getThisName = 0;
		nNames = *ptr;
		ptr++;

		for (j=0; j< nNames; j++)
		{			
			nearZ = *ptr;
			ptr++;
			farZ = *ptr;
			ptr++;
			objectName = *ptr;
			ptr++;

			if (objectName!=IVF_NONAME)
			{
				m_selectedShapes.push_back(m_allObjects[objectName]);

				if (nearZ<minZ)
				{
					minZ = nearZ;
					m_selectedShape = m_allObjects[objectName];
				}
			}
		}
	}
}

int BufferSelection::getSize()
{
	return static_cast<int>(m_selectedShapes.size());
}

Shape* BufferSelection::getSelectedShape(int idx)
{
	if ((idx>=0)&&(idx<(int)m_selectedShapes.size()))
		return m_selectedShapes[idx];
	else
		return 0;
}

SelectedShapesVector& BufferSelection::getSelectedShapes()
{
	return m_selectedShapes;
}

// ------------------------------------------------------------
bool BufferSelection::ensurePickBuffer(int width, int height)
{
	if (width <= 0 || height <= 0)
		return false;

	if ((m_pickFbo != 0) && (width == m_pickWidth) && (height == m_pickHeight))
		return true;

	releasePickBuffer();

	glGenFramebuffers(1, &m_pickFbo);
	glGenRenderbuffers(1, &m_pickColorRb);
	glGenRenderbuffers(1, &m_pickDepthRb);

	// Single sampled and plain RGBA8 on purpose. A multisampled target would
	// average neighbouring ids into values that decode to some unrelated shape,
	// and an sRGB one would gamma-convert them. This is data, not a picture.

	glBindRenderbuffer(GL_RENDERBUFFER, m_pickColorRb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, m_pickDepthRb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_pickFbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_pickColorRb);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_pickDepthRb);

	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		releasePickBuffer();
		return false;
	}

	m_pickWidth = width;
	m_pickHeight = height;
	return true;
}

// ------------------------------------------------------------
void BufferSelection::releasePickBuffer()
{
	if (m_pickFbo != 0)
		glDeleteFramebuffers(1, &m_pickFbo);
	if (m_pickColorRb != 0)
		glDeleteRenderbuffers(1, &m_pickColorRb);
	if (m_pickDepthRb != 0)
		glDeleteRenderbuffers(1, &m_pickDepthRb);

	m_pickFbo = 0;
	m_pickColorRb = 0;
	m_pickDepthRb = 0;
	m_pickWidth = 0;
	m_pickHeight = 0;
}

// ------------------------------------------------------------
int BufferSelection::pickColorId(int x, int y)
{
	if ((m_composite == nullptr) || (m_camera == nullptr))
		return -1;

	int viewX, viewY, viewWidth, viewHeight;
	m_camera->getViewPort(viewX, viewY, viewWidth, viewHeight);

	if (!ensurePickBuffer(viewX + viewWidth, viewY + viewHeight))
		return -1;

	ShaderProgram* previousShader = rcUsePickShader();

	if (rcShader() == nullptr)
		return -1;

	// Save enough state to put the caller's frame back the way it was. A pick
	// runs in the middle of an application's own rendering, not instead of it.

	GLint previousFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFbo);

	const GLboolean hadBlend = glIsEnabled(GL_BLEND);
	const GLboolean hadDither = glIsEnabled(GL_DITHER);
	const GLboolean hadDepth = glIsEnabled(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, m_pickFbo);

	// Anything that can change a written value would corrupt the id.

	glDisable(GL_BLEND);
	glDisable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // id 0 is "nothing here"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rcSetPickMode(true);
	rcBeginFrame();

	m_camera->initialize();
	m_camera->render();
	m_composite->render();

	rcSetPickMode(false);

	// Read a small region rather than a single pixel, matching the 4x4 box the
	// GL_SELECT path used: clicking exactly on a thin line is otherwise a matter
	// of luck.

	const int regionSize = 4;
	const int half = regionSize / 2;

	int readX = x - half;
	int readY = (viewY + viewHeight) - y - half;

	readX = std::max(0, std::min(readX, m_pickWidth - regionSize));
	readY = std::max(0, std::min(readY, m_pickHeight - regionSize));

	std::vector<unsigned char> pixels((size_t)regionSize * regionSize * 4, 0);
	std::vector<float> depths((size_t)regionSize * regionSize, 1.0f);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(readX, readY, regionSize, regionSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glReadPixels(readX, readY, regionSize, regionSize, GL_DEPTH_COMPONENT, GL_FLOAT, depths.data());

	// Restore

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

	if (hadBlend)
		glEnable(GL_BLEND);
	if (hadDither)
		glEnable(GL_DITHER);
	if (!hadDepth)
		glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)previousFbo);

	rcSetShader(previousShader);
	rcUseShader();

	// Decode. Each distinct id in the region is a hit, and the nearest of them
	// is the selected shape -- the same two answers processHits() produced from
	// the select buffer's near-z values.

	float minDepth = 2.0f;

	for (int i = 0; i < regionSize * regionSize; i++)
	{
		bool valid = false;
		const unsigned int name = RenderContext::decodePickName(
			pixels[i * 4 + 0], pixels[i * 4 + 1], pixels[i * 4 + 2], valid);

		if (!valid || (name >= m_allObjects.size()))
			continue;

		Shape* shape = m_allObjects[name];

		if (std::find(m_selectedShapes.begin(), m_selectedShapes.end(), shape) == m_selectedShapes.end())
			m_selectedShapes.push_back(shape);

		if (depths[i] < minDepth)
		{
			minDepth = depths[i];
			m_selectedShape = shape;
		}
	}

	return (int)m_selectedShapes.size();
}
