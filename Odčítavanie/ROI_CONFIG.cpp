// ROI_CONFIG.cpp : implementation file
//

#include "stdafx.h"
#include "OdcitavanieHodnot.h"
#include "ROI_CONFIG.h"
#include "afxdialogex.h"
#include "CAM_CONFIG.h"
#include "DataGetter.h"

#include <afxstr.h>
#include <atlstr.h>
#include <atlimage.h>



#include <string>
#include <time.h>

//using namespace std;

// ROI_CONFIG dialog

IMPLEMENT_DYNAMIC(ROI_CONFIG, CDialog)

ROI_CONFIG::ROI_CONFIG(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_GUI_ROI, pParent)
{

}

ROI_CONFIG::~ROI_CONFIG()
{
}

void ROI_CONFIG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MY, pbmp);
}


BEGIN_MESSAGE_MAP(ROI_CONFIG, CDialog)
	ON_STN_CLICKED(IDC_STATIC_MY, &ROI_CONFIG::OnStnClickedStaticMy)
	ON_BN_CLICKED(IDC_NEW_ROI, &ROI_CONFIG::OnBnClickedNewRoi)
	ON_BN_CLICKED(IDC_SAVE_ROI, &ROI_CONFIG::OnBnClickedSaveRoi)
	ON_BN_CLICKED(IDC_DELETE_ROI, &ROI_CONFIG::OnBnClickedDeleteRoi)
	ON_BN_CLICKED(ID_START, &ROI_CONFIG::OnBnClickedStart)
	ON_BN_CLICKED(IDC_CHECK_ANA, &ROI_CONFIG::OnBnClickedCheckAna)
	ON_BN_CLICKED(IDC_CHECK_DIG, &ROI_CONFIG::OnBnClickedCheckDig)
	ON_BN_CLICKED(IDC_END, &ROI_CONFIG::OnBnClickedEnd)
	ON_BN_CLICKED(IDC_BTN_SAVE_CF, &ROI_CONFIG::OnBnClickedBtnSaveCf)
	ON_CBN_SELCHANGE(IDC_COMBO_ROI, &ROI_CONFIG::OnCbnSelchangeComboRoi)
	ON_BN_CLICKED(IDC_BTN_BTCFG, &ROI_CONFIG::OnBnClickedBtnBtcfg)
	ON_BN_CLICKED(IDC_DEL_SEL_ROI, &ROI_CONFIG::OnBnClickedDelSelRoi)
	ON_BN_CLICKED(IDC_BUTTON_ENDAPP, &ROI_CONFIG::OnBnClickedButtonEndapp)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MINR, &ROI_CONFIG::OnNMCustomdrawMinr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MAXR, &ROI_CONFIG::OnNMCustomdrawMaxr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MING, &ROI_CONFIG::OnNMCustomdrawMing)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MAXG, &ROI_CONFIG::OnNMCustomdrawMaxg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MINB, &ROI_CONFIG::OnNMCustomdrawMinb)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MAXB, &ROI_CONFIG::OnNMCustomdrawMaxb)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_KONTRAST, &ROI_CONFIG::OnNMCustomdrawKon2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_JAS, &ROI_CONFIG::OnNMCustomdrawJas)
END_MESSAGE_MAP()


// ROI_CONFIG message handlers


// IMPLEMENTATION\\

//  thread

DWORD WINAPI opencv_mainloop(void* pContext);



// GUI_Configuration message handlers


void ROI_CONFIG::OnStnClickedStaticMy()
{
	// TODO: Add your control notification handler code here
}


//INT_PTR GUI_Configuration::DoModal()
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialog::DoModal();
//}


