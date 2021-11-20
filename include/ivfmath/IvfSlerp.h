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

#ifndef _CIvfSlerp_h_ 
#define _CIvfSlerp_h_

#include <ivf/ivfconfig.h>

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfSlerpSegment.h>

IvfSmartPointer(CSlerp);

/**
 * Slerp class
 *
 * Interface not finished
 */
class IVFMATH_API CSlerp : public CMathBase {
private:
	std::vector<CSlerpSegment*> m_segments;
	std::vector<CQuat*> m_quats;
	int m_size;
	void initSlerp();
public:
	CSlerp();
	virtual ~CSlerp();

	IvfClassInfo("CSlerp",CMathBase);

	void clear();
	void update();

	void setSize(int size);
	int getSize();
	
	void setQuat(int idx, CQuat& q);
	void setQuatAxisAngle(int idx, double vx, double vy, double vz, double angle);
	CQuat& getQuat(int idx);
	CQuat& getQuat(double t);
};

#endif 
