// DuiVisionDesignerView.cpp : implementation of the CDuiVisionDesignerView class
//

#include "stdafx.h"
#include "DuiVisionDesigner.h"
#include "DuiVisionDesignerDoc.h"
#include "DuiVisionDesignerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView

IMPLEMENT_DYNCREATE(CDuiVisionDesignerView, CView)

BEGIN_MESSAGE_MAP(CDuiVisionDesignerView, CView)
	//{{AFX_MSG_MAP(CDuiVisionDesignerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView construction/destruction

CDuiVisionDesignerView::CDuiVisionDesignerView()
{
	// TODO: add construction code here
	m_ulRefCount = 0;
	m_pDuiPluginPanelObject = NULL;
	m_uTimerAnimation = 0;
	m_uTimerAutoClose = 0;
	m_nTooltipCtrlID = 0;
}

CDuiVisionDesignerView::~CDuiVisionDesignerView()
{
	CTimer::KillTimer(m_uTimerAnimation);

	// 释放DuiVision插件的Panel对象实例
	if((theApp.m_pIPlatUI != NULL) && (m_pDuiPluginPanelObject != NULL))
	{
		theApp.m_pIPlatUI->ReleaseVciObject(m_pDuiPluginPanelObject, FALSE);
	}
}

BOOL CDuiVisionDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// 定时器消息
void CDuiVisionDesignerView::OnTimer(UINT uTimerID)
{
	if(m_uTimerAnimation == uTimerID)	// 动画定时器
	{
		if(m_pDuiPluginPanelObject)
		{
			m_pDuiPluginPanelObject->OnTimer(uTimerID, NULL);
		}
	}else
	if(m_uTimerAutoClose == uTimerID)	// 窗口自动关闭的定时器
	{
		CTimer::KillTimer(m_uTimerAutoClose);
		m_uTimerAutoClose = 0;
		/*if(m_bAutoHide)
		{
			ShowWindow(SW_HIDE);	// 隐藏窗口模式
		}else
		{
			DoClose();	// 关闭窗口模式
		}*/
	}
}

// 定时器消息(带定时器名字的定时函数)
void CDuiVisionDesignerView::OnTimer(UINT uTimerID, CString strTimerName)
{
	// 应用创建的定时器都调用事件处理对象的定时处理函数
	//DuiSystem::Instance()->CallDuiHandlerTimer(uTimerID, strTimerName);
}

// 设置当前的Tooltip
void CDuiVisionDesignerView::SetTooltip(int nCtrlID, LPCTSTR lpszTooltip, CRect rect, int nTipWidth)
{
	m_wndToolTip.DelTool(this, 1);
	m_wndToolTip.Activate(FALSE);
	CString strTooltip = lpszTooltip;
	if(!strTooltip.IsEmpty())
	{
		if(nTipWidth > 0)
		{
			// 指定tooltip的宽度
			m_wndToolTip.SetMaxTipWidth(nTipWidth);
		}
		m_wndToolTip.AddTool(this, strTooltip, rect, 1);	//Tooltip默认都用1作为ID
		m_wndToolTip.Activate(TRUE);
		m_nTooltipCtrlID = nCtrlID;
	}
}

// 清除当前的Tooltip
void CDuiVisionDesignerView::ClearTooltip()
{
	m_wndToolTip.DelTool(this, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView drawing

void CDuiVisionDesignerView::OnDraw(CDC* pDC)
{
	CDuiVisionDesignerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect rect;
	GetClientRect(rect);

	CDC memDC ;
	CBitmap memBitmap ;
	CBitmap* oldBitmap ; // bitmap originally found in CMemDC

	// no real plotting work is performed here, 
	// just putting the existing bitmaps on the client
	// to avoid flicker, establish a memory dc, draw to it 
	// and then BitBlt it to the client
	memDC.CreateCompatibleDC(pDC) ;
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()) ;
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

	// 背景复制到内存dc
	//memDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	// 画白色背景
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, WHITENESS);

	if (memDC.GetSafeHdc() != NULL)
	{
		// first drop the controls on the memory dc
		if(m_pDuiPluginPanelObject)
		{
			m_pDuiPluginPanelObject->DrawControl(memDC, rect);
			// finally send the result to the display
			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), 
				  &memDC, 0, 0, SRCCOPY) ;
		}
	}

	memDC.SelectObject(oldBitmap) ;

	/*
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->DrawControl(*pDC, rect);
	}*/
	
}

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView printing