BOOL ROI_CONFIG::OnInitDialog()
{
	CDialog::OnInitDialog();

	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //okno vzdy na vrchu

	BOOL bEnable = FALSE;     // TRUE to enable and FALSE to disable

	

	UINT nMenuf = bEnable ? (MF_BYCOMMAND) : (MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, nMenuf);
	}
	CSliderCtrl * sliderjas = (CSliderCtrl *)GetDlgItem(IDC_JAS);
	sliderjas->SetRange(0, 100);
	sliderjas->SetPos(cfg.getCam().getJas()); //nastavim jas
	CSliderCtrl * sliderkon = (CSliderCtrl *)GetDlgItem(IDC_KONTRAST);
	sliderkon->SetRange(10, 30);
	sliderkon->SetPos(cfg.getCam().getKontrast() * 10); //nastavim kontrast

	stringstream j;
	j << cfg.getCam().getJas();
	string jj = j.str();
	CA2T l(jj.c_str());
	LPCTSTR jasl = l;
	SetDlgItemText(IDC_J2, jasl); //label jas

	stringstream k;
	k << cfg.getCam().getKontrast();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_K2, konl); //label kontrast


	if (zmena == false) {
		CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MINR);
		slider->SetRange(0, 255);
		slider->SetPos(0);

		slider = (CSliderCtrl *)GetDlgItem(IDC_MING);
		slider->SetRange(0, 255);
		slider->SetPos(0);

		slider = (CSliderCtrl *)GetDlgItem(IDC_MINB);
		slider->SetRange(0, 255);
		slider->SetPos(0);

		slider = (CSliderCtrl *)GetDlgItem(IDC_MAXR);
		slider->SetRange(0, 255);
		slider->SetPos(255);

		slider = (CSliderCtrl *)GetDlgItem(IDC_MAXG);
		slider->SetRange(0, 255);
		slider->SetPos(255);

		slider = (CSliderCtrl *)GetDlgItem(IDC_MAXB);
		slider->SetRange(0, 255);
		slider->SetPos(255);
	}
	else {
		CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MINR);
		slider->SetRange(0, 255);
		slider->SetPos(cfg.getRmin());

		slider = (CSliderCtrl *)GetDlgItem(IDC_MING);
		slider->SetRange(0, 255);
		slider->SetPos(cfg.getGmin());

		slider = (CSliderCtrl *)GetDlgItem(IDC_MINB);
		slider->SetRange(0, 255);
		slider->SetPos(cfg.getBmin());

		slider = (CSliderCtrl *)GetDlgItem(IDC_MAXR);
		slider->SetRange(0, 255);
		slider->SetPos(cfg.getRmax());

		slider = (CSliderCtrl *)GetDlgItem(IDC_MAXG);
		slider->SetRange(0, 255);
		slider->SetPos(cfg.getGmax());

		slider = (CSliderCtrl *)GetDlgItem(IDC_MAXB);
		slider->SetRange(0, 255);
		slider->SetPos(cfg.getBmax());
	}


	// TODO:  Add extra initialization here
	GetDlgItem(IDC_END)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MIX_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POSITION_XY)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_OUTPUT_BOX)->ShowWindow(SW_HIDE);

	setComboList();

	if (cfg.getCam().getID() == -1) {
		cap = new VideoCapture(0);  // open the default camera

	}
	else {
		// otvori kameru a spusti thread 
		cap = new VideoCapture(cfg.getCam().getID());  // open the selected camera
		if (!(cap->isOpened())) {
			cap = new VideoCapture(0);
		}
		
	}


	hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);


	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DIG);
	pBtn->SetCheck(1);// check it

	CComboBox * pComboo = (CComboBox *)GetDlgItem(IDC_COMBO_ROI);
	pComboo->SetWindowTextW(L"Vyberte ROI");
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}




