
#ifndef _SERVER_H_
#define _SERVER_H_

#include "Config.h"
#include "CMEdgeAdapter.h"
#include "PingAgent.h"
#include "TaskService.h"
#include <Windows.h>

class CServer
{
public:
    CServer();
    ~CServer();

    void Run();

private:
    bool Initialize();
#ifdef _DEBUG
public:
#endif // _DEBUG
    void Uninitialize();
#ifdef _DEBUG
private:
#endif // _DEBUG

    void LoadConfig(CConfig& config);
    bool InitLogService(const CConfig& config);
    bool InitEdgeAdapter(const CConfig& config);
    bool InitPingAgent(const CConfig& config);
    bool InitTaskService(const CConfig& config);

private:
    HANDLE exit_event_;
    CCMEdgeAdapter edge_adapter_;
    CPingAgent ping_agent_;
    CTaskService task_service_;
};

#endif //!_SERVER_H_
