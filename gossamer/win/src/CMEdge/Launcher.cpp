
#include "Launcher.h"
#include "ServiceManager.h"
#include "EdgeBaseDefine.h"
#include <algorithm>    // transform
#include <functional>   // not1¡¢ptr_fun

void CLauncher::Run()
{
#ifndef RUN_AS_SERVICE
    RunAsCmdLine();
#else
    RunAsService();
#endif
}

void CLauncher::RunAsCmdLine()
{
    CServer server_;
    server_.Run();
}

void CLauncher::RunAsService()
{
    CServiceManager service_manager;

    std::string cmd_line = GetCommandLine();
    LOG_T("cmd line: %s", cmd_line.c_str());

    cmd_line.erase(std::find_if(cmd_line.rbegin(), cmd_line.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), cmd_line.end());
    std::transform(cmd_line.begin(), cmd_line.end(), cmd_line.begin(), tolower);

    int pos = cmd_line.rfind(KEYWORD_INSTALL);
    if (-1 != pos && strlen(KEYWORD_INSTALL) == cmd_line.substr(pos).length())
    {
        service_manager.Install();
        return;
    }

    pos = cmd_line.rfind(KEYWORD_UNINSTALL);
    if (-1 != pos && strlen(KEYWORD_UNINSTALL) == cmd_line.substr(pos).length())
    {
        service_manager.Uninstall();
        return;
    }

    SERVICE_TABLE_ENTRY service_table[] =
    {
        { NAME_SERVICE, (LPSERVICE_MAIN_FUNCTION)CServiceManager::ServiceMain },
        { 0, 0 }
    };

    if (!::StartServiceCtrlDispatcher(service_table))
    {
        LOG_E("Start service failed. error_code=[%u]", GetLastError());
        return;
    }
}
