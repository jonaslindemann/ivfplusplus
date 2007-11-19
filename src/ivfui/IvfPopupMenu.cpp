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

#include <ivfui/IvfPopupMenu.h>

#include <FL/Fl_Group.H>

static void menu_cb(Fl_Widget* w, void* p) 
{
	CIvfPopupMenu* menu = (CIvfPopupMenu*)p;
	Fl_Menu_Button* flMenu = menu->getMenu();
	const struct Fl_Menu_Item* item = flMenu->mvalue();
	int id = menu->getMenuId(item);
	menu->onMenu(id, CIvfPopupMenu::MT_NORMAL);
}


CIvfPopupMenu::CIvfPopupMenu()
{
	m_menu = new Fl_Menu_Button(0,0,300,300,"Ivf++");
	m_menu->type(Fl_Menu_Button::POPUP3);
	m_menuSize = 0;
	m_menuEnabled = false;
}

CIvfPopupMenu::~CIvfPopupMenu()
{
	delete m_menu;
}

void CIvfPopupMenu::popup()
{
	if (m_menuEnabled)
	{
		m_menu->popup();
		m_menu->value(0);
	}
}

void CIvfPopupMenu::setParent(Fl_Group *group)
{
	group->add(m_menu);
}

void CIvfPopupMenu::addMenuItem(const char *label, const char *shortcut, TMenuType type, int id)
{
	m_menu->add(label, shortcut, (Fl_Callback*)menu_cb, (void*)this, 0);
	const Fl_Menu_Item* menuItems = m_menu->menu();
	m_menuMap[(void*)&menuItems[m_menuSize++]] = id;
	m_menuEnabled = true;
}

Fl_Menu_Button* CIvfPopupMenu::getMenu()
{
	return m_menu;
}

void CIvfPopupMenu::onMenu(int id, TMenuType type)
{

}

int CIvfPopupMenu::getMenuId(const Fl_Menu_Item *menuItem)
{
	return m_menuMap[(void*)menuItem];
}

void CIvfPopupMenu::enablePopupMenu()
{
	m_menuEnabled = true;
}

void CIvfPopupMenu::disablePopupMenu()
{
	m_menuEnabled = false;
}

bool CIvfPopupMenu::isPopupMenuEnabled()
{
	return m_menuEnabled;
}

void CIvfPopupMenu::setActivationButton(TActivationButton activation)
{
	m_activationButton = activation;
	switch (m_activationButton) {
	case AB_LEFT:
		m_menu->type(Fl_Menu_Button::POPUP1);
		break;
	case AB_MIDDLE:
		m_menu->type(Fl_Menu_Button::POPUP2);
		break;
	case AB_RIGHT:
		m_menu->type(Fl_Menu_Button::POPUP3);
		break;
	default:
		m_menu->type(Fl_Menu_Button::POPUP3);
		break;
	}
}

CIvfPopupMenu::TActivationButton CIvfPopupMenu::getActivationButton()
{
	return m_activationButton;
}
