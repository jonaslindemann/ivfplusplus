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

#include <ivfimage/IvfFileImage.h>

CIvfFileImage::CIvfFileImage()
{
	m_fileName = NULL;
}

CIvfFileImage::CIvfFileImage(const char* name)
{
	m_fileName = NULL;
	setFileName(name);
}

CIvfFileImage::~CIvfFileImage()
{
	delete m_fileName;
}

void CIvfFileImage::setFileName(const char *name)
{
	if (m_fileName!=NULL)
		delete [] m_fileName;

	m_fileName = new char[strlen(name)+1];
	strcpy(m_fileName, name);
}

const char* CIvfFileImage::getFileName()
{
	return m_fileName;
}

bool CIvfFileImage::read()
{
	return true;
}
