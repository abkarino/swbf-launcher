
// SWBFLauncherDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SWBFLauncher.h"
#include "SWBFLauncherDlg.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "InstanceSingleton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "Patches.h"

// CSWBFLauncherDlg dialog

CSWBFLauncherDlg::CSWBFLauncherDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SWBFLAUNCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSWBFLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSWBFLauncherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSWBFLauncherDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ALL_CHK, &CSWBFLauncherDlg::OnBnClickedAllChk)
END_MESSAGE_MAP()


// CSWBFLauncherDlg message handlers

BOOL CSWBFLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// m_pngLogo.Load(IDB_PNG1);
	
	m_logo = (CStatic*)GetDlgItem(IDC_LOGO);
	// m_logo->SetBitmap((HBITMAP)m_pngLogo.Detach());

	// TODO: Add extra initialization here
	m_pftest = (CButton*)GetDlgItem(RADIO_SPTEST);
	m_gog = (CButton*)GetDlgItem(RADIO_GOG);
	m_steam = (CButton*)GetDlgItem(RADIO_STEAM);

	m_args_chk = (CButton*)GetDlgItem(IDC_ARGS_CHK);

	m_all_chk = (CButton*)GetDlgItem(IDC_ALL_CHK);

	m_highRes_chk		= (CButton*)GetDlgItem(IDC_HIGHRES_CHK);
	m_console_chk		= (CButton*)GetDlgItem(IDC_CONSOLE_CHK);
	m_fixedPool_chk		= (CButton*)GetDlgItem(IDC_FIXEDPOOL_CHK);
	m_crouch_chk		= (CButton*)GetDlgItem(IDC_CROUCH_CHK);
	m_moreFollowers_chk	= (CButton*)GetDlgItem(IDC_MOREFOLLOWERS_CHK);
	m_moreFlyers_chk	= (CButton*)GetDlgItem(IDC_FLYERS_CHK);
	m_vision_chk		= (CButton*)GetDlgItem(IDC_VISION_CHK);
	m_spBots_chk		= (CButton*)GetDlgItem(IDC_SPBOTS_CHK);
	m_decal_chk			= (CButton*)GetDlgItem(IDC_DECAL_CHK);
	m_foliage_chk		= (CButton*)GetDlgItem(IDC_FOLIAGE_CHK);
	m_sound_chk			= (CButton*)GetDlgItem(IDC_SOUND_CHK);
	m_aggro_chk			= (CButton*)GetDlgItem(IDC_AGGRO_CHK);

	m_crashlog_chk		= (CButton*)GetDlgItem(IDC_CRASHLOG_CHK);
	m_luaconsole_chk	= (CButton*)GetDlgItem(IDC_LUACONSOLE_CHK);
	m_iconsole_chk		= (CButton*)GetDlgItem(IDC_ICONSOLE_CHK);
	m_posthook_chk		= (CButton*)GetDlgItem(IDC_POSTHOOK_CHK);
	m_plugins_chk       = (CButton*)GetDlgItem(IDC_PLUGINS_CHK);

#ifdef DEBUG
	m_crashlog_chk->EnableWindow(TRUE);
	m_luaconsole_chk->EnableWindow(TRUE);
	m_iconsole_chk->EnableWindow(TRUE);
#endif

	m_args_edit			= (CEdit*)GetDlgItem(IDC_ARGS_EDIT);
	m_args_edit->SetWindowTextW(L"/win /nointro");
#ifdef DEBUG
	m_args_edit->SetWindowTextW(L"/win /nointro /invincible /unlimitedammo");
	m_args_chk->SetCheck(TRUE);
