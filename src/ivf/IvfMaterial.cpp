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

// Implementation of: public class CMaterial

#include <ivf/ivfconfig.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfGlobalState.h>

// Needed by rgb2hsv()
float maxrgb(float r,float g,float b)
{
	float max;

	if( r > g)
		max = r;
	else
		max = g;
	if( b > max )
		max = b;
	return( max );
}


// Needed by rgb2hsv()
float minrgb(float r,float g,float b)
{
	float min;

	if( r < g)
		min = r;
	else
		min = g;
	if( b < min )
		min = b;
	return( min );
}

/* Taken from "Fund'l of 3D Computer Graphics", Alan Watt (1989)
Assumes (r,g,b) range from 0.0 to 1.0
Sets h in degrees: 0.0 to 360.;
s,v in [0.,1.]
*/
void rgb2hsv(float r, float g, float b,
			 float *hout, float *sout, float *vout)
{
	float h=0,s=1.0,v=1.0;
	float max_v,min_v,diff,r_dist,g_dist,b_dist;
	float undefined = 0.0;

	max_v = maxrgb(r,g,b);
	min_v = minrgb(r,g,b);
	diff = max_v - min_v;
	v = max_v;

	if( max_v != 0 )
		s = diff/max_v;
	else
		s = 0.0;
	if( s == 0 )
		h = undefined;
	else {
		r_dist = (max_v - r)/diff;
		g_dist = (max_v - g)/diff;
		b_dist = (max_v - b)/diff;
		if( r == max_v )
			h = b_dist - g_dist;
		else
			if( g == max_v )
				h = 2 + r_dist - b_dist;
			else
				if( b == max_v )
					h = 4 + g_dist - r_dist;
				else
					printf("rgb2hsv::How did I get here?\n");
				h *= 60;
				if( h < 0)
					h += 360.0;
	}
	*hout = h;
	*sout = s;
	*vout = v;
}

/* Taken from "Fund'l of 3D Computer Graphics", Alan Watt (1989)
Assumes H in degrees, s,v in [0.,1.0];
(r,g,b) range from 0.0 to 1.0
*/
void hsv2rgb(float hin, float s, float v,
			 float *rout, float *gout, float *bout)
{
	float h;
	float r=0,g=0,b=0;
	float f,p,q,t;
	int i;

	h = hin;
	if( s == 0 ) {
		r = v;
		g = v;
		b = v;
	}
	else {
		if(h == 360.)
			h = 0.0;
		h /= 60.;
		i = (int) h;
		f = h - i;
		p = v*(1-s);
		q = v*(1-(s*f));
		t = v*(1-s*(1-f));
		switch(i) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		case 5:
			r = v;
			g = p;
			b = q;
			break;
		default:
			r = 1.0;
			b = 1.0;
			b = 1.0;
			//printf("hsv2rgb::How did I get here?\n");
			// printf("h: %f, s: %f, v: %f; i:  %d\n",hin,s,v,i);
			break;
		}
	}
	*rout = r;
	*gout = g;
	*bout = b;
}

// ------------------------------------------------------------
CMaterial::CMaterial ()
:CGLBase()
//TODO: check and complete member initialisation list!
{
	this->setDiffuseColor(0.5, 0.5, 0.5, 1.0);
	this->setSpecularColor(0.0, 0.0, 0.0, 1.0);
	this->setAmbientColor(0.0, 0.0, 0.0, 1.0);
	this->setShininess(100.0f);
	this->setEmissionColor(0.0, 0.0, 0.0, 1.0);

	m_colorMaterial = false;
}

// ------------------------------------------------------------
CMaterial::~CMaterial ()
{
}

// ------------------------------------------------------------
void CMaterial::setDiffuseColor (const float red, const float green, const float blue, const float alfa)
{
	m_diffuseColor[0] = red;
	m_diffuseColor[1] = green;
	m_diffuseColor[2] = blue;
	m_diffuseColor[3] = alfa;
}

// ------------------------------------------------------------
void CMaterial::setSpecularColor (const float red, const float green, const float blue, const float alfa)
{
	m_specularColor[0] = red;
	m_specularColor[1] = green;
	m_specularColor[2] = blue;
	m_specularColor[3] = alfa;
}

// ------------------------------------------------------------
void CMaterial::setAmbientColor (const float red, const float green, const float blue, const float alfa)
{
	m_ambientColor[0] = red;
	m_ambientColor[1] = green;
	m_ambientColor[2] = blue;
	m_ambientColor[3] = alfa;
}

// ------------------------------------------------------------
void CMaterial::getDiffuseColor (float &red, float &green, float &blue, float &alfa)
{
	red = m_diffuseColor[0];
	green = m_diffuseColor[1];
	blue = m_diffuseColor[2];
	alfa = m_diffuseColor[3];
}

// ------------------------------------------------------------
void CMaterial::getSpecularColor (float &red, float &green, float &blue, float &alfa)
{
	red = m_specularColor[0];
	green = m_specularColor[1];
	blue = m_specularColor[2];
	alfa = m_specularColor[3];
}

// ------------------------------------------------------------
void CMaterial::getAmbientColor (float &red, float &green, float &blue, float &alfa)
{
	red = m_ambientColor[0];
	green = m_ambientColor[1];
	blue = m_ambientColor[2];
	alfa = m_ambientColor[3];
}