DWORD WINAPI opencv_mainloop(void* pContext)
{
	
	ROI_CONFIG *dlg = (ROI_CONFIG *)pContext;

	boolean playVideo = true;


	while (dlg->koniec==false)
	{		
		Mat frame,frame0;
		Mat frame_for_analyse;
		try {
			(*dlg->cap) >> frame;
			(*dlg->cap) >> dlg->frame2;			
			

			//------- prahovanie a zmena jasu a kontrastu----------	
			Mat mask;
			inRange(frame, Scalar(dlg->cfg.getBmin(), dlg->cfg.getGmin(), dlg->cfg.getRmin()), Scalar(dlg->cfg.getBmax(), dlg->cfg.getGmax(), dlg->cfg.getRmax()), mask);
			Mat vysl;
			bitwise_or(frame, frame, vysl, mask);
			frame = vysl;

		

			frame.convertTo(frame, -1, dlg->cfg.getCam().getKontrast(), dlg->cfg.getCam().getJas());
			//--------------------------------------

			BITMAPINFO bmi = { 0 };
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = 640;
			bmi.bmiHeader.biHeight = -480;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 24;
			CClientDC clientDC(&(dlg->pbmp));


				for (Roi r : dlg->cfg.getRoiList()) {
					Rect rect_drawn = Rect(r.getX(), r.getY(), r.getWidth(), r.getHeight());
					rectangle(frame, rect_drawn, Scalar(0, 0, 255), 2, 8, 0);
				}


				if (dlg->doRun) {

					int cas = (((std::clock() - dlg->cas) + 500) / 1000);

					if (cas > dlg->timeElapsed) {

						dlg->timeElapsed = cas;
						//ElapsedTime
						string te = "Elapsed time: " + to_string(dlg->timeElapsed) + " s";
						CA2T e(te.c_str());
						LPCTSTR labelText = e;
						dlg->SetDlgItemText(IDC_ELAPSED_TIME, labelText);

						if (dlg->timeElapsed % dlg->cfg.getInterval() == 0) {							

							(*dlg->cap) >> frame_for_analyse;

							//------- prahovanie a zmena jasu a kontrastu----------	
							/*Mat mask;
							inRange(frame_for_analyse, Scalar(dlg->cfg.getBmin(), dlg->cfg.getGmin(), dlg->cfg.getRmin()), Scalar(dlg->cfg.getBmax(), dlg->cfg.getGmax(), dlg->cfg.getRmax()), mask);
							Mat vysl;
							bitwise_or(frame_for_analyse, frame_for_analyse, vysl, mask);
							frame_for_analyse = vysl;*/


							Mat mask;
							Mat gray;
							Mat vysl;
							blur(frame_for_analyse, frame_for_analyse, Size(3, 3));
							cvtColor(frame_for_analyse, gray, CV_BGR2GRAY);
							inRange(frame_for_analyse, Scalar(dlg->cfg.getBmin(), dlg->cfg.getGmin(), dlg->cfg.getRmin()), Scalar(dlg->cfg.getBmax(), dlg->cfg.getGmax(), dlg->cfg.getRmax()), mask);							
							bitwise_or(gray, gray, vysl, mask);
							frame_for_analyse = vysl;

							frame_for_analyse.convertTo(frame_for_analyse, -1, dlg->cfg.getCam().getKontrast(), dlg->cfg.getCam().getJas());
							
							//--------------------------------------


							//tu sa zavolaju funckie na ziskanie hodnot
							//hodnoty sa zapisu do files a aj vypisu do textboxu riadok po riadku
							Config& cfg = dlg->cfg;
							for (Roi& r : cfg.getRoiList()) {

								Mat input = frame_for_analyse(Rect2d(r.getX(), r.getY(), r.getWidth(), r.getHeight()));
								DataGetter dg = DataGetter(input);
								try {
									double value = dg.getData() / pow(10.0, (double)r.getPDM());


									String title = "Èas: " + to_string(dlg->timeElapsed) + " Odmerana hodnota z " + r.getName() + " je " + to_string(value) + r.getUnits() + " \r\n " 
										"----------------------------------------------------- \r\n";
									CA2T a(title.c_str());
									LPCTSTR labelTexta = a;
									CString pom;
									dlg->GetDlgItemText(IDC_OUTPUT_BOX, pom);

									dlg->SetDlgItemText(IDC_OUTPUT_BOX, labelTexta + pom);
									r.addValue(value);
									dlg->cfg.getDataWriter().writeData(r.getName(), to_string(dlg->timeElapsed), to_string(value), r.getUnits());


								}
								catch (Exception e) {
									String title = "Èas: " + to_string(dlg->timeElapsed) + " Nepodarilo sa odcitat hodnotu z roi " + r.getName() + " \r\n";
									CA2T a(title.c_str());
									LPCTSTR labelTexta = a;
									CString pom;
									dlg->GetDlgItemText(IDC_OUTPUT_BOX, pom);

									dlg->SetDlgItemText(IDC_OUTPUT_BOX, labelTexta + pom);
								}
							}

						}

						if ((dlg->cfg.getIntAvgMed() > 0 && dlg->timeElapsed % dlg->cfg.getIntAvgMed() == 0) && (dlg->cfg.doAverage() || dlg->cfg.doMedian())) {
							//vypocita sa priemer a median podla nastavenia
							//premaze sa textbox kvoli pamati


							CString clear = L"";
							dlg->SetDlgItemText(IDC_OUTPUT_BOX, clear);


							if (dlg->cfg.doAverage()) {
								for (Roi r : dlg->cfg.getRoiList()) {
									double avg = r.getAverage();
									dlg->cfg.getDataWriter().writeAverage(r.getName(), to_string(dlg->timeElapsed), to_string(avg), r.getUnits());
								}
							}

							if (dlg->cfg.doMedian()) {
								for (Roi r : dlg->cfg.getRoiList()) {
									double med = r.getAverage();
									dlg->cfg.getDataWriter().writeMedian(r.getName(), to_string(dlg->timeElapsed), to_string(med), r.getUnits());
								}
							}

							for (Roi r : dlg->cfg.getRoiList()) r.eraseValues();


						}

						//ak sa nepocita priemer, vymaz textbox po 30 pocitaniach
						else if (!(dlg->cfg.doAverage() || dlg->cfg.doMedian()) && dlg->timeElapsed / dlg->cfg.getInterval() % 30 == 0) {
							CString clear = L"";
							dlg->SetDlgItemText(IDC_OUTPUT_BOX, clear);
						}
					}
				}


				SetDIBitsToDevice(clientDC, 0, 0, 640, 480, 0, 0, 0, 480, frame.data, &bmi, 0);
				waitKey(0);
			}

			catch (Exception e) {

			}
		
	}
	return false;

}

