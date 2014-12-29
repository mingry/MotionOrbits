	
#include "GLUU/gluu.h"


namespace mg
{
static bool g_flag_shiny_mat = true;
static bool g_flag_soft_color_mat = false;

static GLfloat g_shiny_mat_shininess[] = {128};

static const GLfloat g_1111f[] = {1, 1, 1, 1};
static const GLfloat g_0001f[] = {0, 0, 0, 1};

static const int g_indexed_mtrl_num = 42;
static const double g_indexed_mtrl[g_indexed_mtrl_num][3] = { 
			{0.7f, 1.0f, 1.0f},
			{1.0f, 0.7f, 0.7f},
			{0.7f, 1.0f, 0.7f},
			{0.7f, 0.7f, 1.0f},
			{0/255.0,	204/255.0,	255/255.0},	
			{204/255.0,	255/255.0,	204/255.0},
			{255/255.0,	255/255.0,	153/255.0},
			{153/255.0,	204/255.0,	255/255.0},
			{255/255.0,	153/255.0,	204/255.0},
			{204/255.0,	153/255.0,	255/255.0},	// 10
			{255/255.0,	204/255.0,	153/255.0},
			{51/255.0,	102/255.0,	255/255.0},
			{51/255.0,	204/255.0,	204/255.0},
			{153/255.0,	204/255.0,	0/255.0},
			{255/255.0,	204/255.0,	0/255.0},
			{255/255.0,	153/255.0,	0/255.0},
			{255/255.0,	102/255.0,	0/255.0},
			{102/255.0,	102/255.0,	153/255.0},
			{150/255.0,	150/255.0,	150/255.0},
			{0/255.0,	51/255.0,	102/255.0},	// 20
			{51/255.0,	153/255.0,	102/255.0},
			{0/255.0,	51/255.0,	0/255.0},
			{51/255.0,	51/255.0,	0/255.0},
			{153/255.0,	51/255.0,	0/255.0},
			{51/255.0,	51/255.0,	153/255.0},
			{51/255.0,	51/255.0,	51/255.0},
			{128/255.0,	0/255.0,	0/255.0},
			{0/255.0,	128/255.0,	0/255.0},
			{0/255.0,	0/255.0,	128/255.0},
			{128/255.0,	128/255.0,	0/255.0},	// 30
			{128/255.0,	0/255.0,	128/255.0},
			{0/255.0,	128/255.0,	128/255.0},
			{192/255.0,	192/255.0,	192/255.0},
			{128/255.0,	128/255.0,	128/255.0},
			{153/255.0,	153/255.0,	255/255.0},
			{153/255.0,	51/255.0,	102/255.0},
			{255/255.0,	255/255.0,	204/255.0},
			{204/255.0,	255/255.0,	255/255.0},
			{102/255.0,	0/255.0,	102/255.0},
			{255/255.0,	128/255.0,	128/255.0},	// 40
			{0/255.0,	102/255.0,	204/255.0},
			{204/255.0,	204/255.0,	255/255.0}
			};

static int g_recently_used_color_index = 0;

bool mgluSoftColorMaterial()
{
	return g_flag_soft_color_mat;
}

void mgluSoftColorMaterial(bool flag)
{
	g_flag_soft_color_mat = flag;
}

bool mgluShinyMaterial()
{
	return g_flag_shiny_mat;
}

void mgluShinyMaterial(bool flag)
{
	g_flag_shiny_mat = flag;
	if ( flag ) g_shiny_mat_shininess[0] = 128;
	else g_shiny_mat_shininess[0] = 0;
}


void mgluColor(math::vector c)
{
	glColor3f(c[0], c[1], c[2]);
}

void mgluColor(math::vector c, GLdouble a)
{
	glColor4f(c[0], c[1], c[2], a);
}

void mgluColor(GLdouble r, GLdouble g, GLdouble b)
{
	glColor3f(r, g, b);
}

void mgluColor(::GLdouble r, GLdouble g, GLdouble b, GLdouble a)
{
	glColor4f(r, g, b, a);
}

void mgluColor(unsigned int i)
{
	mgluColor(mgluGetIndexColor(i));
}

void mgluColor(unsigned int i, GLdouble a)
{
	mgluColor(mgluGetIndexColor(i), a);
}



void mgluSimpleMaterial(math::vector c)
{
	mgluSimpleMaterial(c[0], c[1], c[2]);
}

void mgluSimpleMaterial(math::vector c, GLdouble a)
{
	mgluSimpleMaterial(c[0], c[1], c[2], a);
}


void mgluSimpleMaterial(GLdouble r, GLdouble g, GLdouble b)
{
	GLfloat diffuseV[4];
	GLfloat ambientV[4];

	if ( g_flag_soft_color_mat )
	{
		diffuseV[0] = r * 0.8f;
		diffuseV[1] = g * 0.8f;
		diffuseV[2] = b * 0.8f;
		diffuseV[3] = 1.0f;

		ambientV[0] = r * 0.2f;
		ambientV[1] = g * 0.2f;
		ambientV[2] = b * 0.2f;
		ambientV[3] = 1.0f;
	}
	else
	{
		diffuseV[0] = r;
		diffuseV[1] = g;
		diffuseV[2] = b;
		diffuseV[3] = 1.0f;

		ambientV[0] = 0.2f;
		ambientV[1] = 0.2f;
		ambientV[2] = 0.2f;
		ambientV[3] = 1.0f;
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientV);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseV);

