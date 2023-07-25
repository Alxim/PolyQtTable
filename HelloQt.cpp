//
// �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
// 
// ������ ����������� �����������, ��� �������������� ����� �� ����, ���
// ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
// ������ ����������� ����������� ����� �������������� ��� ���������� � �������
// � ������ ������������� ����������� ��������� ��� ���������� �������
// �������������, ����������� � ������������� �������� �������������
// �� ������������� ��������� ��� ��� ���������� nanoCAD��.
// 
// ������ ����������� ����������� �������� � ������������ � �����������������
// ���������� ��������� �� ���������������� ������������� � ��������������
// ��������� ������.
// 
// ��������� ������ ����������� �����������,  ��� ������������ �
// ���������������� ��������� �� ������������ � ��������� �������������,
// ���������� ����. 
//

#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
#include "PolyQtTable.h"

extern "C" __declspec(dllexport) bool showDialog(HWND parent)
{
  auto win = new QWinWidget(parent);
  win->showCentered();

  QMessageBox::about(win, "HelloQt.dll", "Hello, Qt in nanoCAD!");

  delete win;

  return TRUE;
}

void helloQtModalDlgCmd()
{
  acutPrintf(L"\nHello, hostQt.dll!\n");

  showDialog(adsw_acadMainWnd());
}

hostUiPaletteSet* m_pPalSet = NULL;

HINSTANCE _hdllInstance =NULL ;
AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

class polyQtTablePalette : public hostQtPalette
{
  DECLARE_DYNAMIC(polyQtTablePalette)

public:
  polyQtTablePalette() {};

  afx_msg void    OnSize(UINT nType, int cx, int cy)
  {
    if (m_pWinWidget)
    {
      HWND wnd = (HWND)m_pWinWidget->windowHandle()->winId();
      ::SetWindowPos(wnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER);
    }
  }

  DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(polyQtTablePalette, hostQtPalette)
  //{{AFX_MSG_MAP(polyQtTablePalette)
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(polyQtTablePalette, hostQtPalette);

void polyQtTablePaletteCmd()
{
  if (!m_pPalSet)
  {
    CAcModuleResourceOverride ThisRes;
    m_pPalSet = new hostUiPaletteSet();
    m_pPalSet->Create(L"Test Qt Palette Set", WS_CHILD | WS_DLGFRAME | WS_VISIBLE, CRect(30, 50, 870, 300),
      CWnd::FromHandle(adsw_acadMainWnd()), PSS_SNAP | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP | PSS_CLOSE_BUTTON);
    m_pPalSet->EnableDocking(CBRS_ALIGN_ANY);
    m_pPalSet->RestoreControlBar();

    polyQtTablePalette* pPal = new polyQtTablePalette();
    pPal->Create(WS_CHILD | WS_VISIBLE, L"Poly Table Qt Palette1", m_pPalSet, 0);
    m_pPalSet->AddPalette(pPal);

    QWidget* pPaletteWidget1 = pPal->paletteWidget();

    PolyQtTableWidget* pWidgetsClass = new PolyQtTableWidget(pPaletteWidget1);

    QVBoxLayout* vbox = new QVBoxLayout(pPaletteWidget1);
    vbox->setSpacing(0);
    vbox->setMargin(0);
    vbox->addWidget(pWidgetsClass);
    vbox->addStretch();

    //WId winId = le3->winId(); // Make Qt windows real HWND windows

    pPaletteWidget1->setLayout(vbox);
    pPaletteWidget1->show();

    CRect cr;
    m_pPalSet->GetClientRect(&cr);
    pPal->OnSize(0, cr.Width(), cr.Height()); // Force to resize palette widget, needed when system scale !=100%

    m_pPalSet->Show(!m_pPalSet->IsWindowVisible());
  }
  else
  {
    m_pPalSet->Show(!m_pPalSet->IsWindowVisible());
  }
}

void initApp()
{
  acedRegCmds->addCommand(L"HELLOQT_GROUP",
                          L"_HELLOQTMODALDLG",
                          L"HELLOQTMODALDLG",
                          ACRX_CMD_TRANSPARENT,
                          helloQtModalDlgCmd);

  acedRegCmds->addCommand(L"HELLOQT_GROUP",
                          L"_polyQtTablePalette",
                          L"polyQtTablePalette",
                          ACRX_CMD_TRANSPARENT,
                          polyQtTablePaletteCmd);
}

void uninitApp()
{
  acedRegCmds->removeGroup(L"HELLOQT_GROUP");

  if (m_pPalSet)
  {
    m_pPalSet->DestroyWindow();
    m_pPalSet = 0;
  }
}


void clearReactors();
void watchDb();


void createNcEditorReactor();
void clearNcEditorReactor();

extern "C" __declspec(dllexport) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId) 
{
  switch (msg) 
  {
  case AcRx::kInitAppMsg:
      //  �������� ��� ��������� �������

    acrxDynamicLinker->unlockApplication(appId);
    acrxDynamicLinker->registerAppMDIAware(appId);
    initApp();
    //watchDb();    
    createNcEditorReactor();

    //  ������ ����������� ��������

    //acrxDynamicLinker->unlockApplication(appId);
    //acrxDynamicLinker->registerAppNotMDIAware(appId);
    //acedRegCmds->addCommand("ASDK_NOTIFY_TEST",
    //    "ASDK_WATCH",
    //    "WATCH",
    //    ACRX_CMD_TRANSPARENT,
    //    watchDb);
    //acedRegCmds->addCommand("ASDK_NOTIFY_TEST",
    //    "ASDK_CLEAR",
    //    "CLEAR",
    //    ACRX_CMD_TRANSPARENT,
    //    clearReactors);
    break;

  case AcRx::kUnloadAppMsg:
      // �������� ��� �������� �������
    uninitApp();

    //  ������ �������� ��������

    //clearReactors();
    clearNcEditorReactor();


    //acedRegCmds->removeGroup("ASDK_NOTIFY_TEST");
    break;
  }

  return AcRx::kRetOK;
}