bool Validate_Int(CString str)
{
	for (int i = 0; i < str.GetLength(); i++) {
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

bool Validate_String(CString str)
{
	for (int i = 0; i < str.GetLength(); i++) {
		if (!std::isalpha(str[i]))
			return false;
	}
	return true;
}



void ROI_CONFIG::OnBnClickedNewRoi()
{
	//Get value from dialog attributes
	CDialog::UpdateData(true);

	TerminateThread(hThread, 0);
	CloseHandle(hThread);


	SetDlgItemText(IDC_ROI_NAME, L"");
	SetDlgItemText(IDC_CURRENT, L"");
	SetDlgItemText(IDC_COUNT_FP, L"");
	SetDlgItemText(IDC_EDIT_MIX_XY, L"");
	SetDlgItemText(IDC_EDIT_MIN_VALUE, L"");
	SetDlgItemText(IDC_EDIT_POSITION_XY, L"");
	SetDlgItemText(IDC_EDIT_MAX_XY, L"");
	SetDlgItemText(IDC_EDIT_MAX_VALUE, L"");
	
	// Select ROI
	bool showCrosshair = false;
	bool fromCenter = false;
	roi = selectROI("SELECT_ROI", frame2, showCrosshair, fromCenter);
	
	//ROI ODSEKNUTA
	Mat imCrop = frame2(roi); // crop image 
	//imshow("E", imCrop);

	stringstream ss;
	ss << "X1=" << roi.x << "; Y1=" << roi.y << "; | X2=" << roi.x + roi.width << "; Y2=" << roi.y + roi.height << ";";
	string str = ss.str();
	CA2T t(str.c_str());
	LPCTSTR roi_position = t;

	SetDlgItemText(IDC_ROI_POSITION, roi_position);

	if (waitKey(1) >= 0) return;

	// TODO: Add your control notification handler code here
}


void ROI_CONFIG::OnBnClickedSaveRoi()
{
	CString ROI_NAME;
	GetDlgItemText(IDC_ROI_NAME, ROI_NAME);
	CString ROI_CURRENT;
	GetDlgItemText(IDC_CURRENT, ROI_CURRENT);
	CString ROI_FP;
	GetDlgItemText(IDC_COUNT_FP, ROI_FP);

	CString ROI_MIN_XY;
	GetDlgItemText(IDC_EDIT_MIX_XY, ROI_MIN_XY);
	CString ROI_MIN_VALUE;
	GetDlgItemText(IDC_EDIT_MIN_VALUE, ROI_MIN_VALUE);
	CString ROI_POSITION_XY;
	GetDlgItemText(IDC_EDIT_POSITION_XY, ROI_POSITION_XY);
	CString ROI_MAX_XY;
	GetDlgItemText(IDC_EDIT_MAX_XY, ROI_MAX_XY);
	CString ROI_MAX_VALUE;
	GetDlgItemText(IDC_EDIT_MAX_VALUE, ROI_MAX_VALUE);


	// KONTROLA ZADANYCH PARAMETROV U DIGITALNYCH ZARIADENI
	CButton *but_dig = (CButton*)GetDlgItem(IDC_CHECK_DIG);
	bool is_dig = (but_dig->GetCheck() == 1);
	if (is_dig) {
		if (ROI_NAME != "" && ROI_CURRENT != "" && ROI_FP != "" && Validate_Int(ROI_FP)) {
				stringstream ss;
				CT2CA pszConvertedAnsiString(ROI_NAME);
				string roi_name1(pszConvertedAnsiString);
				ss << "[ " << roi_name1 << " ] ROI bola uložená!";
				string str = ss.str();
				CA2T t(str.c_str());
				LPCTSTR roi_name = t;
				LPCTSTR msg = roi_name;
				LPCTSTR title = L"Výber ROI";
				

				CT2CA pszConvertedAnsiString2(ROI_CURRENT);
				string roi_units(pszConvertedAnsiString2);

				CT2CA pszConvertedAnsiString3(ROI_FP);
				string roi_pdm_str(pszConvertedAnsiString3);
				int roi_pdm = stoi(roi_pdm_str);

				if (cfg.RoiWithNameExists(roi_name1)) {
					LPCTSTR title = L"Výber ROI";
					LPCTSTR msg = L"Oblas záujmu s takýmto názvom už existuje, prosím vyberte iný!";
					::MessageBox(0, msg, title, MB_OK);
					hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);
					return;
				}

				Roi new_roi = Roi(roi_name1, roi.x, roi.y, roi.width, roi.height, roi_pdm, roi_units, "digital");
				cfg.addRoi(new_roi);

				setComboList();

				SetDlgItemText(IDC_ROI_POSITION, L"");
				destroyWindow("SELECT_ROI");


				::MessageBox(0, msg, title, MB_OK);

				hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);
				return;
		}
		else {
			LPCTSTR msg = L"Neboli zadané potrebné atribúty alebo sú nekorektné vstupy!";
			LPCTSTR title = L"Výber ROI";
			::MessageBox(0, msg, title, MB_OK);
			hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);
			return;
		}
	}

	// KONTROLA ZADANYCH PARAMETROV U ANALOGOVYCH ZARIADENI
	CButton *but_ana = (CButton*)GetDlgItem(IDC_CHECK_ANA);
	bool is_ana = (but_ana->GetCheck() == 1);
	if (is_ana) {
		if (ROI_NAME != "" && ROI_CURRENT != "" && ROI_FP != "" && ROI_MIN_XY != "" 
			&& ROI_MIN_VALUE != "" && ROI_POSITION_XY != "" && ROI_MAX_XY != "" 
			&& ROI_MAX_VALUE != "" && Validate_Int(ROI_FP)) {
				stringstream ss;
				CT2CA pszConvertedAnsiString(ROI_NAME);
				string roi_name1(pszConvertedAnsiString);
				ss << "[ " << roi_name1 << " ] ROI bola uložená!";
				string str = ss.str();
				CA2T t(str.c_str());
				LPCTSTR roi_name = t;
				LPCTSTR msg = roi_name;
				LPCTSTR title = L"Výber ROI";
				::MessageBox(0, msg, title, MB_OK);

				SetDlgItemText(IDC_ROI_POSITION, L"");
				destroyWindow("SELECT_ROI");
				hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);
				return;
		}
		else {
			LPCTSTR msg = L"Neboli zadané potrebné atribúty alebo nekorektné vstupy!";
			LPCTSTR title = L"Výber ROI";
			::MessageBox(0, msg, title, MB_OK);

			SetDlgItemText(IDC_ROI_POSITION, L"");
			destroyWindow("SELECT_ROI");
			hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);
			return;
		}
	}



	// TODO: Add your control notification handler code here
}


