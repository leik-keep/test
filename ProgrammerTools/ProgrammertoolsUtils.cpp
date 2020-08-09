#include "ProgrammertoolsUtils.h"
#include "GLDFileUtils.h"
#include "GLDUIUtils.h"
#include <QDir>
#include <QString>

ProgrammerToolsUtils::ProgrammerToolsUtils()
{
    QDir dir;
    QString sCodeBranchFile = dir.currentPath() + "/CodeBranch.ini";
    QString sOptionFile = dir.currentPath() + "/ToolOption.ini";
    if ((!fileExists(sCodeBranchFile)) || (!fileExists(sOptionFile)))
    {
        showPrompt("文件不存在");
        return;
    }

    pCodeBranchFile =  new GLDIniFile(sCodeBranchFile);
    pOptionFile = new GLDIniFile(sOptionFile);
}

void ProgrammerToolsUtils::GetAllRegions(GStringObjectList *AAllRegions)
{
    if(pCodeBranchFile == NULL)
    {
        return;
    }
    pCodeBranchFile->readSections(AAllRegions);
}

void ProgrammerToolsUtils::GetRegionBranches(GStringObjectList *AAllRegions, const GString &ARegionName)
{
    if(pCodeBranchFile == NULL)
    {
        return;
    }
    pCodeBranchFile->readSectionValues(ARegionName, AAllRegions);
}

QString ProgrammerToolsUtils::GetIDEPath()
{
    if (pOptionFile == NULL)
    {
        return "";
    }
    return pOptionFile->readString("IDE", "BDS", "");
}

QString ProgrammerToolsUtils::GetGrandResourcePath(const QString ARegionName)
{
    if (pOptionFile == NULL)
    {
        return "";
    }
    return pOptionFile->readString("GrandResource", ARegionName, "");
}

QString ProgrammerToolsUtils::GetXSDPath()
{
    if (pOptionFile == NULL)
    {
        return "";
    }
    return pOptionFile->readString("XSD", "XSDPath", "");
}

QString ProgrammerToolsUtils::GetVersionsPath()
{
    if (pOptionFile == NULL)
    {
        return "";
    }
    return pOptionFile->readString("Versions", "VersionsPath", "");
}

