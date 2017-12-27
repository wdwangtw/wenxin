
#include "ServiceManager.h"
#include "EdgeBaseDefine.h"
#include "utils\Comm.h"

SERVICE_STATUS_HANDLE CServiceManager::service_ctrl_handle_ = 0;

CServiceManager::CServiceManager()
{
}

CServiceManager::~CServiceManager()
{
}

void WINAPI CServiceManager::ServiceMain()
{
    SERVICE_STATUS service_status;
    ZeroMemory(&service_status, sizeof(SERVICE_STATUS));

    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
    service_status.dwCurrentState = SERVICE_START_PENDING;
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    //注册服务控制
    service_ctrl_handle_ = RegisterServiceCtrlHandler(NAME_SERVICE, ServiceCtrl);
    if (service_ctrl_handle_ == NULL)
    {
        LOG_E("register service control handler failed. error_code=[%u]", GetLastError());
        return;
    }
    SetServiceStatus(service_ctrl_handle_, &service_status);

    service_status.dwWin32ExitCode = S_OK;
    service_status.dwCheckPoint = 0;
    service_status.dwWaitHint = 0;
    service_status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(service_ctrl_handle_, &service_status);

    LOG_T("service running.");

    CServer server;
    server.Run();
    
    service_status.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(service_ctrl_handle_, &service_status);
}

void WINAPI CServiceManager::ServiceCtrl(DWORD dwOpcode)
{
    SERVICE_STATUS service_status;
    ZeroMemory(&service_status, sizeof(SERVICE_STATUS));

    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
    {
        HANDLE exit_event = CreateEvent(0, TRUE, FALSE, NAME_EVENT_SERVICEEXIT);
        SetEvent(exit_event);

        service_status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(service_ctrl_handle_, &service_status);
        LOG_T("service stoped.");
    }
        break;

    case SERVICE_CONTROL_PAUSE:
        break;

    case SERVICE_CONTROL_CONTINUE:
        break;

    case SERVICE_CONTROL_INTERROGATE:
        break;

    case SERVICE_CONTROL_SHUTDOWN:
    {
        HANDLE exit_event = CreateEvent(0, TRUE, FALSE, NAME_EVENT_SERVICEEXIT);
        SetEvent(exit_event);

        service_status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(service_ctrl_handle_, &service_status);
        LOG_T("service shutdown.");
    }
        break;

    default:
        break;
    }
}

bool CServiceManager::IsInstalled()
{
    bool result = false;

    //打开服务控制管理器  
    SC_HANDLE sc_manager_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager_handle == NULL)
    {
        LOG_E("open service control manager failed. error_code=[%u]", GetLastError());
        return false;
    }

    //打开服务  
    SC_HANDLE sc_service_handle = OpenService(sc_manager_handle, NAME_SERVICE, SERVICE_QUERY_CONFIG);
    if (sc_service_handle != NULL)
    {
        result = true;
    }

    DWORD error = GetLastError();

    CloseServiceHandle(sc_service_handle);
    CloseServiceHandle(sc_manager_handle);

    return result;
}

bool CServiceManager::Install()
{
    if (IsInstalled())
    {
        LOG_D("service has installed.")
        return true;
    }

    //打开服务控制管理器  
    SC_HANDLE sc_manager_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager_handle == NULL)
    {
        LOG_E("open service control manager failed. error_code=[%u]", GetLastError());
        return false;
    }

    std::string binary_path_name;
    if (0 == Comm::GetModulePath(binary_path_name))
    {
        LOG_E("get module path failed. error_code=[%u]", GetLastError());
        CloseServiceHandle(sc_manager_handle);
        return false;
    }

    //创建服务  
    SC_HANDLE sc_service_handle = CreateService(sc_manager_handle, NAME_SERVICE, NAME_SERVICE,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, binary_path_name.c_str(), NULL, NULL, "", NULL, NULL);

    if (sc_service_handle == NULL)
    {
        LOG_E("create service failed. error_code=[%u]", GetLastError());
        CloseServiceHandle(sc_manager_handle);
        return false;
    }

    CloseServiceHandle(sc_service_handle);
    CloseServiceHandle(sc_manager_handle);

    LOG_T("install service succeed.");
    return true;
}

bool CServiceManager::Uninstall()
{
    if (!IsInstalled())
    {
        LOG_D("service not installed.")
        return true;
    }

    SC_HANDLE sc_manager_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager_handle == NULL)
    {
        LOG_E("open service control manager failed. error_code=[%u]", GetLastError());
        return false;
    }

    SC_HANDLE sc_service_handle = OpenService(sc_manager_handle, NAME_SERVICE, SERVICE_STOP | DELETE);

    if (sc_service_handle == NULL)
    {
        LOG_E("open service failed. error_code=[%u]", GetLastError());
        CloseServiceHandle(sc_manager_handle);
        return false;
    }

    SERVICE_STATUS status;
    ControlService(sc_service_handle, SERVICE_CONTROL_STOP, &status);

    //删除服务  
    BOOL delete_result = ::DeleteService(sc_service_handle);
    CloseServiceHandle(sc_service_handle);
    CloseServiceHandle(sc_manager_handle);

    LOG_E("uninstall service succeed.");

    if (delete_result)
    {
        return true;
    }

    return false;
}
