#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/ocl.hpp>

#include <math.h>
#include <string>
#include "afxwin.h"
#include "Config.h"

using namespace cv;

// CCAM_CONFIG dialog

class CCAM_CONFIG : public CDialogEx
{
	DECLARE_DYNAMIC(CCAM_CONFIG)
	

public:
	CCAM_CONFIG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCAM_CONFIG();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	Config cfg = Config();
	String save_file_path;
	String config_file_path;
	bool zmena = false;
	int vsetky = -1;

	// Implementation
protected:
	HICON m_hIcon;

	string getFileName(const string & s);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
//DECLARE_MESSAGE_MAP()

public:
	CString configFilePath;
	CString saveFilePath;


	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMCustomdrawJas(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawKon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedDigitsBright();
	afx_msg void OnBnClickedDigitsDark();
};