	if ( g_flag_shiny_mat )
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_1111f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, g_shiny_mat_shininess);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_0001f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, g_shiny_mat_shininess);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, g_0001f);
	}

	
}

void mgluSimpleMaterial(GLdouble r, GLdouble g, GLdouble b, GLdouble a)
{
	GLfloat diffuseV[4];
	GLfloat ambientV[4];

	if ( g_flag_soft_color_mat )
	{
		diffuseV[0] = r * 0.8f;
		diffuseV[1] = g * 0.8f;
		diffuseV[2] = b * 0.8f;
		diffuseV[3] = a;

		ambientV[0] = r * 0.2f;
		ambientV[1] = g * 0.2f;
		ambientV[2] = b * 0.2f;
		ambientV[3] = a;
	}
	else
	{
		diffuseV[0] = r;
		diffuseV[1] = g;
		diffuseV[2] = b;
		diffuseV[3] = a;

		ambientV[0] = 0.2f;
		ambientV[1] = 0.2f;
		ambientV[2] = 0.2f;
		ambientV[3] = a;
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientV);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseV);

	if ( g_flag_shiny_mat )
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_1111f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, g_shiny_mat_shininess);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, g_0001f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, g_shiny_mat_shininess);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, g_0001f);
	}
}

void mgluSimpleMaterial(unsigned int i)
{
	mgluSimpleMaterial(mgluGetIndexColor(i));
}

void mgluSimpleMaterial(unsigned int i, GLdouble a)
{
	mgluSimpleMaterial(mgluGetIndexColor(i), a);
}



void mgluColorMaterial(math::vector c)
{
	mgluColorMaterial(c[0], c[1], c[2]);
}

void mgluColorMaterial(math::vector c, GLdouble a)
{
	mgluColorMaterial(c[0], c[1], c[2], a);
}

void mgluColorMaterial(GLdouble r, GLdouble g, GLdouble b)
{
	glColor3f(r, g, b);
	mgluSimpleMaterial(r, g, b);
}

void mgluColorMaterial(GLdouble r, GLdouble g, GLdouble b, GLdouble a)
{
	glColor4f(r, g, b, a);
	mgluSimpleMaterial(r, g, b, a);
}

void mgluColorMaterial(unsigned int i)
{
	mgluColorMaterial(mgluGetIndexColor(i));
}

void mgluColorMaterial(unsigned int i, GLdouble a)
{
	mgluColorMaterial(mgluGetIndexColor(i), a);
}



math::vector mgluGetIndexColor(unsigned int i)
{
	int r = i % g_indexed_mtrl_num;
	g_recently_used_color_index = r;
	return math::vector(g_indexed_mtrl[r][0], g_indexed_mtrl[r][1], g_indexed_mtrl[r][2]);
}

unsigned int mgluGetRecentlyUsedColorIndex()
{
	return g_recently_used_color_index;
}


};





