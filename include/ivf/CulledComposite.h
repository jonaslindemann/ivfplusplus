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

#include <ivf/Composite.h>
#include <ivf/View.h>

#include <ivfmath/MatrixStack.h>
#include <ivfmath/ViewFrustum.h>

namespace ivf {

IvfSmartPointer(CCulledComposite);

/** Obsolete use CIvfCulling instead */
class IVF_API CCulledComposite : public CComposite {
private:
	CMatrixStack* m_matrixStack;
	CViewFrustum* m_frustum;
	CView* m_cullView;
	int m_cullCount;
	bool m_useCulling;

	void cullChildren(CShape* shape);
	bool intersectFrustum(CBoundingSphere *bSphere);
public:
	CCulledComposite();
	virtual ~CCulledComposite();

	IvfClassInfo("CIvfCulledComposite",CComposite);

	void addChild(CShape* shape);

	void setUseCulling(bool flag);
	bool getUseCulling();
	
	void setCullView(CView* view);
	CView* getCullView();

	int getCullCount();

protected:
	virtual void cull();
	virtual void doCreateGeometry();
};

}