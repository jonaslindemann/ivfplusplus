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

#ifndef _CIvfSingletonDestroyer_h_
#define _CIvfSingletonDestroyer_h_

//
// The following code based on code from the article
//
// To Kill a Singleton
// John Vlissides
//
// John Vlissides is a member of the research staff at IBM's
// Thomas J. Watson Research Center in Hawthorne, New York.
// He can be reached at vlis@watson.ibm.com
//

/**
 * Singleton destroyer template class
 *
 * This class can handle the destruction of singletons
 * in an orderly fashion.
 */
template <class T>
class CIvfSingletonDestroyer {
public:
	/** Class constructor */
	CIvfSingletonDestroyer(T* = 0);

	/** Class destructor */
	~CIvfSingletonDestroyer();

	void setSingleton(T*);
private:
	// Prevent users from making copies of a
	// CIvfSingletonDestroyer to avoid double deletion:
	CIvfSingletonDestroyer(const CIvfSingletonDestroyer<T>&);
	CIvfSingletonDestroyer<T>& operator=(const CIvfSingletonDestroyer<T>&);
private:
	T* _T;
};

template <class T>
CIvfSingletonDestroyer<T>::CIvfSingletonDestroyer (T* d) {
	_T = d;
}

template <class T>
CIvfSingletonDestroyer<T>::~CIvfSingletonDestroyer () {
	delete _T;
}

template <class T>
void CIvfSingletonDestroyer<T>::setSingleton (T* d) {
	_T = d;
}

#endif
