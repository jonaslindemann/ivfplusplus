// Joystick.cpp: implementation of the CJoystick class.
//
//////////////////////////////////////////////////////////////////////

#include "Joystick.h"

#include <iostream>

#include <dinput.h>

CJoystick::CJoystick()
{
#ifdef WIN32
	m_joyCaps = new JOYCAPS;
	m_joyInfo = new JOYINFOEX;
	m_inputDevice = ID_JOYSTICK1;


	MMRESULT mmResult;
	
	switch (m_inputDevice) {
	case ID_JOYSTICK1:
		mmResult = joyGetDevCaps(JOYSTICKID1, m_joyCaps, sizeof(*m_joyCaps));
		break;
	case ID_JOYSTICK2:
		mmResult = joyGetDevCaps(JOYSTICKID2, m_joyCaps, sizeof(*m_joyCaps));
		break;
	default:

		break;
	}

	switch (mmResult) {
	case MMSYSERR_NODRIVER:
		m_status = JS_NO_DRIVER;
		break;
	case MMSYSERR_INVALPARAM:
		m_status = JS_INVALID_PARAM;
		break;
	default:
		m_status = JS_OK;
		break;
	}

	m_joyInfo->dwSize = sizeof(*m_joyInfo);
	m_joyInfo->dwFlags = JOY_RETURNALL|JOY_USEDEADZONE;

	mmResult = joyGetPosEx(JOYSTICKID1, m_joyInfo);

	switch (mmResult) {
	case MMSYSERR_NODRIVER:
		m_status = JS_NO_DRIVER;
		break;
	case MMSYSERR_INVALPARAM:
		m_status = JS_INVALID_PARAM;
		break;
	case MMSYSERR_BADDEVICEID:
		m_status = JS_BAD_DEVICE_ID;
		break;
	case JOYERR_UNPLUGGED:
		m_status = JS_UNPLUGGED;
	default:
		m_status = JS_OK;
		break;
	}

#endif
}

CJoystick::~CJoystick()
{

}

void CJoystick::initJoystick()
{
}

TJoystickStatus CJoystick::getStatus()
{
	return m_status;
}

void CJoystick::poll()
{
	MMRESULT mmResult;
	JOYINFOEX ji;


	ZeroMemory(&ji, sizeof(JOYINFOEX));
	ji.dwSize = sizeof(JOYINFOEX);
	ji.dwFlags = JOY_RETURNALL | JOY_USEDEADZONE;
	mmResult = joyGetPosEx(JOYSTICKID1, &ji);
	
	m_joyYpos = ji.dwYpos;
	std::cout << m_joyYpos << ", " << ji.dwYpos << std::endl;

	switch (mmResult) {
	case MMSYSERR_NODRIVER:
		m_status = JS_NO_DRIVER;
		break;
	case MMSYSERR_INVALPARAM:
		m_status = JS_INVALID_PARAM;
		break;
	case MMSYSERR_BADDEVICEID:
		m_status = JS_BAD_DEVICE_ID;
		break;
	case JOYERR_UNPLUGGED:
		m_status = JS_UNPLUGGED;
	default:
		m_status = JS_OK;
		break;
	}
}

double CJoystick::getX()
{
	std::cout << m_joyInfo->dwXpos << std::endl;
	if (m_status == JS_OK)
		return -(1.0 - 2.0*((double)m_joyInfo->dwXpos/(double)m_joyCaps->wXmax));
	else
		return 0.0;
}

double CJoystick::getY()
{
	if (m_status == JS_OK)
		return 1.0 - 2.0*((double)m_joyYpos/(double)m_joyCaps->wYmax);
	else
		return 0.0;
}

double CJoystick::getZ()
{
	if (m_status == JS_OK)
		return 1.0 - (double)m_joyInfo->dwZpos/(double)m_joyCaps->wZmax;
	else
		return 0.0;
}

double CJoystick::getR()
{
	if (m_status == JS_OK)
		return 1.0 - 2.0*((double)m_joyInfo->dwRpos/(double)m_joyCaps->wRmax);
	else
		return 0.0;
}

int CJoystick::getButton()
{
	if (m_status == JS_OK)
		return m_joyInfo->dwButtons;
	else
		return -1;
}

