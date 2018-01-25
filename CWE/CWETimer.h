#pragma once

#include <chrono>

namespace CWE
{


//�����Ԍv���@�\���}�C�N���b�P�ʂŒ񋟂���N���X
class Timer
{
private:
	std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration>
		m_Start,
		m_Now;
	std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period>
		m_During;

	long long m_SetTime; //0�����ł������ꍇ�G���[

	bool m_Started;

public:
	Timer();
	~Timer();

	//�J�E���g���X�^�[�g���Ă��邩
	inline const bool& GetStarted() {return m_Started;}

	//�o�ߎ��Ԃ��L�^
	//(Timer���g�p����ꍇ���̊֐���1���[�v�ɂ�1��Ăяo��)
	//(�Ȃ��A�����Ȑ��x���K�v�ȏꍇ�͌o�ߎ��Ԏ擾�n�֐��̌Ăяo���O�ɌĂяo��)
	void Update();

	//�J�E���g�X�^�[�g(���X�^�[�g���\)
	void Start();

	//�o�ߎ��Ԃ��擾 (�}�C�N���b)
	//1�b = 1,000,000�}�C�N���b
	//(time<0) = �G���[
	void GetTime(long long& time);

	//�o�ߎ��Ԃ��擾 (�b)
	//(time<0) = �G���[
	void GetTime(double& time);

	//�^�C�}�[�̃Z�b�g(long long)
	//true=���� false=���s
	bool Set(const long long& setTime);

	//�^�C�}�[�̃Z�b�g(double)
	//true=���� false=���s
	bool Set(const double& setTime);

	//�Z�b�g�������Ԃ��o�߂������ǂ���
	//true=�o�� false=���o��
	bool TimeIsPassed();

};


}//CWE


