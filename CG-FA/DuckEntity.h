#pragma once

#include "Entities.h"

class DuckEntity : public BaseEntity
{
public:
	DuckEntity();
	~DuckEntity() = default;

private:

	// Inherited via BaseEntity
	virtual void Draw() override;
};