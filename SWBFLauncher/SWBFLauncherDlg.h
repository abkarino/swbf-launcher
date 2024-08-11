
// SWBFLauncherDlg.h : header file
//

#pragma once


// CSWBFLauncherDlg dialog
class CSWBFLauncherDlg : public CDialogEx
{
// Construction
public:
	CSWBFLauncherDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SWBFLAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	// CPngImage m_pngLogo;
	CStatic* m_logo;

	HICON m_hIcon;
	CButton* m_pftest;
	CButton* m_gog;
	CButton* m_steam;

	CButton* m_args_chk;

	CButton* m_all_chk;

	CButton* m_highRes_chk;
	CButton* m_console_chk;
	CButton* m_fixedPool_chk;
	CButton* m_crouch_chk;
	CButton* m_moreFollowers_chk;
	CButton* m_moreFlyers_chk;
	CButton* m_vision_chk;
	CButton* m_spBots_chk;
	CButton* m_decal_chk;
	CButton* m_foliage_chk;
	CButton* m_sound_chk;
	CButton* m_aggro_chk;

	CButton* m_luaconsole_chk;
	CButton* m_iconsole_chk;
	CButton* m_crashlog_chk;
	CButton* m_posthook_chk;
	CButton* m_plugins_chk;

	CEdit* m_args_edit;

	BOOL SaveProfile();
	BOOL LoadProfile();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAllChk();
};
