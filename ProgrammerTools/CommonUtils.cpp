#include "CommonUtils.h"
#include "GLDFileUtils.h"
#include <QFile>

QString CommonUtils::loadQssFile(const QString &fileName)
{
    if (!fileExists(fileName))
    {
        return QString("");
    }

    QFile oFile(fileName);
    QString sRule;
    if (oFile.open(QFile::ReadOnly))
    {
        sRule = oFile.readAll();
    }
    else
    {
        sRule = "";
    }

    oFile.close();
    return sRule;
}
