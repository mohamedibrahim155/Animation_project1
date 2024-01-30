#include "Animation.h"

void Animation::AddPositionKeyFrame(const glm::vec3& position, double time, EasingType easeType)
{
	positionKeyFrameList.push_back(PositionKeyFrame(position, time, easeType));
}

void Animation::AddRotationKeyFrame(const glm::vec3& rotation, double time, EasingType easeType)
{
	rotationKeyFrameList.push_back(RotationKeyFrame(rotation, time, easeType));
}

void Animation::AddScaleKeyFrame(const glm::vec3& scale, double time, EasingType easeType)
{
	scaleKeyFrameList.push_back(ScaleKeyFrame(scale, time, easeType));
}