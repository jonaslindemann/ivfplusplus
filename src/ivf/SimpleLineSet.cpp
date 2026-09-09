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

// Implementation of: public class SimpleLineSet

#include <ivf/config.h>
#include <ivf/SimpleLineSet.h>
#include <ivf/rc.h>
#include <ivf/LegacyGL.h>

#include <vector>

using namespace ivf;

// ------------------------------------------------------------
SimpleLineSet::SimpleLineSet ()
		:Shape()
{
	m_nCoords = 0;
	m_nCoordIndex = 0;
	m_nColors = 0;
	m_coords = nullptr;
	m_coordIndex = nullptr;
	m_colors = nullptr;
}

// ------------------------------------------------------------
SimpleLineSet::~SimpleLineSet ()
{
	if (m_coordIndex!=nullptr)
		delete [] m_coordIndex;
	if (m_coords!=nullptr)
		delete [] m_coords;
	if (m_colors!=nullptr)
		delete [] m_colors;
}

// ------------------------------------------------------------
void SimpleLineSet::doCreateGeometry()
{
	int i;

	if (rcIsShaderActive())
	{
		// Same strip walk as below, but emitted as independent line segments:
		// one draw call cannot express several strips, and this geometry is
		// unlit either way, which is exactly what drawUnlit() provides.

		std::vector<float> positions;
		std::vector<float> colors;

		int prev = -1;

		auto emit = [&](int idx) {
			positions.push_back((float)m_coords[idx][0]);
			positions.push_back((float)m_coords[idx][1]);
			positions.push_back((float)m_coords[idx][2]);

			if (m_colors != nullptr)
			{
				colors.push_back((float)m_colors[idx][0]);
				colors.push_back((float)m_colors[idx][1]);
				colors.push_back((float)m_colors[idx][2]);
			}
			else
			{
				colors.push_back(1.0f);
				colors.push_back(1.0f);
				colors.push_back(1.0f);
			}

			colors.push_back(1.0f);
		};

		for (i = 0; i < m_nCoordIndex; i++)
		{
			const int idx = m_coordIndex[i];

			if (idx == -1)
			{
				prev = -1;
				continue;
			}

			if (prev >= 0)
			{
				emit(prev);
				emit(idx);
			}

			prev = idx;
		}

		if (positions.empty())
			return;

		if (rcDrawUnlit(GL_LINES, positions.data(), colors.data(), (int)(positions.size() / 3)))
			return;
	}

	lgPushAttrib(GL_LIGHTING);
	lgDisableLegacy(GL_LIGHTING);

	// A -1 in the index array ends the current strip. Track whether one is open
	// rather than assuming the array ends with a terminator: an index array
	// without one used to leave the primitive unclosed, and an unmatched
	// glBegin() wedges the context for the rest of the frame -- every later call
	// fails with "invalid from the current immediate mode state", which reads as
	// a fault in whatever happened to be drawn next.

	bool stripOpen = false;

	for (i = 0; i<m_nCoordIndex; i++)
	{
		if ((m_colors!=nullptr)&&(m_coordIndex[i]>=0))
		{
			lgColor3d(
				m_colors[ m_coordIndex[i] ][0],
				m_colors[ m_coordIndex[i] ][1],
				m_colors[ m_coordIndex[i] ][2]);
		}
		if (m_coordIndex[i] == -1)
		{
			if (stripOpen)
			{
				lgEnd();
				stripOpen = false;
			}
		}
		else
		{
			if (!stripOpen)
			{
				lgBegin(GL_LINE_STRIP);
				stripOpen = true;
			}

			lgVertex3dv(m_coords[ m_coordIndex[i] ]);
		}
	}

	if (stripOpen)
		lgEnd();

	lgPopAttrib();
}

// ------------------------------------------------------------
void SimpleLineSet::setCoordSize(int n)
{
	m_nCoords = n;
	if (m_coords!=nullptr)
		delete [] m_coords;
	m_coords = new double[m_nCoords][3];
}

// ------------------------------------------------------------
void SimpleLineSet::setLineSize(int n)
{
	m_nCoordIndex = n;
	if (m_coordIndex!=nullptr)
		delete [] m_coordIndex;
	m_coordIndex = new int[m_nCoordIndex];
}

// ------------------------------------------------------------
void SimpleLineSet::setCoord(int n, double x, double y, double z)
{
	if (m_coords!=nullptr)
	{
		if (n<m_nCoords)
		{
			m_coords[n][0] = x;
			m_coords[n][1] = y;
			m_coords[n][2] = z;
		}
	}
}

// ------------------------------------------------------------
void SimpleLineSet::setLineIndex(int n, int idx)
{
	if (m_coordIndex!=nullptr)
	{
		if (n<m_nCoordIndex)
		{
			m_coordIndex[n] = idx;
		}
	}
}

// ------------------------------------------------------------
void SimpleLineSet::setColorSize(int n)
{
	m_nColors = n;
	if (m_colors!=nullptr)
		delete [] m_colors;
	m_colors = new double[m_nColors][3];
}

// ------------------------------------------------------------
void SimpleLineSet::setColor(int n, double red, double green, double blue)
{
	if (m_colors!=nullptr)
	{
		if (n<m_nColors)
		{
			m_colors[n][0] = red;
			m_colors[n][1] = blue;
			m_colors[n][2] = green;
		}
	}
}


// ------------------------------------------------------------
bool SimpleLineSet::hasModernPath()
{
	return true;
}
