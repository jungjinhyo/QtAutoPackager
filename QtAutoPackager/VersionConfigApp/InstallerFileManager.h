#ifndef INSTALLERFILEMANAGER_H
#define INSTALLERFILEMANAGER_H

#include <QString>
#include <QDomDocument>

class InstallerFileManager
{
public:
    InstallerFileManager(const QString& programName, const QString& version, const QString& installPath);

    bool createDirectoryStructure();  // 디렉터리 구조 생성
    bool createConfigXml();           // config.xml 생성
    bool createPackageXml();          // package.xml 생성
    bool createLicenseFile();         // license.txt 생성

private:
    QString programName;
    QString version;
    QString installPath;

    QDomElement createElement(QDomDocument& doc, const QString& tag, const QString& value);
    bool saveXmlToFile(const QDomDocument& doc, const QString& filePath);
};

#endif // INSTALLERFILEMANAGER_H
