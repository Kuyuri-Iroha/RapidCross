#include "CWEIncludedAPI.h"
#include "CWEThread.h"
#include <algorithm>



namespace CWE
{



// ========================= WorkerThread::Thread =============================================

// �R���X�g���N�^
WorkerThread::Thread::Thread(const bool& threadAble):
	mTaskBuffer(), mThread(Activity, std::ref(*this), std::ref(threadAble))
{

}

// �R�s�[�R���X�g���N�^
WorkerThread::Thread::Thread(Thread& thread):
	mTaskBuffer(thread.mTaskBuffer)
{
	mThread.swap(thread.mThread);
}

// �f�X�g���N�^
WorkerThread::Thread::~Thread()
{
	mThread.join();
}


// �X���b�h����
void WorkerThread::Thread::Activity(Thread& thisData, const volatile bool& threadAble)
{
	std::function<void()> task;

	//�^�X�N�҂��p���[�v
	while(threadAble)
	{
		Sleep(1); //TODO: ���[�v����������ƋN����n���O�A�b�v(�umTaskBuffer�v�̃������j��)�̌��������Ɖ���

		//�^�X�N�������
		if(!thisData.mTaskBuffer.empty())
		{
			task =thisData.mTaskBuffer.front().mTask;

			if(!threadAble) {return;}
			task();

			thisData.mMutex.lock();

			thisData.mTaskBuffer.pop_front();

			thisData.mMutex.unlock();
		}
	}
}



// =============================== WorkerThread ===============================================

#define THREAD_NUM 4

// �R���X�g���N�^
WorkerThread::WorkerThread():
	m_Able(true)
{
	mThreadGroup.reserve(THREAD_NUM); //TODO: �œK��
	m_NextID.mNumber =INT_MIN;
}

// �f�X�g���N�^
WorkerThread::~WorkerThread()
{
	Deactivation();
}


// �X���b�h�̋N��
void WorkerThread::Activation()
{
	for(unsigned i=0; i<THREAD_NUM; ++i)
	{
		mThreadGroup.emplace_back(m_Able);
	}
}


// �X���b�h�̏I��
void WorkerThread::Deactivation()
{
	m_Able =false;
	mThreadGroup.clear();
}


// �X���b�h���N�����Ă��邩
bool WorkerThread::Activated() const
{
	return !mThreadGroup.empty();
}


// �^�X�N���S�ďI�����Ă��邩
bool WorkerThread::AllTaskEnded() const
{
	for(unsigned i=0; i<mThreadGroup.size(); ++i)
	{
		if(!mThreadGroup[i].mTaskBuffer.empty()) {return false;}
	}

	if(!mTaskContainer.empty()) {return false;}

	return true;
}


// �^�X�N�R���e�i�̓��e��S�ď���
void WorkerThread::TaskClear()
{
	mTaskContainer.clear();
}


// �e�X���b�h�Ƀ^�X�N�̐U�蕪��
void WorkerThread::Update()
{
	if(mTaskContainer.empty() || mThreadGroup.empty()) {return;}
	const int containerSize =mTaskContainer.size();
	int divideTaskNum =containerSize /mThreadGroup.size();
	if(int(divideTaskNum*mThreadGroup.size()) < containerSize) {++divideTaskNum;} //�؎̂Ď��ɋN����덷�̏C��

	int cnt=0, actuallyDivNum=0;
	for(unsigned i=0; i<mThreadGroup.size(); ++i)
	{
		if(containerSize <= cnt) {return;} //�^�X�N�̐U�蕪�����I����Ă���

		if(containerSize <= cnt+divideTaskNum) //���̃��[�v�ŐU�蕪�����I���̂Ȃ�
		{
			actuallyDivNum =(cnt+divideTaskNum) -containerSize;
			actuallyDivNum =divideTaskNum -actuallyDivNum +cnt;
		}
		else
		{
			actuallyDivNum =divideTaskNum +cnt;
		}

		mThreadGroup[i].mMutex.lock();

		//�^�X�N�̕��z
		for(; cnt<actuallyDivNum; ++cnt)
		{
			mThreadGroup[i].mTaskBuffer.emplace_back(mTaskContainer[cnt]);
		}

		mThreadGroup[i].mMutex.unlock();
	}

	mTaskContainer.clear();
}


// �^�X�N�̃��N�G�X�g
const WorkerThread::TaskID WorkerThread::TaskRequest(const std::function<void()>& task)
{
	mTaskContainer.emplace_back(task, m_NextID);

	TaskID result(m_NextID);
	++m_NextID.mNumber;

	return result;
}


// �^�X�N���I�����Ă��邩
bool WorkerThread::TaskEnded(const TaskID& id) const
{
	for(unsigned i=0; i<mThreadGroup.size(); ++i)
	{
		for(const Task& task : mThreadGroup[i].mTaskBuffer)
		{
			if(id.mNumber == task.mID.mNumber)
			{
				return false;
			}
		}
	}

	for(const Task& task : mTaskContainer)
	{
		if(id.mNumber == task.mID.mNumber)
		{
			return false;
		}
	}

	return true;
}


}