
#ifndef _TASK_DATA_H_
#define _TASK_DATA_H_

#include <string>
#include <Windows.h>

class CTaskData
{
public:
    CTaskData(UINT task_id = 0, const std::string& server_addr = "");
    CTaskData(const CTaskData& task_data);
    ~CTaskData();

    CTaskData& operator=(const CTaskData& task_data);

    UINT GetTaskID() const;
    void SetTaskID(UINT task_id);

    const std::string& GetServerAddr() const;
    void SetServerAddr(const std::string& server_addr);

private:
    UINT task_id_;
    std::string server_addr_;
};

#endif // !_TASK_DATA_H_