void ROI_CONFIG::OnBnClickedDeleteRoi()
{
	SetDlgItemText(IDC_ROI_POSITION, L"");
	destroyWindow("SELECT_ROI");

	hThread = CreateThread(NULL, 0, opencv_mainloop, this, 0L, NULL);
	// TODO: Add your control notification handler code here
}


void ROI_CONFIG::OnBnClickedStart()
{
	GetDlgItem(IDC_ROI_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COUNT_FP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CURRENT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DELETE_ROI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POSITION_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_ANA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_DIG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NEW_ROI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SAVE_ROI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_SAVE_CF)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_EDIT_MIX_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ROI_POSITION)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_START)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BTCFG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_ROI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC12)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DEL_SEL_ROI)->ShowWindow(SW_HIDE);


	GetDlgItem(IDC_ELAPSED_TIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_OUTPUT_BOX)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_END)->ShowWindow(SW_SHOW);


	doRun = true;
	cas = std::clock();
	
	cfg.setDataWriter();


	//timeElapsed = 0;
	//runTime = thread(&ROI_CONFIG::run, this);
	//runTime.join();


	// TODO: Add your control notification handler code here
}




void ROI_CONFIG::OnBnClickedCheckAna()
{
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK_ANA);
	pBtn1->SetCheck(1);// check it
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DIG);
	pBtn->SetCheck(0);// uncheck it

	GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MAX_XY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_POSITION_XY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MIX_XY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC7)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC9)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC10)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC11)->ShowWindow(SW_SHOW);
	// TODO: Add your control notification handler code here
}


