#pragma once


#include "../CWE/CWELib.h"




// �J�����N���X
class Camera
{
private:
	CD3D11_VIEWPORT mViewport;
	CWE::math::Float4x4 mProjection;

public:
	CWE::Eye mEye;

	Camera();
	~Camera();

	void Update();
	void Set();
	void Set2D();

};