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


// Implementation of: public class Texture

#include <ivf/config.h>
#include <ivf/Texture.h>

using namespace ivf;

// ------------------------------------------------------------
Texture::Texture ()
:GLBase()
{
	m_bound = false;
	m_generateMipmaps = false;
	m_imageMap = nullptr;
	m_ivfImage = nullptr;
	m_wrapT = GL_REPEAT;
	m_wrapS = GL_REPEAT;
	m_textureMode = GL_DECAL;
	m_magFilter = GL_LINEAR;
	m_minFilter = GL_LINEAR;
	m_textureEnvColor[0] = 1.0;
	m_textureEnvColor[1] = 1.0;
	m_textureEnvColor[2] = 1.0;
	m_textureEnvColor[3] = 1.0;
	m_width = 0;
	m_height = 0;
	m_texScaleX = 1.0;
	m_texScaleY = 1.0;
	m_texRotate = 0.0;
	m_texTransX = 0.0;
	m_texTransY = 0.0;
	m_generateName = true;
	m_loadImages = true;
	m_active = true;
}

// ------------------------------------------------------------
Texture::~Texture ()
{
	if (m_generateName)
		if (isBound() == TRUE)
			glDeleteTextures(1, &m_textureName);
}

// ------------------------------------------------------------
bool Texture::isBound()
{
	return m_bound;
}

// ------------------------------------------------------------
GLuint Texture::getName()
{
	// return texture name
	
	return m_textureName;
}

// ------------------------------------------------------------
void Texture::bind()
{
#ifndef _NO_TEXTURE_NAMES
	if (isBound())
		glDeleteTextures(1, &m_textureName);
	
	if (m_generateName)
		glGenTextures(1, &m_textureName);

	glBindTexture(GL_TEXTURE_2D, m_textureName);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_textureMode);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, m_textureEnvColor);

	if (!m_loadImages)
	{
		m_bound = true;
		return;
	}

	if (m_ivfImage!=nullptr)
	{
		if (!m_generateMipmaps)
			glTexImage2D(GL_TEXTURE_2D, 0, m_ivfImage->getInternalFormat(), m_ivfImage->getWidth(), m_ivfImage->getHeight(),
				0, m_ivfImage->getFormat(), GL_UNSIGNED_BYTE, m_ivfImage->getImageMap());
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, /*0,*/ m_ivfImage->getInternalFormat(), m_ivfImage->getWidth(), m_ivfImage->getHeight(), 
			/*0,*/ m_ivfImage->getFormat(), GL_UNSIGNED_BYTE, m_ivfImage->getImageMap());
	}

	if (m_imageMap!=nullptr)
	{
		if (!m_generateMipmaps)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageMap);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, /*0,*/ GL_RGBA, m_width, m_height, 
			/*0,*/ GL_RGBA, GL_UNSIGNED_BYTE, m_imageMap);
	}
	
	m_bound = true;

	if (!m_active)
		glDisable(GL_TEXTURE_2D);
}

// ------------------------------------------------------------
void Texture::apply()
{
	if (m_active)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureName);
		
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_textureMode);
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, m_textureEnvColor);

		glMatrixMode (GL_TEXTURE); 
		glLoadIdentity ();
		if ((m_texTransX!=0.0)||(m_texTransY!=0.0))
			glTranslated(m_texTransX, m_texTransY, 0.0);
		if (m_texRotate!=0.0)
			glRotated(m_texRotate, 0.0, 0.0, 1.0);
		if ((m_texScaleX!=0.0)||(m_texScaleY!=0.0))
			glScaled (m_texScaleX, m_texScaleY, 1.0); 
		glMatrixMode (GL_MODELVIEW);
	}
	else
		glDisable(GL_TEXTURE_2D);
}

// ------------------------------------------------------------
void Texture::setTextureRepeat(GLenum repeatS, GLenum repeatT)
{
	m_wrapS = repeatS;
	m_wrapT = repeatT;
}

