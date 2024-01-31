#include "Animation.h"

void Animation::AddPositionKeyFrame(const glm::vec3& position, double time, EasingType easeType)
{
	positionKeyFrameList.push_back(PositionKeyFrame(position, time, easeType));
}

void Animation::AddRotationKeyFrame(const glm::quat& rotation, double time, EasingType easeType)
{
	rotationKeyFrameList.push_back(RotationKeyFrame(rotation, time, easeType));
}

void Animation::AddScaleKeyFrame(const glm::vec3& scale, double time, EasingType easeType)
{
	scaleKeyFrameList.push_back(ScaleKeyFrame(scale, time, easeType));
}

void Animation::SetAnimationState(const AnimationState& animationState)
{
	currentAnimationState = animationState;
}

AnimationState Animation::GetCurrentAnimationState()
{
	return currentAnimationState;
}

double Animation::GetTotalAnimationTime()
{

	return 0.0;
}
