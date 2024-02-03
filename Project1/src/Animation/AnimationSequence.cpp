#include "AnimationSequence.h"
#include "AnimationSystem.h"

AnimationSequence::AnimationSequence()
{
	AnimationSystem::GetInstance().AddAnimationSequence(this);
}

void AnimationSequence::AddAnimationClip(Animation* animationClip, Model* model)
{
	animationClipsWithObjectsList[model] = animationClip;

	CalculateSequenceTotalTime(model);
}

void AnimationSequence::RemoveAnimationClip(Animation* animationClip)
{
	//std::vector<Animation*>::iterator removeIterator = std::remove(animationClipsWithObjectsList.begin(), animationClipsWithObjectsList.end(), animationClip);

//	animationClipsWithObjectsList.erase(removeIterator, animationClipsWithObjectsList.end());
}

void AnimationSequence::SetAnimationSequenceTime(float time)
{
	sequenceTotalTime = time;
}

void AnimationSequence::CalculateSequenceTotalTime(Model* model)
{
	Animation * animation = animationClipsWithObjectsList[model];
	
		for (PositionKeyFrame position : animation->positionKeyFrameList)
		{
			if (position.time >= sequenceTotalTime)
			{
				sequenceTotalTime = position.time;
			}
			
		}

		for (RotationKeyFrame rotation : animation->rotationKeyFrameList)
		{
			if (rotation.time >= sequenceTotalTime)
			{
				sequenceTotalTime = rotation.time;
			}

		}
		for (ScaleKeyFrame scale : animation->scaleKeyFrameList)
		{
			if (scale.time >= sequenceTotalTime)
			{
				sequenceTotalTime = scale.time;
			}

		}
		for (ColorKeyFrame color : animation->colorKeyFrameList)
		{
			if (color.time >= sequenceTotalTime)
			{
				sequenceTotalTime = color.time;
			}

		}

	
}

void AnimationSequence::UpdateCurrentSequenceTime(float deltaTime)
{
	sequenceDeltaTime += deltaTime;

	if (sequenceDeltaTime >  sequenceTotalTime)
	{
		sequenceDeltaTime = sequenceTotalTime;
	}

	std::map<Model*, Animation*>::iterator it;

	for (it = animationClipsWithObjectsList.begin(); it !=  animationClipsWithObjectsList.end(); it++)
	{
		Model* model = it->first;     //Model 

		Animation* clip = it->second; // Animation clip

		clip->time = sequenceDeltaTime;

		double time = clip->time;

		//Position
		{

			if (clip->positionKeyFrameList.size() == 1)
			{
				clip->SetAnimationState(AnimationState::NONE);
				model->transform.position = clip->positionKeyFrameList[0].position;
			}
			else if (clip->positionKeyFrameList.size() > 1)
			{
				int keyFrameEndIndex = 0;

				for (; keyFrameEndIndex < clip->positionKeyFrameList.size(); keyFrameEndIndex++)
				{
					if (clip->positionKeyFrameList[keyFrameEndIndex].time > time)
					{
						break;

					}
				}

				if (keyFrameEndIndex >= clip->positionKeyFrameList.size())
				{

					model->transform.position = clip->positionKeyFrameList[keyFrameEndIndex - 1].position;

					return;
				}
				int keyFrameStartIndex = keyFrameEndIndex - 1;

				PositionKeyFrame startKeyFrame = clip->positionKeyFrameList[keyFrameStartIndex];
				PositionKeyFrame endKeyFrame = clip->positionKeyFrameList[keyFrameEndIndex];

				float percent = (time - startKeyFrame.time) / (endKeyFrame.time - startKeyFrame.time);
				float result = 0.0f;


				switch (endKeyFrame.easeType)
				{
				case EasingType::Linear:
					result = percent;
					break;

				case EasingType::sineEaseIn:
					result = glm::sineEaseIn(percent);
					break;

				case EasingType::sineEaseOut:
					result = glm::sineEaseOut(percent);
					break;

				case EasingType::sineEaseInOut:
					result = glm::sineEaseInOut(percent);
					break;
				}

				glm::vec3 delta = (endKeyFrame.position - startKeyFrame.position);

				model->transform.SetPosition(startKeyFrame.position + delta * result);

			}
		}

	}
	
}

float AnimationSequence::GetTotalTimeFrame()
{
	return sequenceTotalTime;
}




void AnimationSequence::SetCurrentAnimation(int index)
{
	currentIndex = index;
}

void AnimationSequence::SetCurrentAnimation(Animation* animation)
{


	std::map<Model*, Animation* >::iterator it;

	
	int index = 0;
	for (it = animationClipsWithObjectsList.begin(); it != animationClipsWithObjectsList.end(); it++)
	{
		if (it->second == animation)
		{
			currentIndex = index;
			return;
		}

		index++;
	}

}

void AnimationSequence::ResetTime()
{
	switch (SequenceMode)
	{
	case NORMAL:

		sequenceDeltaTime = 0;

		break;
	case REWIND:
		sequenceDeltaTime = sequenceTotalTime;
		break;
	}
}