#endif // DEBUG

	m_pftest->SetCheck(TRUE);
	LoadProfile();
	((CButton*)GetDlgItem(IDC_HEAP_CHK))->SetCheck(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSWBFLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSWBFLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool injectDLL(wstring path) {
	return (new AllocatorPatch((GameVersion)NULL, MAX_PATH * 2, PAGE_EXECUTE_READWRITE, [path](LPVOID ptr) -> bool {
		HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
		VOID* lb = GetProcAddress(hKernel32, "LoadLibraryW");

		LPPROCESS_INFORMATION pinfo = InstanceSingleton::getInstance();
		const BYTE* base = (BYTE*)InstanceSingleton::getBase();

		bool patched = (new InPlacePatchVariant((GameVersion)NULL, (BYTE*)ptr - base, (BYTE*)path.c_str(), path.length()*2))->patch();
		if (patched) {
			/*HANDLE thread = */CreateRemoteThread(pinfo->hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lb, ptr, 0, NULL);
			//QueueUserAPC((PAPCFUNC)lb, pinfo->hThread, (ULONG_PTR)ptr);
		}
		return patched;
		}))->patch();
}

void CSWBFLauncherDlg::OnBnClickedOk()
{
	SaveProfile();
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
	GameVersion mGameVersion;

	if (m_pftest->GetCheck()) {
		mGameVersion = SPTEST;
	} else if (m_gog->GetCheck()) {
		mGameVersion = GOG;
	} else if (m_steam->GetCheck()) {
		mGameVersion = STEAM;
	} else return;

	CString args = L"";
	if(m_args_chk->GetCheck()) m_args_edit->GetWindowTextW(args);

	if (InstanceSingleton::getInstance() == NULL)
		InstanceSingleton::createInstance(mGameVersion, wstring(args));

	if (InstanceSingleton::getInstance() == NULL)
		return;

	Patches p = Patches();
	bool result = p.patches["Heap"]->runPatch(mGameVersion);
	if (result && m_highRes_chk->GetCheck()) {
		result &= p.patches["HighRes"]->runPatch(mGameVersion);
		result &= p.patches["MatrixPool"]->runPatch(mGameVersion);
		result &= p.patches["FixEffects"]->runPatch(mGameVersion);
	}
	if (result && m_console_chk->GetCheck() && mGameVersion == SPTEST) {
		result &= p.patches["Console"]->runPatch(mGameVersion);
	}
	if (result && m_fixedPool_chk->GetCheck()) {
		result &= p.patches["FixedPools"]->runPatch(mGameVersion);
	}
	if (result && m_crouch_chk->GetCheck() && mGameVersion != SPTEST) {
		result &= p.patches["Crouch"]->runPatch(mGameVersion);
	}
	if (result && m_moreFollowers_chk->GetCheck()) {
		result &= p.patches["MoreFollowers"]->runPatch(mGameVersion);
	}
	if (result && m_moreFlyers_chk->GetCheck()) {
		result &= p.patches["MoreFlyers"]->runPatch(mGameVersion);
	}
	if (result && m_vision_chk->GetCheck()) {
		result &= p.patches["RayRequest"]->runPatch(mGameVersion);
	}
	if (result && m_spBots_chk->GetCheck()) {
		result &= p.patches["AIUnlock"]->runPatch(mGameVersion);
	}
	if (result && m_decal_chk->GetCheck()) {
		result &= p.patches["Decal"]->runPatch(mGameVersion);
	}
	if (result && m_foliage_chk->GetCheck()) {
		result &= p.patches["Foliage"]->runPatch(mGameVersion);
	}
	if (result && m_sound_chk->GetCheck()) {
		result &= p.patches["Sound"]->runPatch(mGameVersion);
	}
	if (result && m_aggro_chk->GetCheck()) {
		result &= p.patches["Aggro"]->runPatch(mGameVersion);
	}
	if (result && m_posthook_chk->GetCheck()) {
		result &= p.patches["PostHook"]->runPatch(mGameVersion);
	}

	if (result && m_crashlog_chk->GetCheck()) {
		wchar_t name[MAX_PATH];
		{
			GetModuleFileNameW(NULL, name, MAX_PATH);
			PathRemoveFileSpecW(name);
			wsprintfW(name + wcslen(name),
				L"\\crashlog.dll");
		}
		if (!injectDLL(name)) {
			::MessageBox(NULL, wstring(L"Failed to inject CrashLog DLL.\n").append(GetLastErrorAsString()).c_str(), L"Error", MB_ICONERROR);
		}
	}

	if (result && m_iconsole_chk->GetCheck() && mGameVersion == STEAM) {
		p.patches["InjectConsole"]->runPatch(mGameVersion);
	}

	if (result && m_luaconsole_chk->GetCheck()) {
		wchar_t name[MAX_PATH];
		{
			GetModuleFileNameW(NULL, name, MAX_PATH);
			PathRemoveFileSpecW(name);
			wsprintfW(name + wcslen(name),
				L"\\LuaHook.dll");
		}
		if (!injectDLL(name)) {
			::MessageBox(NULL, wstring(L"Failed to inject LuaConsole DLL.\n").append(GetLastErrorAsString()).c_str(), L"Error", MB_ICONERROR);
		}
	}

	if (result && m_plugins_chk->GetCheck()) {
		//result &= p.patches["Plugins"]->runPatch(mGameVersion);

		// loop on files within the game Plugins folder
		TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);

		wstring gameBase = NPath;
		gameBase += L"\\";
		wstring dir = gameBase + (mGameVersion == SPTEST ? L"Debug\\Plugins\\" : L"GameData\\Plugins\\");
		wstring search = dir + L"*.dll";
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(search.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				Sleep(20);
				wstring dllPath = dir + fd.cFileName;
				if (!injectDLL(dllPath)) {
					::MessageBox(NULL, wstring(L"Failed to inject DLL.\n").append(dllPath).append(L"\n").append(GetLastErrorAsString()).c_str(), L"Error", MB_ICONERROR);
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
	}
	
	if (result) {
		InstanceSingleton::resume();
		InstanceSingleton::closeInstance();
		exit(0);
	}
	else {
		InstanceSingleton::closeInstance();
		::MessageBox(NULL, wstring(L"One or more patches failed to apply.\n").append(GetLastErrorAsString()).c_str(), L"Error", MB_ICONERROR);
	}

}


void CSWBFLauncherDlg::OnBnClickedAllChk()
{
	bool checked = m_all_chk->GetCheck();
	m_highRes_chk->SetCheck(checked);
	m_console_chk->SetCheck(checked);
	m_fixedPool_chk->SetCheck(checked);
	m_crouch_chk->SetCheck(checked);
	m_moreFollowers_chk->SetCheck(checked);
	m_moreFlyers_chk->SetCheck(checked);
	m_vision_chk->SetCheck(checked);
	//m_mpBots_chk->SetCheck(checked);
	m_decal_chk->SetCheck(checked);
	m_foliage_chk->SetCheck(checked);
	m_sound_chk->SetCheck(checked);
	m_aggro_chk->SetCheck(checked);
}

#define SettingsOption(V)	\
  V(m_pftest)				\
  V(m_gog)					\
  V(m_steam)				\
  V(m_args_chk)				\
							\
  V(m_highRes_chk)			\
  V(m_console_chk)			\
  V(m_fixedPool_chk)		\
  V(m_crouch_chk)			\
  V(m_moreFollowers_chk)	\
  V(m_moreFlyers_chk)		\
  V(m_vision_chk)			\
  V(m_spBots_chk)			\
  V(m_decal_chk)			\
  V(m_foliage_chk)			\
  V(m_sound_chk)			\
  V(m_aggro_chk)			\
							\
  V(m_crashlog_chk)			\
  V(m_luaconsole_chk)		\
  V(m_iconsole_chk)			\
  V(m_posthook_chk)         \
  V(m_plugins_chk)

#define SettingsDefineBOOL(name) BOOL name;
#define SettingsGetBool(V) settings.##V = V->GetCheck();
#define SettingsSetBool(V) V->SetCheck(settings.##V);

struct Settings {
	SettingsOption(SettingsDefineBOOL)
};

BOOL CSWBFLauncherDlg::SaveProfile() {
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	wstring profile = NPath;
	profile += L"\\profile.ini";
	Settings settings;
	SettingsOption(SettingsGetBool);
	CString args;
	m_args_edit->GetWindowTextW(args);
	return WritePrivateProfileStructW(L"EGM", L"profile", &settings, sizeof(settings), profile.c_str())
		&& WritePrivateProfileStringW(L"EGM", L"args", args, profile.c_str());
}

BOOL CSWBFLauncherDlg::LoadProfile() {
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	wstring profile = NPath;
	profile += L"\\profile.ini";
	Settings settings;
	if (GetPrivateProfileStructW(L"EGM", L"profile", &settings, sizeof(settings), profile.c_str())) {
		SettingsOption(SettingsSetBool);
		wchar_t args[1024];
		if (GetPrivateProfileStringW(L"EGM", L"args", L"/win /nointro", args, 1024, profile.c_str()))
			m_args_edit->SetWindowTextW(args);
	}
	return false;
}
