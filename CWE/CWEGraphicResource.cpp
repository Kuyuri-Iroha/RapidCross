#include "CWEGraphicResource.h"
#include "CWEGraphicManager.h"
#include "CWEPolygon.h"
#include <algorithm>


namespace CWE
{


// �R���X�g���N�^
Resource::Resource():
	mGraphicM(Graphic::GetInstance()), mSoundM(MasterSound::GetInstance()),
	mpWorker(0), mHandleNumber(INT_MIN)
{
}

// �f�X�g���N�^
Resource::~Resource()
{
	End();
}

// �C���X�^���X�̎擾
Resource& Resource::GetInstance()
{
	static Resource instance;
	return instance;
}


// �I��
void Resource::End()
{
	mSounds.clear();
	mMotions.clear();
	mPolygons.clear();

	// �X���b�h�̌㏈��
	if(mpWorker)
	{
		mpWorker->join();
		SAFE_DELETE(mpWorker);
	}
}


// ���\�[�X�̃��[�h
void Resource::LoadExecution(bool ending)
{
	if(!mTaskBuffer.empty())
	{
		// �S�̂��I������Ƃ�
		if(ending)
		{
			mTaskBuffer.clear();
			return;
		}

		if(!mpWorker)
		{
			// ���[�J�[�X���b�h�𗧂��グ��
			mpWorker =new std::thread(CreateAll, std::ref(*this));
		}
	}
	else
	{
		if(mpWorker)
		{
			mpWorker->detach();
			SAFE_DELETE(mpWorker);
		}
	}
}


// ���\�[�X�̐���
ResourceHandle Resource::Create(const wchar_t *filePath, const FileFormat& format)
{
	ResourceHandle handle;
	//�n���h���̐���
	handle.mHandle =mHandleNumber;
	++mHandleNumber;
	//4,294,967,295�ȏ�̃��\�[�X�𐶐�����ƈ��S�����ۏႳ��Ȃ��Ȃ�

	switch(format)
	{
	// �|���S���t�@�C��
	case FileFormat::PMX:
	case FileFormat::Picture:
		mMutex.lock();
		mPolygons[handle.mHandle].mFilePath =filePath;
		mMutex.unlock();
		mPolygons.at(handle.mHandle).mFileFormat =format;

		LoadRequest(handle, format);

		++mPolygons.at(handle.mHandle).mReferenceCount;
		break;

	// �A�j���[�V�����t�@�C��
	case FileFormat::VMD:
		mMutex.lock();
		mMotions[handle.mHandle].mFilePath =filePath;
		mMutex.unlock();
		mMotions.at(handle.mHandle).mFileFormat =format;

		LoadRequest(handle, format);

		++mMotions.at(handle.mHandle).mReferenceCount;
		break;

	// �����t�@�C��
	case FileFormat::WAVE:
		mMutex.lock();
		mSounds[handle.mHandle].mFilePath =filePath;
		mMutex.unlock();
		mSounds.at(handle.mHandle).mFileFormat =format;

		LoadRequest(handle, format);

		++mSounds.at(handle.mHandle).mReferenceCount;
		break;

	// �s���t�@�C��
	case FileFormat::Unknown:
		CWE_ASSERT(false);
		break;

	// ��Ή��񋓎q
	default:
		CWE_ASSERT(false);
		break;
	}
	

	return handle;
}


// ���\�[�X�n���h���̃R�s�[
const ResourceHandle& Resource::CopyPolygonHandle(const ResourceHandle& handle)
{
	CWE_ASSERT(mPolygons.find(handle.mHandle) != mPolygons.end()); //�s���ȃn���h���̋֎~
	++mPolygons.at(handle.mHandle).mReferenceCount;
	return handle;
}
const ResourceHandle& Resource::CopyMotionHandle(const ResourceHandle& handle)
{
	CWE_ASSERT(mMotions.find(handle.mHandle) != mMotions.end()); //�s���ȃn���h���̋֎~
	++mMotions.at(handle.mHandle).mReferenceCount;
	return handle;
}
const ResourceHandle& Resource::CopySoundHandle(const ResourceHandle& handle)
{
	CWE_ASSERT(mSounds.find(handle.mHandle) != mSounds.end()); //�s���ȃn���h���̋֎~
	++mSounds.at(handle.mHandle).mReferenceCount;
	return handle;
}


// ���\�[�X�̊J��
void Resource::PolygonRelease(const ResourceHandle& handle)
{
	if(mPolygons.find(handle.mHandle) == mPolygons.end()) {return;} //�s���ȃn���h���̖���
	PolygonData& refP =mPolygons.at(handle.mHandle);
	--refP.mReferenceCount;
	// �Q�ƃJ�E���g��0�ɂȂ�����폜
	if(refP.mReferenceCount <=0)
	{
		mMutex.lock();
		mPolygons.erase(handle.mHandle);
		mMutex.unlock();
	}
}
void Resource::MotionRelease(const ResourceHandle& handle)
{
	if(mMotions.find(handle.mHandle) == mMotions.end()) {return;} //�s���ȃn���h���̖���
	MotionData& refA =mMotions.at(handle.mHandle);
	--refA.mReferenceCount;
	// �Q�ƃJ�E���g��0�ɂȂ�����폜
	if(refA.mReferenceCount <=0)
	{
		mMutex.lock();
		mMotions.erase(handle.mHandle);
		mMutex.unlock();
	}
}
void Resource::SoundRelease(const ResourceHandle& handle)
{
	if(mSounds.find(handle.mHandle) == mSounds.end()) {return;} //�s���ȃn���h���̖���
	SoundData& refA =mSounds.at(handle.mHandle);
	--refA.mReferenceCount;
	// �Q�ƃJ�E���g��0�ɂȂ�����폜
	if(refA.mReferenceCount <=0)
	{
		mMutex.lock();
		mSounds.erase(handle.mHandle);
		mMutex.unlock();
	}
}


// ���\�[�X�f�[�^�̎󂯓n��(�|���S���f�[�^)
PolygonData& Resource::GetPolygonData(const ResourceHandle& handle)
{
	CWE_ASSERT(mPolygons.find(handle.mHandle) != mPolygons.end()); //�s���ȃn���h���̋֎~
	return mPolygons.at(handle.mHandle);
}
const PolygonData& Resource::GetPolygonData(const ResourceHandle& handle)const
{
	CWE_ASSERT(mPolygons.find(handle.mHandle) != mPolygons.end()); //�s���ȃn���h���̋֎~
	return mPolygons.at(handle.mHandle);
}


// ���\�[�X�f�[�^�̎󂯓n��(���[�V�����f�[�^)
MotionData& Resource::GetMotionData(const ResourceHandle& handle)
{
	CWE_ASSERT(mMotions.find(handle.mHandle) != mMotions.end()); //�s���ȃn���h���̋֎~
	return mMotions.at(handle.mHandle);
}
const MotionData& Resource::GetMotionData(const ResourceHandle& handle)const
{
	CWE_ASSERT(mMotions.find(handle.mHandle) != mMotions.end()); //�s���ȃn���h���̋֎~
	return mMotions.at(handle.mHandle);
}


// ���\�[�X�f�[�^�̎󂯓n��(�T�E���h�f�[�^)
SoundData& Resource::GetSoundData(const ResourceHandle& handle)
{
	CWE_ASSERT(mSounds.find(handle.mHandle) != mSounds.end()); //�s���ȃn���h���̋֎~
	return mSounds.at(handle.mHandle);
}
const SoundData& Resource::GetSoundData(const ResourceHandle& handle)const
{
	CWE_ASSERT(mSounds.find(handle.mHandle) != mSounds.end()); //�s���ȃn���h���̋֎~
	return mSounds.at(handle.mHandle);
}



// ====================== �����g�p�֐� ==========================


// ���\�[�X�̐���(�X���b�h�֐�)
void Resource::CreateAll(Resource &resource)
{
	std::function<void(Resource&, int)> task;

	while(!resource.mTaskBuffer.empty())
	{
		Sleep(10);

		task =resource.mTaskBuffer.front().mTask;
		task(resource, resource.mTaskBuffer.front().mHandle.mHandle);
		if(!resource.mTaskBuffer.empty())
		{
			resource.mMutex.lock();
			resource.mTaskBuffer.pop_front();
			resource.mMutex.unlock();
		}
	}
}


// ���\�[�X�̃��[�h�̃��N�G�X�g
void Resource::LoadRequest(const ResourceHandle &handle, const FileFormat& format)
{
	mMutex.lock();
	switch(format)
	{
	case FileFormat::PMX:
		mTaskBuffer.emplace_back(Resource::CreateFromPMX, handle);
		break;

	case FileFormat::Picture:
		mTaskBuffer.emplace_back(Resource::CreateFromPicture, handle);
		break;

	case FileFormat::VMD:
		mTaskBuffer.emplace_back(Resource::CreateFromVMD, handle);
		break;

	case FileFormat::WAVE:
		mTaskBuffer.emplace_back(Resource::CreateFromWAVE, handle);
		break;

	case FileFormat::Unknown:
		CWE_ASSERT(false); //�t�H�[�}�b�g�s���͂��肦�Ȃ�
		break;

	default:
		CWE_ASSERT(false); //��Ή��񋓎q
		break;
	}
	mMutex.unlock();
}


// �e��t�@�C�����烊�\�[�X�̐���(�{��)
// PMX�t�@�C��
void Resource::CreateFromPMX(Resource &resource, int handle)
{
	FileLoader loader;
	PMXData data;
	unsigned i=0;

	PolygonData& refP =resource.mPolygons.at(handle);

	if(!loader.PMX(data, refP.mFilePath))
	{
		// ���[�h�����s�����ꍇ
		refP.mEnabled =Task::Failed;
		return;
	}

	// ���_�o�b�t�@�̐���
	D3D11_BUFFER_DESC vertexBuffer;
	vertexBuffer.Usage =D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth =sizeof(math::Float3) *data.mVertices.size();
	vertexBuffer.BindFlags =D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags =0;
	vertexBuffer.MiscFlags =0;
	vertexBuffer.StructureByteStride =0;
		//���_�z��̒��o
	std::vector<math::Float3> vertices;
	vertices.resize(data.mVertices.size());
	for(i=0; i<vertices.size(); ++i) {vertices[i] =data.mVertices[i].mVertex;}
	D3D11_SUBRESOURCE_DATA vertexBufferSub;
	vertexBufferSub.pSysMem =&vertices[0];
	vertexBufferSub.SysMemPitch =0;
	vertexBufferSub.SysMemSlicePitch =0;
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[0])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// UV���W�̍쐬
	vertexBuffer.ByteWidth =sizeof(math::Float2) *data.mVertices.size();
	std::vector<math::Float2> uv;
	uv.resize(data.mVertices.size());
	for(i=0; i<uv.size(); ++i) {uv[i] =data.mVertices[i].mUV;}
	vertexBufferSub.pSysMem =&uv[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[1])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// �@���o�b�t�@�̍쐬
	vertexBuffer.ByteWidth =sizeof(math::Float3) *data.mVertices.size();
	for(i=0; i<vertices.size(); ++i) {vertices[i] =data.mVertices[i].mNormal;}
	vertexBufferSub.pSysMem =&vertices[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[2])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// �{�[���C���f�b�N�X�o�b�t�@�̍쐬
	vertexBuffer.ByteWidth =sizeof(math::Int4) *data.mVertices.size();
	std::vector<math::Int4> bIndx;
	bIndx.resize(data.mVertices.size());
	for(i=0; i<bIndx.size(); ++i)
	{
		bIndx[i].x() =data.mVertices[i].mWeight.mBorn1;
		bIndx[i].y() =data.mVertices[i].mWeight.mBorn2;
		bIndx[i].z() =data.mVertices[i].mWeight.mBorn3;
		bIndx[i].w() =data.mVertices[i].mWeight.mBorn4;
	}
	vertexBufferSub.pSysMem =&bIndx[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[3])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// �{�[���E�F�C�g�o�b�t�@�̍쐬
	vertexBuffer.ByteWidth =sizeof(math::Float4) *data.mVertices.size();
	std::vector<math::Float4> bWeight;
	bWeight.resize(data.mVertices.size());
	for(i=0; i<bWeight.size(); ++i)
	{
		bWeight[i].x() =data.mVertices[i].mWeight.mWeight1;
		bWeight[i].y() =data.mVertices[i].mWeight.mWeight2;
		bWeight[i].z() =data.mVertices[i].mWeight.mWeight3;
		bWeight[i].w() =data.mVertices[i].mWeight.mWeight4;
	}
	vertexBufferSub.pSysMem =&bWeight[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[4])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// SDEF�pC�p�����[�^�o�b�t�@�̍쐬
	vertexBuffer.ByteWidth =sizeof(math::Float3) *data.mVertices.size();
	for(i=0; i<vertices.size(); ++i) {vertices[i] =data.mVertices[i].mWeight.mC;}
	vertexBufferSub.pSysMem =&vertices[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[5])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}
	// SDEF�pR0�p�����[�^�o�b�t�@�̍쐬
	for(i=0; i<vertices.size(); ++i) {vertices[i] =data.mVertices[i].mWeight.mR0;}
	vertexBufferSub.pSysMem =&vertices[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[6])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}
	// SDEF�pR1�p�����[�^�o�b�t�@�̍쐬
	for(i=0; i<vertices.size(); ++i) {vertices[i] =data.mVertices[i].mWeight.mR1;}
	vertexBufferSub.pSysMem =&vertices[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexBuffer, &vertexBufferSub, &refP.mpVertexBuffers[7])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	D3D11_BUFFER_DESC indexBuffer;
	indexBuffer.Usage =D3D11_USAGE_DEFAULT;
	indexBuffer.ByteWidth =sizeof(PMXData::Surface) *data.mSurfaces.size();
	indexBuffer.BindFlags =D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags =0;
	indexBuffer.MiscFlags =0;
	indexBuffer.StructureByteStride =0;
	D3D11_SUBRESOURCE_DATA indexBufferSub;
	indexBufferSub.pSysMem =&data.mSurfaces[0];
	indexBufferSub.SysMemPitch =0;
	indexBufferSub.SysMemSlicePitch =0;
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&indexBuffer, &indexBufferSub, &refP.mpIndexBuffer)))
	{
		refP.mEnabled =Task::Failed;
		return;
	}
	// �C���f�b�N�X�o�b�t�@�̑������i�[
	refP.mIndexCount =data.mSurfaces.size();

	// ���b�V���̍쐬
	refP.mMesh.resize(data.mMaterials.size());
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	D3DX11_IMAGE_INFO imageInfo;
	ZeroMemory(&imageInfo, sizeof(imageInfo));
	for(i=0; i<refP.mMesh.size(); ++i)
	{
		// �e�N�X�`���Ȃ��̃��b�V���͔�Ή�
		CWE_ASSERT(data.mMaterials[i].mColorMapTextureIndex != 255);

		// �V�F�[�_���\�[�X�r���[�̐ݒ�
		if(FAILED(D3DX11GetImageInfoFromFileW(data.mTexturePaths[data.mMaterials[i].mColorMapTextureIndex].c_str(), NULL, &imageInfo, NULL)))
		{
			refP.mEnabled =Task::Failed;
			return;
		}
		loadInfo.Width =imageInfo.Width;
		loadInfo.Height =imageInfo.Height;
		loadInfo.Depth =imageInfo.Depth;
		loadInfo.FirstMipLevel =0;
		loadInfo.MipLevels =imageInfo.MipLevels;
		loadInfo.Usage =D3D11_USAGE_DEFAULT;
		loadInfo.BindFlags =D3D11_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags =NULL;
		loadInfo.MiscFlags =NULL;
		loadInfo.Format =DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		loadInfo.Filter =D3DX11_FILTER_LINEAR|D3DX11_FILTER_SRGB_IN;
		loadInfo.MipFilter =D3DX11_FILTER_LINEAR;
		loadInfo.pSrcInfo =&imageInfo;

		// �V�F�[�_���\�[�X�r���[�̍쐬
		if(FAILED(D3DX11CreateShaderResourceViewFromFileW(resource.mGraphicM.mpDevice, data.mTexturePaths[data.mMaterials[i].mColorMapTextureIndex].c_str(), &loadInfo, NULL, &refP.mMesh[i].mpTexture, nullptr)))
		{
			refP.mEnabled =Task::Failed;
			return;
		}
		
		// ���b�V���̒��_��
		refP.mMesh[i].mVertexNum =data.mMaterials[i].mVertexNum;

		// ���b�V���̃X�y�L�����[�W��
		refP.mMesh[i].mSpecular =data.mMaterials[i].mSpecularity;
	}
	refP.mMesh.shrink_to_fit(); //���ʂȃ��������Ȃ���
	

	// �{�[���̍쐬
	unsigned boneNum =data.mBones.size();
	refP.mBones.resize(boneNum);
	for(i=0; i<boneNum; ++i)
	{
		// ���Ȃ̃C���f�b�N�X
		refP.mBones[i].mSelf =i;
		// �e�{�[��
		refP.mBones[i].mFather =data.mBones[i].mFatherIndex;
		//�{�[�������ʒu
		refP.mBones[i].mPosition =data.mBones[i].mPosition;
		// �{�[����
		refP.mBones[i].mName =data.mBones[i].mName;
	}
	refP.mBones.shrink_to_fit();


	// �K�w�D��\�[�g�Ń{�[���|�C���^�z��̍쐬
	unsigned filledCnt=0, prevFilledCnt=0;
	refP.mpHierarchy.resize(boneNum);
		//�{�[���̃|�C���^���󂯓n��
	for(i=0; i<boneNum; ++i)
	{
		refP.mpHierarchy[i] =&refP.mBones[i];
	}
	refP.mpHierarchy.shrink_to_fit();
	std::vector<Bone*> tmp(refP.mpHierarchy);
		//�e�{�[���ԍ����Ⴂ���Ƀ\�[�g
	std::sort(tmp.begin(), tmp.end(), 
		[](const Bone* b1, const Bone* b2)->bool{return b1->mFather < b2->mFather;});
		//���[�g�{�[�����i�[
	for(i=0; i<boneNum; ++i)
	{
		if(tmp[i]->mFather != -1) {break;}
		refP.mpHierarchy[i] =tmp[i];
		++prevFilledCnt;
	}
	filledCnt +=prevFilledCnt;
	unsigned lower=0, upper=0;
	unsigned indx=0;
	unsigned k=0;
		//�\�[�g���[�v
	while(true)
	{
		i =filledCnt -prevFilledCnt;
		prevFilledCnt =0;
			//�O��̃��[�v�Ŋi�[�����{�[���𑖍�
		for(; i<filledCnt; ++i)
		{
				//�w��{�[����e�{�[���Ɏ��{�[���̍ŏ��ƍŌ�̓Y�������Z�o
			resource.EqualRange(&lower, &upper, refP.mpHierarchy[i]->mSelf, tmp);

				//�Z�o�����{�[�����i�[
			for(indx=lower, k=filledCnt+prevFilledCnt; indx<upper; ++indx, ++k)
			{
				refP.mpHierarchy[k] =tmp[indx];
				++prevFilledCnt;
			}//for
		}//for
		filledCnt +=prevFilledCnt;
			//�ŉ��w���B
		if(prevFilledCnt == 0) {break;}
	}//while

	// �������s��쐬
	math::Float3A tmpPos;
	for(i=0; i<boneNum; ++i)
	{
		//�t��Ώ����s��쐬
		math::MatrixTranslation(&refP.mpHierarchy[i]->mInvOffsetPose, refP.mpHierarchy[i]->mPosition);
		refP.mpHierarchy[i]->mPose =refP.mpHierarchy[i]->mInvOffsetPose;
		math::MatrixInverse(&refP.mpHierarchy[i]->mInvOffsetPose, refP.mpHierarchy[i]->mInvOffsetPose);

		if(refP.mpHierarchy[i]->mFather != -1) //�e�������
		{
			//���Έʒu���Z�o
			math::VectorSubtract(&tmpPos, refP.mpHierarchy[i]->mPosition, refP.mBones[refP.mpHierarchy[i]->mFather].mPosition);
			//�������Ύp���s��쐬
			math::MatrixTranslation(&refP.mpHierarchy[i]->mOffsetRelatPose, tmpPos);
		}
		else
		{
			//�������Ύp���s��쐬
			math::MatrixTranslation(&refP.mpHierarchy[i]->mOffsetRelatPose, refP.mpHierarchy[i]->mPosition);
		}
		refP.mpHierarchy[i]->mRelatPose =refP.mpHierarchy[i]->mOffsetRelatPose;
	}


	// IK�{�[���쐬
	refP.mIKBone.reserve(10);
	for(i=0; i<boneNum; ++i)
	{
		if(data.mBones[i].mIKTargetIndex != -1) // IK�{�[���Ȃ��
		{
			refP.mIKBone.emplace_back(refP.mBones[i]);
		}
	}
	refP.mIKBone.shrink_to_fit();
	const unsigned ikBoneNum =refP.mIKBone.size();
		//IK�p�����[�^�̐ݒ�
	for(unsigned h=0; h<ikBoneNum; ++h)
	{
		PMXData::Bone& refBone =data.mBones[refP.mIKBone[h].mpSelf->mSelf];
			//�^�[�Q�b�g�{�[��
		refP.mIKBone[h].mpTarget =&refP.mBones[refBone.mIKTargetIndex];
		refP.mIKBone[h].mLoop =(refBone.mIKLoopCount<0)?0:refBone.mIKLoopCount;
			//�P�ʊp
		refP.mIKBone[h].mUnitAngle =refBone.m_IKUnitAngle;
			//IK�����N
		refP.mIKBone[h].mLinks.resize(refBone.mIKLinks.size());
		for(i=0; i<refP.mIKBone[h].mLinks.size(); ++i)
		{
			refP.mIKBone[h].mLinks[i].mpBone =&refP.mBones[refBone.mIKLinks[i].mIndex];
			refP.mIKBone[h].mLinks[i].mExistAngleLimited =refBone.mIKLinks[i].mExistAngleLimited;
			refP.mIKBone[h].mLinks[i].mLimitAngleMin =refBone.mIKLinks[i].mLimitAngleMin;
			refP.mIKBone[h].mLinks[i].mLimitAngleMax =refBone.mIKLinks[i].mLimitAngleMax;
		}
	}


	// ���[�h����
	refP.mEnabled =Task::Success;
}

