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

#ifndef _CIvfSlerpSegment_h_
#define _CIvfSlerpSegment_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfQuat.h>

/**
 * Slerp segment class
 *
 * Interface not finished
 */
class IVFMATH_API CIvfSlerpSegment : public CIvfMathBase {
private:
	void initSlerp();
	CIvfQuat* m_q1;
	CIvfQuat* m_q2;
	double m_theta;
public:
	void update();
	CIvfSlerpSegment();
	virtual ~CIvfSlerpSegment();

	const void setEndQuat(CIvfQuat* q);
	const void setStartQuat(CIvfQuat* q);

	CIvfQuat& getQuat(double t);
};

#endif 
