#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QWindow>
#include <QDir>
#include "GLDFileUtils.h"

#include "programmertools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProgrammerTools oProgrammerTools;
    oProgrammerTools.show();

    return a.exec();
}
