#include "AnimationSystem.h"
#include "../model.h"

AnimationSystem& AnimationSystem::GetInstance()
{
	static AnimationSystem instance;
	return instance;
}



void AnimationSystem::UpdateAnimationSequence(float deltaTime)
{
	if (!isAnimationPlaying) return;

	if (currentSequence == nullptr) return;

	animationSystemTime = deltaTime * animationSpeed;

	currentSequence->UpdateCurrentSequenceTime(animationSystemTime);
}

void AnimationSystem::ToggleRewind()
{
	isRewind = !isRewind;

	currentSequence->SetModeState(isRewind ? REWIND : NORMAL);
}



void AnimationSystem::TogglePlayOrPause()
{
	isAnimationPlaying = !isAnimationPlaying;
}

void AnimationSystem::AddAnimationSequence(AnimationSequence* animation)
{
	animationSequences.push_back(animation);
}

void AnimationSystem::RemoveAnimationSequence(AnimationSequence* animation)
{
	std::vector<AnimationSequence*>::iterator removeIterator = std::remove(animationSequences.begin(), animationSequences.end(), animation);

	animationSequences.erase(removeIterator, animationSequences.end());
}

void AnimationSystem::SetSequence(AnimationSequence* sequence)
{
	currentSequence = sequence;

	//for (size_t i = 0; i < animationSequences.size(); i++)
	//{
	//	if (animationSequences[i] == sequence)
	//	{
	//		currentSequenceIndex = i;
	//		return;
	//	}
	//}


	//animationSequences.push_back(sequence);

	//currentSequenceIndex = animationSequences.size() - 1;

}

void AnimationSystem::SetAnimationSpeed(int speed)
{
	if (speed > 0)
	{
		animationSpeed = speed;
	}
	else if (speed <=  0)
	{
		animationSpeed = 1;
	}
}

void AnimationSystem::NextSequence()
{
	currentSequenceIndex++;

	currentSequence->ResetTime();

	currentSequence->SetVisibilityRenderers(false);

	if (currentSequenceIndex >= animationSequences.size())
	{
		currentSequenceIndex = 0;
	}

	SetSequence(animationSequences[currentSequenceIndex]);

	currentSequence->SetVisibilityRenderers(true);

}

