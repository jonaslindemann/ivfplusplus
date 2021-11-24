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
//
// Written by Jonas Lindemann
//

#pragma once

namespace ivf {

/**
 * Smart pointer class
 *
 * Pointer handles the Ivf++ reference counting scheme of
 * the CIvfBase class. To declare a Ivf++ smart pointer use the 
 * IvfSmartPointer() macro. See the following example:
 *
 * \code
 * int main()
 * {
 *    Pointer<Material> material = new Material(); // addReference() called.
 *    Pointer<Material> material2;
 *    material2 = material; // addReference() called 
 *    .
 *    .
 *    
 *    return 0;
 * } 
 * // material calls deleteReference()
 * // material2 calls deleteRefernce() and deletes Material object
 * \endcode
 */
template <class T> class Pointer {
private:
	T* m_object;
public:
	Pointer(T* object = 0)
	{
		m_object = object;
		if (m_object)
		{
			IvfDbg1("Pointer: created for " << m_object->getClassName());
			m_object->addReference();
		}
		else
		{
			IvfDbg1("Pointer: assigned 0");
		}
	}

	Pointer(const Pointer& ivfObject)
	{
		m_object = ivfObject.m_object;
		if (m_object)
		{
			IvfDbg1("Pointer: assigned " << m_object->getClassName());
			m_object->addReference();
		}
		else
		{
			IvfDbg1("Pointer: assigned 0");
		}
	}

	virtual ~Pointer()
	{
		if (m_object)
		{
			m_object->deleteReference();
			IvfDbg1("Pointer: " << m_object->getClassName() << " dereferenced.");
			if (!m_object->referenced())
			{
				delete m_object;
				IvfDbg1("Pointer: " << m_object->getClassName() << " deleted.");
			}
		}
	}

	operator T* () const { return m_object; }
	T& operator* () const { return *m_object; }
	T* operator-> () const { return m_object; }

	Pointer& operator= (const Pointer& ivfPointer)
	{
		if (m_object!=ivfPointer.m_object)
		{
			if (m_object)
			{
				m_object->deleteReference();
				IvfDbg1("Pointer(=): " << m_object->getClassName() << " dereferenced.");
				if (!m_object->referenced())
				{
					delete m_object;
					IvfDbg1("Pointer(=): " << m_object->getClassName() << " deleted.");
				}
			}

			m_object = ivfPointer.m_object;

			if (m_object)
			{
				IvfDbg1("Pointer(=): assigned " << m_object->getClassName());
				m_object->addReference();
			}
			else
			{
				IvfDbg1("Pointer(=): assigned 0");
			}
		}
		return *this;
	}

	Pointer& operator= (T* ivfObject)
	{
		if (m_object!=ivfObject)
		{
			if (m_object)
			{
				m_object->deleteReference();
				IvfDbg1("Pointer(=): " << m_object->getClassName() << " dereferenced.");
				if (!m_object->referenced())
				{
					delete m_object;
					IvfDbg1("Pointer(=): " << m_object->getClassName() << " deleted.");
				}
			}

			m_object = ivfObject;

			if (m_object)
			{
				IvfDbg1("Pointer(=): assigned " << m_object->getClassName());
				m_object->addReference();
			}
			else
			{
				IvfDbg1("Pointer(=): assigned 0");
			}
		}
		return *this;
	}

	bool operator== (T* ivfObject) const { return m_object == ivfObject; }
	bool operator!= (T* ivfObject) const { return m_object != ivfObject; }
	bool operator== (const Pointer& ivfPointer) const 
	{
		return m_object == ivfPointer.m_object;
	}

	bool operator!= (const Pointer& ivfPointer) const
	{
		return m_object != ivfPointer.m_object;
	}
};

}