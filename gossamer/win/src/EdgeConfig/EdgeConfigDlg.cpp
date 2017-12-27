
// EdgeConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EdgeConfig.h"
#include "EdgeConfigDlg.h"
#include "afxdialogex.h"
#include "SuperNodeDlg.h"
#include "EdgeDlg.h"
#include "KeepLiveDlg.h"
#include "VncDlg.h"
#include "utils\Comm.h"
#include "EdgeBaseDefine.h"
#include <Winsvc.h>

BEGIN_MESSAGE_MAP(CEdgeConfigDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_Save, &CEdgeConfigDlg::OnBnClickedSave)
    ON_NOTIFY(TCN_SELCHANGE, IDC_ConfigSwitchTab, &CEdgeConfigDlg::OnTcnSelChangeConfigSwitch)
END_MESSAGE_MAP()

CEdgeConfigDlg::CEdgeConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDGECONFIG_DIALOG, pParent),
    dlg_super_node_(0),
    dlg_edge_(0),
    dlg_keeplive_(0),
    dlg_vnc_(0),
    restart_service_(false)
{
	icon_handle_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEdgeConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ConfigSwitchTab, tab_config_switch_);
}

BOOL CEdgeConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(icon_handle_, TRUE);
	SetIcon(icon_handle_, FALSE);

    tab_config_switch_.InsertItem(IDC_BtTabEdge, _T("客户端"));
    tab_config_switch_.InsertItem(IDC_BtTabSuperNode, _T("服  务"));
    tab_config_switch_.InsertItem(IDC_BtTabKeepLive, _T("心  跳"));
    tab_config_switch_.InsertItem(IDC_BtTabVNC, _T(" VNC "));

    InitSubDlg();

    MoveWindow(0, 0, 400, 340);
    CenterWindow();

    InstallVirtualLAN();

	return TRUE;
}

void CEdgeConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, icon_handle_);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CEdgeConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(icon_handle_);
}

void CEdgeConfigDlg::OnTcnSelChangeConfigSwitch(NMHDR *nmhdr, LRESULT *result)
{
    *result = 0;

    int current_select = tab_config_switch_.GetCurSel();
    switch (current_select)
    {
    case IDC_BtTabSuperNode:
        SwitchSubDlgVisibleStatus(IDD_SuperNodeDlg);
        break;

    case IDC_BtTabEdge:
        SwitchSubDlgVisibleStatus(IDD_EdgeDlg);
        break;

    case IDC_BtTabKeepLive:
        SwitchSubDlgVisibleStatus(IDD_KeepLiveDlg);
        break;

    case IDC_BtTabVNC:
        SwitchSubDlgVisibleStatus(IDD_VncDlg);
        break;

    default:
        break;
    }
}

void CEdgeConfigDlg::InitSubDlg()
{
    CString path;
    GetServiceConfigFile(path);

    TCHAR super_node_address[128];
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_SUPERNODE), _T(NAME_CONFIG_KEY_SUPERNODE_IP), _T(""), super_node_address, 128, path);
    USHORT super_node_port = GetPrivateProfileInt(_T(NAME_CONFIG_SECTION_SUPERNODE), _T(NAME_CONFIG_KEY_SUPERNODE_PORT), 0, path);
    dlg_super_node_ = new CSuperNodeDlg(super_node_address, super_node_port);
    if (FALSE == dlg_super_node_->Create(IDD_SuperNodeDlg, this))
    {
        delete dlg_super_node_;
        dlg_super_node_ = 0;
        return;
    }
    dlg_super_node_->ShowWindow(SW_HIDE);

    TCHAR edge_addr[128];
    TCHAR edge_interface[64];
    TCHAR edge_group_name[64];
    TCHAR edge_password[64];
    TCHAR edge_mac[64];
    TCHAR mask[16];
    int mtu = GetPrivateProfileInt(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_MTU), 1400, path);
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_IP), _T(""), edge_addr, 128, path);
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_INTERFACE), _T(""), edge_interface, 64, path);
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_GROUPNAME), _T(""), edge_group_name, 64, path);
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_PASSWORD), _T(""), edge_password, 64, path);
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_MAC_ADDR), _T(""), edge_mac, 64, path);
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_MASK), _T("255.255.255.0"), mask, 16, path);
    dlg_edge_ = new CEdgeDlg(edge_addr, edge_interface, edge_group_name, edge_password, edge_mac, mask, mtu);
    if (FALSE == dlg_edge_->Create(IDD_EdgeDlg, this))
    {
        delete dlg_edge_;
        dlg_edge_ = 0;
        return;
    }
    dlg_edge_->ShowWindow(SW_SHOW);

    TCHAR keeplive_addr[128];
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_PING), _T(NAME_CONFIG_KEY_PING_DSTADDR), _T(""), keeplive_addr, 128, path);
    UINT keeplive_interval = GetPrivateProfileInt(_T(NAME_CONFIG_SECTION_PING), _T(NAME_CONFIG_KEY_PING_INTERVAL), 0, path);
    dlg_keeplive_ = new CKeepLiveDlg(keeplive_addr, keeplive_interval);
    if (FALSE == dlg_keeplive_->Create(IDD_KeepLiveDlg, this))
    {
        delete dlg_keeplive_;
        dlg_keeplive_ = 0;
        return;
    }
    dlg_keeplive_->ShowWindow(SW_HIDE);

    TCHAR vnc_path[1024];
    GetPrivateProfileString(_T(NAME_CONFIG_SECTION_VNC), _T(NAME_CONFIG_KEY_VNC_PATH), _T(""), vnc_path, 1024, path);
    dlg_vnc_ = new CVncDlg(vnc_path);
    if (FALSE == dlg_vnc_->Create(IDD_VncDlg, this))
    {
        delete dlg_vnc_;
        dlg_vnc_ = 0;
        return;
    }
    dlg_vnc_->ShowWindow(SW_HIDE);
}