void ROI_CONFIG::OnBnClickedCheckDig()
{
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_CHECK_DIG);
	pBtn1->SetCheck(1);// check it
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ANA);
	pBtn->SetCheck(0);// uncheck it

	GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POSITION_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MIX_XY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);



	// TODO: Add your control notification handler code here
}


void ROI_CONFIG::OnBnClickedEnd()
{
	
	//PostQuitMessage(0);
	//ASSERT(AfxGetMainWnd() != NULL);
	//AfxGetMainWnd()->SendMessage(WM_CLOSE);

	GetDlgItem(IDC_ROI_NAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COUNT_FP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CURRENT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DELETE_ROI)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_ANA)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_DIG)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NEW_ROI)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SAVE_ROI)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_SAVE_CF)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ROI_POSITION)->ShowWindow(SW_SHOW);
	GetDlgItem(ID_START)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_BTCFG)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_ROI)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC12)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DEL_SEL_ROI)->ShowWindow(SW_SHOW);


	GetDlgItem(IDC_ELAPSED_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OUTPUT_BOX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_END)->ShowWindow(SW_HIDE);

	//runTime.~thread();

	doRun = false;
	cas = 0;
	timeElapsed = 0;
	//ElapsedTime
	string te = "Elapsed time: " + to_string(timeElapsed) + " s";
	CA2T e(te.c_str());
	LPCTSTR labelText = e;
	SetDlgItemText(IDC_ELAPSED_TIME, labelText);

	CString clear = L"";
	SetDlgItemText(IDC_OUTPUT_BOX, clear);


	// TODO: Add your control notification handler code here
}


