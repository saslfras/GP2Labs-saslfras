//
//  Component.h
//  GP2BaseCode
//
//  Created by Brian on 30/10/2014.
//  Copyright (c) 2014 Glasgow Caledonian University. All rights reserved.
//

#ifndef Component_h
#define Component_h

#include <string>

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void init();

	virtual void update();

	virtual void render();

	virtual void destroy();

	const std::string& getType();

	bool isActive();

	void toggleActive();

	void setParent(GameObject * object);
protected:
	std::string m_Type;
	bool m_Active;
	GameObject * m_Parent;
private:
};

#endif