// �摜�t�@�C��
void Resource::CreateFromPicture(Resource &resource, int handle)
{
	PolygonData& refP =resource.mPolygons.at(handle);

	// ���b�V���̍쐬
	refP.mMesh.resize(1);
	refP.mMesh.shrink_to_fit();
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	D3DX11_IMAGE_INFO imageInfo;
	ZeroMemory(&imageInfo, sizeof(imageInfo));

	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	if(FAILED(D3DX11GetImageInfoFromFileW(refP.mFilePath, NULL, &imageInfo, NULL)))
	{
		refP.mEnabled =Task::Failed;
		return;
	}
	loadInfo.Width =imageInfo.Width;
	loadInfo.Height =imageInfo.Height;
	loadInfo.Depth =imageInfo.Depth;
	loadInfo.FirstMipLevel =0;
	loadInfo.MipLevels =imageInfo.MipLevels;
	loadInfo.Usage =D3D11_USAGE_DEFAULT;
	loadInfo.BindFlags =D3D11_BIND_SHADER_RESOURCE;
	loadInfo.CpuAccessFlags =NULL;
	loadInfo.MiscFlags =NULL;
	loadInfo.Format =DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	loadInfo.Filter =D3DX11_FILTER_LINEAR|D3DX11_FILTER_SRGB_IN;
	loadInfo.MipFilter =D3DX11_FILTER_LINEAR;
	loadInfo.pSrcInfo =&imageInfo;

	// �V�F�[�_���\�[�X�r���[�̍쐬
	if(FAILED(D3DX11CreateShaderResourceViewFromFileW(resource.mGraphicM.mpDevice, refP.mFilePath, &loadInfo, NULL, &refP.mMesh[0].mpTexture, nullptr)))
	{
		refP.mEnabled =Task::Failed;
		return;
	}
		
	// ���b�V���̒��_��
	refP.mMesh[0].mVertexNum =6;

	// ���b�V���̃X�y�L�����[�W��
	refP.mMesh[0].mSpecular =0.0f;

	// ���_�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage =D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth =sizeof(math::Float3) *4;
	vertexDesc.BindFlags =D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags =0;
	vertexDesc.MiscFlags =0;
	vertexDesc.StructureByteStride =0;
	float magnifX =0.5f /resource.mGraphicM.mWindows.GetWindowSizeX(),
		  magnifY =0.5f /resource.mGraphicM.mWindows.GetWindowSizeY();
	math::Float3 vertices[4] ={
		{-float(imageInfo.Width)*magnifX, float(imageInfo.Height)*magnifY, 0.0f}, {float(imageInfo.Width)*magnifX, float(imageInfo.Height)*magnifY, 0.0f},
		{-float(imageInfo.Width)*magnifX, -float(imageInfo.Height)*magnifY, 0.0f}, {float(imageInfo.Width)*magnifX, -float(imageInfo.Height)*magnifY, 0.0f},
	};
	D3D11_SUBRESOURCE_DATA vertexBufferSub;
	vertexBufferSub.pSysMem =&vertices[0];
	vertexBufferSub.SysMemPitch =0;
	vertexBufferSub.SysMemSlicePitch =0;
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexDesc, &vertexBufferSub, &refP.mpVertexBuffers[0])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// UV���W�̍쐬
	vertexDesc.ByteWidth =sizeof(math::Float2) *4;
	math::Float2 uv[4] ={
		{0.0f, 0.0f}, {1.0f, 0.0f},
		{0.0f, 1.0f}, {1.0f, 1.0f}
	};
	vertexBufferSub.pSysMem =&uv[0];
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&vertexDesc, &vertexBufferSub, &refP.mpVertexBuffers[1])))
	{
		refP.mEnabled =Task::Failed;
		return;
	}

	// �@���o�b�t�@�͖���

	// �C���f�b�N�X�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC indexBuffer;
	indexBuffer.Usage =D3D11_USAGE_DEFAULT;
	indexBuffer.ByteWidth =sizeof(unsigned) *6;
	indexBuffer.BindFlags =D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags =0;
	indexBuffer.MiscFlags =0;
	indexBuffer.StructureByteStride =0;
	unsigned indices[6] ={
		0, 1, 2,
		2, 1, 3,
	};
	D3D11_SUBRESOURCE_DATA indexBufferSub;
	indexBufferSub.pSysMem =&indices[0];
	indexBufferSub.SysMemPitch =0;
	indexBufferSub.SysMemSlicePitch =0;
	if(FAILED(resource.mGraphicM.mpDevice->CreateBuffer(&indexBuffer, &indexBufferSub, &refP.mpIndexBuffer)))
	{
		refP.mEnabled =Task::Failed;
		return;
	}
	// �C���f�b�N�X�o�b�t�@�̐����i�[
	refP.mIndexCount =6;


	// ���[�h����
	refP.mEnabled =Task::Success;
}


