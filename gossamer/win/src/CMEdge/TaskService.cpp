
#include "TaskService.h"
#include "DataModule.h"
#include "utils\Comm.h"
#include "StartupVpnProcessor.h"
#include "GetVersionProcessor.h"

CTaskService::CTaskService()
    :gsoap_(0),
    startup_vpn_processor_(0),
    get_version_processor_(0)
{
}

CTaskService::~CTaskService()
{
}

bool CTaskService::Initialize()
{
    if (0 == startup_vpn_processor_)
    {
        startup_vpn_processor_ = new CStartupVpnProcessor(this);
    }

    if (0 == get_version_processor_)
    {
        get_version_processor_ = new CGetVersionProcessor;
    }

    if (0 == gsoap_)
    {
        gsoap_ = new CGSoap();
    }

    gsoap_->AddProcessor(NAME_PROCESSOR_STARTUPVNC, startup_vpn_processor_);
    gsoap_->AddProcessor(NAME_PROCESSOR_GETVERSION, get_version_processor_);
    if (false == gsoap_->Initialize(11502))
    {
        return false;
    }

    return ((0 == OpenThread()) ? false : true);
}

void CTaskService::Uninitialize()
{
    if (gsoap_)
    {
        gsoap_->Uninitialize();
        delete gsoap_;
        gsoap_ = 0;
    }

    CloseThread();

    if (startup_vpn_processor_)
    {
        delete startup_vpn_processor_;
        startup_vpn_processor_ = 0;
    }
}

void CTaskService::SetConfigFile(const std::string& vnc_file)
{
    CRITICAL_LOCK(config_file_lock, vnc_file_cs_);

    vnc_file_ = vnc_file;
}

int CTaskService::Scheduler(CThreadExcuteData* thread_data)
{
    return 0;
    UINT task_id = CDataModule::Instance()->GetNextPrepareTask();
    if (0 == task_id)
    {
        return 0;
    }

    CTaskData task_data;
    if (FALSE == CDataModule::Instance()->GetTask(task_id, task_data))
    {
        return 0;
    }
    CDataModule::Instance()->RemoveTask(task_id);

    std::string vnc_file;
    {
        CRITICAL_LOCK(config_file_lock, vnc_file_cs_);
        vnc_file = vnc_file_;
    }

    STARTUPINFOA startup_info;
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags = STARTF_USESHOWWINDOW;
    startup_info.wShowWindow = SW_SHOWNORMAL;

    PROCESS_INFORMATION process_info;
    ZeroMemory(&process_info, sizeof(process_info));
    if (FALSE == CreateProcessA(vnc_file.c_str(), 0, 0, 0, 0, 0, 0, 0, &startup_info, &process_info))
    {
        return 0;
    }

    HWND hwnd = 0;
    UINT times = 0;
    do
    {
        hwnd = FindWindowExA(0, 0, "#32770", "VNC Viewer : Connection Details");
        while (IsWindow(hwnd))
        {
            DWORD pid = 0;
            GetWindowThreadProcessId(hwnd, &pid);
            if (process_info.dwProcessId == pid)
            {
                break;
            }

            hwnd = FindWindowExA(0, hwnd, "#32770", "VNC Viewer : Connection Details");
        }
        times++;

        Sleep(1);
    } while (NULL == hwnd && times < 30*1000);

    if (NULL == hwnd)
    {
        return 0;
    }

    HWND combobox_wnd = FindSubWnd(hwnd, 1002);
    if (FALSE == IsWindow(combobox_wnd))
    {
        return 0;
    }

    HWND dst_wnd = FindSubWnd(combobox_wnd, 1001);
    if (FALSE == IsWindow(dst_wnd))
    {
        return 0;
    }

    std::wstring addr;
    Comm::AscToUnicode(task_data.GetServerAddr(), addr);
    for (size_t i = 0; i < addr.length(); i++)
    {
        PostMessage(dst_wnd, WM_CHAR, addr.at(i), 0);
    }

    HWND bt_wnd = FindSubWnd(hwnd, 1);
    if (FALSE == IsWindow(bt_wnd))
    {
        return 0;
    }

    RECT button_rect;
    GetWindowRect(bt_wnd, &button_rect);

    POINT click_pos;
    click_pos.x = (button_rect.right - button_rect.left) / 2;
    click_pos.y = (button_rect.bottom - button_rect.top) / 2;
    PostMessage(bt_wnd, WM_LBUTTONDOWN, 0, MAKELPARAM(click_pos.x, click_pos.y));
    PostMessage(bt_wnd, WM_LBUTTONUP, 0, MAKELPARAM(click_pos.x, click_pos.y));

    BringWindowToTop(hwnd);

    return 0;
}

HWND CTaskService::FindSubWnd(HWND parent, UINT id)
{
    UINT times = 0;
    HWND hwnd = GetDlgItem(parent, id);

    while(NULL == hwnd && times < 30 * 1000)
    {
        Sleep(1);
        hwnd = GetDlgItem(parent, id);
        times++;
    }

    return hwnd;
}
