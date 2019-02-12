// CAM_CONFIG.cpp : implementation file
//

#include "stdafx.h"
#include "ROI_CONFIG.h"
#include "CAM_CONFIG.h"
#include "OdcitavanieHodnot.h"
#include "afxdialogex.h"
#include "Config.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#include <afxstr.h>
#include <atlstr.h>
#include <atlimage.h>

#include <string>

#include <Windows.h>
#include <dshow.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "strmiids")

using namespace std;
using namespace cv;

// CCAM_CONFIG dialog

IMPLEMENT_DYNAMIC(CCAM_CONFIG, CDialogEx)

CCAM_CONFIG::CCAM_CONFIG(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERA_CONFIG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCAM_CONFIG::~CCAM_CONFIG()
{
}

void CCAM_CONFIG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCAM_CONFIG, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCAM_CONFIG::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CCAM_CONFIG::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CCAM_CONFIG::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCAM_CONFIG::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_DIGITS_DARK, &CCAM_CONFIG::OnBnClickedDigitsDark)
	ON_BN_CLICKED(IDC_DIGITS_BRIGHT, &CCAM_CONFIG::OnBnClickedDigitsBright)
END_MESSAGE_MAP()


// CCAM_CONFIG message handlers



// COdcitavanieHodnotDlg message handlers
//vytvori mi z pathu filename
string CCAM_CONFIG::getFileName(const string& s) {
	char sep = '/';
#ifdef _WIN32
	sep = '\\';
#endif
	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}
	return(s);
}

