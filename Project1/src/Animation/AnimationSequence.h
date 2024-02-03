#pragma once

#include "Animation.h"

class AnimationSystem;

enum Mode
{
	NORMAL = 0,
	REWIND = 1
};


class AnimationSequence
{

public:
	AnimationSequence();

	void AddAnimationClip(Animation* animationClip, Model*  model);
	void RemoveAnimationClip(Animation* animationClip);

	void SetAnimationSequenceTime(float time);

	void CalculateSequenceTotalTime(Model* model);

	void UpdateCurrentSequenceTime(float deltaTime);

	float GetTotalTimeFrame();
	std::vector<Animation*> GetAnimationList();



	void SetCurrentAnimation(int index);

	void SetCurrentAnimation(Animation* animation);

	void ResetTime();

	Mode SequenceMode = Mode::NORMAL;

private:
	std::map<Model*,Animation*> animationClipsWithObjectsList;
	float sequenceTotalTime = 0;

	float sequenceDeltaTime = 0;

	int currentIndex = 0;




};

