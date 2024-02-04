#include "AnimationSequence.h"
#include "AnimationSystem.h"

AnimationSequence::AnimationSequence()
{
	AnimationSystem::GetInstance().AddAnimationSequence(this);
}

AnimationSequence::~AnimationSequence()
{
	AnimationSystem::GetInstance().RemoveAnimationSequence(this);
}

void AnimationSequence::AddAnimationClip(Animation* animationClip, Model* model)
{
	animationClipsWithObjectsList[model] = animationClip;

	CalculateSequenceTotalTime(animationClip);
}

void AnimationSequence::RemoveAnimationClip(Animation* animationClip)
{
    std::map<Model*, Animation*>::iterator findIterator;

    for (findIterator = animationClipsWithObjectsList.begin(); findIterator != animationClipsWithObjectsList.end(); ++findIterator)
    {
        if (findIterator->second == animationClip)
        {
           
            findIterator->second = nullptr;

			delete  findIterator->second;
           
            animationClipsWithObjectsList.erase(findIterator);
            
            return;
        }
    }
}

void AnimationSequence::SetAnimationSequenceTime(float time)
{
	sequenceTotalTime = time;
}

void AnimationSequence::CalculateSequenceTotalTime(Animation* animation)
{
	
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

	switch (SequenceMode)
	{
	case NORMAL:

		currentTime += deltaTime;

		
		break;
	case REWIND:

		currentTime -= deltaTime;

		
		break;

	}
	
	if (currentTime < 0)
	{
		currentTime = 0;
	}

	if (currentTime > sequenceTotalTime)
	{
		currentTime = sequenceTotalTime;
	}


	
	for (clipsIterator = animationClipsWithObjectsList.begin(); clipsIterator !=  animationClipsWithObjectsList.end(); clipsIterator++)
	{
		Model* entity = clipsIterator->first;     //Model 

		Animation* animation = clipsIterator->second; // Animation clip

		animation->time = currentTime;

		double time = animation->time;

		//Position
		{

			if (animation->positionKeyFrameList.size() == 1)
			{
				
				entity->transform.position = animation->positionKeyFrameList[0].position;
			}
			else if (animation->positionKeyFrameList.size() > 1)
			{
				int keyFrameEndIndex = 0;

				for (; keyFrameEndIndex < animation->positionKeyFrameList.size(); keyFrameEndIndex++)
				{
					if (animation->positionKeyFrameList[keyFrameEndIndex].time > time)
					{
						break;

					}
				}

				if (keyFrameEndIndex >= animation->positionKeyFrameList.size())
				{

					entity->transform.position = animation->positionKeyFrameList[keyFrameEndIndex - 1].position;

					return;
				}
				int keyFrameStartIndex = keyFrameEndIndex - 1;

				PositionKeyFrame startKeyFrame = animation->positionKeyFrameList[keyFrameStartIndex];
				PositionKeyFrame endKeyFrame = animation->positionKeyFrameList[keyFrameEndIndex];

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

				entity->transform.SetPosition(startKeyFrame.position + delta * result);

			}
		}

		//Rotation
		{
			if (animation->rotationKeyFrameList.size() == 1)
			{
				
				entity->transform.SetQuatRotation(animation->rotationKeyFrameList[0].rotation);
			}
			else if (animation->rotationKeyFrameList.size() > 1)
			{
				int keyFrameEndIndex = 0;

				for (; keyFrameEndIndex < animation->rotationKeyFrameList.size(); keyFrameEndIndex++)
				{
					if (animation->rotationKeyFrameList[keyFrameEndIndex].time > time)
					{
						break;

					}
				}

				if (keyFrameEndIndex >= animation->rotationKeyFrameList.size())
				{
					
					entity->transform.SetQuatRotation(animation->rotationKeyFrameList[keyFrameEndIndex - 1].rotation);

					return;
				}
				int keyFrameStartIndex = keyFrameEndIndex - 1;

				RotationKeyFrame startKeyFrame = animation->rotationKeyFrameList[keyFrameStartIndex];
				RotationKeyFrame endKeyFrame = animation->rotationKeyFrameList[keyFrameEndIndex];

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

				entity->transform.SetQuatRotation(glm::slerp(startKeyFrame.rotation, endKeyFrame.rotation, result));

			}
		}



            #pragma region SCALE

		//Scale
		{
			if (animation->scaleKeyFrameList.size() == 1)
			{
			
				entity->transform.SetScale(animation->scaleKeyFrameList[0].scale);
			}
			else if (animation->scaleKeyFrameList.size() > 1)
			{
				int keyFrameEndIndex = 0;

				for (; keyFrameEndIndex < animation->scaleKeyFrameList.size(); keyFrameEndIndex++)
				{
					if (animation->scaleKeyFrameList[keyFrameEndIndex].time > time)
					{
						break;

					}
				}

				if (keyFrameEndIndex >= animation->scaleKeyFrameList.size())
				{
					entity->transform.SetScale(animation->scaleKeyFrameList[keyFrameEndIndex - 1].scale);
					return;
				}
				int keyFrameStartIndex = keyFrameEndIndex - 1;

				ScaleKeyFrame startKeyFrame = animation->scaleKeyFrameList[keyFrameStartIndex];
				ScaleKeyFrame endKeyFrame = animation->scaleKeyFrameList[keyFrameEndIndex];

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

				glm::vec3 delta = (endKeyFrame.scale - startKeyFrame.scale);

				entity->transform.SetScale(startKeyFrame.scale + delta * result);

			}
		}

#pragma endregion

             #pragma region Color
			{
				if (animation->colorKeyFrameList.size() == 1)
				{
					if (Model* modelEntity = dynamic_cast<Model*>(entity))
					{
						modelEntity->meshes[0]->meshMaterial->material()->SetBaseColor
						(glm::vec4(animation->colorKeyFrameList[0].color,1));
					}
				}
				else if (animation->colorKeyFrameList.size() > 1)
				{
					int keyFrameEndIndex = 0;

					for (; keyFrameEndIndex < animation->colorKeyFrameList.size(); keyFrameEndIndex++)
					{
						if (animation->colorKeyFrameList[keyFrameEndIndex].time > time)
						{
							break;

						}
					}

					if (keyFrameEndIndex >= animation->colorKeyFrameList.size())
					{
						if (Model* modelEntity = dynamic_cast<Model*>(entity))
						{
							modelEntity->meshes[0]->meshMaterial->material()->SetBaseColor
							(glm::vec4(animation->colorKeyFrameList[keyFrameEndIndex - 1].color, 1));
						}
					
						return;
					}
					int keyFrameStartIndex = keyFrameEndIndex - 1;

					ColorKeyFrame startKeyFrame = animation->colorKeyFrameList[keyFrameStartIndex];
					ColorKeyFrame endKeyFrame = animation->colorKeyFrameList[keyFrameEndIndex];

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

					glm::vec3 delta = (endKeyFrame.color - startKeyFrame.color);


					if (Model* modelEntity = dynamic_cast<Model*>(entity))
					{
						modelEntity->meshes[0]->meshMaterial->material()->SetBaseColor
						(glm::vec4(startKeyFrame.color + delta * result, 1));
					}

				}
			}

             #pragma endregion


#pragma region Event
			{
				if (animation->eventKeyFrameList.size() == 1)
				{
					if (!animation->eventKeyFrameList[0].isEventTriggered)
					{
						animation->eventKeyFrameList[0].callBack();
						animation->eventKeyFrameList[0].isEventTriggered = true;

					}

				}
				else if (animation->eventKeyFrameList.size() > 1)
				{
					int keyFrameEndIndex = 0;

					for (; keyFrameEndIndex < animation->eventKeyFrameList.size(); keyFrameEndIndex++)
					{
						if (animation->eventKeyFrameList[keyFrameEndIndex].time > time)
						{
							break;

						}
					}

					if (keyFrameEndIndex >= animation->eventKeyFrameList.size())
					{
					

						return;
					}
					int keyFrameStartIndex = keyFrameEndIndex - 1;

					EventKeyFrame startKeyFrame = animation->eventKeyFrameList[keyFrameStartIndex];
					EventKeyFrame endKeyFrame = animation->eventKeyFrameList[keyFrameEndIndex];

					float percent = (time - startKeyFrame.time) / (endKeyFrame.time - startKeyFrame.time);
					float result = 0.0f;


				}
			}

#pragma endregion


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

void AnimationSequence::SetModeState(const Mode& sequenceMode)
{
	this->SequenceMode = sequenceMode;
}



void AnimationSequence::ResetTime()
{
	switch (SequenceMode)
	{
	case NORMAL:
		currentTime = 0;
		break;
	case REWIND:
		currentTime = sequenceTotalTime;
	//	SequenceMode = NORMAL;
		break;
	}

	
}

void AnimationSequence::ResetPositions()
{
	int index = 0;

	
	switch (SequenceMode)
	{
	case NORMAL:
	/*	for (std::pair<Model*, Animation*> animationObject : animationClipsWithObjectsList)
		{
			if (animationObject.second->positionKeyFrameList.size() >= 1)
			{
				animationObject.first->transform.SetPosition(animationObject.second->positionKeyFrameList[0].position);
			}
			if (animationObject.second->rotationKeyFrameList.size() >= 1);
			{
				animationObject.first->transform.SetQuatRotation(animationObject.second->rotationKeyFrameList[0].rotation);

			}

			if (animationObject.second->scaleKeyFrameList.size() >= 1)
			{
				animationObject.first->transform.SetScale(animationObject.second->scaleKeyFrameList[0].scale);

			}

			if (animationObject.second->colorKeyFrameList.size() >= 1)
			{
				animationObject.first->meshes[0]->meshMaterial->material()->
					SetBaseColor(glm::vec4(animationObject.second->colorKeyFrameList[0].color, 1));
			}
		}*/
		break;
	case REWIND:
		//for (std::pair<Model*, Animation*> animationObject : animationClipsWithObjectsList)
		//{
		//	if (animationObject.second->positionKeyFrameList.size() >= 1)
		//	{
		//		animationObject.first->transform.SetPosition
		//		(animationObject.second->positionKeyFrameList[animationObject.second->positionKeyFrameList.size() - 1].position);
		//	}
		//	if (animationObject.second->rotationKeyFrameList.size() >= 1)
		//	{
		//		animationObject.first->transform.SetQuatRotation
		//		(animationObject.second->rotationKeyFrameList[animationObject.second->rotationKeyFrameList.size() - 1].rotation);

		//	}

		//	if (animationObject.second->scaleKeyFrameList.size() >= 1)
		//	{
		//		animationObject.first->transform.SetScale
		//		(animationObject.second->scaleKeyFrameList[animationObject.second->scaleKeyFrameList.size() - 1].scale);

		//	}

		//	if (animationObject.second->colorKeyFrameList.size() >= 1)
		//	{
		//		animationObject.first->meshes[0]->meshMaterial->material()->
		//			SetBaseColor(glm::vec4(animationObject.second->colorKeyFrameList[animationObject.second->colorKeyFrameList.size() - 1].color, 1));
		//	}
		//}
		


		for (const std::pair<Model*, Animation*>& animationObject : animationClipsWithObjectsList)
		{
			Animation& animation = *animationObject.second;
			Transform& transform = animationObject.first->transform;

			const std::vector<PositionKeyFrame>& posKeyFrames = animation.positionKeyFrameList;
			const std::vector<RotationKeyFrame>& rotKeyFrames = animation.rotationKeyFrameList;
			const std::vector<ScaleKeyFrame>& scaleKeyFrames = animation.scaleKeyFrameList;
			const std::vector<ColorKeyFrame>& colorKeyFrames = animation.colorKeyFrameList;

			if (!posKeyFrames.empty())
				transform.SetPosition(posKeyFrames[0].position);

			if (!rotKeyFrames.empty())
				transform.SetQuatRotation(rotKeyFrames[0].rotation);

			if (!scaleKeyFrames.empty())
				transform.SetScale(scaleKeyFrames[0].scale);

			if (!colorKeyFrames.empty())
			{
				const glm::vec3& color = colorKeyFrames[0].color;
				animationObject.first->meshes[0]->meshMaterial->material()->
					SetBaseColor(glm::vec4(color, 1));
			}
		}
		break;
	}

}


void AnimationSequence::SetVisibilityRenderers(bool isActive)
{
	for (std::pair<Model*, Animation* > animationObject : animationClipsWithObjectsList)
	{
		animationObject.first->isVisible = isActive;
	}
}
