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
    installerPath = installPath + "/" + programName + "_Installer" + ".exe";
}

QString QtInstallerCreator::getRepogenPath() const {
    QString repogenPath = QDir(QCoreApplication::applicationDirPath()).filePath("../bin/repogen.exe");
    return repogenPath;
}

QString QtInstallerCreator::getBinarycreatorPath() const {
    QString binarycreatorPath = QDir(QCoreApplication::applicationDirPath()).filePath("../bin/binarycreator.exe");
    return binarycreatorPath;
}

bool QtInstallerCreator::generateRepository() {
    QProcess process;
    QStringList arguments;

    QString configPath = installPath + "/config/config.xml";
    QString packagesPath = installPath + "/packages";

    arguments << "--update" << "-p" << packagesPath << repositoryPath;

    process.start(getRepogenPath(), arguments);

    if (!process.waitForStarted()) {
        QMessageBox::warning(nullptr, "Error", "Failed to start repogen process.");
        return false;
    }

    process.waitForFinished();

    QString errorOutput = process.readAllStandardError();
    if (process.exitCode() != 0) {
        QMessageBox::warning(nullptr, "Repository Generation Error", "Failed to generate repository:\n");
        return false;
    }

    qDebug() << "Repository generated successfully at " << repositoryPath;
    return true;
}

bool QtInstallerCreator::createInstallerExecutable() {
    QProcess process;
    QStringList arguments;

    QString configPath = installPath + "/config/config.xml";
    arguments << "-c" << configPath << "-p" << installPath + "/packages" << installerPath;

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
