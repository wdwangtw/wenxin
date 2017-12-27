#pragma once


// CKeepLiveDlg 对话框

class CKeepLiveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeepLiveDlg)

public:
	CKeepLiveDlg(const CString& address, UINT interval, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeepLiveDlg();

    const CString& GetAddress() const;
    UINT GetInterval() const;

// 对话框数据
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
