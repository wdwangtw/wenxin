
#include "TaskData.h"

CTaskData::CTaskData(UINT task_id, const std::string& server_addr)
    :task_id_(task_id),
    server_addr_(server_addr)
{
}

CTaskData::CTaskData(const CTaskData& task_data)
    :task_id_(task_data.task_id_),
    server_addr_(task_data.server_addr_)
{
}

CTaskData::~CTaskData()
{
}

CTaskData& CTaskData::operator=(const CTaskData& task_data)
{
    task_id_ = task_data.task_id_;
    server_addr_ = task_data.server_addr_;

    return *this;
}

UINT CTaskData::GetTaskID() const
{
    return task_id_;
}

void CTaskData::SetTaskID(UINT task_id)
{
    task_id_ = task_id;
}

const std::string& CTaskData::GetServerAddr() const
{
    return server_addr_;
}

void CTaskData::SetServerAddr(const std::string& server_addr)
{
    server_addr_ = server_addr;
}
