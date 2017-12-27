// KeepLiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EdgeConfig.h"
#include "KeepLiveDlg.h"
#include "afxdialogex.h"


// CKeepLiveDlg 对话框

IMPLEMENT_DYNAMIC(CKeepLiveDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CKeepLiveDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

CKeepLiveDlg::CKeepLiveDlg(const CString& address, UINT interval, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KeepLiveDlg, pParent)
    , address_(address)
    , interval_(interval)
{

}

CKeepLiveDlg::~CKeepLiveDlg()
{
}

const CString& CKeepLiveDlg::GetAddress() const
{
    return address_;
}

UINT CKeepLiveDlg::GetInterval() const
{
    return interval_;
}

void CKeepLiveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EtAddress, address_);
    DDX_Text(pDX, IDC_EtInterval, interval_);
}

void CKeepLiveDlg::OnSize(UINT nType, int cx, int cy)
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
        wnd->MoveWindow(pos.x, pos.y, cx - 2 * margins.cx - text_width - interval, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }

    // static 端口
    pos.x = 20;
    pos.y += margins.cy + 20;
    wnd = GetDlgItem(IDC_IntervalText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    // 端口Edit
    wnd = GetDlgItem(IDC_EtInterval);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, cx - 2 * margins.cx - text_width - interval, 20);
        ((CEdit*)wnd)->SetSel(0, 0);
    }
}

void CKeepLiveDlg::OnPaint()
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
    dc.LineTo(138, 0);

    dc.MoveTo(0, 0);
    dc.LineTo(93, 0);

    dc.SelectObject(old_pen);
}
