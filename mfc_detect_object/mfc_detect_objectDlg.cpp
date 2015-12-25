
// mfc_detect_objectDlg.cpp : 实现文件
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
static string window_name = "人脸识别";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// Cmfc_detect_objectDlg 对话框

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


// Cmfc_detect_objectDlg 消息处理程序

BOOL Cmfc_detect_objectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//SetTimer(1,100,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cmfc_detect_objectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cmfc_detect_objectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmfc_detect_objectDlg::OnBnClickedOpenButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置过滤器   
    //TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||"); 
	//KillTimer(1);
	//将进度条重新置位为0
	m_Progress.SetPos(0);
	TCHAR szFilter[] = _T("JPEG files(*.jpeg *.jpg *.jpe)|*.jpeg;*.jpg;*.jpe|JPEG 2000 files(*.jp2)|*.jp2|Portable Network Graphics(*.png)|*.png|Portable image format(*.pbm *.pgm *.ppm)|*.pbm;*.pgm;*.ppm|Sun rasters(*.sr *.ras)|*.sr;*.ras|TIFF files(*.tiff *.tif)|*.tiff;*.tif|Windows bitmaps(*.bmp *dib)|*.bmp;*.dib|所有文件(*.*)|*.*||");
    // 构造打开文件对话框   
    CFileDialog fileDlg(TRUE, _T("jpg"), NULL, 0, szFilter, this);   
    // 显示打开文件对话框   
	CString strFilePath;  
    if (IDOK == fileDlg.DoModal())   
    {   
        // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
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
		//退出程序
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
	// TODO: 在此添加控件通知处理程序代码
	//-- Load the cascades
	//UINT_PTR SetTimer
	//(
    //HWND hWnd, // 窗口句柄
    //UINT_PTR nIDEvent, // 定时器ID，多个定时器时，可以通过该ID判断是哪个定时器
    //UINT nElapse, // 时间间隔,单位为毫秒
    //TIMERPROC lpTimerFunc // 回调函数
    //);
	//定义一个定时器
    SetTimer(1,100,NULL);
	if( !face_cascade.load( face_cascade_name ) ){ 
		TRACE("--(!)Error loading\n"); 
		ExitProcess(-1);
	};
	Mat frame_gray;
	std::vector<Rect> faces;
	image = imread(cv::String(image_file_name_),CV_LOAD_IMAGE_COLOR);
	//Haar特征基于灰度图
	cvtColor( image, frame_gray, COLOR_BGR2GRAY);
	//变换后的图像进行直方图均衡化处理
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
	//这里的用户按键不能按，按了之后会出错，一直没有找到准确的原因。
	//在imshow()之后如果没有waitKey()有时候不能正常显示图像
	waitKey(0);
	destroyWindow(window_name);
}


void Cmfc_detect_objectDlg::OnNMCustomdrawDetectProgress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void Cmfc_detect_objectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nCurPos = m_Progress.GetPos(); 
   m_Progress.SetPos(nCurPos+20); 
   triggerCount += 1;
   CDialog::OnTimer(nIDEvent); 
   CDialogEx::OnTimer(nIDEvent); 
   if(triggerCount == 5)
   {
	   KillTimer(nIDEvent);
	   triggerCount = 0;
	   //等待按键，否则在tiggerCount == 5时,可能会出现进度条一直为空
	   //中间肯定有变化，但没来得及显示就被重置为0了
	   //waitKey(0);
	   //m_Progress.SetPos(0);
   }
}


void Cmfc_detect_objectDlg::OnBnClickedSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("JPEG files(*.jpeg *.jpg *.jpe)|*.jpeg;*.jpg;*.jpe|JPEG 2000 files(*.jp2)|*.jp2|Portable Network Graphics(*.png)|*.png|Portable image format(*.pbm *.pgm *.ppm)|*.pbm;*.pgm;*.ppm|Sun rasters(*.sr *.ras)|*.sr;*.ras|TIFF files(*.tiff *.tif)|*.tiff;*.tif|Windows bitmaps(*.bmp *dib)|*.bmp;*.dib|所有文件(*.*)|*.*||");   
	// 构造保存文件对话框   
	CFileDialog fileDlg(FALSE, _T(""), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	image_file_name_.Delete(image_file_name.GetLength() - 4, image_file_name.GetLength());
	CString output_file_name =image_file_name_ +  "_result.jpg";
	imwrite(cv::String(output_file_name), image);
	outPutFileName = output_file_name;
	UpdateData(FALSE);
	MessageBoxA("检测到的文件已经被成功保存!");
	waitKey(0);
	ExitProcess(-1);
}
