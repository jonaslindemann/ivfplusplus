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

#pragma once

#include <ivffile/FileBase.h>

#include <ivf/Shape.h>

namespace ivf {

IvfSmartPointer(CFileReader);

/**
 * Base class for a file reader
 *
 * This class implements basic functions for parsing
 * a text based 3d geometry file. It also serves as a
 * base class for file readers. File readers should 
 * override the read() method and assign imported 
 * geometry using the setShape() method in the CFileBase
 * class.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API CFileReader : public CFileBase {
private:
	char m_rawRow[256];
	double m_min[3];
	double m_max[3];
	double m_scaleFactor;
	bool m_reverseFace;
public:
	/** CFileReader constructor */
	CFileReader();

	/** CFileReader destructor */
	virtual ~CFileReader();

	IvfClassInfo("CFileReader",CFileBase);

	/** 
	 * Returns min/max size of read geometry 
	 * 
	 * For this method to work, any derived file loaders
	 * must use the maxMinCalc() and resetSize() when reading
	 * the file. (Not completely implemented yet...)
	 */
	void getSize(double* min, double* max);

	/** Set handling of face orientation (Not completely impl.) */
	void setReverseFace(bool flag);
	
	/** Get handling of face orientation (Not completely impl.) */
	bool getReverseFace();

	/** Set scaling of loaded model (Not completely impl.) */
	void setScaling(double scaleFactor);

	/** Get scaling of loaded model (Not completely impl.) */
	double getScaling();

	/** Return midpoint of loaded model */
	void getMidpoint(double &x, double &y, double &z);

	/** 
	 * Virtual method for reading the model 
	 *  
	 * This method is implemented in derived file loaders.
	 */
	virtual void read();

protected:
	void getLine(std::istream &in, std::string &row);
	bool findPos(const char* keyword, std::string &row, int &pos);
	bool find(const char* keyword, std::string &row);
	void getLine(std::string &row);
	void maxMinCalc(double x, double y, double z);
	void resetSize();
};

/** \example ac3dreader.cpp */

}