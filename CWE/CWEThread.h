#pragma once

#include <vector>
#include <deque>
#include <list>
#include <thread>
#include <functional>
#include <mutex>



namespace CWE
{


// ���[�J�[�X���b�h
class WorkerThread
{
public:
	struct TaskID
	{
		int mNumber;
	};

private:
	struct Task
	{
		std::function<void()> mTask;
		TaskID mID;

		Task(const std::function<void()>& task, const TaskID& id): mTask(task), mID(id) {}
		~Task() {}
	};

	struct Thread
	{
		std::thread mThread;
		std::deque<Task> mTaskBuffer;
		std::mutex mMutex;
		static void Activity(Thread& thisData, const volatile bool& able);

		Thread(const bool& threadAble);
		Thread(Thread& thread);
		~Thread();
	};

private:
	std::vector<Thread> mThreadGroup;
	bool m_Able;
	std::vector<Task> mTaskContainer;
	TaskID m_NextID;

public:
	WorkerThread();
	~WorkerThread();

	// �X���b�h�̋N��
	void Activation();
	// �X���b�h�̏I��
	void Deactivation();
	// �X���b�h���N�����Ă��邩
	bool Activated() const;
	// �^�X�N���S�ďI�����Ă��邩
	bool AllTaskEnded() const;
	// �^�X�N�R���e�i�̓��e��S�ď���
	void TaskClear();
	// �e�X���b�h�Ƀ^�X�N�̐U�蕪��
	void Update();
	// �^�X�N�̃��N�G�X�g
	const TaskID TaskRequest(const std::function<void()>& task);
	// �^�X�N���I�����Ă��邩
	bool TaskEnded(const TaskID& id) const;

};



}