// ------------------------------------------------------------
void CMaterial::doCreateMaterial()
{
	if (glIsEnabled(GL_LIGHTING))
	{
		if (CGlobalState::getInstance()->isGreyscaleRenderingEnabled())
		{
			float temp[4];

			toGreyscale(m_ambientColor, temp); glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
			toGreyscale(m_diffuseColor, temp); glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
			toGreyscale(m_specularColor, temp); glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
			toGreyscale(m_emissionColor, temp); glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, temp);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
		}
		else
		{
			if (!m_colorMaterial)
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambientColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuseColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specularColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissionColor);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
			}
			else
			{
				glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specularColor);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissionColor);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
			}
		}
	}
	else
        if (CGlobalState::getInstance()->isColorOutputEnabled())
            glColor4fv(m_diffuseColor);
}

void CMaterial::setEmissionColor(const float red, const float green, const float blue, const float alfa)
{
	m_emissionColor[0] = red;
	m_emissionColor[1] = green;
	m_emissionColor[2] = blue;
	m_emissionColor[3] = alfa;
}

void CMaterial::getEmissionColor(float &red, float &green, float &blue, float &alfa)
{
	red = m_emissionColor[0];
	green = m_emissionColor[1];
	blue = m_emissionColor[2];
	alfa = m_emissionColor[3];
}

void CMaterial::setShininess(float shininess)
{
	m_shininess = shininess;
}

void CMaterial::getShininess(float &shininess)
{
	shininess = m_shininess;
}

void CMaterial::setAlphaValue(float alpha)
{
	m_diffuseColor[3] = alpha;
	m_ambientColor[3] = alpha;
	m_emissionColor[3] = alpha;
	m_specularColor[3] = alpha;
}

void CMaterial::assignFrom(CMaterial *material)
{
	float r, g, b, a, s;

	material->getSpecularColor(r, g, b, a);
	this->setSpecularColor(r, g, b, a);
	material->getDiffuseColor(r, g, b, a);
	this->setDiffuseColor(r, g, b, a);
	material->getAmbientColor(r, g, b, a);
	this->setAmbientColor(r, g, b, a);
	material->getEmissionColor(r, g, b, a);
	this->setEmissionColor(r, g, b, a);
	material->getShininess(s);
	this->setShininess(s);
}

void CMaterial::multiplySpecular(double factor)
{
	m_specularColor[0]*=factor;
	m_specularColor[1]*=factor;
	m_specularColor[2]*=factor;
	m_specularColor[3]*=factor;

	int i;

	for (i=0; i<4; i++)
		if (m_specularColor[i]>1.0f)
			m_specularColor[i] = 1.0f;
}

void CMaterial::multiplyDiffuse(double factor)
{
	m_diffuseColor[0]*=factor;
	m_diffuseColor[1]*=factor;
	m_diffuseColor[2]*=factor;
	m_diffuseColor[3]*=factor;

	int i;

	for (i=0; i<4; i++)
		if (m_diffuseColor[i]>1.0f)
			m_diffuseColor[i] = 1.0f;
}

void CMaterial::multiplyAmbient(double factor)
{
	m_ambientColor[0]*=factor;
	m_ambientColor[1]*=factor;
	m_ambientColor[2]*=factor;
	m_ambientColor[3]*=factor;

	int i;

	for (i=0; i<4; i++)
		if (m_ambientColor[i]>1.0f)
			m_ambientColor[i] = 1.0f;
}

void CMaterial::multiplyEmission(double factor)
{
	m_emissionColor[0]*=factor;
	m_emissionColor[1]*=factor;
	m_emissionColor[2]*=factor;
	m_emissionColor[3]*=factor;

	int i;

	for (i=0; i<4; i++)
		if (m_emissionColor[i]>1.0f)
			m_emissionColor[i] = 1.0f;
}

void CMaterial::multiplyColor(double factor)
{
	multiplySpecular(factor);
	multiplyDiffuse(factor);
	multiplyAmbient(factor);
	multiplyEmission(factor);
}

void CMaterial::modifyBrightness(float *color, double factor)
{
	float h, s, v;

	rgb2hsv(
		color[0], color[1], color[2],
		&h, &s, &v
	);

	v+=factor;
	if (v>1.0f)
		v = 1.0f;

	if (v<0.0f)
		v = 0.0f;

	hsv2rgb(
		h, s, v,
		&color[0], &color[1], &color[2]
	);
}

void CMaterial::brightnessSpecular(double factor)
{
	modifyBrightness(m_specularColor, factor);
}

void CMaterial::brightnessDiffuse(double factor)
{
	modifyBrightness(m_diffuseColor, factor);
}

void CMaterial::brightnessAmbient(double factor)
{
	modifyBrightness(m_ambientColor, factor);
}

void CMaterial::brightnessEmission(double factor)
{
	modifyBrightness(m_emissionColor, factor);
}


void CMaterial::toGreyscale(float *color, float *grey)
{
	float mean = (color[0] + color[1] + color[2])/3.0f;
	grey[0] = mean;
	grey[1] = mean;
	grey[2] = mean;
}

void CMaterial::setColorMaterial(bool flag)
{
	m_colorMaterial = flag;
}

bool CMaterial::getColorMaterial()
{
	return m_colorMaterial;
}
