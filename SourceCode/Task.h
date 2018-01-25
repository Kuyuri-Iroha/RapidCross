#pragma once


#include <list>
#include <unordered_map>



class TaskBase;


// �^�X�N��
enum class TaskName
{
	Battle,
	FPS,
	DebugString,
	Title,
	Loading,
	Result,
	Unnamed
};


// �^�X�N�̎��s�N���X
class TaskExec
{
public:
	//�O������^�X�N�𐧌䂷��ׂ̃I�u�W�F�N�g
	class List final
	{
		friend class TaskExec;
	private:
		std::list<TaskBase*> mAllTask;
		std::unordered_map<TaskName, TaskBase*> mTaskMap;

	public:
		List();
		~List();

		//�^�X�N�̒ǉ�
		//(true: �^�X�N�}�b�v�ǉ�����,  false: �^�X�N�}�b�v�ǉ����s)
		bool Append(TaskBase* pTask, const TaskName& name);
		//�^�X�N�̏����擾
		//(���s������nullptr��Ԃ�)
		const TaskBase* const GetTaskData(const TaskName& name) const;

		//�^�X�N�̃N���A
		void Clear();
	};

private:

protected:
	using ListIterator =std::list<TaskBase*>::iterator;

public:
	List mTaskList;

	TaskExec();
	virtual ~TaskExec();

	void AllTaskUpdate();
	void AllTaskDraw();

	void AllTaskEnd() {mTaskList.Clear();}

};


// �^�X�N�V�X�e���x�[�X
class TaskBase
{
private:

protected:
	bool mUpdate; //�X�V���s����
	bool mDraw; //�`����s����
	bool mEnable; //�^�X�N���L�����ǂ���
	const int mPriority; //�D��x
	const TaskName mName;

public:
	TaskBase(): mUpdate(true), mDraw(true), mEnable(true), mPriority(0), mName(TaskName::Unnamed) {}
	TaskBase(int priority, TaskName name): mUpdate(true), mDraw(true), mEnable(true), mPriority(priority), mName(name) {}
	virtual ~TaskBase(){}

	bool IsUpdate() const {return mUpdate;}
	bool IsDraw() const {return mDraw;}
	bool IsEnable() const {return mEnable;}
	int GetPriority() const {return mPriority;}
	TaskName GetName() const {return mName;}

	virtual void Update(TaskExec::List& taskList) =0;
	virtual void Draw() =0;
	virtual void End() =0;

};