void CDuiVisionDesignerView::OnFilePrintPreview() 
{
	//DoPrintPreview (this);
}

BOOL CDuiVisionDesignerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return TRUE;
}

void CDuiVisionDesignerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDuiVisionDesignerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView diagnostics

#ifdef _DEBUG
void CDuiVisionDesignerView::AssertValid() const
{
	CView::AssertValid();
}

void CDuiVisionDesignerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDuiVisionDesignerDoc* CDuiVisionDesignerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDuiVisionDesignerDoc)));
	return (CDuiVisionDesignerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDuiVisionDesignerView message handlers
void CDuiVisionDesignerView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	get_dll_resource();
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_OWM);
	reset_dll_resource();
	CMenu* pmenu = (CMenu*)pMenu->GetSubMenu(0);
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
			point.x, point.y, this);
	delete pMenu;
}


//////////////////////////////////////////////////////////////////////////////
// 窗口切换时调用
void CDuiVisionDesignerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		// 在属性窗口显示文档信息
		GetDocument()->RefreshDocProperty();
	}
/*
	if (bActivate)
	{
		// 窗口激活时显示OWM菜单
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_MENU_DuiVisionDesigner);
		reset_dll_resource();
		theApp.m_pIPlatUI->AddExtMenu(pMenuTerm);
	}else
	{
		// 窗口去激活时删除 OWM菜单
		get_dll_resource();
		CMenu* pMenuTerm = new CMenu();
		pMenuTerm->LoadMenu(IDR_MENU_DuiVisionDesigner);
		reset_dll_resource();
		theApp.m_pIPlatUI->DelExtMenu(pMenuTerm);
	}
*/	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

int CDuiVisionDesignerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

// 窗口初始化
void CDuiVisionDesignerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if(theApp.m_pIPlatUI == NULL)
	{
		return;
	}

	HWND hWnd = GetSafeHwnd();
	CDuiVisionDesignerDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	//CString strResourceXml = "";
	CString strFile = pDoc->GetPathName();

	// 每个窗口需要创建一个DuiVision插件的Panel对象实例
	m_pDuiPluginPanelObject = (IDuiPluginPanel*)theApp.m_pIPlatUI->CreateVciObject("org.vci.duivision", strFile);
	if(m_pDuiPluginPanelObject == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(rect);
	m_pDuiPluginPanelObject->OnInit(IDD_ABOUTBOX, hWnd, strFile, rect, &m_xDuiVisionDesignerView);

	//启动动画定时器
	m_uTimerAnimation = CTimer::SetTimer(30);

	// 初始化Tooltip
	m_wndToolTip.Create(this);

	/*strFile.MakeLower();
	strFile.Replace("\\", "/");
	int nPos = strFile.ReverseFind('/');
	if(nPos != -1)
	{
		CString strFileName = strFile;
		CString strDir = strFile.Left(nPos);
		strFileName.Delete(0, nPos+1);
		strResourceXml = strDir;
		strResourceXml += "xml\\resource.xml";
	}*/
}

// 窗口大小变化
void CDuiVisionDesignerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pDuiPluginPanelObject)
	{
		CRect rect;
		GetClientRect(rect);
		m_pDuiPluginPanelObject->SetRect(rect);
	}
}

void CDuiVisionDesignerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnMouseMove(nFlags, point);
	}

	CView::OnMouseMove(nFlags, point);
}

void CDuiVisionDesignerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnLButtonDown(nFlags, point);
	}

	CView::OnLButtonDown(nFlags, point);
}

void CDuiVisionDesignerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnLButtonUp(nFlags, point);
	}

	CView::OnLButtonUp(nFlags, point);
}

void CDuiVisionDesignerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnLButtonDblClk(nFlags, point);
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CDuiVisionDesignerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnRButtonDown(nFlags, point);
	}

	CView::OnRButtonDown(nFlags, point);
}

void CDuiVisionDesignerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnRButtonUp(nFlags, point);
	}

	CView::OnRButtonUp(nFlags, point);
}

void CDuiVisionDesignerView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnRButtonDblClk(nFlags, point);
	}

	CView::OnRButtonDblClk(nFlags, point);
}

