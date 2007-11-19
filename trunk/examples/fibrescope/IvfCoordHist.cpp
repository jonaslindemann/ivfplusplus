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

#include "IvfCoordHist.h"

CIvfCoordHist::CIvfCoordHist()
{
	m_discreteTime = NULL;
}

CIvfCoordHist::~CIvfCoordHist()
{
	this->clear();
}

void CIvfCoordHist::addList(CIvfCoordList* list)
{
	m_lists.push_back(CIvfCoordListPtr(list));
}

CIvfCoordList* CIvfCoordHist::getList(long idx)
{
	if ( (idx>=0)&&(idx<m_lists.size()) )
		return m_lists[idx];
	else
		return NULL;
}

long CIvfCoordHist::getSize()
{
	return m_lists.size();
}

void CIvfCoordHist::clear()
{
	m_lists.clear();
}

CIvfCoordList* CIvfCoordHist::getList()
{
	if (m_discreteTime!=NULL)
	{
		long timeStep = m_discreteTime->getCurrentTime();
		if ((timeStep>=0)&&(timeStep<m_lists.size()))
			return m_lists[timeStep];
		else
			return NULL;
	}
	else
		return m_lists[0];
}

void CIvfCoordHist::setDiscreteTime(CIvfDiscreteTime *discreteTime)
{
	m_discreteTime = discreteTime;
}

void CIvfCoordHist::saveToStream(std::ostream &out)
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

void CIvfCoordHist::readFromStream(std::istream &in)
{
	using namespace std;
	long i;
	long nLists;
	CIvfCoordList* coordList;

	this->clear();

	in >> nLists;

	for (i=0; i<nLists; i++)
	{
		coordList = new CIvfCoordList();
		coordList->readFromStream(in);
		m_lists.push_back(coordList);
	}
}
