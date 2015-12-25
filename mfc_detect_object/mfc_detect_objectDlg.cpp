
// mfc_detect_objectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_detect_object.h"
#include "mfc_detect_objectDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <cctype>
#include <iterator>
#include "cv.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui/highgui_c.h"

using namespace std;
using namespace cv;
static CString image_file_name = _T("");
static CString image_file_name_= _T("");
string face_cascade_name = "D:/OpenCv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
Mat image;
static string window_name = "����ʶ��";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cmfc_detect_objectDlg �Ի���

Cmfc_detect_objectDlg::Cmfc_detect_objectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmfc_detect_objectDlg::IDD, pParent)
	, outPutFileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	outPutFileName = _T("");
	triggerCount = 0;
}

void Cmfc_detect_objectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DETECT_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_SAVE_EDIT, outPutFileName);
}

BEGIN_MESSAGE_MAP(Cmfc_detect_objectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_BUTTON, &Cmfc_detect_objectDlg::OnBnClickedOpenButton)
	ON_BN_CLICKED(IDC_DETECT_BUTTON, &Cmfc_detect_objectDlg::OnBnClickedDetectButton)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DETECT_PROGRESS, &Cmfc_detect_objectDlg::OnNMCustomdrawDetectProgress)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &Cmfc_detect_objectDlg::OnBnClickedSaveButton)
END_MESSAGE_MAP()


// Cmfc_detect_objectDlg ��Ϣ�������

BOOL Cmfc_detect_objectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//SetTimer(1,100,NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cmfc_detect_objectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cmfc_detect_objectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cmfc_detect_objectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmfc_detect_objectDlg::OnBnClickedOpenButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����   
    //TCHAR szFilter[] = _T("�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*||"); 
	//KillTimer(1);
	//��������������λΪ0
	m_Progress.SetPos(0);
	TCHAR szFilter[] = _T("JPEG files(*.jpeg *.jpg *.jpe)|*.jpeg;*.jpg;*.jpe|JPEG 2000 files(*.jp2)|*.jp2|Portable Network Graphics(*.png)|*.png|Portable image format(*.pbm *.pgm *.ppm)|*.pbm;*.pgm;*.ppm|Sun rasters(*.sr *.ras)|*.sr;*.ras|TIFF files(*.tiff *.tif)|*.tiff;*.tif|Windows bitmaps(*.bmp *dib)|*.bmp;*.dib|�����ļ�(*.*)|*.*||");
    // ������ļ��Ի���   
    CFileDialog fileDlg(TRUE, _T("jpg"), NULL, 0, szFilter, this);   
    // ��ʾ���ļ��Ի���   
	CString strFilePath;  
    if (IDOK == fileDlg.DoModal())   
    {   
        // ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
        strFilePath = fileDlg.GetPathName(); 
		image_file_name = strFilePath;
		image_file_name_ = strFilePath;
        SetDlgItemText(IDC_OPEN_EDIT, strFilePath);   
    }
	//Mat image = imread( cv::String(image_file_name));
	IplImage* img = cvLoadImage(image_file_name);
	if(!img)
	{
		MessageBoxA("No data!---Please open an image first!");	
		//�˳�����
		ExitProcess(-1);
	}
	cvNamedWindow(image_file_name,CV_WINDOW_AUTOSIZE);
	cvShowImage(image_file_name,img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow(image_file_name);
}

void Cmfc_detect_objectDlg::OnBnClickedDetectButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//-- Load the cascades
	//UINT_PTR SetTimer
	//(
    //HWND hWnd, // ���ھ��
    //UINT_PTR nIDEvent, // ��ʱ��ID�������ʱ��ʱ������ͨ����ID�ж����ĸ���ʱ��
    //UINT nElapse, // ʱ����,��λΪ����
    //TIMERPROC lpTimerFunc // �ص�����
    //);
	//����һ����ʱ��
    SetTimer(1,100,NULL);
	if( !face_cascade.load( face_cascade_name ) ){ 
		TRACE("--(!)Error loading\n"); 
		ExitProcess(-1);
	};
	Mat frame_gray;
	std::vector<Rect> faces;
	image = imread(cv::String(image_file_name_),CV_LOAD_IMAGE_COLOR);
	//Haar�������ڻҶ�ͼ
	cvtColor( image, frame_gray, COLOR_BGR2GRAY);
	//�任���ͼ�����ֱ��ͼ���⻯����
	equalizeHist( frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(1,1));

	for(size_t i = 0; i < faces.size(); i++ )
	{
		Point center( cvRound(faces[i].x + faces[i].width*0.5), cvRound(faces[i].y + faces[i].height*0.5) );
		ellipse( image, center, Size( cvRound(faces[i].width*0.5), cvRound(faces[i].height*0.5)), 0, 0, 360, Scalar( 0, 255, 0 ), 2, 8, 0 );
	}
	faces.clear();
	//-- Show what you got
	imshow(window_name, image);
	//������û��������ܰ�������֮������һֱû���ҵ�׼ȷ��ԭ��
	//��imshow()֮�����û��waitKey()��ʱ����������ʾͼ��
	waitKey(0);
	destroyWindow(window_name);
}


void Cmfc_detect_objectDlg::OnNMCustomdrawDetectProgress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void Cmfc_detect_objectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nCurPos = m_Progress.GetPos(); 
   m_Progress.SetPos(nCurPos+20); 
   triggerCount += 1;
   CDialog::OnTimer(nIDEvent); 
   CDialogEx::OnTimer(nIDEvent); 
   if(triggerCount == 5)
   {
	   KillTimer(nIDEvent);
	   triggerCount = 0;
	   //�ȴ�������������tiggerCount == 5ʱ,���ܻ���ֽ�����һֱΪ��
	   //�м�϶��б仯����û���ü���ʾ�ͱ�����Ϊ0��
	   //waitKey(0);
	   //m_Progress.SetPos(0);
   }
}


void Cmfc_detect_objectDlg::OnBnClickedSaveButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("JPEG files(*.jpeg *.jpg *.jpe)|*.jpeg;*.jpg;*.jpe|JPEG 2000 files(*.jp2)|*.jp2|Portable Network Graphics(*.png)|*.png|Portable image format(*.pbm *.pgm *.ppm)|*.pbm;*.pgm;*.ppm|Sun rasters(*.sr *.ras)|*.sr;*.ras|TIFF files(*.tiff *.tif)|*.tiff;*.tif|Windows bitmaps(*.bmp *dib)|*.bmp;*.dib|�����ļ�(*.*)|*.*||");   
	// ���챣���ļ��Ի���   
	CFileDialog fileDlg(FALSE, _T(""), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	image_file_name_.Delete(image_file_name.GetLength() - 4, image_file_name.GetLength());
	CString output_file_name =image_file_name_ +  "_result.jpg";
	imwrite(cv::String(output_file_name), image);
	outPutFileName = output_file_name;
	UpdateData(FALSE);
	MessageBoxA("��⵽���ļ��Ѿ����ɹ�����!");
	waitKey(0);
	ExitProcess(-1);
}
