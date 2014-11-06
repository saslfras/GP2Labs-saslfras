//
//  Camera.h
//  GP2BaseCode
//
//  Created by Brian on 31/10/2014.
//  Copyright (c) 2014 Glasgow Caledonian University. All rights reserved.
//

#ifndef Camera_h
#define Camera_h


#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

class Camera :public Component
{
public:
	Camera();
	~Camera();

	void update();

	void setLook(float x, float y, float z);

	void setUp(float x, float y, float z);

	void setNearClip(float near);
	void setFarClip(float far);
	void setFOV(float FOV);

	void setAspectRatio(float ratio);

	mat4& getView();
	mat4& getProjection();
protected:
private:
	vec3 m_LookAt;
	vec3 m_Up;

	mat4 m_View;
	mat4 m_Projection;

	float m_NearClip;
	float m_FarClip;
	float m_FOV;
	float m_AspectRatio;
};

#endif