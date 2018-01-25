#pragma once

#include "../CWE/CWELib.h"


// 臒l�Ńp�����[�^��\������N���X
class Parameter
{
public:
	Parameter() =default;
	~Parameter();

	bool LoadSucceeded() const;
	void Create(const wchar_t* maskPath, const wchar_t* shapePath);
	void Draw();
	void End();

private:
	CWE::Polygon mMask;
	CWE::Polygon mShape;
};