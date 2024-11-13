#ifndef QTINSTALLERCREATOR_H
#define QTINSTALLERCREATOR_H

#include <QString>

class QtInstallerCreator {
public:
    QtInstallerCreator(const QString& programName, const QString& version, const QString& installPath);
    bool generateRepository();
    bool createInstallerExecutable();

private:
    QString programName;
    QString version;
    QString installPath;
    QString repositoryPath;
    QString installerPath;

    QString getRepogenPath() const;
    QString getBinarycreatorPath() const;
};

#endif // QTINSTALLERCREATOR_H
