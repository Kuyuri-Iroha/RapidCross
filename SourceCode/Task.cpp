#include "Task.h"
#include "../CWE/CWEError.h"
#include "../CWE/CWEUsefullyMacro.h"




// =========== TaskExec::List ====================

// �R���X�g���N�^
TaskExec::List::List()
{

}


// �f�X�g���N�^
TaskExec::List::~List()
{
	for(auto& task : mAllTask)
	{
		SAFE_DELETE(task);
	}
	mAllTask.clear();
}


// �^�X�N�̒ǉ�
bool TaskExec::List::Append(TaskBase* pTask, const TaskName& name)
{
	CWE_ASSERT(pTask);

	if(mTaskMap.find(name) != mTaskMap.end()) //���łɓ������̂���������
	{
		return false;
	}

	//�D��x���ɒǉ�
	if(mAllTask.empty()) //�^�X�N���X�g���󂾂�����
	{
		mAllTask.push_front(pTask);
	}
	else
	{
		TaskExec::ListIterator itr =mAllTask.begin();

		for(auto& task : mAllTask)
		{
			CWE_ASSERT(task);

			if(task->GetPriority() < pTask->GetPriority())
			{
				itr =mAllTask.insert(itr, pTask);
				break;
			}
			++itr;
		}

		if(itr == mAllTask.end()) //�D��x��1�ԏ��������
		{
			mAllTask.push_back(pTask);
		}
	}

	//�^�X�N�}�b�v�ɓo�^
	mTaskMap[name] =pTask;
	return true;
}


//�^�X�N�̏����擾
const TaskBase* const TaskExec::List::GetTaskData(const TaskName& name) const
{
	if(mTaskMap.find(name) == mTaskMap.end())
	{
		return nullptr;
	}

	return mTaskMap.at(name);
}


// �^�X�N�̃N���A
void TaskExec::List::Clear()
{
	for(const auto& task : mAllTask)
	{
		CWE_ASSERT(task);
		task->End();
	}
	mAllTask.clear();
	mTaskMap.clear();
}




// =========== TaskExec =======================


// �R���X�g���N�^
TaskExec::TaskExec()
{

}


// �f�X�g���N�^
TaskExec::~TaskExec()
{
	AllTaskEnd();
}


// �X�V
void TaskExec::AllTaskUpdate()
{
	for(ListIterator itr=mTaskList.mAllTask.begin(); itr!=mTaskList.mAllTask.end();)
	{
		CWE_ASSERT(*itr);

		//�X�̃^�X�N�̍X�V
		if((*itr)->IsUpdate())
		{
			(*itr)->Update(mTaskList);
		}

		//�^�X�N���I�����Ă��邩
		if(!(*itr)->IsEnable())
		{
			mTaskList.mTaskMap.erase((*itr)->GetName());
			SAFE_DELETE(*itr);
			itr =mTaskList.mAllTask.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


// �`��
void TaskExec::AllTaskDraw()
{
	for(const auto& task : mTaskList.mAllTask)
	{
		CWE_ASSERT(task);

		if(task->IsDraw())
		{
			task->Draw();
		}
	}
}