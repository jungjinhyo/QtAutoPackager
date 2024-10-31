#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveVersionToXml);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QDomElement MainWindow::createElement(QDomDocument& doc, const QString& tag, const QString& value) {
    QDomElement element = doc.createElement(tag);
    element.appendChild(doc.createTextNode(value));
    return element;
}

void MainWindow::saveVersionToXml() {
    QString version = ui->versionInput->text();

    if (version.isEmpty()) {
        ui->resultLabel->setText("Please enter a version.");
        return;
    }

    QString baseDir = QString("D:/qt/rockpaperscissors-installer/v%1").arg(version);  // 최상위 디렉터리 경로

    if (!createDirectoryStructure(baseDir)) {
        ui->resultLabel->setText("Failed to create directory structure.");
        return;
    }

    if (!createConfigXml(version, baseDir) || !createPackageXml(version, baseDir) || !createLicenseFile(baseDir)) {
        ui->resultLabel->setText("Failed to create XML or license files.");
        return;
    }

    ui->resultLabel->setText("Directory structure and XML files have been created successfully!");
}

// 디렉터리 구조 생성 함수
bool MainWindow::createDirectoryStructure(const QString& baseDir) {
    QDir dir;

    // 최상위 디렉터리와 하위 디렉터리들 생성
    QStringList paths = {
        baseDir + "/config",
        baseDir + "/packages",
        baseDir + "/packages/com.mycompany.rockpaperscissors/meta",
        baseDir + "/packages/com.mycompany.rockpaperscissors/data",
        baseDir + "/repository"
    };

    for (const QString& path : paths) {
        if (!dir.mkpath(path)) {
            QMessageBox::warning(this, "Error", QString("Failed to create directory: %1").arg(path));
            return false;
        }
        // 빈 디렉터리에 __init__.py 파일 생성
        QFile initFile(path + "/__init__.py");
        if (!initFile.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Error", QString("Failed to create __init__.py in: %1").arg(path));
            return false;
        }
        initFile.close();
    }

    return true;
}

// config.xml 생성 함수 (경로 변경됨)
bool MainWindow::createConfigXml(const QString& version, const QString& baseDir) {
    QDomDocument doc;
    QDomElement root = doc.createElement("Installer");
    doc.appendChild(root);

    root.appendChild(createElement(doc, "Name", "Rock Paper Scissors"));
    root.appendChild(createElement(doc, "Version", version));
    root.appendChild(createElement(doc, "Title", "Rock Paper Scissors Game Installer"));
    root.appendChild(createElement(doc, "Publisher", "MyCompany"));
    root.appendChild(createElement(doc, "ProductUrl", "https://jungjinhyo.github.io/rockpaperscissors-installer/"));
    root.appendChild(createElement(doc, "InstallerWindowIcon", "installericon.png"));
    root.appendChild(createElement(doc, "InstallerApplicationIcon", "installericon.png"));
    root.appendChild(createElement(doc, "Logo", "logo.png"));
    root.appendChild(createElement(doc, "Watermark", "watermark.png"));
    root.appendChild(createElement(doc, "RunProgram", "@TargetDir@/RockPaperScissors.exe"));
    root.appendChild(createElement(doc, "RunProgramDescription", "Play the latest version of Rock Paper Scissors!"));
    root.appendChild(createElement(doc, "StartMenuDir", "RockPaperScissors"));
    root.appendChild(createElement(doc, "TargetDir", QString("@DesktopDir@/RockPaperScissors_v.%1").arg(version)));
    root.appendChild(createElement(doc, "AdminTargetDir", "@RootDir@/RockPaperScissors"));

    QDomElement repositories = doc.createElement("RemoteRepositories");
    QDomElement repository = doc.createElement("Repository");
    repository.appendChild(createElement(doc, "Url", QString("https://jungjinhyo.github.io/rockpaperscissors-installer/v%1").arg(version)));
    repository.appendChild(createElement(doc, "Enabled", "1"));
    repository.appendChild(createElement(doc, "DisplayName", QString("Rock Paper Scissors v%1 Repository").arg(version)));
    repositories.appendChild(repository);
    root.appendChild(repositories);

    return saveXmlToFile(doc, baseDir + "/config/config.xml");
}

// package.xml 생성 함수 (경로 변경됨)
bool MainWindow::createPackageXml(const QString& version, const QString& baseDir) {
    QDomDocument doc;
    QDomElement root = doc.createElement("Package");
    doc.appendChild(root);

    root.appendChild(createElement(doc, "DisplayName", QString("Rock Paper Scissors Game (v%1)").arg(version)));
    root.appendChild(createElement(doc, "Description", "A simple Rock Paper Scissors game."));
    root.appendChild(createElement(doc, "Version", version));
    root.appendChild(createElement(doc, "ReleaseDate", QDate::currentDate().toString("yyyy-MM-dd")));
    root.appendChild(createElement(doc, "Name", "com.mycompany.rockpaperscissors"));

    QDomElement licenses = doc.createElement("Licenses");
    QDomElement license = doc.createElement("License");
    license.setAttribute("name", "License Agreement");
    license.setAttribute("file", "license.txt");
    licenses.appendChild(license);
    root.appendChild(licenses);

    root.appendChild(createElement(doc, "DownloadableArchives", QString("RockPaperScissors_v%1.7z").arg(version)));
    root.appendChild(createElement(doc, "UpdateText", "This release includes bug fixes and new features."));
    root.appendChild(createElement(doc, "Default", "true"));
    root.appendChild(createElement(doc, "ForcedInstallation", "false"));
    root.appendChild(createElement(doc, "ForcedUpdate", "false"));

    return saveXmlToFile(doc, baseDir + "/packages/com.mycompany.rockpaperscissors/meta/package.xml");
}

// license.txt 파일 생성 함수 (경로 변경됨)
bool MainWindow::createLicenseFile(const QString& baseDir) {
    QFile file(baseDir + "/packages/com.mycompany.rockpaperscissors/meta/license.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to create license.txt");
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

bool MainWindow::saveXmlToFile(const QDomDocument& doc, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", QString("Failed to create %1").arg(filePath));
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
    return true;
}