// ------------------------------------------------------------
void Texture::setRepeat(GLenum repeatS, GLenum repeatT)
{
	m_wrapS = repeatS;
	m_wrapT = repeatT;
}

// ------------------------------------------------------------
void Texture::setFilters(GLenum minFilter, GLenum magFilter)
{
	m_minFilter = minFilter;
	m_magFilter = magFilter;
}

// ------------------------------------------------------------
void Texture::setTextureMode(GLenum mode)
{
	m_textureMode = mode;
}

// ------------------------------------------------------------
void Texture::setMode(GLenum mode)
{
	m_textureMode = mode;
}

// ------------------------------------------------------------
void Texture::setGenerateMipmaps(bool flag)
{
	m_generateMipmaps = flag;
}

// ------------------------------------------------------------
void Texture::setTextureEnvColor(float red, float green, float blue, float alfa)
{
	m_textureEnvColor[0] = red;
	m_textureEnvColor[1] = green;
	m_textureEnvColor[2] = blue;
	m_textureEnvColor[3] = alfa;
}

// ------------------------------------------------------------
void Texture::setEnvColor(float red, float green, float blue, float alfa)
{
	m_textureEnvColor[0] = red;
	m_textureEnvColor[1] = green;
	m_textureEnvColor[2] = blue;
	m_textureEnvColor[3] = alfa;
}

// ------------------------------------------------------------
void Texture::getTextureEnvColor(float &red, float &green, float &blue, float &alfa)
{
	red   = m_textureEnvColor[0];
	green = m_textureEnvColor[1];
	blue  = m_textureEnvColor[2];
	alfa  = m_textureEnvColor[3];
}

// ------------------------------------------------------------
void Texture::getEnvColor(float &red, float &green, float &blue, float &alfa)
{
	red   = m_textureEnvColor[0];
	green = m_textureEnvColor[1];
	blue  = m_textureEnvColor[2];
	alfa  = m_textureEnvColor[3];
}

// ------------------------------------------------------------
void Texture::getSize(int &width, int &height)
{
	width = m_width;
	height = m_height;
}

// ------------------------------------------------------------
void Texture::setTextureModifier(double scaleX, double scaleY, double rotate)
{
	m_texScaleX = scaleX;
	m_texScaleY = scaleY;
	m_texRotate = rotate;
}

// ------------------------------------------------------------
void Texture::setModifier(double scaleX, double scaleY, double rotate)
{
	m_texScaleX = scaleX;
	m_texScaleY = scaleY;
	m_texRotate = rotate;
}

// ------------------------------------------------------------
void Texture::setSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

// ------------------------------------------------------------
void Texture::setImageMap(void *imageMap)
{
	m_imageMap = imageMap;
}

// ------------------------------------------------------------
void Texture::setImage(Image *image)
{
	m_ivfImage = image;
}

// ------------------------------------------------------------
void Texture::setName(GLuint name)
{
	if (m_generateName)
		if (isBound() == TRUE)
			glDeleteTextures(1, &m_textureName);
	
	m_textureName = name;
	m_bound = true;
}

// ------------------------------------------------------------
void Texture::setGenerateName(bool flag)
{
	m_generateName = flag;
}

// ------------------------------------------------------------
void Texture::setLoadImages(bool flag)
{
	m_loadImages = flag;
}

// ------------------------------------------------------------
void Texture::setTextureTranslate(double x, double y)
{
	m_texTransX = x;
	m_texTransY = y;
}

// ------------------------------------------------------------
void Texture::setTranslate(double x, double y)
{
	m_texTransX = x;
	m_texTransY = y;
}

// ------------------------------------------------------------
void Texture::activate()
{
	m_active = true;
}

// ------------------------------------------------------------
void Texture::deactivate()
{
	m_active = false;
}

// ------------------------------------------------------------
bool Texture::isActive()
{
	return m_active;
}
