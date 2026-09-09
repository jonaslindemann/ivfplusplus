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
#include <ivf/LegacyGL.h>
#include <ivf/rc.h>

#include <glm/glm.hpp>

#include <cmath>

using namespace ivf;

static GLuint s_currentBoundTexture = 0;

// ------------------------------------------------------------
Texture::Texture ()
:GLBase()
{
	m_bound = false;
	m_textureDataUploaded = false;
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
		{
			glDeleteTextures(1, &m_textureName);
			if (s_currentBoundTexture == m_textureName)
				s_currentBoundTexture = 0;
		}
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
	// Only generate texture name once
	if (!isBound() && m_generateName)
	{
		glGenTextures(1, &m_textureName);
		m_bound = true;
	}

	// Bind texture only if not already bound
	if (s_currentBoundTexture != m_textureName)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureName);
		s_currentBoundTexture = m_textureName;
	}
#endif
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);

	lgTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_textureMode);
	lgTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, m_textureEnvColor);
	syncToRenderContext();

	if (!m_loadImages)
	{
		return;
	}

	// Only upload texture data once
	if (!m_textureDataUploaded)
	{
		// gluBuild2DMipmaps was the one GLU call in this library that ran on every
		// profile rather than only the legacy path, and GLU is not available in
		// core. glTexImage2D plus glGenerateMipmap does the same job and has been
		// core since 3.0. It also drops GLU's power-of-two rescale, which nothing
		// has needed since non-power-of-two textures became standard.

		if (m_ivfImage!=nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, m_ivfImage->getInternalFormat(), m_ivfImage->getWidth(), m_ivfImage->getHeight(),
				0, m_ivfImage->getFormat(), GL_UNSIGNED_BYTE, m_ivfImage->getImageMap());

			if (m_generateMipmaps)
				glGenerateMipmap(GL_TEXTURE_2D);
		}

		if (m_imageMap!=nullptr)
		{
			// The internal format here used to be GL_RGB while the data handed in
			// was GL_RGBA, so the alpha channel was discarded on upload and every
			// texel came back opaque. The mipmapped branch alongside it already
			// used GL_RGBA, so the two disagreed about the same image.

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageMap);

			if (m_generateMipmaps)
				glGenerateMipmap(GL_TEXTURE_2D);
		}

		m_textureDataUploaded = true;
	}

	if (!m_active)
		lgDisableLegacy(GL_TEXTURE_2D);
}

// ------------------------------------------------------------
void Texture::apply()
{
	if (m_active)
	{
		// Only bind if not already bound
		if (s_currentBoundTexture != m_textureName)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureName);
			s_currentBoundTexture = m_textureName;
		}

		lgTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, m_textureMode);
		lgTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, m_textureEnvColor);

		lgMatrixMode(GL_TEXTURE);
		lgLoadIdentity();
		if ((m_texTransX!=0.0)||(m_texTransY!=0.0))
			lgTranslated(m_texTransX, m_texTransY, 0.0);
		if (m_texRotate!=0.0)
			lgRotated(m_texRotate, 0.0, 0.0, 1.0);
		if ((m_texScaleX!=0.0)||(m_texScaleY!=0.0))
			lgScaled(m_texScaleX, m_texScaleY, 1.0);
		lgMatrixMode(GL_MODELVIEW);

		syncToRenderContext();
	}
	else
		lgDisableLegacy(GL_TEXTURE_2D);
}

// ------------------------------------------------------------
void Texture::invalidateBindCache()
{
	s_currentBoundTexture = 0;
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
	m_textureDataUploaded = false;
}

// ------------------------------------------------------------
void Texture::setImage(Image *image)
{
	m_ivfImage = image;
	m_textureDataUploaded = false;
}

// ------------------------------------------------------------
void Texture::setName(GLuint name)
{
	if (m_generateName)
		if (isBound() == TRUE)
		{
			glDeleteTextures(1, &m_textureName);
			if (s_currentBoundTexture == m_textureName)
				s_currentBoundTexture = 0;
		}

	m_textureName = name;
	m_bound = true;
	m_textureDataUploaded = false;
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

// ------------------------------------------------------------
void Texture::refresh()
{
	m_textureDataUploaded = false;
}

// ------------------------------------------------------------
bool Texture::hasModernPath()
{
	return true;
}

// ------------------------------------------------------------
void Texture::syncToRenderContext()
{
	// The texture environment and the GL_TEXTURE matrix stack both disappear in
	// a core profile, so hand the same information to the shader, which applies
	// them itself.

	int mode = 0; // modulate

	switch (m_textureMode)
	{
	case GL_DECAL:   mode = 1; break;
	case GL_REPLACE: mode = 2; break;
	case GL_BLEND:   mode = 3; break;
	default:         mode = 0; break;
	}

	rcSetTextureMode(mode);
	rcSetTextureEnvColor(m_textureEnvColor[0], m_textureEnvColor[1],
	                     m_textureEnvColor[2], m_textureEnvColor[3]);

	// The same translate, rotate, scale the legacy path builds above, as one 3x3
	// affine transform on texture coordinates, in that order.
	//
	// Built by hand rather than with glm::translate/rotate/scale for mat3: those
	// live in GLM_GTX_matrix_transform_2d, which is an experimental extension and
	// refuses to compile without GLM_ENABLE_EXPERIMENTAL. Three literals are
	// cheaper than taking that on.

	const float tx = (float)m_texTransX;
	const float ty = (float)m_texTransY;
	const float c  = std::cos(glm::radians((float)m_texRotate));
	const float s  = std::sin(glm::radians((float)m_texRotate));
	const float sx = (float)m_texScaleX;
	const float sy = (float)m_texScaleY;

	// glm::mat3 takes columns, so each row below is one column of the matrix.

	const glm::mat3 translate(1.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f,
	                          tx,   ty,   1.0f);

	const glm::mat3 rotate(c,    s,    0.0f,
	                       -s,   c,    0.0f,
	                       0.0f, 0.0f, 1.0f);

	const glm::mat3 scale(sx,   0.0f, 0.0f,
	                      0.0f, sy,   0.0f,
	                      0.0f, 0.0f, 1.0f);

	rcSetTextureMatrix(translate * rotate * scale);
}
