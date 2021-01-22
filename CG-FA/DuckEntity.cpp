#include "DuckEntity.h"

#include "objloader.h"

DuckEntity::DuckEntity()
	: BaseEntity(R"(C:\Users\bramp\Desktop\duck\duck.obj)", init_material()), material(default_mat)
{
	material->color = { 0.9f, 0.2f, 0.8f };
}

const BaseMaterial* DuckEntity::init_material()
{
	if (default_mat == 0)
		default_mat = std::make_shared<DuckMaterial>();
	material = default_mat;

	return material.get();
}

void DuckEntity::draw()
{
	material->Bind();
	glBindVertexArray(vao->ID);
	glDrawArrays(GL_TRIANGLES, 0, vao->VertexCount);
}