void CEdgeConfigDlg::SwitchSubDlgVisibleStatus(UINT id)
{
    if (dlg_super_node_)
    {
        dlg_super_node_->ShowWindow((IDD_SuperNodeDlg == id) ? SW_SHOW : SW_HIDE);
    }

    if (dlg_edge_)
    {
        dlg_edge_->ShowWindow((IDD_EdgeDlg == id) ? SW_SHOW : SW_HIDE);
    }

    if (dlg_keeplive_)
    {
        dlg_keeplive_->ShowWindow((IDD_KeepLiveDlg == id) ? SW_SHOW : SW_HIDE);
    }

    if (dlg_vnc_)
    {
        dlg_vnc_->ShowWindow((IDD_VncDlg == id) ? SW_SHOW : SW_HIDE);
    }
}

void CEdgeConfigDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    CRect sub_dlg_rect(0, 0, cx, cy);
    if (IsWindow(tab_config_switch_.m_hWnd))
    {
        CRect wnd_client;
        GetClientRect(&wnd_client);

        tab_config_switch_.GetWindowRect(&sub_dlg_rect);
        ScreenToClient(&sub_dlg_rect);
        sub_dlg_rect.right = wnd_client.Width() - 2 * sub_dlg_rect.left;
        sub_dlg_rect.top = sub_dlg_rect.bottom;
        sub_dlg_rect.bottom = wnd_client.Height() - sub_dlg_rect.top - 30;
    }

    if (dlg_super_node_ && IsWindow(dlg_super_node_->m_hWnd))
    {
        dlg_super_node_->MoveWindow(&sub_dlg_rect);
    }

    if (dlg_edge_ && IsWindow(dlg_edge_->m_hWnd))
    {
        dlg_edge_->MoveWindow(&sub_dlg_rect);
    }

    if (dlg_keeplive_ && IsWindow(dlg_keeplive_->m_hWnd))
    {
        dlg_keeplive_->MoveWindow(&sub_dlg_rect);
    }

    if (dlg_vnc_ && IsWindow(dlg_vnc_->m_hWnd))
    {
        dlg_vnc_->MoveWindow(&sub_dlg_rect);
    }

    CWnd* bt = GetDlgItem(IDC_Save);
    if (NULL == bt || FALSE == IsWindow(bt->m_hWnd))
    {
        return;
    }

    CRect bt_rect;
    bt->GetClientRect(&bt_rect);

    sub_dlg_rect.left = sub_dlg_rect.right - bt_rect.Width();
    sub_dlg_rect.top = sub_dlg_rect.bottom + 20;
    sub_dlg_rect.bottom = sub_dlg_rect.top + bt_rect.Height();

    bt->MoveWindow(&sub_dlg_rect);
}

