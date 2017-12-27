
#ifndef _STARTUP_VPN_PROCESSOR_H_
#define _STARTUP_VPN_PROCESSOR_H_

#include "ProtocolProcessor.h"
#include "EdgeBaseDefine.h"
#include "utils\cJSON.h"
#include <string>

class CStartupVpnProcessor : public CProtocolProcessor
{
public:
    CStartupVpnProcessor(void* task_service);
    ~CStartupVpnProcessor();

    virtual int Dispatch(const std::string& request, std::string& response);

protected:
    void GenerateResponse(EErrorCode error_code, std::string& response);

private:
    bool CheckVNCValid();
    bool GetConfigFile(std::string& file);
    DWORD GetModulePath(std::string& file_name, HMODULE handle);

private:
    void* task_service_;
};

#endif // !_STARTUP_VPN_PROCESSOR_H_