void ROI_CONFIG::OnBnClickedBtnSaveCf()
{
	//funguje
	if (!cfg.writeToFile()) {
		LPCTSTR msg = L"Konfiguráciu sa nepodarilo uloži!";
		LPCTSTR title = L"Chyba ukladania";
		::MessageBox(0, msg, title, MB_OK);
	}
	else {
		LPCTSTR msg = L"Konfiguráciu sa podarilo uloži!";
		LPCTSTR title = L"Ukladanie úspešné";
		::MessageBox(0, msg, title, MB_OK);
	}
}


void ROI_CONFIG::OnCbnSelchangeComboRoi()
{
	CComboBox * pComboRoi = (CComboBox *)GetDlgItem(IDC_COMBO_ROI);
	int idx = pComboRoi->GetCurSel();
	CString pom;
	pComboRoi->GetLBText(idx, pom);

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(pom);

	// construct a std::string using the LPCSTR input
	string roi_name(pszConvertedAnsiString);

	Roi roi = cfg.getRoiByName(roi_name);

	if (roi_name == roi.getName()) {
		GetRoiInfo(roi);
	}



	// TODO: Add your control notification handler code here
}

void ROI_CONFIG::setComboList()
{
	CComboBox * pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_ROI);
	pCombo->ResetContent();

	for each (Roi roi in cfg.getRoiList())
	{
		
		string str = roi.getName();
		CA2T t(str.c_str());
		LPCTSTR roi_name = t;
		pCombo->AddString(roi_name);
	}
}

void ROI_CONFIG::GetRoiInfo(Roi roi)
{

	SetDlgItemText(IDC_ROI_NAME, CA2T(roi.getName().c_str()));
	SetDlgItemText(IDC_COUNT_FP, CA2T(to_string(roi.getPDM()).c_str()));
	SetDlgItemText(IDC_CURRENT, CA2T(roi.getUnits().c_str()));


	if (roi.getType() == "digital") {

		OnBnClickedCheckDig();

	}

	else {
		OnBnClickedCheckAna();

		vector<AnalogPoint> points = roi.getPoints();

		AnalogPoint bod1 = points[0];
		AnalogPoint bod2 = points[1];
		AnalogPoint bod3 = points[2];

		string pom = to_string(bod1.getX()) + " , " + to_string(bod1.getY());
		SetDlgItemText(IDC_EDIT_MAX_XY, CA2T(pom.c_str()));

		pom = to_string(bod2.getX()) + " , " + to_string(bod2.getY());
		SetDlgItemText(IDC_EDIT_MIX_XY, CA2T(pom.c_str()));

		pom = to_string(bod3.getX()) + " , " + to_string(bod3.getY());
		SetDlgItemText(IDC_EDIT_POSITION_XY, CA2T(pom.c_str()));
	}

}


void ROI_CONFIG::OnBnClickedBtnBtcfg()
{
	koniec = true;
	CDialog::UpdateData(true);
	CDialog::OnOK();
	
	CCAM_CONFIG CK;
	CK.cfg = cfg;
	CK.zmena = true;	
	CK.DoModal();
}


string ROI_CONFIG::getFileName(const string& s) {
	char sep = '/';
#ifdef _WIN32
	sep = '\\';
#endif
	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}
	return("");
}

void ROI_CONFIG::run()
{
	//std::this_thread::sleep_for(1s);
}




