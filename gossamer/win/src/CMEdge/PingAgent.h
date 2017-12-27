
#ifndef _AGENT_H_
#define _AGENT_H_

#include "utils\ThreadService.h"
#include <Windows.h>
#include <string>

#define _CM_EventError      WM_USER + 1024
#define _CM_PingResult      WM_USER + 1025

class CPingAgent : public CThreadService
{
public:
    CPingAgent();
    ~CPingAgent();

    void Start(UINT interval_time, const std::string& dst_address);
    void Stop();

protected:
    virtual int Scheduler(CThreadExcuteData* thread_data);

    BOOL DoPing(std::string& out);

private:
    UINT interval_time_;
    std::string dst_address_;
    HANDLE wait_event_;
};

#endif //!_AGENT_H_
