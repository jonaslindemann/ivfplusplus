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

#ifndef _CIvfMaterialSet_h_
#define _CIvfMaterialSet_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfMaterial.h>

IvfSmartPointer(CIvfMaterialSet);

/**
 * Material set class
 *
 * This class is used with the CIvfGLPrimitive class and
 * CIvfPolySet class to define materials on a primitive level.
 * The CIvfAc3DReader class reads CIvfPolySet:s with material 
 * sets
 * @author Jonas Lindemann
 */
class IVF_API CIvfMaterialSet : public CIvfBase {
private:
	std::vector<CIvfMaterialPtr> m_materials;
public:
	/** CIvfMaterialSet constructor */
	CIvfMaterialSet();

	/** CIvfMaterialSet destructor */
	virtual ~CIvfMaterialSet();

	IvfClassInfo("CIvfMaterialSet",CIvfBase);

	/** Add material to set */
	void addMaterial(CIvfMaterial* material);

	/** Clear materials from set */
	void clear();

	/** Return material at @param pos */
	CIvfMaterial* getMaterial(int pos);

	/** Return number of materials in set */
	int getSize();
};

#endif 
