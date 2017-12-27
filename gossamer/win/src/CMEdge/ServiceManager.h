
#ifndef _SERVICE_MANAGER_H_
#define _SERVICE_MANAGER_H_

#include <Windows.h>
#include <string>
#include "Server.h"
#include "utils\LogService.h"

class CServiceManager
{
public:
    CServiceManager();
    ~CServiceManager();

    bool Install();

    bool Uninstall();

    static void WINAPI ServiceMain();

private:
    static void WINAPI ServiceCtrl(DWORD dwOpcode);

    bool IsInstalled();

private:
    static SERVICE_STATUS_HANDLE service_ctrl_handle_;
};

#endif // !_SERVICE_MANAGER_H_
