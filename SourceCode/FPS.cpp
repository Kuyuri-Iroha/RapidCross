#include "FPS.h"
#include "Information.h"
#include <cmath>



// �R���X�g���N�^
FPS::FPS():
	TaskBase(INT_MIN, TaskName::FPS),
	m2DMagnifX(1.0f /float(CWE_DEFAULT_WINDOW_SIZE_X)), m2DMagnifY(1.0f /float(CWE_DEFAULT_WINDOW_SIZE_Y)),
	mTime(1.0)
{
	float offsetX =-944*m2DMagnifX,
		  offsetY =524*m2DMagnifY;
	mPos.x() =offsetX;
	mPos.y() =offsetY;
}


// �f�X�g���N�^
FPS::~FPS()
{
	End();
}


// ���[�h�̊�����ʒm
bool FPS::LoadResult() const
{
	return Information::GetInfo().DebugCharsLoaded();
}


// FPS���Z�o
void FPS::Calculation()
{
	if(mTimer.GetStarted())
	{
		mTimer.Update();
		mTimer.GetTime(mTime); //�o�ߎ��Ԃ��擾
	}

	if(mTime < 1.0)
	{
		++mPassingFrameCnt;
	}
	else
	{
		mFPS =mPassingFrameCnt;
		mPassingFrameCnt =0;
		mTimer.Start();
	}
}


// �X�V
void FPS::Update(TaskExec::List& taskList)
{
	if(LoadResult())
	{
		mDraw =true;

		Calculation();
	}
	else
	{
		mDraw =false;
	}
}


// �`��
void FPS::Draw()
{
	using namespace CWE;
	Information& info =Information::GetInfo();
	Graphic& manager =Graphic::GetInstance();

	//�V�F�[�_�̃Z�b�g
	info.mUIShader.Begin();

	//�萔�o�b�t�@�̗L����
	info.mUIShader.EnableConstantBuffer();

	info.mUIShader.UpdateProjection();

	//FPS�̌�����`��
	int index;
	float adding =0;
	manager.OMSetDepthBuffer(false);
	for(int i=int(log10f(float(mFPS)))+1; 0<i; --i)
	{
		index =mFPS;

		//�`�悷�錅�̐����Z�o
		for(int j=i-1; 0<j; --j) {index /=10;}
		index %=10;

		//�萔�o�b�t�@�̍X�V
		mPos.x() -=adding;
		math::World2DTp(&info.mUIShader.GetCBData()->mWorld, mPos);
		mPos.x() +=adding;
		info.mUIShader.mCB.Map();
		CopyMemory(info.mUIShader.mCB.Access(), info.mUIShader.GetCBData(), sizeof(*info.mUIShader.GetCBData()));
		info.mUIShader.mCB.Unmap();

		//�`��
		info.DrawDebugChar(index);

		//���̌��̈ʒu���Z�o
		adding -=mPos.x()+0.501f;
	}
	manager.OMSetDepthBuffer(true);
}


void FPS::End()
{
	Information::GetInfo().mUIShader.DisableConstantBuffer();
}