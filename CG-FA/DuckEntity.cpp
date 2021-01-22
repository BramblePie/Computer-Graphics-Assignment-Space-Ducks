#include "DuckEntity.h"

#include "objloader.h"

DuckEntity::DuckEntity()
	: BaseEntity(R"(C:\Users\bramp\Desktop\duck\duck.obj)")
{
}

void DuckEntity::draw()
{
	material.Bind();
	glBindVertexArray(vao->ID);
	glDrawArrays(GL_TRIANGLES, 0, vao->VertexCount);
}