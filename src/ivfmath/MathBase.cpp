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

#include <ivf/config.h>
#include <ivfmath/MathBase.h>

using namespace ivf;

// ------------------------------------------------------------
MathBase::MathBase()
{
	// Set initial reference count

	m_ref = 0;
}

// ------------------------------------------------------------
MathBase::~MathBase()
{

}

// ------------------------------------------------------------
void MathBase::addReference()
{
	m_ref++;
}

// ------------------------------------------------------------
void MathBase::deleteReference()
{
	if (m_ref>0)
		m_ref--;
}

// ------------------------------------------------------------
bool MathBase::referenced()
{
	if (m_ref>0)
		return true;
	else
		return false;
}

// ------------------------------------------------------------
const std::string MathBase::getClassNameThis()
{
	return "MathBase";
}

// ------------------------------------------------------------
const std::string MathBase::getClassName()
{
	return "MathBase";
}

// ------------------------------------------------------------
bool MathBase::isClass(const std::string& name)
{
	std::string className = this->getClassNameThis();

	if (!className.empty())
	{
		if (className==name)
			return true;
		else
			return false;
	}
	else
		return false;
}

// ------------------------------------------------------------
int MathBase::getRefCount()
{
	return m_ref;
}

