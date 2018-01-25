#pragma once




//���S�ȃf�o�C�X�Ǘ������̊J��
#define SAFE_UNACQUIRE(device) {if((device)){(device)->Unacquire();}}
//���S�ȃ����[�X
#define SAFE_RELEASE(ptr) {if((ptr)){(ptr)->Release(); (ptr)=nullptr;}}
//���S�ȃf���[�g
#define SAFE_DELETE(ptr) {if((ptr)){delete(ptr); (ptr)=nullptr;}}
//���S�Ȕz��f���[�g
#define SAFE_DELETE_ARRAY(ptr) {if((ptr)){delete[](ptr); (ptr)=nullptr;}}
//Safe the Destory function.
template<class pT>
void SafeDestroy(pT& p)
{
	if(p){
		p->DestroyVoice();
		p =nullptr;
	}
}

