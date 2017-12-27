
class CSuperNodeDlg;
class CEdgeDlg;
class CKeepLiveDlg;
class CVncDlg;

class CEdgeConfigDlg : public CDialogEx
{
public:
	CEdgeConfigDlg(CWnd* pParent = NULL);

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnPaint();
    afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedSave();
    afx_msg void OnTcnSelChangeConfigSwitch(NMHDR *nmhdr, LRESULT *result);
	DECLARE_MESSAGE_MAP()

private:
    void InitSubDlg();
    void SwitchSubDlgVisibleStatus(UINT id);
    void InstallVirtualLAN();
    void RestartService();

    void GetServiceConfigFile(CString& file);

    void CheckStringValid(const CString& dst, UINT& error_dlg_id, UINT cur_dlg_id, UINT& error_bt_id, UINT cur_bt_id, CString& error_tips, const CString& cur_error_text);
    void CheckNumberValid(const CString& dst, UINT& error_dlg_id, UINT cur_dlg_id, UINT& error_bt_id, UINT cur_bt_id, CString& error_tips, const CString& cur_error_text);

protected:
    HICON icon_handle_;

private:
    CTabCtrl tab_config_switch_;

    CSuperNodeDlg* dlg_super_node_;
    CEdgeDlg* dlg_edge_;
    CKeepLiveDlg* dlg_keeplive_;
    CVncDlg* dlg_vnc_;
    bool restart_service_;
    
};