// VMD�t�@�C��
void Resource::CreateFromVMD(Resource& resource, int handle)
{
	FileLoader loader;
	VMDData data;
	FrameData frameData;
	unsigned j=0, paramNum =0;

	MotionData& refA =resource.mMotions.at(handle);

	if(!loader.VMD(data, refA.mFilePath))
	{
		// ���[�h�����s�����ꍇ
		refA.mEnabled =Task::Failed;
		return;
	}

	// �ǂݍ���
	for(unsigned i=0; i<data.mFrameData.size(); ++i)
	{
		// �t���[���ԍ�
		frameData.mFrameNumber =data.mFrameData[i].mFrameNumber;
		// �{�[���ʒu
		frameData.mBonePos =data.mFrameData[i].mBonePos;
		// �{�[����]�N�H�[�^�j�I��
		frameData.mBoneQuaternion =data.mFrameData[i].mBoneQuaternion;
		// �x�W�F�Ȑ�����_�p�����[�^�[
		paramNum =0;
		for(j=0; j<4; ++j)
		{
			frameData.mXInterpolation[j] =static_cast<float>(data.mFrameData[i].mInterpolationParam[paramNum]);
			frameData.mYInterpolation[j] =static_cast<float>(data.mFrameData[i].mInterpolationParam[++paramNum]);
			frameData.mZInterpolation[j] =static_cast<float>(data.mFrameData[i].mInterpolationParam[++paramNum]);
			frameData.mRInterpolation[j] =static_cast<float>(data.mFrameData[i].mInterpolationParam[++paramNum]);
			++paramNum;
		}

		// �x�N�^�[�ɒǉ�
		refA.mFrameData[data.mFrameData[i].mBoneName].first.emplace_back(frameData);
	}

	// ���[�V�����f�[�^���L�[�t���[����Ő���
	for(auto& data : refA.mFrameData)
	{
		std::sort(data.second.first.begin(), data.second.first.end());

		//���[�V�����f�[�^�̍ŏI�L�[�t���[�����擾
		if(refA.mLastKeyFrame < data.second.first[data.second.first.size()-1].mFrameNumber)
		{
			refA.mLastKeyFrame =data.second.first[data.second.first.size()-1].mFrameNumber;
		}
	}


	// ���[�h����
	refA.mEnabled =Task::Success;
}


