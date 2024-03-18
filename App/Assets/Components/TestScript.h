#pragma once

#include <Xeph2D.h>

class TestScript : public Xeph2D::Component
{
	XEPH2D_REG_COMP(0xa45ff78b);
private:
	void Serializables() override;

	void Start() override;
	void Update() override;

};