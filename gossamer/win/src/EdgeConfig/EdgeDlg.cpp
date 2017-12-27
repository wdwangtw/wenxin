// EdgeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EdgeConfig.h"
#include "EdgeDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CEdgeDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CEdgeDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

CEdgeDlg::CEdgeDlg(const CString& address, const CString& interface_name, const CString& group_name, const CString& password, const CString& mac_addr, const CString& mask, int mtu, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EdgeDlg, pParent)
    , address_(address)
    , interface_(interface_name)
    , group_name_(group_name)
    , password_(password)
    , mac_addr_(mac_addr)
    , mask_(mask)
    , mtu_(mtu)
{
}

CEdgeDlg::~CEdgeDlg()
{
}

const CString& CEdgeDlg::GetAddress() const
{
    return address_;
}

const CString& CEdgeDlg::GetInterface() const
{
    return interface_;
}

const CString& CEdgeDlg::GetGroupName() const
{
    return group_name_;
}

const CString& CEdgeDlg::GetPassword() const
{
    return password_;
}

const CString& CEdgeDlg::GetMacAddr() const
{
    return mac_addr_;
}

const int CEdgeDlg::GetMTU() const
{
    return mtu_;
}

const CString& CEdgeDlg::GetMask() const
{
    return mask_;
}

void CEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EtAddress, address_);
    DDX_Text(pDX, IDC_EtInterface, interface_);
    DDX_Text(pDX, IDC_EtGroupName, group_name_);
    DDX_Text(pDX, IDC_EtPassword, password_);
    DDX_Text(pDX, IDC_EtMacAddr, mac_addr_);
    DDX_Text(pDX, IDC_EtMask, mask_);
    DDV_MaxChars(pDX, mask_, 15);
    DDX_Text(pDX, IDC_EtMtu, mtu_);
}

void CEdgeDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (0 == cx || 0 == cy)
    {
        return;
    }

    const CSize margins(20, 20);
    CPoint pos(20, 20);
    const int text_width = 24;
    const int interval = 5;
    const int single_edit_width = cx - 2 * margins.cx - text_width - interval;
    const int double_edit_width = (single_edit_width - 3 * interval - text_width) / 2;
    // static 地址
    CWnd* wnd = GetDlgItem(IDC_AddressText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // 地址Edit
    wnd = GetDlgItem(IDC_EtAddress);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, single_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // 接口 static
    pos.x = 20;
    pos.y += margins.cy + 20;
    wnd = GetDlgItem(IDC_InterfaceText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // 接口 Edit
    wnd = GetDlgItem(IDC_EtInterface);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, double_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // MTU static
    pos.x += double_edit_width + 2 * interval;
    wnd = GetDlgItem(IDC_MtuText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // MTU Edit
    wnd = GetDlgItem(IDC_EtMtu);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, double_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // 组名 static
    pos.x = 20;
    pos.y += margins.cy + 20;
    wnd = GetDlgItem(IDC_GroupNameText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // 组名 Edit
    wnd = GetDlgItem(IDC_EtGroupName);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, double_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // 密码 static
    pos.x += double_edit_width + 2 * interval;
    wnd = GetDlgItem(IDC_PasswordText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // 密码 Edit
    wnd = GetDlgItem(IDC_EtPassword);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, double_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // MAC static
    pos.x = 20;
    pos.y += margins.cy + 20;
    wnd = GetDlgItem(IDC_MacText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // MAC Edit
    wnd = GetDlgItem(IDC_EtMacAddr);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, single_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // 掩码 static
    pos.x = 20;
    pos.y += margins.cy + 20;
    wnd = GetDlgItem(IDC_MaskText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // 掩码 Edit
    wnd = GetDlgItem(IDC_EtMask);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, single_edit_width, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }
}

void CEdgeDlg::OnPaint()
{
    CPaintDC dc(this);

    CRect client_rect;
    GetClientRect(&client_rect);
    client_rect.bottom -= 1;
    client_rect.right -= 1;

    CPen pen(PS_SOLID, 1, RGB(217, 217, 217));
    CPen* old_pen = dc.SelectObject(&pen);

    dc.MoveTo(0, 0);
    dc.LineTo(0, client_rect.bottom);
    dc.LineTo(client_rect.right, client_rect.bottom);
    dc.LineTo(client_rect.right, 0);
    dc.LineTo(50, 0);

    dc.SelectObject(old_pen);
}
