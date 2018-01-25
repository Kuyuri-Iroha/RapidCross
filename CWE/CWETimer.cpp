#include "CWETimer.h"


namespace CWE
{


//�R���X�g���N�^
Timer::Timer():
m_SetTime(-1), m_Started(false)
{

}

//�f�X�g���N�^
Timer::~Timer()
{

}


//�o�ߎ��Ԃ��L�^
//(CTimer���g�p����ꍇ���̊֐���1���[�v�ɂ�1��Ăяo��)
void Timer::Update()
{
	m_Now =std::chrono::system_clock::now();
	m_During =m_Now -m_Start;
}


//�J�E���g�X�^�[�g
void Timer::Start()
{
	m_Start =std::chrono::system_clock::now();
	m_Started =true;
}


//�o�ߎ��Ԃ��擾 (�}�C�N���b)
//1�b = 1,000,000�}�C�N���b
//(time<0) = �G���[
void Timer::GetTime(long long& time)
{
	if(!m_Started) {time =-1; return;}
	time =std::chrono::duration_cast<std::chrono::microseconds>(m_During).count();
}


//�o�ߎ��Ԃ��擾 (�b)
//(time<0) = �G���[
void Timer::GetTime(double& time)
{
	if(!m_Started) {time =-1; return;}
	time =static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(m_During).count())/1000000;
}


//�o�ߎ��Ԕ���̐ݒ�
//true=���� false=���s
bool Timer::Set(const long long& setTime)
{
	if(setTime<0) {m_SetTime =-1; return false;}

	m_SetTime =setTime;
	return true;
}


//�o�ߎ��Ԕ���̐ݒ�(double)
//true=���� false=���s
bool Timer::Set(const double& setTime)
{
	if(setTime<0) {m_SetTime =-1; return false;}

	m_SetTime =static_cast<long long>(setTime*1000000);
	return true;
}


//�Z�b�g�������Ԃ��o�߂������ǂ���
//true=�o�� false=���o��
bool Timer::TimeIsPassed()
{
	if((!m_Started) | (m_SetTime == -1)) {return false;}

	if(m_SetTime <= std::chrono::duration_cast<std::chrono::microseconds>(m_During).count())
	{
		return true;
	}
	return false;
}


}//CWE