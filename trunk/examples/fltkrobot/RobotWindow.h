// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef RobotWindow_h
#define RobotWindow_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "RobotView.h"

class CRobotWindow {
public:
  CRobotWindow();
  Fl_Double_Window *wndMain;
  CRobotView *robotView;
  void show();
};
#endif
