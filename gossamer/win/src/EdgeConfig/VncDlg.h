
#ifndef _VNC_DLG_H_
#define _VNC_DLG_H_

class CVncDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVncDlg)

public:
	CVncDlg(const CString& vnc_path, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVncDlg();

    const CString& GetVNCPath() const;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VncDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnPaint();
    afx_msg void OnBnClickedBtBrowser();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
    CString vnc_path_;
};

#endif // !_VNC_DLG_H_
