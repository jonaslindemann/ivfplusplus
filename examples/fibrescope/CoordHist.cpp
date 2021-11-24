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

#include "CoordHist.h"

using namespace ivf;

CoordHist::CoordHist()
{
	m_discreteTime = nullptr;
}

CoordHist::~CoordHist()
{
	this->clear();
}

void CoordHist::addList(CoordList* list)
{
	m_lists.push_back(CoordListPtr(list));
}

CoordList* CoordHist::getList(long idx)
{
	if ( (idx>=0)&&(idx<m_lists.size()) )
		return m_lists[idx];
	else
		return nullptr;
}

long CoordHist::getSize()
{
	return static_cast<int>(m_lists.size());
}

void CoordHist::clear()
{
	m_lists.clear();
}

CoordList* CoordHist::getList()
{
	if (m_discreteTime!=nullptr)
	{
		long timeStep = m_discreteTime->getCurrentTime();
		if ((timeStep>=0)&&(timeStep<m_lists.size()))
			return m_lists[timeStep];
		else
			return nullptr;
	}
	else
		return m_lists[0];
}

void CoordHist::setDiscreteTime(DiscreteTime *discreteTime)
{
	m_discreteTime = discreteTime;
}

void CoordHist::saveToStream(std::ostream &out)
{
	using namespace std;
	
	long i;

	out << m_lists.size() << endl;
	
	out << endl;
	out << endl;

	for (i=0; i<m_lists.size(); i++)
	{
		m_lists[i]->saveToStream(out);
		out << endl;
	}
}

void CoordHist::readFromStream(std::istream &in)
{
	using namespace std;
	long i;
	long nLists;
	CoordList* coordList;

	this->clear();

	in >> nLists;

	for (i=0; i<nLists; i++)
	{
		coordList = new CoordList();
		coordList->readFromStream(in);
		m_lists.push_back(coordList);
	}
}
