
#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Light_Button.H>

#include "FLTKU/AnimationViewer.h"
#include "FLTKU/AnimationFrameController.h"
#include "FLTKU/AnimationBrowser.h"
#include "FLTKU/AnimationControlBox.h"

namespace mg
{

class AnimationApp : public Fl_Window
{
public:
	AnimationApp(const char *s=0);

	virtual void Init();

	AnimationViewer *ani_viewer_;
	AnimationBrowser *ani_browser_;
	AnimationFrameController *ani_frame_control_;
	AnimationControlBox *ani_control_box_;

	Fl_Light_Button *align_btn_;
};


};