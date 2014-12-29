

#pragma once



#include "FLTKU/AnimationFrameListener.h"
#include "FLTKU/AnimationListener.h"
#include "mathclass/vector.h"
#include "mathclass/quater.h"
#include "mathclass/transf.h"
#include <string>
#include <vector>

namespace mg
{

class AnimationViewer;


class Animation : public AnimationFrameListener
{
public:
	Animation()
	{
		flag_shadow_phase_ = false;
		fps_ = 40;
		cur_frame_ = 0;
		type_ = "Animation";
		name_ = "noname_";
		global_transf_.setRotation(math::quater(1, 0, 0, 0));
		global_transf_.setTranslation(math::vector(0, 0, 0));
	}

	virtual void Draw() { Draw(cur_frame_); }
	virtual void Draw(int frame)=0;
	virtual void Draw(int from, int to, int step);
	virtual void DrawForPicking() 
	{
		////// Example
		/*glInitNames();

		glPushName(1);
		drawBody();
		glPopName();

		glPushName(2);
		drawHead();
		drawEyes();
		glPopName();*/
	}


	/// AnimationFrameListener
	virtual void FrameChanged(int frame);
	

	virtual int CountFrames() const { return 1; }
	virtual void PlayOneFrame() { cur_frame_ = cur_frame_+1; }
	virtual void SeekFrame(int f);
	

	virtual void AddAnimationListener(AnimationListener* al) { ani_listeners_.push_back(al); }

	virtual void NotifyAnimationUpdated();
	virtual void AddAniViewer(AnimationViewer *a);
	virtual void RemoveAniViewer(AnimationViewer *a);
	virtual int HandleFLTK(AnimationViewer *w, int event) { return 0; }

	virtual math::transf global_transf() const { return global_transf_; }
	virtual int cur_frame() const { return cur_frame_; }

	virtual void name(const std::string &n) { name_ = n; }
	virtual std::string name() const { return name_; }
	virtual void fps(int f) { fps_ = f ; }
	virtual double fps() const { return fps_; }

	const std::string& type() const { return type_; }
	
protected:
	virtual void InitStencilForShadow();
	virtual void BeginShadow(double r=0.0, double g=0.0, double b=0.0, double alpha=0.5);
	virtual void EndShadow();

	

protected:
	double fps_;
	int cur_frame_;

	bool flag_shadow_phase_;

	std::string name_;

	std::vector<AnimationViewer*> ani_viewers_;
	std::vector<AnimationListener*> ani_listeners_;

	math::transf global_transf_;

	std::string type_;
};



class AnimationList : public std::vector<Animation*>
{
public:
	int GetMaxFrameNum() const
	{
		int max = 0;
		for ( int i=0; i<(int)size(); i++ )
		{
			int tmp = (*this)[i]->CountFrames();
			if ( max < tmp ) max = tmp;
		}

		return max;
	};

	int countAnimations() const
	{ 
		return (int)size(); 
	}
};

};