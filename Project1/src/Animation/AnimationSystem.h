#pragma once

#include "../EntityManager/Entity.h"
#include <vector>
#include "Animation.h"

class AnimationSystem
{
public :

	static AnimationSystem& GetInstance();

	void Update(float deltaTime);

	void AddAnimationEntity(Entity* entity);
	void AddAnimation(Animation* animation);

	void RewindAnimation();

	std::vector<Entity*> listOfEntitiesToAnimate;
	std::vector<Animation*> animationSequenceList;

	bool isAnimationPause = false;

	double animationTimeElapsed = 0;

};

