#include "StarField.h"

#include <ivfmath/Vec3d.h>

#include <ivfimage/SgiImage.h>

#include <time.h>

using namespace ivf;

StarField::StarField(Camera* camera)
{
	m_texturedStars = new QuadSet();
	m_texturedStars->setUseColor(false);

	m_pointStars = new PointSet();
	m_pointStars->setUseColor(true);

	m_nebulaSphere = new Sphere();
	m_nebulaSphere->setRadius(61.0);

	m_planets = new QuadSet();
	m_planets->setUseColor(false);

	auto image = SgiImage::create();
	image->setFileName("data/images/nebula02.rgb");
	image->read();

	auto texture = Texture::create();
	texture->setImage(image);
	//texture->load("textures/nebula02.rgb");
	m_nebulaSphere->setTexture(texture);

	m_camera = camera;
	initializeStars();
}

StarField::~StarField()
{
}

void StarField::doCreateGeometry()
{
	double x, y, z;
	m_camera->getPosition(x, y, z);
	m_texturedStars->setPosition(x, y, z);
	m_pointStars->setPosition(x, y, z);
	m_nebulaSphere->setPosition(x, y, z);
	m_planets->setPosition(x, y, z);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//m_nebulaSphere->render();
	m_pointStars->render();
	m_texturedStars->render();

	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glAlphaFunc(GL_GEQUAL ,0.9);
	glEnable(GL_ALPHA_TEST);

	m_planets->render();

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

}

void StarField::initializeStars()
{
	int i;

	Vec3d p1;
	Vec3d p2;
	Vec3d p3;
	Vec3d p4;
	Vec3d pos;
	Vec3d normal;
	Vec3d s;
	Vec3d t;

	double x, y, z;
	double alfa, beta;
	float c;

	double starSize = 0.5;
	double nTexturedStars = 200;
	double nPointStars = 3000;
	double nPlanets = 1;
	double planetSize = 30.0;

	m_texturedStars->clear();

	auto image = SgiImage::create();
	image->setFileName("data/images/star02.rgb");
	image->read();

	auto texture = Texture::create();
	texture->setImage(image);
	texture->setTextureMode(GL_DECAL);

	m_texturedStars->setTexture(texture);

	srand( (unsigned)time( nullptr ) );

	for (i=0; i<nTexturedStars*4; i++)
	{
		alfa = 2*M_PI*((double)(rand())/(double)RAND_MAX);
		beta = M_PI/2.0 - M_PI*((double)(rand())/(double)RAND_MAX);
		c = 0.5 + 0.5*((double)(rand())/(double)RAND_MAX);

		getPolarVectors(alfa, beta, normal, s, t);

		pos = 60.0 * normal;

		starSize = 0.3 + 0.5*((double)(rand())/(double)RAND_MAX);

		p1 = pos - starSize * s - starSize * t;
		p2 = pos + starSize * s - starSize * t;
		p3 = pos + starSize * s + starSize * t;
		p4 = pos - starSize * s + starSize * t;

		m_texturedStars->addCoord(p1[0], p1[1], p1[2]);
		m_texturedStars->addTextureCoord(0.0, 0.0);
		m_texturedStars->addCoord(p2[0], p2[1], p2[2]);
		m_texturedStars->addTextureCoord(1.0, 0.0);
		m_texturedStars->addCoord(p3[0], p3[1], p3[2]);
		m_texturedStars->addTextureCoord(1.0, 1.0);
		m_texturedStars->addCoord(p4[0], p4[1], p4[2]);
		m_texturedStars->addTextureCoord(0.0, 1.0);
	}

	IndexPtr idx = Index::create();
	idx->createLinear(0, nTexturedStars*4);
	m_texturedStars->addCoordIndex(idx);

	idx = Index::create();
	idx->createLinear(0, nTexturedStars*4);
	m_texturedStars->addTextureIndex(idx);

	for (i=0; i<nPointStars; i++)
	{
		x = 0.5 - (double)(rand())/(double)RAND_MAX;
		y = 0.5 - (double)(rand())/(double)RAND_MAX;
		z = 0.5 - (double)(rand())/(double)RAND_MAX;
		c = (double)(rand())/(double)RAND_MAX;

		pos.setComponents(x, y, z);
		pos.normalize();
		pos = 60.0 * pos;

		m_pointStars->addCoord(pos[0], pos[1], pos[2]);
		m_pointStars->addColor(c * 0.8f, c * 0.8f,  c);
	}

	idx = new Index();
	idx->createLinear(0, nPointStars);
	m_pointStars->addCoordIndex(idx);

	idx = new Index();
	idx->createLinear(0, nPointStars);
	m_pointStars->addColorIndex(idx);

	image = SgiImage::create();
	image->setFileName("data/images/planet01.rgb");
	image->setAlphaChannel(true);
	image->read();
	image->createAlphaMask(255,0,0,255);
	image->replaceColor(255, 255, 255, 0, 0, 0);
	image->setInternalFormat(GL_RGB10_A2);

	texture = Texture::create();
	texture->setImage(image);
	texture->setTextureMode(GL_BLEND);

	m_planets->setTexture(texture);

	for (i=0; i<nPlanets*4; i++)
	{
		alfa = 2*M_PI*((double)(rand())/(double)RAND_MAX);
		beta = M_PI/2.0 - M_PI*((double)(rand())/(double)RAND_MAX);
		c = 0.5 + 0.5*((double)(rand())/(double)RAND_MAX);

		getPolarVectors(alfa, beta, normal, s, t);

		pos = 50.0 * normal;

		p1 = pos - planetSize * s - planetSize * t;
		p2 = pos + planetSize * s - planetSize * t;
		p3 = pos + planetSize * s + planetSize * t;
		p4 = pos - planetSize * s + planetSize * t;

		m_planets->addCoord(p1[0], p1[1], p1[2]);
		m_planets->addTextureCoord(0.0, 0.0);
		m_planets->addCoord(p2[0], p2[1], p2[2]);
		m_planets->addTextureCoord(1.0, 0.0);
		m_planets->addCoord(p3[0], p3[1], p3[2]);
		m_planets->addTextureCoord(1.0, 1.0);
		m_planets->addCoord(p4[0], p4[1], p4[2]);
		m_planets->addTextureCoord(0.0, 1.0);
	}

	idx = new Index();
	idx->createLinear(0, nPlanets*4);
	m_planets->addCoordIndex(idx);

	idx = new Index();
	idx->createLinear(0, nPlanets*4);
	m_planets->addTextureIndex(idx);
}

void StarField::setCamera(Camera *camera)
{
	m_camera = camera;
}

void StarField::getPolarVectors(double alfa, double beta, Vec3d &normal, Vec3d &s, Vec3d &t)
{
	double x, y, z;

	y = sin(beta);
	x = cos(alfa) * cos(beta);
	z = sin(alfa) * cos(beta);

	normal.setComponents(x, y, z);
	normal.normalize();

	x = sin(alfa);
	z = -cos(alfa);
	y = 0.0;

	s.setComponents(x, y, z);

	t = normal * s;
	t.normalize();
}