void CEdgeConfigDlg::OnBnClickedSave()
{
    CString super_node_address;
    CString super_node_port = 0;
    if (dlg_super_node_ && IsWindow(dlg_super_node_->m_hWnd))
    {
        dlg_super_node_->UpdateData(TRUE);

        super_node_address = dlg_super_node_->GetAddress();
        super_node_port.Format(_T("%u"), dlg_super_node_->GetPort());
    }

    CString edge_addr(_T(""));
    CString edge_interface(_T(""));
    CString edge_group_name(_T(""));
    CString edge_password(_T(""));
    CString edge_mac(_T(""));
    CString edge_mask(_T(""));
    CString edge_mtu(_T(""));
    if (dlg_edge_ && IsWindow(dlg_edge_->m_hWnd))
    {
        dlg_edge_->UpdateData(TRUE);

        edge_addr = dlg_edge_->GetAddress();
        edge_interface = dlg_edge_->GetInterface();
        edge_group_name = dlg_edge_->GetGroupName();
        edge_password = dlg_edge_->GetPassword();
        edge_mac = dlg_edge_->GetMacAddr();
        edge_mask = dlg_edge_->GetMask();
        edge_mtu.Format(_T("%u"), dlg_edge_->GetMTU());
    }

    CString keeplive_addr(_T(""));
    CString keeplive_interval = 0;
    if (dlg_keeplive_ && IsWindow(dlg_keeplive_->m_hWnd))
    {
        dlg_keeplive_->UpdateData(TRUE);

        keeplive_addr = dlg_keeplive_->GetAddress();
        keeplive_interval.Format(_T("%u"), dlg_keeplive_->GetInterval());
    }

    CString vnc_path(_T(""));
    if (dlg_vnc_ && IsWindow(dlg_vnc_->m_hWnd))
    {
        dlg_vnc_->UpdateData(TRUE);

        vnc_path = dlg_vnc_->GetVNCPath();
    }

    CString error_text(_T(""));
    UINT dlg_id = -1;
    UINT bt_id = -1;

    CheckStringValid(edge_addr, dlg_id, IDD_EdgeDlg, bt_id, IDC_BtTabEdge, error_text, _T("[客户端地址]不允许为空"));
    CheckStringValid(edge_interface, dlg_id, IDD_EdgeDlg, bt_id, IDC_BtTabEdge, error_text, _T("[客户端接口名]不允许为空"));
    CheckStringValid(edge_group_name, dlg_id, IDD_EdgeDlg, bt_id, IDC_BtTabEdge, error_text, _T("[客户端组名]不允许为空"));
    CheckStringValid(edge_password, dlg_id, IDD_EdgeDlg, bt_id, IDC_BtTabEdge, error_text, _T("[客户端密码]不允许为空"));

    CheckStringValid(super_node_address, dlg_id, IDD_SuperNodeDlg, bt_id, IDC_BtTabSuperNode, error_text, _T("[服务器地址]不允许为空"));
    CheckNumberValid(super_node_port, dlg_id, IDD_SuperNodeDlg, bt_id, IDC_BtTabSuperNode, error_text, _T("[服务器端口]必须有效"));

    CheckStringValid(keeplive_addr, dlg_id, IDD_KeepLiveDlg, bt_id, IDC_BtTabKeepLive, error_text, _T("[心跳地址]不允许为空"));
    CheckNumberValid(keeplive_interval, dlg_id, IDD_KeepLiveDlg, bt_id, IDC_BtTabKeepLive, error_text, _T("[心跳间隔]必须有效"));
    
    //CheckStringValid(vnc_path, dlg_id, IDD_VncDlg, bt_id, IDC_BtTabVNC, error_text, _T("[VNC路径]不允许为空"));
    if (error_text.GetLength())
    {
        if (-1 != dlg_id)
        {
            tab_config_switch_.SetCurSel(bt_id);
            SwitchSubDlgVisibleStatus(dlg_id);
        }

        MessageBox(error_text, _T("错误"), MB_OK | MB_ICONERROR);
        return;
    }

    CString file;
    GetServiceConfigFile(file);

    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_SUPERNODE), _T(NAME_CONFIG_KEY_SUPERNODE_IP), super_node_address, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_SUPERNODE), _T(NAME_CONFIG_KEY_SUPERNODE_PORT), super_node_port, file);
    
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_IP), edge_addr, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_INTERFACE), edge_interface, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_GROUPNAME), edge_group_name, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_PASSWORD), edge_password, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_MAC_ADDR), edge_mac, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_MTU), edge_mtu, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_EDGE), _T(NAME_CONFIG_KEY_EDGE_MASK), edge_mask, file);

    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_PING), _T(NAME_CONFIG_KEY_PING_DSTADDR), keeplive_addr, file);
    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_PING), _T(NAME_CONFIG_KEY_PING_INTERVAL), keeplive_interval, file);

    WritePrivateProfileString(_T(NAME_CONFIG_SECTION_VNC), _T(NAME_CONFIG_KEY_VNC_PATH), vnc_path, file);

    restart_service_ = true;

    MessageBox(_T("配置信息保存完毕！"), _T("成功"), MB_OK);
}

