#pragma once


// CEdgeDlg 对话框

class CEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEdgeDlg)

public:
	CEdgeDlg(const CString& address, const CString& interface_name, const CString& group_name, const CString& password, const CString& mac_addr, const CString& mask, int mtu, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEdgeDlg();

    const CString& GetAddress() const;
    const CString& GetInterface() const;
    const CString& GetGroupName() const;
    const CString& GetPassword() const;
    const CString& GetMacAddr() const;
    const int GetMTU() const;
    const CString& GetMask() const;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EdgeDlg };
#endif

protected:
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

private:
    CString address_;
    CString interface_;
    CString group_name_;
    CString password_;
    CString mac_addr_;
    CString mask_;
    int mtu_;
};
