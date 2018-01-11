// generated by Fast Light User Interface Designer (fluid) version 1.0400

#include "DiagramFrame.h"
static int x; 

CDiagramFrame::CDiagramFrame() {
  { wndDiagram = new Fl_Double_Window(411, 300, "Stress");
    wndDiagram->user_data((void*)(this));
    { chartDiagram = new Fl_Chart(5, 5, 400, 290);
      chartDiagram->box(FL_NO_BOX);
      chartDiagram->color(FL_BACKGROUND_COLOR);
      chartDiagram->selection_color(FL_BACKGROUND_COLOR);
      chartDiagram->labeltype(FL_NORMAL_LABEL);
      chartDiagram->labelfont(0);
      chartDiagram->labelsize(14);
      chartDiagram->labelcolor(FL_FOREGROUND_COLOR);
      chartDiagram->align(Fl_Align(FL_ALIGN_CENTER));
      chartDiagram->when(FL_WHEN_RELEASE);
    } // Fl_Chart* chartDiagram
    wndDiagram->set_non_modal();
    wndDiagram->end();
    wndDiagram->resizable(wndDiagram);
  } // Fl_Double_Window* wndDiagram
}

void CDiagramFrame::show() {
  wndDiagram->show();
}

void CDiagramFrame::hide() {
  wndDiagram->hide();
}
