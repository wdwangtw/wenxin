#include "stdafx.h"
#include "PingAgent.h"
#include <time.h>
#include <sstream>

CPingAgent::CPingAgent()
    :interval_time_(0),
    wait_event_(0)
{
}

CPingAgent::~CPingAgent()
{
}

void CPingAgent::Start(UINT interval_time, const std::string& dst_address)
{
    if (NULL == wait_event_)
    {
        wait_event_ = CreateEvent(0, TRUE, FALSE, 0);
    }

    interval_time_ = interval_time;
    dst_address_ = dst_address;

    OpenThread();
}

void CPingAgent::Stop()
{
    SetEvent(wait_event_);
    CloseThread();

    if (wait_event_)
    {
        CloseHandle(wait_event_);
        wait_event_ = 0;
    }
}

int CPingAgent::Scheduler(CThreadExcuteData* thread_data)
{
    const DWORD wait_result = WaitForSingleObject(wait_event_, interval_time_ * 1000);
    if (WAIT_OBJECT_0 == wait_result)
    {
        return -1;
    }

    if (WAIT_TIMEOUT != wait_result)
    {
        return -1;
    }

    std::string text = "";
    //{
    //    char time_buff[1024];
    //    ZeroMemory(time_buff, sizeof(time_buff));

    //    time_t current_time = thread_data->GetCurTime();
    //    
    //    strftime(time_buff, 1024, "--------------------- %F %T ---------------------\n", localtime(&current_time));

    //    text = time_buff;
    //}

    if (FALSE == DoPing(text))
    {
        return 0;
    }
    text += _T("\r\n");

    return 0;
}

BOOL CPingAgent::DoPing(std::string& out)
{
    std::string cmd("ping ");
    cmd += dst_address_;

    //HANDLE pipe_read_handle = NULL;
    //HANDLE pipe_write_handle = NULL;

    //SECURITY_ATTRIBUTES security_attributes;
    //ZeroMemory(&security_attributes, sizeof(SECURITY_ATTRIBUTES));
    //security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    //security_attributes.bInheritHandle = TRUE;
    //// 创建管道
    //if (FALSE == CreatePipe(&pipe_read_handle, &pipe_write_handle, &security_attributes, 0))
    //{
    //    return FALSE;
    //}

    BOOL result = FALSE;
    PROCESS_INFORMATION process_infomation;
    ZeroMemory(&process_infomation, sizeof(process_infomation));

    do
    {
        // 控制命令行窗口信息
        STARTUPINFO startup_info;
        startup_info.cb = sizeof(STARTUPINFO);
        GetStartupInfo(&startup_info);
        startup_info.hStdError = 0;
        startup_info.hStdOutput = 0;
        startup_info.wShowWindow = SW_HIDE;  // 隐藏命令行窗口
        startup_info.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        // 创建获取命令行进程
        result = CreateProcessA(0, const_cast<LPSTR>(cmd.c_str()), 0, 0, TRUE, 0, 0, 0, &startup_info, &process_infomation);
        if (FALSE == result)
        {
            break;
        }

        WaitForSingleObject(process_infomation.hProcess, INFINITE);

        result = TRUE;
    } while (FALSE);

    if (process_infomation.hThread)
    {
        CloseHandle(process_infomation.hThread);
    }

    if (process_infomation.hProcess)
    {
        CloseHandle(process_infomation.hProcess);
    }

    return result;
}
