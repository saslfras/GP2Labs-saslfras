//
//  Component.cpp
//  GP2BaseCode
//
//  Created by Brian on 30/10/2014.
//  Copyright (c) 2014 Glasgow Caledonian University. All rights reserved.
//

#include "Component.h"

Component::Component()
{
    m_Type="Component";
    m_Active=true;
}

Component::~Component()
{
    
}

void Component::init()
{
    
}

void Component::update()
{
    
}

void Component::render()
{
    
}

void Component::destroy()
{
    
}

const std::string& Component::getType()
{
    return m_Type;
}

bool Component::isActive()
{
    return m_Active;
}

void Component::toggleActive()
{
    m_Active=!m_Active;
}

void Component::setParent(GameObject *object)
{
    m_Parent=object;
}
