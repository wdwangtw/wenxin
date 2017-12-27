#ifndef _SUPER_NODE_DLG_H_
#define _SUPER_NODE_DLG_H_

class CSuperNodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSuperNodeDlg)

public:
	CSuperNodeDlg(const CString& address, short port, CWnd* parent = NULL);   // 标准构造函数
	virtual ~CSuperNodeDlg();

    const CString& GetAddress() const;
    short GetPort() const;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SuperNodeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT type, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
    CString address_;
    short port_;
};

#endif // _SUPER_NODE_DLG_H_
