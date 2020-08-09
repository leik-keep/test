#include "programmertools.h"
#include <QtGui>
#include <QtWidgets>
#include <QString>
#include <qnamespace.h>
#include "GLDFileUtils.h"
#include "GLDUIUtils.h"
#include "programmertoolsutils.h"
#include "GLDStringObjectList.h"
#include "CommonUtils.h"

ProgrammerTools::ProgrammerTools(QWidget *parent) : QDialog(parent)
{
    m_Regions << "shanxi" << "xinjiang" << "gansu" << "jilin" << "shandong" << "beijing";
    m_RegionsCH << QStringLiteral("ɽ��") << QStringLiteral("�½�") << QStringLiteral("����") << QStringLiteral("����") <<
                   QStringLiteral("ɽ��") << QStringLiteral("����");

    setupUI();

    ptUtils = new ProgrammerToolsUtils;
    this->InitData();

    RegisterToolsHotKey();
}

ProgrammerTools::~ProgrammerTools()
{
    delete ptUtils;
    UnRegisterToolsHotKey();
}

void ProgrammerTools::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

bool ProgrammerTools::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
   if(eventType == "windows_generic_MSG")
       {
           MSG* msg = static_cast<MSG*>(message);
           if(msg->message==WM_HOTKEY)
           {
               UINT fuModifiers = (UINT) LOWORD(msg->lParam);  // ģʽ
               UINT uVirtKey = (UINT) HIWORD(msg->lParam);     // ��ֵ
               if(fuModifiers == MOD_ALT && uVirtKey == VK_F8)
               {
                   emit hotkeyShotBgPressed();
                   qDebug("ALT+F8 is Pressed");
               }
               return true;
           }
       }
   return false;
}

