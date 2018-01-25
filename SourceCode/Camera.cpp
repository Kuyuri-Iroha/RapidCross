#include "Camera.h"
#include "Information.h"



// �R���X�g���N�^
Camera::Camera()
{
	//�r���[�|�[�g�̍쐬
	CWE::Graphic& manager =CWE::Graphic::GetInstance();
	mViewport.Width =static_cast<float>(manager.mWindows.GetWindowSizeX());
	mViewport.Height =static_cast<float>(manager.mWindows.GetWindowSizeY());
	mViewport.TopLeftX =0.0f;
	mViewport.TopLeftY =0.0f;
	mViewport.MinDepth =0.0f;
	mViewport.MaxDepth =1.0f;

	//�e����̃Z�b�g
	CWE::math::PerspectiveFovLHTp(&mProjection, CWE::math::ConvertToRadians(50.0f), 16.0f/9.0f, 1.0f, 1000.0f);
	
	mEye.PosReset(0.0f, 50.0f, -160.0f);
	mEye.FocusReset(0.0f, 20.0f, 0.0f);
	mEye.UpDirReset(0.0f, 1.0f);
}


// �f�X�g���N�^
Camera::~Camera()
{

}


// �X�V
void Camera::Update()
{

}


// ���_�ɃZ�b�g
void Camera::Set()
{
	Information& info =Information::GetInfo();

	CWE_ASSERT(info.mModelShader.GetCBData());

	//�r���[�ϊ�
	CWE::math::LookAtLHTp(&info.mModelShader.GetCBData()->mView, mEye.GetPos(), mEye.GetFocusPos(), mEye.GetUpDir());
	info.mModelShader.GetCBData()->mEyePos.x() =mEye.GetPos().x();
	info.mModelShader.GetCBData()->mEyePos.y() =mEye.GetPos().y();
	info.mModelShader.GetCBData()->mEyePos.z() =mEye.GetPos().z();
	info.mModelShader.GetCBData()->mEyePos.w() =0.0f;

	//�v���W�F�N�V�����ϊ�
	info.mModelShader.GetCBData()->mProjection =mProjection;

	Set2D();
}


// 2D���_�ɃZ�b�g
void Camera::Set2D()
{
	//�r���[�|�[�g�̃Z�b�g
	CWE::Graphic::GetInstance().DeviceContext().RSSetViewports(1, &mViewport);
}