#ifndef PROGRAMMERTOOLSUTILS_H
#define PROGRAMMERTOOLSUTILS_H

#include <Vector>
#include <GLDIniFiles.h>
#include <QString>

class ProgrammerToolsUtils
{
public:
    ProgrammerToolsUtils();

public:
    void GetAllRegions(GStringObjectList *AAllRegions);
    void GetRegionBranches(GStringObjectList *ARegionBranches, const GString &ARegionName);
    QString GetIDEPath();
    QString GetGrandResourcePath(const QString ARegionName);
    QString GetXSDPath();
    QString GetVersionsPath();
private:
    GLDIniFile *pCodeBranchFile;
    GLDIniFile *pOptionFile;
};

#endif // PROGRAMMERTOOLSUTILS_H