void CEdgeConfigDlg::GetServiceConfigFile(CString& file)
{
    std::string path;
    if (0 == Comm::GetModulePath(path))
    {
        path = "C:";
    }

    size_t pos = path.rfind('\\');
    if (std::string::npos == pos)
    {
        path = "C:";
    }
    else
    {
        path.resize(pos);
    }

    path += "/";
    path += NAME_CONFIG_FILE;

    file = ATL::CA2T(path.c_str());
}

void CEdgeConfigDlg::InstallVirtualLAN()
{
    std::string path;
    if (0 == Comm::GetModulePath(path))
    {
        return;
    }

    size_t pos = path.rfind('\\');
    if (std::string::npos == pos)
    {
        return;
    }

    path.resize(pos);
    path += "\\";
    path += "tap-windows.exe";

    WIN32_FIND_DATAA find_data;
    ZeroMemory(&find_data, sizeof(find_data));
    HANDLE find_file = FindFirstFileA(path.c_str(), &find_data);
    if (INVALID_HANDLE_VALUE == find_file)
    {
        return;
    }
    FindClose(find_file);

    STARTUPINFOA startup_info;
    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);

    PROCESS_INFORMATION process_info;
    ZeroMemory(&process_info, sizeof(process_info));

    std::string cmd = path + " /S /SELECT_UTILITIES=1";
    if (FALSE == CreateProcessA(path.c_str(), const_cast<char*>(cmd.c_str()), 0, 0, FALSE, 0, NULL, 0, &startup_info, &process_info))
    {
        return;
    }

    WaitForSingleObject(process_info.hProcess, INFINITE);
    DeleteFileA(path.c_str());
}

void CEdgeConfigDlg::RestartService()
{
    SC_HANDLE sc_manager_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (sc_manager_handle == NULL)
    {
        return;
    }

    SC_HANDLE sc_service_handle = OpenServiceA(sc_manager_handle, NAME_SERVICE, SERVICE_STOP | SERVICE_START | SERVICE_QUERY_STATUS);
    if (sc_service_handle == NULL)
    {
        CloseServiceHandle(sc_manager_handle);
        return;
    }

    SERVICE_STATUS status;
    ControlService(sc_service_handle, SERVICE_CONTROL_STOP, &status);

    SERVICE_STATUS service_status;
    while (QueryServiceStatus(sc_service_handle, &service_status))
    {
        if (SERVICE_STOPPED == service_status.dwCurrentState)
        {
            StartService(sc_service_handle, 0, 0);
            break;
        }
    }

    CloseServiceHandle(sc_service_handle);
    CloseServiceHandle(sc_manager_handle);
}

void CEdgeConfigDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    if (restart_service_)
    {
        RestartService();
    }
}

void CEdgeConfigDlg::CheckStringValid(const CString& dst, UINT& error_dlg_id, UINT cur_dlg_id, UINT& error_bt_id, UINT cur_bt_id, CString& error_tips, const CString& cur_error_text)
{
    if (dst.GetLength())
    {
        return;
    }

    if (-1 == error_dlg_id)
    {
        error_dlg_id = cur_dlg_id;
        error_bt_id = cur_bt_id;
    }

    error_tips += cur_error_text;
    error_tips += "\n";
}

void CEdgeConfigDlg::CheckNumberValid(const CString& dst, UINT& error_dlg_id, UINT cur_dlg_id, UINT& error_bt_id, UINT cur_bt_id, CString& error_tips, const CString& cur_error_text)
{
    if (dst.GetLength() && 0 != dst.Compare(_T("0")))
    {
        return;
    }

    if (-1 == error_dlg_id)
    {
        error_dlg_id = cur_dlg_id;
    }

    error_tips += cur_error_text;
    error_tips += "\n";
}
