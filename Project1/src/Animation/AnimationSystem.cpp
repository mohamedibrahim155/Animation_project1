#include "AnimationSystem.h"
#include <glm/gtx/easing.hpp>
#include "../model.h"

AnimationSystem& AnimationSystem::GetInstance()
{
	static AnimationSystem instance;
	return instance;
}

void AnimationSystem::Update(float deltaTime)
{
	if (isAnimationPause) return;


	for (Entity* entity : listOfEntitiesToAnimate)
	{
		if (entity->animation!=nullptr)
		{
			Animation* animation = entity->animation;

			animation->time += deltaTime;

			if (animation->GetCurrentAnimationState() == AnimationState::COMPLETE)
			{
				// Replays the animation in a loop
			//	animation->time = 0;
				//animation->SetAnimationState(AnimationState::NONE);
			}
			
			double time = animation->time;

			//Position
			{

				if (animation->positionKeyFrameList.size() == 1)
				{
					animation->SetAnimationState(AnimationState::NONE);
					entity->transform.position = animation->positionKeyFrameList[0].position;
				}
				else if(animation->positionKeyFrameList.size() > 1)
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


           #pragma region Rotation

			//Rotation
			/*{
				if (animation->rotationKeyFrameList.size() == 1)
				{
					animation->SetAnimationState(AnimationState::NONE);
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
			}*/

           #pragma endregion

           #pragma region Scale

			//Scale
			/*{
				if (animation->scaleKeyFrameList.size() == 1)
				{
					animation->SetAnimationState(AnimationState::NONE);
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
			}*/

            #pragma endregion

             #pragma region Color
			{
				if (animation->colorKeyFrameList.size() == 1)
				{
					animation->SetAnimationState(AnimationState::NONE);
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

		}
	}

}

void AnimationSystem::AddAnimationEntity(Entity* entity)
{

	listOfEntitiesToAnimate.push_back(entity);
}

void AnimationSystem::AddAnimation(Animation* animation)
{
	animationSequenceList.push_back(animation);
}

void AnimationSystem::RewindAnimation()
{
	for (Animation* animation : animationSequenceList)
	{
		std::vector<PositionKeyFrame> positionKeyFrame = animation->positionKeyFrameList;

		animation->positionKeyFrameList.clear();

		for (int  i = positionKeyFrame.size()-1; i >= 0; i--)
		{
			PositionKeyFrame index = positionKeyFrame[i];
			animation->AddPositionKeyFrame(index.position, index.time, index.easeType);
		}



	}
}

