
#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Slider.H>

#ifdef WIN32
#include <windows.h>
#endif
#include <vector>

#include "FLTKU/AnimationFrameListener.h"
#include "FLTKU/AnimationBrowserListener.h"
#include "FLTKU/AnimationListener.h"

namespace mg
{

class AnimationFrameController : public Fl_Window, public AnimationBrowserListener, public AnimationListener
{
public:
	enum PlayMode {ONE_TIME, ONE_TIME_AND_REWIND, REPEAT, TO_INFINITE};

	AnimationFrameController(int x, int y, int w, int h, const char *s=0);

	// play
	virtual void PlayToggle();
	virtual void SeekFrame(int f);
	virtual bool flag_play() const { return flag_play_; }
	virtual int cur_frame() const { return (int)frame_slider_->value(); }
	void play_mode(PlayMode pm) { play_mode_ = pm; }
	PlayMode play_mode() const { return play_mode_; }

	virtual void AddAniFrameListener(AnimationFrameListener* a) { frame_listeners_.push_back(a); }

	// from AnimationBrowserListener
	virtual void BrowserSelectionsChanged(const AnimationList& ani_list);
	virtual void BrowserItemsChanged(const AnimationList& ani_list);

	// from AnimationListener
	virtual void AnimationChanged(const Animation *ani);

	
protected:
	static void StaticResetBtnCB(Fl_Widget *w, void *d);
	static void StaticPlayBtnCB(Fl_Widget *w, void *d);
	static void StaticFrameSliderCB(Fl_Widget *w, void *d);
	void ResetBtnCB();
	void PlayBtnCB();
	void FrameSliderCB();

	void NotifyFrameReset();
	void NotifyFrameChanged();
	void NotifyPlayStarted();
	void NotifyPlayStopped();

	static void StaticTimerCB(void *d);
	void TimerCB();


protected:
	bool flag_play_;
	PlayMode play_mode_;

	double time_delay_;

	Fl_Button *play_btn_;
	Fl_Button *reset_btn_;
	Fl_Value_Slider *frame_slider_;

	std::vector<AnimationFrameListener*> frame_listeners_;
};

};