void ProgrammerTools::setupUI()
{
    setWindowIcon(QIcon(":/ToolsBox.png"));
    QIcon icon = QIcon(":/ToolsBox.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip(QStringLiteral("����Ա������"));
    QString titlec=QStringLiteral("����Ա������");
    QString textc=QStringLiteral("����Ա������");

    trayIcon->show();
    trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);

    //��ӵ�/˫�������Ӧ
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this,SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));

    //����������Ϊ
    minimizeAction = new QAction(QStringLiteral("��С�� (&I)"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    restoreAction = new QAction(QStringLiteral("���������� (&R)"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(QStringLiteral("�˳� (&Q)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    //�����Ҽ������˵�
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    lblRegion = new QLabel(    QStringLiteral("������      "));
    lblCodeBranch = new QLabel(QStringLiteral("�����֧��"));
    cbRegions = new QComboBox;
    cbCodeBranch = new QComboBox;
    btnOpenCode = new QPushButton(QStringLiteral("��IDE�д�"));
    btnOpenCodePath = new QPushButton(QStringLiteral("���ļ����д�"));
    btnSwithGrandResource = new QPushButton(QStringLiteral("�л�������Դ"));
    btnOpenExe = new QPushButton(QStringLiteral("�򿪼Ƽ����"));
    btnOpenXSD = new QPushButton(QStringLiteral("�򿪽ӿڱ�׼"));
    btnOpenVersion = new QPushButton(QStringLiteral("�򿪰汾·��"));

    layout1 = new QHBoxLayout;
    layout2 = new QHBoxLayout;
    layout3 = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    rightSplitter1 = new QSplitter;
    rightSplitter1->setOrientation(Qt::Horizontal);
    rightSplitter2 = new QSplitter;
    rightSplitter2->setOrientation(Qt::Horizontal);
    layout1->addWidget(lblRegion);
    layout1->addWidget(cbRegions);
    layout1->addWidget(rightSplitter1);
    layout2->addWidget(lblCodeBranch);
    layout2->addWidget(cbCodeBranch);
    cbCodeBranch->setFixedSize(600, 20);
    layout2->addStretch();
    layout3->addWidget(btnOpenCode);
    layout3->addWidget(btnOpenCodePath);
    layout3->addWidget(btnSwithGrandResource);
    layout3->addWidget(btnOpenExe);
    layout3->addWidget(btnOpenXSD);
    layout3->addWidget(btnOpenVersion);
    layout3->addWidget(rightSplitter2);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    this->setLayout(mainLayout);

    connect(cbRegions,SIGNAL(currentIndexChanged(int)), this, SLOT(RegionChanged()));
    connect(btnOpenCode, SIGNAL(clicked()), this, SLOT(OpenCodeClick()));
    connect(btnOpenCodePath, SIGNAL(clicked()), this, SLOT(OpenCodePathClick()));
    connect(btnSwithGrandResource, SIGNAL(clicked()), this, SLOT(SwithGrandResourceClick()));
    connect(btnOpenExe, SIGNAL(clicked()), this, SLOT(OpenExeClick()));
    connect(btnOpenXSD, SIGNAL(clicked()), this, SLOT(OpenXSDClick()));
    connect(btnOpenVersion, SIGNAL(clicked()), this, SLOT(OpenVersionClick()));
    connect(this, SIGNAL(hotkeyShotBgPressed()), this, SLOT(ShowMainWindow()));

    this->setStyleSheet(CommonUtils::loadQssFile(exePath() + "..\\Qsses\\main.qss"));
    this->setFixedSize(700, 120);
    this->setWindowTitle(QStringLiteral("������"));
}

void ProgrammerTools::InitData()
{
    GStringObjectList *oAllRegions = new GStringObjectList;
    ptUtils->GetAllRegions(oAllRegions);
    for(int i = 0; i < oAllRegions->count(); i++)
        cbRegions->addItem(oAllRegions->valueFromIndex(i));

    GStringObjectList *oRegionBranches = new GStringObjectList;
    ptUtils->GetRegionBranches(oRegionBranches, cbRegions->currentText());
    cbCodeBranch->clear();
    for(int i = 0; i < oRegionBranches->count(); i++)
        cbCodeBranch->addItem(oRegionBranches->valueFromIndex(i));

    delete oAllRegions;
    delete oRegionBranches;
}

void ProgrammerTools::RegisterToolsHotKey()
{
    int HotKeyId  = GlobalAddAtom(LPCTSTR("MyHotKey"));
    RegisterHotKey(HWND(this->winId()), HotKeyId, MOD_ALT, VK_F8);
}

void ProgrammerTools::UnRegisterToolsHotKey()
{
    int HotKeyId  = GlobalAddAtom(LPCTSTR("MyHotKey"));
    UnregisterHotKey(HWND(this->winId()), HotKeyId);
}

void ProgrammerTools::CopyCustomFunctionsToRes()
{
    QString sCustomFunctions = extractFilePath(extractFileDir(cbCodeBranch->currentText())) + "Release\\Bin\\GBQ4.CustomFunctions.dll";
    QString sNewCustomFunctions = "D:\\GrandResource\\GrandSoft\\GrandSmartPlatform\\Bin\\GBQ4.CustomFunctions.dll";
    if(fileExists(sCustomFunctions))
        copyFile(sCustomFunctions, sNewCustomFunctions, false);
}

LRESULT ProgrammerTools::OnHotKey(WPARAM wParam, LPARAM lParam)
{
    showPrompt("aa");
    return 0;
}

void ProgrammerTools::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
           //��������ͼ��
       case QSystemTrayIcon::DoubleClick:
           //˫������ͼ��
           this->showNormal();
           this->raise();
           break;
       default:
           break;
    }
}

void ProgrammerTools::OpenCodeClick()
{
    if (IsP5())
    {
        QString sCodeBranch = QString("\"") + cbCodeBranch->currentText() + QString("\"");
        ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sCodeBranch.utf16()), NULL, NULL, SW_NORMAL);
    }
    else
    {
        QString sGrandResource = ptUtils->GetGrandResourcePath(cbRegions->currentText());
        ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sGrandResource.utf16()), NULL, NULL, SW_NORMAL);

        QString sCodeBranch = QString("\"") + cbCodeBranch->currentText() + QString("\"");
        ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sCodeBranch.utf16()), NULL, NULL, SW_NORMAL);

        CopyCustomFunctionsToRes();
    }
    this->hide();
}

