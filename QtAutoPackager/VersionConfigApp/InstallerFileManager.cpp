#include "installerfilemanager.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDate>

InstallerFileManager::InstallerFileManager(const QString& programName, const QString& version, const QString& installPath)
    : programName(programName), version(version), installPath(installPath)
{
}

// 디렉터리 구조 생성
bool InstallerFileManager::createDirectoryStructure() {
    QDir dir;

    QStringList paths = {
        installPath + "/config",
        installPath + "/packages",
        installPath + "/packages/com.mycompany." + programName.toLower().replace(" ", "") + "/meta",
        installPath + "/packages/com.mycompany." + programName.toLower().replace(" ", "") + "/data",
        installPath + "/repository"
    };

    for (const QString& path : paths) {
        if (!dir.mkpath(path)) {
            QMessageBox::warning(nullptr, "Error", QString("Failed to create directory: %1").arg(path));
            return false;
        }

        QFile initFile(path + "/__init__.py");
        if (!initFile.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(nullptr, "Error", QString("Failed to create __init__.py in: %1").arg(path));
            return false;
        }
        initFile.close();
    }

    return true;
}

// config.xml 생성
bool InstallerFileManager::createConfigXml() {
    QDomDocument doc;
    QDomElement root = doc.createElement("Installer");
    doc.appendChild(root);

    QString modifiedProgramName = programName;
    modifiedProgramName.replace(" ", "-");

    root.appendChild(createElement(doc, "Name", programName));
    root.appendChild(createElement(doc, "Version", version));
    root.appendChild(createElement(doc, "Title", programName + " Installer"));
    root.appendChild(createElement(doc, "Publisher", "MyCompany"));
    root.appendChild(createElement(doc, "ProductUrl", "https://jungjinhyo.github.io/" + modifiedProgramName.toLower() + "-installer/"));
    root.appendChild(createElement(doc, "InstallerWindowIcon", "installericon.png"));
    root.appendChild(createElement(doc, "InstallerApplicationIcon", "installericon.png"));
    root.appendChild(createElement(doc, "Logo", "logo.png"));
    root.appendChild(createElement(doc, "Watermark", "watermark.png"));
    root.appendChild(createElement(doc, "RunProgram", QString("@TargetDir@/%1.exe").arg(programName.replace(" ", ""))));
    root.appendChild(createElement(doc, "RunProgramDescription", "Play the latest version of " + programName + "!"));
    root.appendChild(createElement(doc, "StartMenuDir", programName));
    root.appendChild(createElement(doc, "TargetDir", QString("@DesktopDir@/%1_v%2").arg(programName).arg(version)));
    root.appendChild(createElement(doc, "AdminTargetDir", "@RootDir@/" + programName));

    QDomElement repositories = doc.createElement("RemoteRepositories");
    QDomElement repository = doc.createElement("Repository");
    repository.appendChild(createElement(doc, "Url", QString("https://jungjinhyo.github.io/%1-installer/v%2").arg(modifiedProgramName.toLower()).arg(version)));
    repository.appendChild(createElement(doc, "Enabled", "1"));
    repository.appendChild(createElement(doc, "DisplayName", programName + " v" + version + " Repository"));
    repositories.appendChild(repository);
    root.appendChild(repositories);

    return saveXmlToFile(doc, installPath + "/config/config.xml");
}

// package.xml 생성
bool InstallerFileManager::createPackageXml() {
    QDomDocument doc;
    QDomElement root = doc.createElement("Package");
    doc.appendChild(root);

    root.appendChild(createElement(doc, "DisplayName", programName + " (v" + version + ")"));
    root.appendChild(createElement(doc, "Description", "A simple " + programName + " game."));
    root.appendChild(createElement(doc, "Version", version));
    root.appendChild(createElement(doc, "ReleaseDate", QDate::currentDate().toString("yyyy-MM-dd")));
    root.appendChild(createElement(doc, "Name", "com.mycompany." + programName.toLower().replace(" ", "")));

    QDomElement licenses = doc.createElement("Licenses");
    QDomElement license = doc.createElement("License");
    license.setAttribute("name", "License Agreement");
    license.setAttribute("file", "license.txt");
    licenses.appendChild(license);
    root.appendChild(licenses);

    root.appendChild(createElement(doc, "DownloadableArchives", programName + "_v" + version + ".7z"));
    root.appendChild(createElement(doc, "UpdateText", "This release includes bug fixes and new features."));
    root.appendChild(createElement(doc, "Default", "true"));
    root.appendChild(createElement(doc, "ForcedInstallation", "false"));
    root.appendChild(createElement(doc, "ForcedUpdate", "false"));

    return saveXmlToFile(doc, installPath + "/packages/com.mycompany." + programName.toLower().replace(" ", "") + "/meta/package.xml");
}

// license.txt 생성
bool InstallerFileManager::createLicenseFile() {
    QFile file(installPath + "/packages/com.mycompany." + programName.toLower().replace(" ", "") + "/meta/license.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Failed to create license.txt");
        return false;
    }

    QTextStream out(&file);
    out << "Rock Paper Scissors Game License Agreement\n";
    out << "------------------------------------------\n\n";
    out << "이 소프트웨어는 jungjinhyo에 의해 만들어졌습니다.\n\n";
    out << "사용자는 이 소프트웨어를 자유롭게 사용할 수 있으며, 상업적 및 비상업적 목적으로 수정, 복제, 배포할 수 있습니다. 단, 아래와 같은 조건을 준수해야 합니다.\n\n";
    out << "1. 저작권 표기 유지:\n";
    out << "   소프트웨어의 원작자인 jungjinhyo의 저작권 표기를 삭제하거나 변경할 수 없습니다.\n\n";
    out << "2. 보증 제한:\n";
    out << "   이 소프트웨어는 \"있는 그대로\" 제공되며, 사용에 따른 문제나 손해에 대해 어떠한 보증도 하지 않습니다.\n\n";
    out << "3. 면책 조항:\n";
    out << "   사용자는 소프트웨어의 사용으로 발생하는 모든 문제에 대해 전적으로 책임을 집니다.\n\n";
    out << "MIT 라이선스와 유사하게, 이 소프트웨어는 자유롭게 사용되고 배포될 수 있지만, 사용에 따른 책임은 사용자에게 있습니다.\n\n";
    out << "발행일: 2024년 10월 31일\n";

    file.close();
    return true;
}

// XML 요소 생성 함수
QDomElement InstallerFileManager::createElement(QDomDocument& doc, const QString& tag, const QString& value) {
    QDomElement element = doc.createElement(tag);
    element.appendChild(doc.createTextNode(value));
    return element;
}

// XML 파일 저장 함수
bool InstallerFileManager::saveXmlToFile(const QDomDocument& doc, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, "Error", QString("Failed to create %1").arg(filePath));
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
    return true;
}
