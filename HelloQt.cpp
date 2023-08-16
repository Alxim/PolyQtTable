#include "stdafx.h"

#include "hostUI.h"
#include "hostQt.h"
#include "PolyQtTableWidget.h"

PolyQtTableWidget* TABLE_W = nullptr;

extern "C" __declspec(dllexport) bool showDialog(HWND parent)
{
	auto win = new QWinWidget(parent);
	win->showCentered();

	QMessageBox::about(win, "HelloQt.dll", "Hello, Qt in nanoCAD!");

	delete win;

	return TRUE;
}

void QtModalDlgCmd()
{
	acutPrintf(L"\nHello, hostQt.dll!\n");

	showDialog(adsw_acadMainWnd());
}

hostUiPaletteSet* m_pPalSet = NULL;

HINSTANCE _hdllInstance = NULL;
AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

class QtTablePalette : public hostQtPalette
{
	DECLARE_DYNAMIC(QtTablePalette)

public:
	QtTablePalette() {};

	afx_msg void    OnSize(UINT nType, int cx, int cy)
	{
		if (m_pWinWidget)
		{
			HWND wnd = (HWND)m_pWinWidget->windowHandle()->winId();
			::SetWindowPos(wnd, nullptr, 0, 0, cx, cy * 2, SWP_NOZORDER);
		}
	}

	DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(QtTablePalette, hostQtPalette)
	//{{AFX_MSG_MAP(QtTablePalette)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(QtTablePalette, hostQtPalette);


void QtTablePaletteCmd()
{
	if (!m_pPalSet)
	{
		CAcModuleResourceOverride ThisRes;
		m_pPalSet = new hostUiPaletteSet();
		m_pPalSet->Create(L"Test Qt Palette Set", WS_CHILD | WS_DLGFRAME | WS_VISIBLE, CRect(30, 50, 870, 300),
			CWnd::FromHandle(adsw_acadMainWnd()), PSS_SNAP | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP | PSS_CLOSE_BUTTON);
		m_pPalSet->EnableDocking(CBRS_ALIGN_ANY);
		m_pPalSet->RestoreControlBar();

		QtTablePalette* pPal = new QtTablePalette();
		pPal->Create(WS_CHILD | WS_VISIBLE, L"Poly Table Qt Palette1", m_pPalSet, 0);
		m_pPalSet->AddPalette(pPal);

		QWidget* pPaletteWidget1 = pPal->paletteWidget();

		if (TABLE_W == nullptr)
			TABLE_W = new PolyQtTableWidget(pPaletteWidget1);

		QVBoxLayout* vbox = new QVBoxLayout(pPaletteWidget1);
		vbox->setSpacing(0);
		vbox->setMargin(0);
		vbox->addWidget(TABLE_W);
		vbox->addStretch();

		//WId winId = le3->winId(); // Make Qt windows real HWND windows

		pPaletteWidget1->setLayout(vbox);
		pPaletteWidget1->show();

		CRect cr;
		m_pPalSet->GetClientRect(&cr);
		pPal->OnSize(0, cr.Width(), cr.Height()); // Force to resize palette widget, needed when system scale !=100%

		m_pPalSet->Show(!m_pPalSet->IsWindowVisible());
		return;
	}

	m_pPalSet->Show(!m_pPalSet->IsWindowVisible());	
}

void initApp()
{
	acedRegCmds->addCommand(L"QT_GROUP",
		L"_QTMODALDLG",
		L"QTMODALDLG",
		ACRX_CMD_TRANSPARENT,
		QtModalDlgCmd);

	acedRegCmds->addCommand(L"QT_GROUP",
		L"_QtTablePalette",
		L"QtTablePalette",
		ACRX_CMD_TRANSPARENT,
		QtTablePaletteCmd);
}

void uninitApp()
{
	acedRegCmds->removeGroup(L"QT_GROUP");

	if (m_pPalSet)
	{
		m_pPalSet->DestroyWindow();
		m_pPalSet = 0;
	}
}


void clearReactors();
void watchDb();



extern "C" __declspec(dllexport) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		//  Действия при установке плагина

		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);
		initApp();
		//watchDb();
		QtTablePaletteCmd();
		break;

	case AcRx::kUnloadAppMsg:
		// Действия при выгрузке плагина
		uninitApp();

		//  Пример удаления реактора
		//clearReactors();
		//acedRegCmds->removeGroup("ASDK_NOTIFY_TEST");
		break;
	}

	return AcRx::kRetOK;
}


// Отладочный метод

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTime>

void writeLog(QString str)
{
	QFile file("C:/Users/Schel/Downloads/PolyQtTable.log");

	file.open(QFile::ReadWrite);

	QTextStream out(&file);

	out.setCodec("UTF-8");
	out.seek(file.size());
	out << "\n" << QTime::currentTime().toString("hh:mm:ss  ") << str;

	file.close();
}

void updateTable()
{
	if (TABLE_W != nullptr)
		TABLE_W->updateTable();
}