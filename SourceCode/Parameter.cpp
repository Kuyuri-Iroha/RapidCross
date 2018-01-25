#include "Parameter.h"
#include "Information.h"

using namespace CWE;

Parameter::~Parameter()
{
	End();
}


bool Parameter::LoadSucceeded() const
{
	if(mMask.LoadResult() == Task::Failed ||
		mShape.LoadResult() == Task::Failed)
	{
		FatalError::GetInstance().Outbreak(L"Failed to load data.\nMay file is corrupted.");
		return false;
	}

	return (mMask.LoadResult() == Task::Success &&
		mShape.LoadResult() == Task::Success);
}


// �쐬
void Parameter::Create(const wchar_t* maskPath, const wchar_t* shapePath)
{
	//�}�X�N
	mMask.Load(maskPath, FileFormat::Picture);

	//�`��
	mShape.Load(shapePath, FileFormat::PMX);
}


// �`��
void Parameter::Draw()
{
	Graphic& gm =Graphic::GetInstance();
	bool suc =false;
	auto mesh =mMask.GetMeshData(&suc);

	gm.PSSetShaderResources(1, 1, &mesh[0].mpTexture);
	mShape.Draw(true);
}


// �I��
void Parameter::End()
{
	mShape.Release();
	mMask.Release();
}