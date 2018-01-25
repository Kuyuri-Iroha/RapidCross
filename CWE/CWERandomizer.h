#pragma once

#include <random>


namespace CWE
{


//����������
class Randomizer
{
private:
	friend class Uniform_int;
	friend class Uniform_double;
	friend class Bernoulli;

	std::random_device mSeed;
	std::mt19937 mMersenneTwister;
	std::uniform_int_distribution<> mUniDist;


	Randomizer();
	~Randomizer();

public:
	//�C���X�^���X�̎擾
	static Randomizer& GetInstance();
	Randomizer(const Randomizer&) =delete;
	Randomizer& operator=(const Randomizer&) =delete;

	//����������Ɏg�������V�[�h�l�̃��Z�b�g(�����̐��x�̌���̂���)
	void ResetSeed();

	//�����̎擾
	//(1�`100000)
	int GetRandom();

};



//��l���z����(int)
class Uniform_int
{
private:
	Randomizer& mRandomizer;
	std::uniform_int_distribution<> mUniDist;

public:
	//�f�t�H���g��0�`100
	Uniform_int();
	Uniform_int(const int& min, const int& max);
	~Uniform_int();

	//�����̎擾
	int GetRandom();

};


//��l���z����(double)
class Uniform_double
{
private:
	Randomizer& mRandomizer;
	std::uniform_real_distribution<> mUniDist;

public:
	//�f�t�H���g��0.0�`1.0
	Uniform_double();
	Uniform_double(const double& min, const double& max);
	~Uniform_double();

	//�����̎擾
	double GetRandom();

};


//�x���k�[�C���z����
class Bernoulli
{
private:
	Randomizer& mRandomizer;
	std::bernoulli_distribution mBernoDist;

public:
	//�f�t�H���g��0.5(50%)
	Bernoulli();
	//probability=true�ɂȂ�m��
	//(0.5=50%)
	Bernoulli(const double& probability);
	~Bernoulli();

	//�����̎擾
	bool GetRandom();

};



}//CWE