// WAVE�t�@�C��
void Resource::CreateFromWAVE(Resource& resource, int handle)
{
	FileLoader loader;

	SoundData& refS =resource.mSounds.at(handle);

	if(!loader.WAV(refS.mSoundData, refS.mFilePath))
	{
		// ���[�h�����s�����ꍇ
		refS.mEnabled =Task::Failed;
		return;
	}

	// Get format of wave file
	CWE::WAVData::WAVFormat &fmt =refS.mSoundData.mFormat;
	WAVEFORMATEX wfx ={};
	wfx.wFormatTag =WAVE_FORMAT_PCM;
	wfx.nChannels =fmt.mChannelsNum;
	wfx.nSamplesPerSec =fmt.mSamplesPerSec;
	wfx.wBitsPerSample =fmt.mBitsPerSample;
	wfx.nBlockAlign =wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec =wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize =0;

	// Create the source voice
	if(FAILED(resource.mSoundM.mpXAudio2->CreateSourceVoice(&refS.mpSourceVoice, &wfx)))
	{
		refS.mEnabled =Task::Failed;
		return;
	}

	// Submit the wave sample data
	XAUDIO2_BUFFER buffer{};
	buffer.pAudioData =reinterpret_cast<const BYTE*>(refS.mSoundData.mpData.get());
	buffer.AudioBytes =refS.mSoundData.mDataSize;
	buffer.Flags =XAUDIO2_END_OF_STREAM;
	if(FAILED(refS.mpSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		refS.mEnabled =Task::Failed;
		return;
	}

	refS.mBuffer =buffer;

	// ���[�h����
	refS.mEnabled =Task::Success;
}


// �q�G�����L�[�\�[�g�pequal_range
void Resource::EqualRange(unsigned* lower, unsigned* upper, int value, const std::vector<Bone*>& tmpB)
{
	unsigned last=0, len=0, half=0, mid=0;

	//lower_range
	*lower =0;
	last =tmpB.size();
	for(len =last-*lower; len != 0; )
	{
		half =len /2;
		mid =*lower;
		mid +=half;
		if(tmpB[mid]->mFather < value)
		{
			len -= half + 1;
			*lower =++mid;
		}
		else
		{
			len =half;
		}
	}

	//upper_range
	*upper =0;
	for (unsigned len =last-*upper; len != 0; )
	{
		half =len /2;
		mid =*upper;
		mid +=half;
		if(!(value < tmpB[mid]->mFather))
		{
			len -=half+1;
			*upper =++mid;
		}
		else
		{
			len =half;
		}
	}
}




}// CWE