void ProgrammerTools::OpenCodePathClick()
{
   QString sCodeBranch;
   if (IsP5())
   {
       sCodeBranch = extractFilePath(cbCodeBranch->currentText()) + QStringLiteral("Release/Construction/Regions/") + cbRegions->currentText() + "/GYS";
   }
   else
   {
       sCodeBranch = QString("\"") + extractFilePath(extractFileDir(cbCodeBranch->currentText())) + QStringLiteral("Release/����/") + cbRegions->currentText() + QString("\"");
   }
   ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sCodeBranch.utf16()), NULL, NULL, SW_NORMAL);
   this->hide();
}

void ProgrammerTools::SwithGrandResourceClick()
{
    QString sGrandResource = ptUtils->GetGrandResourcePath(cbRegions->currentText());
    ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sGrandResource.utf16()), NULL, NULL, SW_NORMAL);

    CopyCustomFunctionsToRes();
    this->hide();
}

void ProgrammerTools::OpenExeClick()
{
    QString sCodeBranch;
    if (IsP5())
    {
        sCodeBranch = extractFilePath(cbCodeBranch->currentText()) + QString("Release/Bin/GFCP564d.exe");
        if (!fileExists(sCodeBranch))
        {
            sCodeBranch = extractFilePath(cbCodeBranch->currentText()) + QString("Release/Bin/GCCP564d.exe");
        }
    }
    else
    {
        sCodeBranch = QString("\"") + extractFilePath(extractFileDir(cbCodeBranch->currentText())) + QString("Release/Bin/GBQ4.exe\"");
    }
    ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sCodeBranch.utf16()), NULL, NULL, SW_NORMAL);
    this->hide();
}

void ProgrammerTools::OpenXSDClick()
{
    QString sXSDPath;
    if (IsP5())
    {
        int nIndex = m_Regions.indexOf(cbRegions->currentText().toLower());

        sXSDPath =  ptUtils->GetXSDPath() + m_RegionsCH[nIndex];
    }
    else
    {
        sXSDPath = ptUtils->GetXSDPath() + cbRegions->currentText();
    }
    ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sXSDPath.utf16()), NULL, NULL, SW_NORMAL);
    this->hide();
}

void ProgrammerTools::OpenVersionClick()
{
    QString sVersionPath;
    if (IsP5())
    {
        int nIndex = m_Regions.indexOf(cbRegions->currentText().toLower());
        sVersionPath = ptUtils->GetVersionsPath() + m_RegionsCH[nIndex];

    }
    else
    {
        sVersionPath = ptUtils->GetVersionsPath() + cbRegions->currentText();
    }
    ShellExecute(NULL, L"open", reinterpret_cast<const WCHAR *>(sVersionPath.utf16()), NULL, NULL, SW_NORMAL);
    this->hide();
}

void ProgrammerTools::RegionChanged()
{
    cbCodeBranch->clear();
    GStringObjectList *oRegionBranches = new GStringObjectList;
    ptUtils->GetRegionBranches(oRegionBranches, cbRegions->currentText());
    for(int i = 0; i < oRegionBranches->count(); i++)
        cbCodeBranch->addItem(oRegionBranches->valueFromIndex(i));
}

void ProgrammerTools::ShowMainWindow()
{
    this->showNormal();
}

bool ProgrammerTools::IsP5()
{
    QString sCodeBranch =  cbCodeBranch->currentText();
    return sCodeBranch.contains("pro");
}

