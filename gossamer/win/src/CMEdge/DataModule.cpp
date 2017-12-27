
#include "DataModule.h"
#include "utils\CriticalLock.h"

CDataModuleImpl::CDataModuleImpl()
    :next_task_id_(0)
{
}

CDataModuleImpl::~CDataModuleImpl()
{
    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    std::map<UINT, CTaskData*>::iterator iter = task_map_.end();
    for (iter = task_map_.begin(); iter != task_map_.end(); iter++)
    {
        delete iter->second;
    }
    task_map_.clear();
}

BOOL CDataModuleImpl::AddTask(const CTaskData& task_data)
{
    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    CTaskData* data = new CTaskData(task_data);

    task_map_[task_data.GetTaskID()] = data;

    return TRUE;
}

BOOL CDataModuleImpl::GetTask(UINT task_id, CTaskData& task_data)
{
    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    std::map<UINT, CTaskData*>::iterator iter = task_map_.find(task_id);
    if (task_map_.end() == iter)
    {
        return FALSE;
    }

    task_data = *(iter->second);
    return TRUE;
}

void CDataModuleImpl::SetTask(const CTaskData& task_data)
{
    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    std::map<UINT, CTaskData*>::iterator iter = task_map_.find(task_data.GetTaskID());
    if (task_map_.end() == iter)
    {
        return;
    }

    *(iter->second) = task_data;
}

void CDataModuleImpl::RemoveTask(UINT task_id)
{
    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    std::map<UINT, CTaskData*>::iterator iter = task_map_.find(task_id);
    if (task_map_.end() == iter)
    {
        return;
    }

    delete iter->second;
    task_map_.erase(iter);
}

UINT CDataModuleImpl::GetNextPrepareTask()
{
    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    if (0 == task_map_.size())
    {
        return 0;
    }

    return task_map_.begin()->first;
}

UINT CDataModuleImpl::GenerateTaskID()
{
    next_task_id_++;

    if (0 == next_task_id_)
    {
        next_task_id_++;
    }

    CRITICAL_LOCK(task_map_lock, task_map_cs_);
    while (task_map_.find(next_task_id_) != task_map_.end())
    {
        next_task_id_++;
    }

    return next_task_id_;
}