BOOL CDuiVisionDesignerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ScreenToClient(&pt);
 	zDelta /= WHEEL_DELTA;
	UINT nFlag = (zDelta>0) ? SB_LINEUP : SB_LINEDOWN;
	zDelta = abs(zDelta);

	BOOL bRet = FALSE;
	if(m_pDuiPluginPanelObject)
	{
		bRet = m_pDuiPluginPanelObject->OnScroll(TRUE, nFlag, pt);
	}

	return bRet;//CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDuiVisionDesignerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pDuiPluginPanelObject)
	{
		m_pDuiPluginPanelObject->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDuiVisionDesignerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 禁止刷新背景,否则界面会闪烁
	return FALSE;
	//return CView::OnEraseBkgnd(pDC);
}

BOOL CDuiVisionDesignerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if ( pMsg->message == WM_MOUSEMOVE ||
		 pMsg->message == WM_LBUTTONDOWN ||
		 pMsg->message == WM_LBUTTONUP )
	{
		m_wndToolTip.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// 插件宿主窗口功能实现
INTERFACE_IMPLEMENT(DuiVisionDesignerView)

// 获取应用程序名
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetAppName()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetAppName();
	}
	return _T("");
}

// 获取平台路径
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatPath()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatPath();
	}
	return _T("");
}

// 获取平台版本
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatVersion()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatVersion();
	}
	return _T("");
}

// 获取当前语言
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetCurrentLanguage()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetCurrentLanguage();
	}
	return 0;
}

// 获取平台注册表根路径
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatRegistry()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatRegistry();
	}
	return _T("");
}

// 获取平台版权字符串
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatCopyRight()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatCopyRight();
	}
	return _T("");
}

// 获取主页URL
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatHomeURL()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatHomeURL();
	}
	return _T("");
}

// 获取下载URL
STDMETHODIMP_(CString)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetPlatDownloadURL()
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->GetPlatDownloadURL();
	}
	return _T("");
}

// 发送消息
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::SendMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->SendMessage(pIn, ppOut);
	}
	return 0;
}

// 平台的消息处理
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::ProcessMessage(CVciMessage* pIn, CVciMessage* ppOut)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->ProcessMessage(pIn, ppOut);
	}
	return 0;
}

// 发送平台命令
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::SendCommand(int nCmd, WPARAM wParam, LPARAM lParam)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->SendCommand(nCmd, wParam, lParam);
	}
	return 0;
}

// 发送平台命令
STDMETHODIMP_(BOOL)
CDuiVisionDesignerView::XDuiVisionDesignerView::SendCommand(int nCmd, WPARAM wParam, LPARAM lParam, LPVOID lpResult)
{
	if(theApp.m_pIPlatUI != NULL)
	{
		return theApp.m_pIPlatUI->SendCommand(nCmd, wParam, lParam, lpResult);
	}
	return 0;
}

// 获取DuiVision应用ID
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetAppID()
{
	return 1117;
}

// 获取窗口背景信息
STDMETHODIMP_(BOOL)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetWindowBkInfo(int& nType, int& nIDResource, COLORREF& clr, CString& strImgFile)
{
	return FALSE;
}

// 设置窗口背景信息
STDMETHODIMP_(BOOL)
CDuiVisionDesignerView::XDuiVisionDesignerView::SetWindowBkInfo(int nType, int nIDResource, COLORREF clr, LPCTSTR lpszImgFile)
{
	return FALSE;
}

// 设置Tooltip
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::SetTooltip(int nCtrlID, LPCTSTR lpszTooltip, CRect rect, int nTipWidth)
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	pObj->SetTooltip(nCtrlID, lpszTooltip, rect, nTipWidth);
}

// 清除Tooltip
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::ClearTooltip()
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	pObj->ClearTooltip();
}

// 设置当前Tooltip控件ID
STDMETHODIMP_(void)
CDuiVisionDesignerView::XDuiVisionDesignerView::SetTooltipCtrlID(int nTooltipCtrlID)
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	if(pObj)
	{
		pObj->SetTooltipCtrlID(nTooltipCtrlID);
	}
}

// 获取当前Tooltip控件ID
STDMETHODIMP_(int)
CDuiVisionDesignerView::XDuiVisionDesignerView::GetTooltipCtrlID()
{
	CDuiVisionDesignerView *pObj = GET_INTERFACE_OBJECT(DuiVisionDesignerView);
	if(pObj)
	{
		return pObj->GetTooltipCtrlID();
	}

	return 0;
}