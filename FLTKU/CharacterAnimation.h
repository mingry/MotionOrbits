

#pragma once


#include <string>
#include "BaseLib/FLTKU/AnimationFrameListener.h"
#include "BaseLib/FLTKU/Animation.h"
#include "BaseLib/Geometry/PrimitiveShape.h"
#include "BaseLib/PMU/PmGL.h"
#include "PmQm/pm.h"

namespace mg
{

class AnimationViewer;



class CharacterAnimation : public Animation
{
public:
	CharacterAnimation();
	CharacterAnimation(PmLinearMotion *motion, std::string name="");
	virtual ~CharacterAnimation();

	virtual void motion(PmLinearMotion *m);
	virtual PmLinearMotion* motion() { return motion_; }

	virtual math::transf global_transf() const;


	virtual int CountFrames() const;
	virtual void Draw(int frame);
	virtual void Draw(int from, int to, int step=10);

protected:
	virtual void DrawPosturePolygon(int frame);
	virtual void DrawPosturePolygon(PmPosture const &pose);
	virtual void DrawPostureLine(int frame);

protected:

	mg::PmBoneGLListInterface *pm_gl_model_;
	PmLinearMotion *motion_;
};




class MultiCharacterAni : public Animation
{
public:
	MultiCharacterAni();
	virtual ~MultiCharacterAni();

	virtual void AddMotion(PmLinearMotion *motion, std::string name="");
	virtual void RemoveAllMotion();

	PmLinearMotion* motion(int i) { return motions_[i]; }
	std::vector<PmLinearMotion *>& motions() { return motions_; }
	virtual math::transf global_transf() const;


	virtual int CountFrames() const;
	virtual void Draw(int frame);
	virtual void Draw(int from, int to, int step=10);

protected:
	virtual void DrawPosturePolygon(int motion_id, int frame);
	virtual void DrawPostureLine(int motion_id, int frame);

protected:

	std::vector<PmLinearMotion *> motions_;
	std::vector<mg::PmBoneGLListInterface*> pm_gl_models_;
	std::vector<std::string> motion_names_;

};


};