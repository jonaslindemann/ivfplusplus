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

#ifndef _CIvfTexture_h_
#define _CIvfTexture_h_

#include <ivf/IvfObject.h>
#include <ivf/IvfImage.h>

IvfSmartPointer(CIvfTexture);

/**
 * OpenGL texture definition
 *
 * The IvfTexture class manages a texture in OpenGL.
 * All shape nodes can be assigned a IvfTexture. To
 * be able to see a texture on a shape it must also
 * render texture coordinates. An example of how a
 * texture is used is shown below:
 *
 * \code
 * CIvfJpegImage* image = new CIvfJpegImage();
 * image->setFileName("textures/architecture8.jpg");
 * image->read();
 * 
 * CIvfTexture* texture = new CIvfTexture();
 * texture->setImage(image);
 * texture->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
 * texture->setGenerateMipmaps(true);
 * 
 * .
 * .
 * 
 * CIvfSphere* sphere = new CIvfSphere();
 * sphere->setMaterial(...)
 * sphere->setTexture(texture);
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfTexture : public CIvfObject {
public:
	/** Constructor */
	CIvfTexture ();

	/** Destructor */
	virtual ~CIvfTexture ();

	IvfClassInfo("CIvfTexture",CIvfObject);

	/** 
	 * Set texture environment color (Obsolete! Use setEnvColor() instead)
	 *
	 * Corresponds to an OpenGL
	 * \code
	 *	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, ...);
     * \endcode
	 */
	void setTextureEnvColor(float red, float green, float blue, float alfa);

	/** 
	 * Set texture environment color 
	 *
	 * Corresponds to an OpenGL
	 * \code
	 *	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, ...);
     * \endcode
	 */
	void setEnvColor(float red, float green, float blue, float alfa);

	/** Return texture environment color (Obsolete! Use getEnvColor() instead) */
	void getTextureEnvColor(float &red, float &green, float &blue, float &alfa);

	/** Return texture environment color */
	void getEnvColor(float &red, float &green, float &blue, float &alfa);

	/** Mipmap generation flag */
	void setGenerateMipmaps(bool flag);

	/** 
	 * Set texture mode (Obsolete! Use setMode() instead)
	 *
	 * Sets the OpenGL texture function used. Values are
	 * 
	 * GL_MODULATE, GL_BLEND, GL_DECAL
	 */
	void setTextureMode(GLenum mode);

	/** 
	 * Set texture mode 
	 *
	 * Sets the OpenGL texture function used. Values are
	 * 
	 * GL_MODULATE, GL_BLEND, GL_DECAL
	 */
	void setMode(GLenum mode);

	/**
	 * Set magnification and minification filters 
	 * 
	 * Values for @param minFilter are:
	 *
	 * GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
	 * GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
	 * GL_LINEAR_MIPMAP_LINEAR
	 *
	 * Values for @param maxFilter are:
	 *
	 * GL_NEAREST, GL_LINEAR
	 */
	void setFilters(GLenum minFilter, GLenum magFilter);

	/**
	 * Set texture repeat mode (Obsolete! Use setRepeat instead.)
	 *
	 * If a parameter is set to GL_CLAMP texture coordinates are
	 * clamped to [0,1] if set to GL_REPEAT OpenGL uses only the 
	 * fractional part, see OpenGL reference.
	 *
	 * @param repeatS can be GL_REPEAT or GL_CLAMP
	 * @param repeatY can be GL_REPEAT or GL_CLAMP
	 */
	void setTextureRepeat(GLenum repeatS, GLenum repeatT);

	/**
	 * Set texture repeat mode 
	 *
	 * If a parameter is set to GL_CLAMP texture coordinates are
	 * clamped to [0,1] if set to GL_REPEAT OpenGL uses only the 
	 * fractional part, see OpenGL reference.
	 *
	 * @param repeatS can be GL_REPEAT or GL_CLAMP
	 * @param repeatY can be GL_REPEAT or GL_CLAMP
	 */
	void setRepeat(GLenum repeatS, GLenum repeatT);

	/**
	 * Set texture modifiers (Obsolete! Use setModifier() instead)
	 *
	 * This method sets additional texture modifiers such as scaling
	 * and rotation, which are applied using the glMatrixMode(GL_TEXTURE)
	 */
	void setTextureModifier(double scaleX, double scaleY, double rotate);

	/**
	 * Set texture modifiers
	 *
	 * This method sets additional texture modifiers such as scaling
	 * and rotation, which are applied using the glMatrixMode(GL_TEXTURE)
	 */
	void setModifier(double scaleX, double scaleY, double rotate);

	/**
	 * Set texture translation (Obsolete! use setTranslate() instead)
	 *
	 * This method sets additional texture modifiers translation,
	 * which are applied using the glMatrixMode(GL_TEXTURE)
	 */
	void setTextureTranslate(double x, double y);

	/**
	 * Set texture translation 
	 *
	 * This method sets additional texture modifiers translation,
	 * which are applied using the glMatrixMode(GL_TEXTURE)
	 */
	void setTranslate(double x, double y);

	/**
	 * Sets texture image (required)
	 *
	 * Assigns an image to the texture. An image can be loaded using 
	 * the ivfimage library. 
	 */
	void setImage(CIvfImage* image);
	
	/**
	 * Set raw image map
	 *
	 * If the image has been loaded using other methods than the
	 * IvfImage class a pointer to the image can be set using this method.
	 */
	void setImageMap(void* imageMap);

	/**
	 * Set texture image size
	 *
	 * This method should be used if a raw image map is used, to 
	 * set the image size information.
	 */
	void setSize(int width, int height);

	/** Returns the size of the current loaded image or imagemap. */
	void getSize(int &width, int &height);

	/** Returns true if a texture is bound */
	bool isBound();

	/** 
	 * Applies the texture
	 *
	 * Sets the texture as the current texture in OpenGL.
	 * The bind() method must be called before the apply() method is 
	 * called.
	 */
	void apply();

	/**
	 * Binds texture in OpenGL
	 *
	 * Binds a texture image to a texture name in OpenGL. Must be called
	 * before apply().
	 */
	void bind();

	/** 
	 * Texture name
	 * 
	 * This name is used in conjunction with the OpenGL
	 * command glBindTexture, to bind the texture with 
	 * the geometry.
	 */
	GLuint getName();

	/**
	 * Image loading flag
	 * 
	 * if set to true textures are not bound using
	 * glTexImage2D or glBuild2dMipmaps
	 */
	void setLoadImages(bool flag);

	/** 
	 * Name generation flag
	 *
	 * if set to true (default) texture names will be automatically
	 * generated. If false the texture name set with setName() will
	 * be used.
	 */
	void setGenerateName(bool flag);

	/**
	 * Set texture name
	 *
	 * If automatic texture generation is not used this method
	 * specifies the name to use instead.
	 */
	void setName(GLuint name);

	/** 
	 * Activates texture
	 *
	 * If active, the texture will be applied in the
	 * rendering process.
	 */
	void activate();

	/** 
	 * Activates texture
	 *
	 * If active, the texture will be applied in the
	 * rendering process.
	 */
	void deactivate();

	/** Returns texture active state */
	bool isActive();

private:
	GLenum m_wrapT;
	GLenum m_wrapS;
	GLenum m_textureMode;
	GLenum m_magFilter;
	GLenum m_minFilter;
	GLuint m_textureName;
	float m_textureEnvColor[4];
	bool m_generateMipmaps;
	bool m_bound;
	bool m_generateName;
	bool m_loadImages;
	CIvfImagePtr m_ivfImage;
	void* m_imageMap;
	int m_width;
	int m_height;
	double m_texScaleX;
	double m_texScaleY;
	double m_texRotate;
	double m_texTransX;
	double m_texTransY;
	bool m_active;
};
/** \example textures.cpp
 * This is an example of how to use the CIvfTexture class.
 */

#endif
