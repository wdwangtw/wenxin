
#include "StartupVpnProcessor.h"
#include "DataModule.h"
#include "utils\LogService.h"
#include "utils\cJSON.h"
#include "utils\Comm.h"
#include "TaskService.h"

CStartupVpnProcessor::CStartupVpnProcessor(void* task_service)
    :task_service_(task_service)
{
}

CStartupVpnProcessor::~CStartupVpnProcessor()
{
}

int CStartupVpnProcessor::Dispatch(const std::string& request, std::string& response)
{
    std::string address = GetKeyValueAsStr(request, GSOAP_FIELD_VNCSTARTUPPARAM);

    EErrorCode error_code = EC_Succeed;
    do
    {
        if (0 == address.length())
        {
            error_code = EC_InvalidParams;
            break;
        }

        if (0 == CheckVNCValid())
        {
            error_code = EC_InvalidVNC;
            break;
        }

        CTaskData data;
        data.SetTaskID(CDataModule::Instance()->GenerateTaskID());
        data.SetServerAddr(address);

        CDataModule::Instance()->AddTask(data);
    } while (false);

    GenerateResponse(error_code, response);
    PackJSONP(request, response);

    LOG_T("new request. result=[%u] address=[%s]", error_code, address.c_str());
    return error_code;
}

void CStartupVpnProcessor::GenerateResponse(EErrorCode error_code, std::string& response)
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "result", error_code);

    char* data = cJSON_Print(root);

    response = data;

    free(data);
    cJSON_Delete(root);
}

bool CStartupVpnProcessor::CheckVNCValid()
{
    std::string file;
    if (false == GetConfigFile(file))
    {
        return false;
    }

    char buff[1024];
    memset(buff, 0, sizeof(buff));
    if (0 == GetPrivateProfileStringA("vnc", "path", "", buff, 1023, file.c_str()))
    {
        return false;
    }

    WIN32_FIND_DATAA find_data;
    HANDLE file_find_handle = FindFirstFileA(buff, &find_data);
    if (INVALID_HANDLE_VALUE == file_find_handle)
    {
        return false;
    }

    FindClose(file_find_handle);

    reinterpret_cast<CTaskService*>(task_service_)->SetConfigFile(buff);
    return true;
}

bool CStartupVpnProcessor::GetConfigFile(std::string& file)
{
    GetModulePath(file, 0);
    size_t pos = file.rfind('\\');
    if (-1 == pos)
    {
        return FALSE;
    }

    file.resize(pos + 1);
    file += NAME_CONFIG_FILE;

    return TRUE;
}


DWORD CStartupVpnProcessor::GetModulePath(std::string& file_name, HMODULE handle)
{
    char* file = new char[MAX_PATH + 1];
    ZeroMemory(file, MAX_PATH + 1);

    DWORD len = 0;
    do
    {
        len = ::GetModuleFileNameA(handle, file, MAX_PATH);
        if (0 == len)
        {
            break;
        }

        if (MAX_PATH >= len)
        {
            break;
        }

        delete[]file;
        file = new char[len + 1];
        ZeroMemory(file, len + 1);

        len = ::GetModuleFileNameA(handle, file, len);

    } while (false);

    if (len)
    {
        file_name = file;
    }
    delete[]file;

    return len;
}