BOOL CCAM_CONFIG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //okno sa zobrazi na vrchu

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here

	CComboBox * pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1);
	pCombo->SetWindowTextW(L"Vyberte kameru");
	CComboBox * pCombo2 = (CComboBox *)GetDlgItem(IDC_COMBO2);
	pCombo2->SetWindowTextW(L"Vyberte rozlíšenie");

	VideoCapture cap;
	int index = 0;
	vector<int> kamery;
	while (TRUE) {
		cap = VideoCapture(index);
		if (!cap.isOpened()) {
			break;
		}
		else {
			kamery.push_back(index);
		}
		cap.release();
		index++;
	}

	for each (int var in kamery) {
		CComboBox * pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1);
		stringstream ss;
		ss << "Kamera " << var + 1;
		string str = ss.str();
		CA2T t(str.c_str());
		LPCTSTR roi_position = t;
		pCombo->AddString(roi_position);
		vsetky++; //kvoli tomu ked sa vo file nachadza kamera z vacsim indexom ako mame my pocet kamier
	}

	stringstream ss;
	ss << "640x480";
	string str = ss.str();
	CA2T t(str.c_str());
	LPCTSTR rozlise = t;
	pCombo2->AddString(rozlise);


	if (!zmena) { //uvodne nacitanie
		/*CString t;
		t.Format(_T("%d"), sliderjas->GetPos());
		SetDlgItemText(IDC_J, t);//jas

		t.Format(_T("%d"), sliderkon->GetPos() / 10.0);
		SetDlgItemText(IDC_K, t); //kontrast*/

		CButton* pBtn = (CButton*)GetDlgItem(IDC_DIGITS_DARK);
		pBtn->SetCheck(1);// check it
	}
	else {//vraciame sa z roiconfigu
		//TODO Rozlisenie a kamera
		CButton* dark = (CButton*)GetDlgItem(IDC_DIGITS_DARK);
		CButton* bright = (CButton*)GetDlgItem(IDC_DIGITS_BRIGHT);

		if (cfg.getCislice() == "dark") {
			dark->SetCheck(1);// check it
			bright->SetCheck(0);// check it
		}
		else if (cfg.getCislice() == "bright") {
			bright->SetCheck(1);// check it
			dark->SetCheck(0);// check it
		}

		Camera camer = cfg.getCam();

		
		CString t;


		t.Format(_T("%d"), cfg.getInterval());
		SetDlgItemText(IDC_EDIT1,t); //inteval odcitania

		t.Format(_T("%d"), cfg.getIntAvgMed());
		SetDlgItemText(IDC_EDIT2, t); //interval pocitania

		stringstream h;
		h << getFileName(cfg.getConfigFile());
		string hh = h.str();
		CA2T e(hh.c_str());
		LPCTSTR confi = e;
		SetDlgItemText(IDC_STATIC_SF, confi); //configfile

		stringstream v;
		v << getFileName(cfg.getOutputFile());
		string vv = v.str();
		CA2T i(vv.c_str());
		LPCTSTR out = i;
		SetDlgItemText(IDC_STATIC_SSF, out); //outputfile

		if (cfg.doAverage()) {
			CButton* btn = (CButton*)GetDlgItem(IDC_CHECK1);
			btn->SetCheck(1);
		}
		if (cfg.doMedian()) {
			CButton* btn = (CButton*)GetDlgItem(IDC_CHECK2);
			btn->SetCheck(1);
		}
		
		if (camer.getID() >= 0 && camer.getID() <= vsetky) {
			CComboBox* kamera = (CComboBox*)GetDlgItem(IDC_COMBO1);
			kamera->SetCurSel(camer.getID());
		}
		else {
			CComboBox* kamera = (CComboBox*)GetDlgItem(IDC_COMBO1);
			kamera->SetCurSel(0);
		}
		CComboBox* rozlis = (CComboBox*)GetDlgItem(IDC_COMBO2);
		rozlis->SetCurSel(0);

		/*if (camer.getRes() >= 0) {
			CComboBox* rozlis = (CComboBox*)GetDlgItem(IDC_COMBO2);
			rozlis->SetCurSel(camer.getRes());
		}
		else if (camer.getRes() == -1) {
			CComboBox* rozlis = (CComboBox*)GetDlgItem(IDC_COMBO2);
			rozlis->SetCurSel(0);
		}*/
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CCAM_CONFIG::OnPaint()
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
HCURSOR CCAM_CONFIG::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCAM_CONFIG::OnLbnSelchangeList2()
{
	// TODO: Add your control notification handler code here
}


void CCAM_CONFIG::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

bool validate_Int(CString str)
{
	for (int i = 0; i < str.GetLength(); i++) {
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

void CCAM_CONFIG::OnBnClickedOk()
{
	//Get value from dialog attributes
	CDialog::UpdateData(true);

	//nastavenie kamery
	CComboBox * pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1); //id
	CComboBox * pCombo2 = (CComboBox *)GetDlgItem(IDC_COMBO2); //res

	Camera cmr = Camera(pCombo->GetCurSel(), pCombo2->GetCurSel(), 50, 1, "auto");
	
	//nastavenie configu

	if (cfg.getOutputFile() == "") {


		time_t rawtime;
		struct tm timeinfo;
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		string file_name = to_string(timeinfo.tm_mday) + '-' + to_string(timeinfo.tm_mon + 1) + '-' + to_string(timeinfo.tm_year + 1900);
		cfg.setOutputFile("output " + file_name + ".txt");
	}
	if (cfg.getConfigFile() == "") {
		time_t rawtime;
		struct tm timeinfo;
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		string file_name = to_string(timeinfo.tm_mday) + '-' + to_string(timeinfo.tm_mon + 1) + '-' + to_string(timeinfo.tm_year + 1900);
		cfg.setConfigFile("config " + file_name + ".csv");
	}	

	CButton* dark = (CButton*)GetDlgItem(IDC_DIGITS_DARK);
	if ((dark->GetCheck()==1)) {
		cfg.setCislice("dark");
	}
	else {
		cfg.setCislice("bright");
	}


	cfg.setCam(cmr); //kamera
	CButton *median = (CButton*)GetDlgItem(IDC_CHECK2);
	cfg.setDoMedian((median->GetCheck() == 1)); //median
	CButton *priemer = (CButton*)GetDlgItem(IDC_CHECK1);
	cfg.setDoAverage((priemer->GetCheck() == 1)); //priemer
	CString intOdcitania;
	GetDlgItemText(IDC_EDIT1, intOdcitania);
	cfg.setInterval(_ttoi(intOdcitania)); //interval odcitania
	CString intPocitania;
	GetDlgItemText(IDC_EDIT2, intPocitania);
	cfg.setAvgMedInterval(_ttoi(intPocitania));//interval median priemer

	//kontrola vstupov èi sú zadane potrebné parametre
	if ((intOdcitania == "" || !validate_Int(intOdcitania)) || ((cfg.doAverage() || cfg.doMedian()) && (intPocitania == "" || !validate_Int(intPocitania)))) {
		LPCTSTR msg = L"Nevyplnené alebo nesprávne vyplnené potrebné parametre";
		LPCTSTR title = L"Chyba";
		::MessageBox(0, msg, title, MB_OK);
		return;
	}
	

	// OnOk zatvori momentalne okno 

	//cfg.writeToFile();
	CDialog::OnOK();
	
	

	ROI_CONFIG Rk; // nastavenie premenných do druhého dialogu
	Rk.cfg = cfg;	
	Rk.zmena = zmena;

	INT_PTR nRet = -1;
	nRet = Rk.DoModal(); // otvorenie okna gui/konfiguracie

	   // Handle the return value from DoModal
	switch (nRet)
	{
	case -1:
		AfxMessageBox(_T("Dialog box could not be created!"));
		break;
	case IDABORT:
		// Do something
		break;
	case IDOK:
		// Do something
		break;
	case IDCANCEL:
		EndDialog(IDCLOSE);
		//CDialogEx::EndDialog(IDCLOSE);
		//this->EndDialog(IDCLOSE);
		break;
	default:
		// Do something
		break;
	};



}



void CCAM_CONFIG::OnBnClickedButton1()
{
	// OPEN SAVEFILE
	TCHAR szFilters[] = _T("Text Files (*.dat)|*.NC|Text Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	// Create an Open dialog; the default file name extension is ".my".

	CFileDialog sf(TRUE/*bOpenFileDialog*/, _T("csv"), _T("*.csv", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, NULL, 0,
		TRUE/*bVistaStyle*/));

	IFileOpenDialog * openDlgPtr = sf.GetIFileOpenDialog();
	if (openDlgPtr != NULL)
	{
		openDlgPtr->SetOptions(FOS_PICKFOLDERS);
		openDlgPtr->Release();
	}
	sf.DoModal();

	CString path_name = sf.GetPathName();
	saveFilePath = path_name;
	CString file_name = sf.GetFileName();

	CT2CA pszConvertedAnsiString(saveFilePath);// Convert a TCHAR string to a LPCSTR	
	string filepath(pszConvertedAnsiString); // construct a std::string using the LPCSTR input
	cfg.setOutputFile(filepath);
	SetDlgItemText(IDC_STATIC_SSF, file_name);
}



void CCAM_CONFIG::OnBnClickedButton2()
{
	// OPEN CONFIGFILE
	TCHAR szFilters[] = _T("Text Files (*.dat)|*.NC|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	// Create an Open dialog; the default file name extension is ".my".

	CFileDialog kf(TRUE/*bOpenFileDialog*/, _T("txt"), _T("*.txt", NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, NULL, 0,
		TRUE/*bVistaStyle*/));

	IFileOpenDialog * openDlgPtr = kf.GetIFileOpenDialog();
	if (openDlgPtr != NULL)
	{
		openDlgPtr->SetOptions(FOS_PICKFOLDERS);
		openDlgPtr->Release();
	}

	kf.DoModal();

	CString path_name = kf.GetPathName();
	configFilePath = path_name;
	CString file_name = kf.GetFileName();

	//konfiguracny subor je vybrany	

	CT2CA pszConvertedAnsiString(configFilePath);// Convert a TCHAR string to a LPCSTR	
	string filepath(pszConvertedAnsiString); // construct a std::string using the LPCSTR input
	//cfg.setConfigFile(filepath);
	try {//nastavenie udajov z configsuboru
		cfg = Config(filepath);
		SetDlgItemText(IDC_STATIC_SF, file_name); //nastavim config

		stringstream ss;
		ss << cfg.getInterval();
		string str = ss.str();
		CA2T t(str.c_str());
		LPCTSTR intervalod = t;
		SetDlgItemText(IDC_EDIT1, intervalod); //nastavim interval odcitania

		stringstream sss;
		sss << cfg.getIntAvgMed();
		string stri = sss.str();
		CA2T b(stri.c_str());
		LPCTSTR intervalpoc = b;
		if (cfg.doAverage() || cfg.doMedian()) {
			SetDlgItemText(IDC_EDIT2, intervalpoc); //nastavim interval pocitania a checkboxy priemer a median
			if (cfg.doAverage()) {
				CButton* btn = (CButton*)GetDlgItem(IDC_CHECK1);
				btn->SetCheck(1);
			}
			else {
				CButton* btn = (CButton*)GetDlgItem(IDC_CHECK1);
				btn->SetCheck(0);
			}
			if (cfg.doMedian()) {
				CButton* btn = (CButton*)GetDlgItem(IDC_CHECK2);
				btn->SetCheck(1);
			}
			else {
				CButton* btn = (CButton*)GetDlgItem(IDC_CHECK2);
				btn->SetCheck(0);
			}
		}
		else {
			CButton* btn = (CButton*)GetDlgItem(IDC_CHECK2);
			btn->SetCheck(0);
			btn = (CButton*)GetDlgItem(IDC_CHECK1);
			btn->SetCheck(0);
		}

		CButton* dark = (CButton*)GetDlgItem(IDC_DIGITS_DARK);
		CButton* bright = (CButton*)GetDlgItem(IDC_DIGITS_BRIGHT);
		if (cfg.getCislice() == "dark") {
			dark->SetCheck(1);
			bright->SetCheck(0);
		}
		else if (cfg.getCislice() == "bright") {
			dark->SetCheck(0);
			bright->SetCheck(1);
		}


		stringstream ssss;
		ssss << getFileName(cfg.getOutputFile());
		string strin = ssss.str();
		CA2T e(strin.c_str());
		LPCTSTR savefile = e;
		SetDlgItemText(IDC_STATIC_SSF, savefile);//nastavim outputfile


		if (cfg.getCam().getID() >= 0 && cfg.getCam().getID() <= vsetky) { //kamera
			CComboBox* kamera = (CComboBox*)GetDlgItem(IDC_COMBO1);
			kamera->SetCurSel(cfg.getCam().getID());
		}
		else {
			CComboBox* kamera = (CComboBox*)GetDlgItem(IDC_COMBO1);
			kamera->SetCurSel(0);
		}

		CComboBox* rozlis = (CComboBox*)GetDlgItem(IDC_COMBO2); //rozlisenie, zatial je iba jedno
		rozlis->SetCurSel(0);
		/*
		if (cfg.getCam().getRes() >= 0) { //rozlisenie
			CComboBox* rozlis = (CComboBox*)GetDlgItem(IDC_COMBO2);
			rozlis->SetCurSel(cfg.getCam().getRes());
		}
		else if (cfg.getCam().getRes() == -1) {
			CComboBox* rozlis = (CComboBox*)GetDlgItem(IDC_COMBO2);
			rozlis->SetCurSel(0);
		}*/

	}
	catch (invalid_argument e) {
		//konfiguracny subor nie je v spravnom formate...
		LPCTSTR msg = L"Konfiguraèný súbor nebol vybraný alebo nieje v správnom formáte.";
		LPCTSTR title = L"Chyba";
		::MessageBox(0, msg, title, MB_OK);
		return;
	}
}




void CCAM_CONFIG::OnBnClickedDigitsDark()
{
	// TODO: Add your control notification handler code here
	CButton* bright = (CButton*)GetDlgItem(IDC_DIGITS_BRIGHT);
	bright->SetCheck(0);

}
void CCAM_CONFIG::OnBnClickedDigitsBright()
{
	// TODO: Add your control notification handler code here
	CButton* dark = (CButton*)GetDlgItem(IDC_DIGITS_DARK);
	dark->SetCheck(0);

}