// generated by Fast Light User Interface Designer (fluid) version 1.0400

#include "SplashFrame.h"
static int x;

void cbHide(void* w) {
  Fl_Window* window = (Fl_Window*)w;
  window->hide();
}

void SplashFrame::cb_btnClose_i(Fl_Button*, void*) {
  wndSplash->hide();
}
void SplashFrame::cb_btnClose(Fl_Button* o, void* v) {
  ((SplashFrame*)(o->parent()->user_data()))->cb_btnClose_i(o,v);
}

SplashFrame::SplashFrame() {
  { wndSplash = new Fl_Double_Window(401, 301);
    wndSplash->user_data((void*)(this));
    { boxImage = new Fl_Box(0, 0, 400, 300);
      boxImage->box(FL_THIN_UP_BOX);
    } // Fl_Box* boxImage
    { btnClose = new Fl_Button(335, 265, 55, 25, "Close");
      btnClose->box(FL_THIN_UP_FRAME);
      btnClose->down_box(FL_THIN_DOWN_FRAME);
      btnClose->labelsize(12);
      btnClose->callback((Fl_Callback*)cb_btnClose);
    } // Fl_Button* btnClose
    wndSplash->clear_border();
    wndSplash->end();
  } // Fl_Double_Window* wndSplash
}

void SplashFrame::show() {
  if (m_timer)
  	Fl::add_timeout(4.0,cbHide,(void*)wndSplash);

  wndSplash->show();
}

void SplashFrame::center() {
  wndSplash->position(Fl::w()/2-wndSplash->w()/2,Fl::h()/2-wndSplash->h()/2);
}

void SplashFrame::setTimer(bool flag) {
  m_timer = flag;
}

void SplashFrame::hide() {
  wndSplash->hide();
}
