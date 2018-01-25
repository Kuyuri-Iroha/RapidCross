#include "CWERandomizer.h"


namespace CWE
{



//======================= ���������� =================================

//�R���X�g���N�^
Randomizer::Randomizer():
mMersenneTwister(mSeed()), mUniDist(1, 100000)
{
}

//�f�X�g���N�^
Randomizer::~Randomizer()
{
}

//�C���X�^���X�̎擾
Randomizer& Randomizer::GetInstance()
{
	static Randomizer instance;
	return instance;
}


//����������Ɏg�������V�[�h�l�̃��Z�b�g(�����̐��x�̌���̂���)
void Randomizer::ResetSeed()
{
	mMersenneTwister =std::mt19937(mSeed());
}


//�����̎擾
//(1�`100000)
int Randomizer::GetRandom()
{
	return mUniDist(mMersenneTwister);
}



//====================== ��l���z(int) =================================

//�R���X�g���N�^
Uniform_int::Uniform_int():
mRandomizer(Randomizer::GetInstance()), mUniDist(0, 100)
{
}

//�����t���R���X�g���N�^
Uniform_int::Uniform_int(const int& min, const int& max):
mRandomizer(Randomizer::GetInstance()), mUniDist(min, max)
{
}

//�f�X�g���N�^
Uniform_int::~Uniform_int()
{
}


//�����̎擾
int Uniform_int::GetRandom()
{
	return mUniDist(mRandomizer.mMersenneTwister);
}



//====================== ��l���z(double) =================================

//�R���X�g���N�^
Uniform_double::Uniform_double():
mRandomizer(Randomizer::GetInstance()), mUniDist(0.0, 1.0)
{

}

//�����t���R���X�g���N�^
Uniform_double::Uniform_double(const double& min, const double& max):
mRandomizer(Randomizer::GetInstance()), mUniDist(min, max)
{
}

//�f�X�g���N�^
Uniform_double::~Uniform_double()
{
}


//�����̎擾
double Uniform_double::GetRandom()
{
	return mUniDist(mRandomizer.mMersenneTwister);
}



//====================== �x���k�[�C���z =================================

//�R���X�g���N�^
Bernoulli::Bernoulli():
mRandomizer(Randomizer::GetInstance()), mBernoDist(0.5)
{

}

//�����t���R���X�g���N�^
Bernoulli::Bernoulli(const double& probability):
mRandomizer(Randomizer::GetInstance()), mBernoDist(probability)
{
}

//�f�X�g���N�^
Bernoulli::~Bernoulli()
{
}


//�����̎擾
bool Bernoulli::GetRandom()
{
	return mBernoDist(mRandomizer.mMersenneTwister);
}



}//CWE