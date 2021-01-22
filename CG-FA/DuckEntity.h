#pragma once

#include "BaseEntity.h"

class DuckEntity : public BaseEntity
{
public:
	DuckEntity();
	~DuckEntity() = default;

private:

	// Inherited via BaseEntity
	virtual void draw() override;
};