void ROI_CONFIG::OnBnClickedDelSelRoi()
{
	//najdi roi podla mena, ktore ziska z listboxu
	//vymaze, updatne roi list

	CComboBox * pComboRoi = (CComboBox *)GetDlgItem(IDC_COMBO_ROI);
	int idx = pComboRoi->GetCurSel();
	CString pom;
	pComboRoi->GetLBText(idx, pom);

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(pom);

	// construct a std::string using the LPCSTR input
	string roi_name(pszConvertedAnsiString);

	if (cfg.RoiWithNameExists(roi_name)) {
		cfg.deleteRoi(cfg.getRoiByName(roi_name));
		setComboList();
	}

	SetDlgItemText(IDC_ROI_NAME, L"");
	SetDlgItemText(IDC_CURRENT, L"");
	SetDlgItemText(IDC_COUNT_FP, L"");
	SetDlgItemText(IDC_EDIT_MIX_XY, L"");
	SetDlgItemText(IDC_EDIT_MIN_VALUE, L"");
	SetDlgItemText(IDC_EDIT_POSITION_XY, L"");
	SetDlgItemText(IDC_EDIT_MAX_XY, L"");
	SetDlgItemText(IDC_EDIT_MAX_VALUE, L"");


}


void ROI_CONFIG::OnBnClickedButtonEndapp()
{
	koniec = true;
	TerminateThread(hThread, 0);
	CloseHandle(hThread);
	OnOK();	
}


void ROI_CONFIG::OnNMCustomdrawMinr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MINR);
	stringstream k;
	k << slider->GetPos();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_LRMIN, konl);
	cfg.setRmin(slider->GetPos());
	*pResult = 0;
}


void ROI_CONFIG::OnNMCustomdrawMaxr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MAXR);
	stringstream k;
	k << slider->GetPos();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_LRMAX, konl);
	cfg.setRmax(slider->GetPos());
	*pResult = 0;
}


void ROI_CONFIG::OnNMCustomdrawMing(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MING);
	stringstream k;
	k << slider->GetPos();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_LGMIN, konl);
	cfg.setGmin(slider->GetPos());
	*pResult = 0;
}


void ROI_CONFIG::OnNMCustomdrawMaxg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MAXG);
	stringstream k;
	k << slider->GetPos();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_LGMAX, konl);
	cfg.setGmax(slider->GetPos());
	*pResult = 0;
}


void ROI_CONFIG::OnNMCustomdrawMinb(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MINB);
	stringstream k;
	k << slider->GetPos();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_LBMIN, konl);
	cfg.setBmin(slider->GetPos());
	*pResult = 0;
}


void ROI_CONFIG::OnNMCustomdrawMaxb(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * slider = (CSliderCtrl *)GetDlgItem(IDC_MAXB);
	stringstream k;
	k << slider->GetPos();
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_LBMAX, konl);
	cfg.setBmax(slider->GetPos());
	*pResult = 0;
}



void ROI_CONFIG::OnNMCustomdrawKon2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * sliderkon = (CSliderCtrl *)GetDlgItem(IDC_KONTRAST);
	stringstream k;
	k << sliderkon->GetPos() / 10.0;
	string kk = k.str();
	CA2T c(kk.c_str());
	LPCTSTR konl = c;
	SetDlgItemText(IDC_K2, konl);
	Camera n = Camera(cfg.getCam().getID(), cfg.getCam().getRes(), cfg.getCam().getJas(), sliderkon->GetPos() / 10.0, "auto");
	cfg.setCam(n);
	*pResult = 0;
}


void ROI_CONFIG::OnNMCustomdrawJas(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CSliderCtrl * sliderjas = (CSliderCtrl *)GetDlgItem(IDC_JAS);
	stringstream j;
	j << sliderjas->GetPos();
	string jj = j.str();
	CA2T l(jj.c_str());
	LPCTSTR jasl = l;
	SetDlgItemText(IDC_J2, jasl);
	Camera n = Camera(cfg.getCam().getID(), cfg.getCam().getRes(), sliderjas->GetPos(), cfg.getCam().getKontrast(), "auto");
	cfg.setCam(n);
	*pResult = 0;
}
