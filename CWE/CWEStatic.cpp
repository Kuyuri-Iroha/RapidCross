#include "CWEStatic.h"
#include "CWEGraphicManager.h"


namespace CWE
{


//�}�E�X�̕\�����Z�b�g
bool ResetMouseVisible(const bool& visible)
{
	if(visible == false)
	{
		while(ShowCursor(FALSE) > -1 ){};
	}
	else
	{
		while(ShowCursor(TRUE) < 0 ){};
	}

	return visible;
}


// �O���t�B�b�N�f�o�C�X���������ς݂����m�F
bool GraphicInitialized()
{
	return Graphic::GetInstance().mpDevice != nullptr;
}


}//CWE