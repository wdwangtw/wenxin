
#ifndef _TASK_SERVICE_H_
#define _TASK_SERVICE_H_

#include "utils\ThreadService.h"
#include "gsoap\gsoap.h"

class CStartupVpnProcessor;
class CGetVersionProcessor;

class CTaskService : public CThreadService
{
public:
    CTaskService();
    virtual ~CTaskService();

    bool Initialize();
    void Uninitialize();

    void SetConfigFile(const std::string& vnc_file);

protected:
    virtual int Scheduler(CThreadExcuteData* thread_data);

    HWND FindSubWnd(HWND parent, UINT id);

private:
    CGSoap* gsoap_;
    CStartupVpnProcessor* startup_vpn_processor_;
    CGetVersionProcessor* get_version_processor_;

    CCriticalSectionEx vnc_file_cs_;
    std::string vnc_file_;
};

#endif // !_TASK_SERVICE_H_
