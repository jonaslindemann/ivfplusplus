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

#ifndef _CIvfCulledComposite_h_
#define _CIvfCulledComposite_h_

#include <ivf/IvfComposite.h>
#include <ivf/IvfView.h>

#include <ivfmath/IvfMatrixStack.h>
#include <ivfmath/IvfViewFrustum.h>

IvfSmartPointer(CIvfCulledComposite);

/** Obsolete use CIvfCulling instead */
class IVF_API CIvfCulledComposite : public CIvfComposite {
private:
	CIvfMatrixStack* m_matrixStack;
	CIvfViewFrustum* m_frustum;
	CIvfView* m_cullView;
	int m_cullCount;
	bool m_useCulling;

	void cullChildren(CIvfShape* shape);
	bool intersectFrustum(CIvfBoundingSphere *bSphere);
public:
	CIvfCulledComposite();
	virtual ~CIvfCulledComposite();

	IvfClassInfo("CIvfCulledComposite",CIvfComposite);

	void addChild(CIvfShape* shape);

	void setUseCulling(bool flag);
	bool getUseCulling();
	
	void setCullView(CIvfView* view);
	CIvfView* getCullView();

	int getCullCount();

protected:
	virtual void cull();
	virtual void createGeometry();
};

#endif 
