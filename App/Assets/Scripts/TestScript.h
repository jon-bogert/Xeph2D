#pragma once

#include <Xeph2D.h>

class TestScript : public Xeph2D::Component
{
private:
	XEPH2D_REG_COMP(0xa45ff78b);

	void Start() override;
	void Update() override;
	void OnDestroy() override;
};