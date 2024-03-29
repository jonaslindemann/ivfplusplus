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

#ifndef _CIvfPointer_h_
#define _CIvfPointer_h_

#include <ivf/config.h>
#include <ivfdef/Def.h>

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
template <class T,class R> class CPointerRefBase {
private:
	T* m_object;
public:
	CPointerRefBase(T* object = 0)
	{
		m_object = object;
		if (m_object)
		{
			IvfDbg1("CIvfPointerRefBase: created for " << m_object->getClassName());
			m_object->R::addReference();
		}
		else
		{
			IvfDbg1("CIvfPointerRefBase: assigned 0");
		}
	}

	CPointerRefBase(const CPointerRefBase& ivfObject)
	{
		m_object = ivfObject.m_object;
		if (m_object)
		{
			IvfDbg1("CIvfPointerRefBase: assigned " << m_object->getClassName());
			m_object->R::addReference();
		}
		else
		{
			IvfDbg1("CIvfPointerRefBase: assigned 0");
		}
	}

	virtual ~CPointerRefBase()
	{
		if (m_object)
		{
			m_object->R::deleteReference();
			IvfDbg1("CIvfPointerRefBase: " << m_object->getClassName() << " dereferenced.");
			if (!m_object->R::referenced())
			{
				IvfDbg1("CIvfPointerRefBase: " << m_object->getClassName() << " deleted.");
				delete m_object;
			}
		}
	}

	operator T* () const { return m_object; }
	T& operator* () const { return *m_object; }
	T* operator-> () const { return m_object; }

	CPointerRefBase& operator= (const CPointerRefBase& IvfPointerRefBase)
	{
		if (m_object!=IvfPointerRefBase.m_object)
		{
			if (m_object)
			{
				m_object->R::deleteReference();
				IvfDbg1("CIvfPointerRefBase(=): " << m_object->getClassName() << " dereferenced.");
				if (!m_object->R::referenced())
				{
					IvfDbg1("CIvfPointerRefBase(=): " << m_object->getClassName() << " deleted.");
					delete m_object;
				}
			}

			m_object = IvfPointerRefBase.m_object;

			if (m_object)
			{
				IvfDbg1("CIvfPointerRefBase(=): assigned " << m_object->getClassName());
				m_object->R::addReference();
			}
			else
			{
				IvfDbg1("CIvfPointerRefBase(=): assigned 0");
			}
		}
		return *this;
	}

	CPointerRefBase& operator= (T* ivfObject)
	{
		if (m_object!=ivfObject)
		{
			if (m_object)
			{
				m_object->R::deleteReference();
				IvfDbg1("CIvfPointerRefBase(=): " << m_object->getClassName() << " dereferenced.");
				if (!m_object->R::referenced())
				{
					IvfDbg1("CIvfPointerRefBase(=): " << m_object->getClassName() << " deleted.");
					delete m_object;
				}
			}

			m_object = ivfObject;

			if (m_object)
			{
				IvfDbg1("CIvfPointerRefBase(=): assigned " << m_object->getClassName());
				m_object->R::addReference();
			}
			else
			{
				IvfDbg1("CIvfPointerRefBase(=): assigned 0");
			}
		}
		return *this;
	}

	bool operator== (T* ivfObject) const { return m_object == ivfObject; }
	bool operator!= (T* ivfObject) const { return m_object != ivfObject; }
	bool operator== (const CPointerRefBase& IvfPointerRefBase) const
	{
		return m_object == IvfPointerRefBase.m_object;
	}

	bool operator!= (const CPointerRefBase& IvfPointerRefBase) const
	{
		return m_object != IvfPointerRefBase.m_object;
	}
};

template <class T> class CPointer {
private:
	T* m_object;
public:
	CPointer(T* object = 0)
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

	CPointer(const CPointer& ivfObject)
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

	virtual ~CPointer()
	{
		if (m_object)
		{
			m_object->deleteReference();
			IvfDbg1("Pointer: " << m_object->getClassName() << " dereferenced.");
			if (!m_object->referenced())
			{
				IvfDbg1("Pointer: " << m_object->getClassName() << " deleted.");
				delete m_object;
			}
		}
	}

	operator T* () const { return m_object; }
	T& operator* () const { return *m_object; }
	T* operator-> () const { return m_object; }

	CPointer& operator= (const CPointer& ivfPointer)
	{
		if (m_object!=ivfPointer.m_object)
		{
			if (m_object)
			{
				m_object->deleteReference();
				IvfDbg1("Pointer(=): " << m_object->getClassName() << " dereferenced.");
				if (!m_object->referenced())
				{
					IvfDbg1("Pointer(=): " << m_object->getClassName() << " deleted.");
					delete m_object;
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

	CPointer& operator= (T* ivfObject)
	{
		if (m_object!=ivfObject)
		{
			if (m_object)
			{
				m_object->deleteReference();
				IvfDbg1("Pointer(=): " << m_object->getClassName() << " dereferenced.");
				if (!m_object->referenced())
				{
					IvfDbg1("Pointer(=): " << m_object->getClassName() << " deleted.");
					delete m_object;
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
	bool operator== (const CPointer& ivfPointer) const 
	{
		return m_object == ivfPointer.m_object;
	}

	bool operator!= (const CPointer& ivfPointer) const
	{
		return m_object != ivfPointer.m_object;
	}
};


#endif 
