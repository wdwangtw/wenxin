// VncDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EdgeConfig.h"
#include "VncDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(CVncDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CVncDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BtBrowser, &CVncDlg::OnBnClickedBtBrowser)
END_MESSAGE_MAP()

CVncDlg::CVncDlg(const CString& vnc_path, CWnd* pParent)
	: CDialogEx(IDD_VncDlg, pParent)
    , vnc_path_(vnc_path)
{
}

CVncDlg::~CVncDlg()
{
}

const CString& CVncDlg::GetVNCPath() const
{
    return vnc_path_;
}

void CVncDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EtPath, vnc_path_);
}

void CVncDlg::OnPaint()
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
    dc.LineTo(181, 0);

    dc.MoveTo(0, 0);
    dc.LineTo(136, 0);

    dc.SelectObject(old_pen);
}

void CVncDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (0 == cx || 0 == cy)
    {
        return;
    }

    const CSize margins(20, 20);
    CPoint pos(20, 20);
    const int text_width = 70;
    const int interval = 5;

    // static ��ַ
    CWnd* wnd = GetDlgItem(IDC_PathText);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->MoveWindow(pos.x, pos.y + 3, text_width, 20);
    }

    CRect bt_rect(0, 0, 0, 0);
    wnd = GetDlgItem(IDC_BtBrowser);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        wnd->GetWindowRect(&bt_rect);
        bt_rect.bottom = pos.y + bt_rect.Height();
        bt_rect.top = pos.y;
        bt_rect.left = cx - margins.cx - bt_rect.Width();
        bt_rect.right = cx - margins.cx;
        wnd->MoveWindow(&bt_rect);
    }

    // ��ַEdit
    wnd = GetDlgItem(IDC_EtPath);
    if (wnd && IsWindow(wnd->m_hWnd))
    {
        pos.x += text_width + interval;
        wnd->MoveWindow(pos.x, pos.y, cx - 2 * margins.cx - text_width - 2 * interval - bt_rect.Width(), bt_rect.Height());
        ((CEdit*)wnd)->SetSel(0, 0);
    }
}

void CVncDlg::OnBnClickedBtBrowser()
{
    OPENFILENAME open_file_name;    // �����Ի���ṹ��
    TCHAR file[MAX_PATH];           // �����ȡ�ļ����ƵĻ�������
                                    // ��ʼ��ѡ���ļ��Ի���
    ZeroMemory(&open_file_name, sizeof(OPENFILENAME));
    open_file_name.lStructSize = sizeof(OPENFILENAME);
    open_file_name.hwndOwner = 0;
    open_file_name.lpstrFile = file;
    open_file_name.lpstrFile[0] = '\0';
    open_file_name.nMaxFile = sizeof(file);
    open_file_name.lpstrFilter = _T("�����ļ�(*.exe)\0\0");
    open_file_name.nFilterIndex = 1;
    open_file_name.lpstrFileTitle = 0;
    open_file_name.nMaxFileTitle = 0;
    open_file_name.lpstrInitialDir = 0;
    open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // ��ʾ��ѡ���ļ��Ի���
    if (GetOpenFileName(&open_file_name))
    {
        vnc_path_ = file;
    }

    UpdateData(FALSE);
}
