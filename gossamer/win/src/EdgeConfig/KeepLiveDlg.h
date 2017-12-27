#pragma once


// CKeepLiveDlg �Ի���

class CKeepLiveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeepLiveDlg)

public:
	CKeepLiveDlg(const CString& address, UINT interval, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeepLiveDlg();

    const CString& GetAddress() const;
    UINT GetInterval() const;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KeepLiveDlg };
#endif

protected:
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
    CString address_;
    UINT interval_;
};
