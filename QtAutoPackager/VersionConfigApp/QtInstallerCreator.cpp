#include "QtInstallerCreator.h"
#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>

QtInstallerCreator::QtInstallerCreator(const QString& programName, const QString& version, const QString& installPath)
    : programName(programName), version(version), installPath(installPath)
{
    repositoryPath = installPath + "/repository";
    installerPath = installPath + "/" + programName + "_Installer_v" + version + ".exe";
}

QString QtInstallerCreator::getRepogenPath() const {
    // repogen 실행 파일의 경로를 반환합니다 (환경에 맞게 수정 필요)
    return "C:/Qt/Tools/QtInstallerFramework/4.8/bin/repogen.exe";
}

QString QtInstallerCreator::getBinarycreatorPath() const {
    // binarycreator 실행 파일의 경로를 반환합니다 (환경에 맞게 수정 필요)
    return "C:/Qt/Tools/QtInstallerFramework/4.8/bin/binarycreator.exe";
}

bool QtInstallerCreator::generateRepository() {
    QProcess process;
    QStringList arguments;

    QString configPath = installPath + "/config/config.xml";
    QString packagesPath = installPath + "/packages";

    arguments << "-p" << packagesPath << "-c" << configPath << repositoryPath;

    process.start(getRepogenPath(), arguments);

    if (!process.waitForStarted()) {
        QMessageBox::warning(nullptr, "Error", "Failed to start repogen process.");
        return false;
    }

    process.waitForFinished();

    QString errorOutput = process.readAllStandardError();
    if (process.exitCode() != 0) {
        QMessageBox::warning(nullptr, "Repository Generation Error", "Failed to generate repository:\n" + errorOutput);
        return false;
    }

    qDebug() << "Repository generated successfully at " << repositoryPath;
    return true;
}

bool QtInstallerCreator::createInstallerExecutable() {
    QProcess process;
    QStringList arguments;

    QString configPath = installPath + "/config/config.xml";
    arguments << "-c" << configPath << "-p" << installPath + "/packages" << "-r" << repositoryPath << installerPath;

    process.start(getBinarycreatorPath(), arguments);

    if (!process.waitForStarted()) {
        QMessageBox::warning(nullptr, "Error", "Failed to start binarycreator process.");
        return false;
    }

    process.waitForFinished();

    QString errorOutput = process.readAllStandardError();
    if (process.exitCode() != 0) {
        QMessageBox::warning(nullptr, "Installer Creation Error", "Failed to create installer executable:\n" + errorOutput);
        return false;
    }

    qDebug() << "Installer executable created successfully at " << installerPath;
    return true;
}
