#pragma once


#include "Task.h"



// �Q�[�����s�N���X
class Game
{
private:
	TaskExec mTask;

public:
	Game();
	~Game();

	bool Run();
	void End();

};