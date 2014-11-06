//
//  Mesh.cpp
//  GP2BaseCode
//
//  Created by Brian on 31/10/2014.
//  Copyright (c) 2014 Glasgow Caledonian University. All rights reserved.
//

#include "Mesh.h"
#include "Vertex.h"

Mesh::Mesh()
{
    m_VertexCount=0;
    m_IndexCount=0;
    m_VBO=0;
    m_EBO=0;
    m_VAO=0;
    m_Type="Mesh";
}

Mesh::~Mesh()
{
    
}

void Mesh::init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	//Create buffer
	glGenBuffers(1, &m_VBO);
	// Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//create buffer
	glGenBuffers(1, &m_EBO);
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    //Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3) + sizeof(vec2)));

}

void Mesh::destroy()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::bind()
{
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

}

void Mesh::copyVertexData(int count,int stride,void ** data)
{
	m_VertexCount = count;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, count * stride, data, GL_STATIC_DRAW);
}

void Mesh::copyIndexData(int count,int stride,void ** data)
{
	m_IndexCount = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * stride, data, GL_STATIC_DRAW);
}

int Mesh::getVertexCount()
{
    return m_VertexCount;
}

int Mesh::getIndexCount()
{
    return m_IndexCount;
}
