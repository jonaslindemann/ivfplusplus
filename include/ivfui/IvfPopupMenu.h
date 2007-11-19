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

#ifndef _CIvfPopupMenu_h_
#define _CIvfPopupMenu_h_

#include <ivf/ivfconfig.h>
#include <ivfdef/IvfDef.h>

#include <FL/Fl.H>
#include <FL/Fl_Menu_Button.H>

#include <map>

class IVFUI_API CIvfPopupMenu {
public:
	enum TMenuType {
		MT_NORMAL,
		MT_INACTIVE,
		MT_TOGGLE,
		MT_RADIO,
		MT_INVISIBLE
	};

	enum TActivationButton {
		AB_LEFT,
		AB_RIGHT,
		AB_MIDDLE
	};
private:
	Fl_Menu_Button* m_menu;
	std::map<void*,int> m_menuMap;
	int m_menuSize;
	bool m_menuEnabled;
	TActivationButton m_activationButton;
public:
	TActivationButton getActivationButton();
	CIvfPopupMenu();
	virtual ~CIvfPopupMenu();

	void popup();
	void addMenuItem(const char* label, const char* shortcut, TMenuType type, int id);
	void enablePopupMenu();
	void disablePopupMenu();
	bool isPopupMenuEnabled();
	void setActivationButton(TActivationButton activation);

	int getMenuId(const Fl_Menu_Item* menuItem);
	Fl_Menu_Button* getMenu();
	void setParent(Fl_Group* group);

	virtual void onMenu(int id, TMenuType type);

};

#endif 
