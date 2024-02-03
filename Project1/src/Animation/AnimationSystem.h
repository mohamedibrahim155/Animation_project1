#pragma once

#include "../EntityManager/Entity.h"
#include "Animation.h"
#include "AnimationSequence.h"

class Model;
class AnimationSystem
{
public :

	static AnimationSystem& GetInstance();

	void Update(float deltaTime);
	void UpdateAnimationSequence(float deltaTime);

	void AddAnimationEntity(Entity* entity);
	void AddAnimation(Animation* animation);

	void RewindAnimation();
	void PlayOrPause();

	std::vector<Entity*> listOfEntitiesToAnimate;
	std::vector<Animation*> animationClipList;

	void AddAnimationSequence(AnimationSequence* animation);
	void RemoveAnimationSequence(AnimationSequence* animation);
	void SetSequence(AnimationSequence* sequence);
	void SetAnimationSpeed(int speed);

	void NextSequence();

	AnimationSequence* currentSequence = nullptr;
	int currentSequenceIndex = 0;

	std::vector<AnimationSequence*> animationSequences;

	bool isAnimationPause = true;

	float animationSystemTime = 0;
	float animationSpeed = 1;

};

