
#pragma once

namespace mg
{

class AnimationFrameListener
{
public:
	virtual void FrameChanged(int f)=0;
	virtual void FrameReset()
	{
		FrameChanged(0);
	}

	virtual void PlayStarted(int f){};
	virtual void PlayStopped(int f){};
};

};

