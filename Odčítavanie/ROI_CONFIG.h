#pragma once
#include "afxwin.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/ocl.hpp>



#include <math.h>
#include "afxwin.h"
#include "Config.h"
#include "Roi.h"

using namespace cv;
using namespace std;
// TIMER 
class timer {
private:
	unsigned long begTime;
public:
	void start() {
		begTime = clock();
	}

	unsigned long elapsedTime() {
		return ((unsigned long)clock() - begTime) / CLOCKS_PER_SEC;
	}

	bool isTimeout(unsigned long seconds) {
		return seconds >= elapsedTime();
	}
};


// ROI_CONFIG dialog

class ROI_CONFIG : public CDialog
{
	DECLARE_DYNAMIC(ROI_CONFIG)

public:
	ROI_CONFIG(CWnd* pParent = NULL);   // standard constructor
	virtual ~ROI_CONFIG();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GUI_ROI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	VideoCapture *cap;
	HANDLE hThread;
	CStatic pbmp;
	std::thread runTime;
	Config cfg;
	Mat frame2;
	Rect2d roi;
	bool koniec = false;

	bool zmena = false;

	bool doRun = false;
	
	clock_t cas;
	int timeElapsed = 0;


	afx_msg void OnStnClickedStaticMy();
	//	virtual INT_PTR DoModal();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedNewRoi();
	afx_msg void OnBnClickedSaveRoi();
	afx_msg void OnBnClickedDeleteRoi();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedCheckAna();
	afx_msg void OnBnClickedCheckDig();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedBtnSaveCf();
	afx_msg void OnCbnSelchangeComboRoi();

	void setComboList();
	void GetRoiInfo(Roi roi);
	string getFileName(const string& s);
	void run();

	afx_msg void OnBnClickedBtnBtcfg();
	afx_msg void OnBnClickedDelSelRoi();
	afx_msg void OnBnClickedButtonEndapp();
	afx_msg void OnNMCustomdrawMinr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawMaxr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawMing(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawMaxg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawMinb(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawMaxb(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedVyprahovane();
	afx_msg void OnNMCustomdrawKon2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawJas(NMHDR *pNMHDR, LRESULT *pResult);
};
