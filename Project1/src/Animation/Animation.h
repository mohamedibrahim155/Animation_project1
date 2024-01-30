#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

enum class EasingType
{
	Linear,
	sineEaseIn,
	sineEaseOut,
	sineEaseInOut
};


struct PositionKeyFrame
{
	PositionKeyFrame(const glm::vec3& _position, double _time, EasingType _easeType = EasingType::Linear) :
		position(_position), 
		time(_time), 
		easeType(_easeType)
	{
	}

	glm::vec3 position;
	EasingType easeType;
	double time;

};



struct RotationKeyFrame
{
	RotationKeyFrame(const glm::vec3& _rotation, double _time, EasingType _easeType = EasingType::Linear) :
		rotation(_rotation),
		time(_time),
		easeType(_easeType)
	{
	}

	glm::vec3 rotation;
	EasingType easeType;
	double time;

};



struct ScaleKeyFrame
{
	ScaleKeyFrame(const glm::vec3& _scale, double _time, EasingType _easeType = EasingType::Linear) :
		scale(_scale),
		time(_time),
		easeType(_easeType)
	{
	}

	glm::vec3 scale;
	EasingType easeType;
	double time;

};

struct EventKeyFrame
{
	EventKeyFrame(double _time) : time(_time)	
	{
	} 

	double time;
};



class Animation 
{
public:

	void AddPositionKeyFrame(const glm::vec3& position, double time, EasingType easeType = EasingType::Linear);
	void AddRotationKeyFrame(const glm::vec3& rotaion, double time, EasingType easeType = EasingType::Linear);
	void AddScaleKeyFrame(const glm::vec3& scale, double time, EasingType easeType = EasingType::Linear);

	std::vector<PositionKeyFrame> positionKeyFrameList;
	std::vector<RotationKeyFrame> rotationKeyFrameList;
	std::vector<ScaleKeyFrame> scaleKeyFrameList;

	double time;

	bool isPaused = false;

};

