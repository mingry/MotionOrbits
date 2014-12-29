

#include "FLTKU/AnimationFrameController.h"

#include <algorithm>

namespace mg
{

AnimationFrameController::AnimationFrameController(int x, int y, int w, int h, const char *s) 
	: Fl_Window(x, y, w, h, s)
{
	play_mode_ = REPEAT;
	flag_play_ = false;

	reset_btn_ = new Fl_Button(0, 0, 30, h, "@<|");
	reset_btn_->box(FL_SHADOW_BOX);
	reset_btn_->down_box(FL_FLAT_BOX);
	reset_btn_->callback((Fl_Callback*)StaticResetBtnCB, (void*)(this));

	play_btn_ = new Fl_Button(30, 0, 30, h, "@>");
	play_btn_->box(FL_SHADOW_BOX);
	play_btn_->down_box(FL_FLAT_BOX);
	play_btn_->callback((Fl_Callback*)StaticPlayBtnCB, (void*)(this));

	frame_slider_ = new Fl_Value_Slider(60, 0, w-60, h);
	frame_slider_->type(5);
	frame_slider_->box(FL_SHADOW_BOX);
	frame_slider_->maximum(100);
	frame_slider_->step(1);
	frame_slider_->callback((Fl_Callback*)StaticFrameSliderCB, (void*)(this));

	resizable(frame_slider_);

	end();
}


void
AnimationFrameController::SeekFrame(int f)
{
	if ( play_mode_ == TO_INFINITE && frame_slider_->maximum() < f )
	{
		frame_slider_->maximum(f);
	}
	frame_slider_->value(f);
	NotifyFrameChanged();
}



void 
AnimationFrameController::BrowserSelectionsChanged(const AnimationList& ani_list)
{
	static AnimationList cur_ani_list;
	
	// remove priviouse animations in the frame_listeners list. 
	if ( cur_ani_list.size() > 0 )
	{
		for ( int i=0; i<(int)cur_ani_list.size(); i++ )
		{
			std::vector<AnimationFrameListener*>::iterator iter 
				= std::find(frame_listeners_.begin(), frame_listeners_.end(), cur_ani_list[i]);

			if ( iter != frame_listeners_.end() )
				frame_listeners_.erase(iter);
		}
	}

	// set new cur_ani_list.
	cur_ani_list.assign(ani_list.cbegin(), ani_list.cend());


	// add new animations to the frame_listeners list
	for ( int i=0; i<(int)cur_ani_list.size(); i++ )
	{
		frame_listeners_.push_back(cur_ani_list[i]);
	}



	// update slider size and frame.
	int total_frame_num = 0;

	if ( ani_list.countAnimations() == 0 )
	{
		total_frame_num = 0;
		time_delay_ = 1;
	}
	else
	{

		total_frame_num = ani_list.GetMaxFrameNum();
		time_delay_ = 1.0/ani_list.front()->fps();
	}

	frame_slider_->step(1);
	frame_slider_->minimum(0);
	frame_slider_->maximum(total_frame_num-1);

	if ( !ani_list.empty() )
	{
		//frame_slider->value(ani_list.front()->getCurFrame());
		SeekFrame(ani_list.front()->cur_frame());
	}

}

void 
AnimationFrameController::BrowserItemsChanged(const AnimationList& ani_list)
{
}


void 
AnimationFrameController::AnimationChanged(const Animation *ani)
{
	if ( ani==0 ) return;
	if ( frame_slider_->maximum() < ani->CountFrames() )
	{
		frame_slider_->maximum(ani->CountFrames());
	}
}


void
AnimationFrameController::NotifyFrameReset()
{
	for ( int i=0; i<(int)frame_listeners_.size(); i++ )
	{
		frame_listeners_[i]->FrameReset();
	}
}


void
AnimationFrameController::NotifyFrameChanged()
{
	int f = (int)frame_slider_->value();

	for ( int i=0; i<(int)frame_listeners_.size(); i++ )
	{
		frame_listeners_[i]->FrameChanged(f);
	}
}

void
AnimationFrameController::NotifyPlayStarted()
{
	int f = (int)frame_slider_->value();

	for ( int i=0; i<(int)frame_listeners_.size(); i++ )
	{
		frame_listeners_[i]->PlayStarted(f);
	}
}


void
AnimationFrameController::NotifyPlayStopped()
{
	int f = (int)frame_slider_->value();

	for ( int i=0; i<(int)frame_listeners_.size(); i++ )
	{
		frame_listeners_[i]->PlayStopped(f);
	}
}








//////////////////////////////////////////////////////////////
// events

void
AnimationFrameController::StaticResetBtnCB(Fl_Widget *w, void *d)
{
	((AnimationFrameController*)d)->ResetBtnCB();

}
void
AnimationFrameController::StaticPlayBtnCB(Fl_Widget *w, void *d)
{
	((AnimationFrameController*)d)->PlayBtnCB();
}

void
AnimationFrameController::StaticFrameSliderCB(Fl_Widget *w, void *d)
{
	((AnimationFrameController*)d)->FrameSliderCB();
}

void
AnimationFrameController::ResetBtnCB()
{
	flag_play_ = false;
	play_btn_->label("@>");
	frame_slider_->value(0);
	NotifyFrameReset();
}

void
AnimationFrameController::PlayBtnCB()
{
	PlayToggle();
}

void
AnimationFrameController::FrameSliderCB()
{
	NotifyFrameChanged();
}



void
AnimationFrameController::PlayToggle()
{
	if ( flag_play_ )
	{
		flag_play_ = false;
		play_btn_->label("@>");
		NotifyPlayStopped();
	}
	else
	{
		flag_play_ = true;
		play_btn_->label("||");
		Fl::add_timeout(time_delay_, StaticTimerCB, this);
		NotifyPlayStarted();
	}
}

void
AnimationFrameController::StaticTimerCB(void *d)
{
	((AnimationFrameController*)d)->TimerCB();
}

void
AnimationFrameController::TimerCB()
{
	if ( flag_play_ )
	{
		Fl::repeat_timeout(time_delay_, StaticTimerCB, this);
		
		if ( frame_slider_->value() >= frame_slider_->maximum() )
		{
			switch ( play_mode_ )
			{
			case ONE_TIME:
				PlayToggle();
				break;
			case ONE_TIME_AND_REWIND:
				SeekFrame(0);
				PlayToggle();
				break;
			case REPEAT:
				SeekFrame(0);
				break;
			case TO_INFINITE:
				SeekFrame((int)frame_slider_->value()+1);
				break;
			};
			
		}
		else
		{
			SeekFrame((int)frame_slider_->value()+1);
		}
	}
}

};