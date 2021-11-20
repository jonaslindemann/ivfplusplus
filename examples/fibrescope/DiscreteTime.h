//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#ifndef _CIvfDiscreteTime_h_
#define _CIvfDiscreteTime_h_

#include <ivf/Base.h>

#define IVF_DT_REWIND 0
#define IVF_DT_STOP   1
#define IVF_DT_REVERSE 2

IvfSmartPointer(CDiscreteTime);

class CDiscreteTime : public ivf::CBase {
private:
	long m_time;
	long m_startTime;
	long m_endTime;
	long m_step;
	int m_endAction;
	bool m_loop;
public:
	void checkStep(int testStep);
	void reset();
	void setLoop(bool flag);
	void last();
	void setEndAction(int action);
	void setTime(long time);
	CDiscreteTime();
	virtual ~CDiscreteTime();

	void rewind();
	void next();
	void previous();

	void setInterval(long startTime, long endTime);
	long getCurrentTime();
};

#endif 
