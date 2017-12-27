
#ifndef _DATA_MODULE_H_
#define _DATA_MODULE_H_

#include <map>
#include "TaskData.h"
#include "utils\Singleton.h"
#include "utils\CriticalSection.h"

class CDataModuleImpl
{
private:
    CDataModuleImpl();
    ~CDataModuleImpl();

    friend CSingleton<CDataModuleImpl>;

public:
    BOOL AddTask(const CTaskData& task_data);
    BOOL GetTask(UINT task_id, CTaskData& task_data);
    void SetTask(const CTaskData& task_data);
    void RemoveTask(UINT task_id);

    UINT GetNextPrepareTask();

    UINT GenerateTaskID();

private:
    CCriticalSectionEx task_map_cs_;
    std::map<UINT, CTaskData*> task_map_;

    UINT next_task_id_;
};

typedef CSingleton<CDataModuleImpl> CDataModule;

#endif // !_DATA_MODULE_H_
