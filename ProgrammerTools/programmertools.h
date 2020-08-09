#ifndef PROGRAMMERTOOLS_H
#define PROGRAMMERTOOLS_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSplitter>
#include <QCloseEvent>
#include <Windows.h>
#include <QDialog>
#include <QAbstractNativeEventFilter>
#include <QMainWindow>
#include <QVector>
#include "programmertoolsutils.h"

class ProgrammerTools : public QDialog
{
    Q_OBJECT
public:
    explicit ProgrammerTools(QWidget *parent = 0);
    ~ProgrammerTools();

    void closeEvent(QCloseEvent *event);

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);


private:
    void setupUI();
    void InitData();
    void RegisterToolsHotKey();
    void UnRegisterToolsHotKey();
    void CopyCustomFunctionsToRes();

    LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

signals:
    void hotkeyShotBgPressed();

private slots:
    void trayiconActivated(QSystemTrayIcon::ActivationReason reason);
    void OpenCodeClick();
    void OpenCodePathClick();
    void SwithGrandResourceClick();
    void OpenExeClick();
    void OpenXSDClick();
    void OpenVersionClick();
    void RegionChanged();
    void ShowMainWindow();
    bool IsP5();
private:
    QSystemTrayIcon *trayIcon;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu   *trayIconMenu;
    QHBoxLayout *layout1;
    QHBoxLayout *layout2;
    QHBoxLayout *layout3;
    QVBoxLayout *mainLayout;
    QLabel *lblRegion;
    QLabel *lblCodeBranch;
    QComboBox *cbRegions;
    QComboBox *cbCodeBranch;
    QPushButton *btnOpenCode;
    QPushButton *btnOpenCodePath;
    QPushButton *btnSwithGrandResource;
    QPushButton *btnOpenExe;
    QPushButton *btnOpenXSD;
    QPushButton *btnOpenVersion;
    QSplitter *rightSplitter1;
    QSplitter *rightSplitter2;
    ProgrammerToolsUtils *ptUtils;
    QVector<QString> m_Regions;
    QVector<QString> m_RegionsCH;
};

#endif // PROGRAMMERTOOLS_H
