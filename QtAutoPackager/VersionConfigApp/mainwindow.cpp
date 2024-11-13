#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "installerfilemanager.h"
#include "QtInstallerCreator.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // UI 설정

    // Save 버튼 클릭 시 compressSelectedFile 함수 호출 연결 compressSelectedFile, saveVersionToXml 함수 호출 연결
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handleSaveButtonClick);

    // 파일 선택 버튼 클릭 시 selectFile 함수 호출 연결
    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::selectFile);

    // 프로그램 이름 또는 버전이 변경될 때 설치 경로를 업데이트
    connect(ui->programNameInput, &QLineEdit::textChanged, this, &MainWindow::updateInstallPath);
    connect(ui->versionInput, &QLineEdit::textChanged, this, &MainWindow::updateInstallPath);

    // 초기 설치 경로 업데이트
    updateInstallPath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleSaveButtonClick() {
    // 압축 함수 호출
    compressSelectedFile();
    // XML 저장 함수 호출
    saveVersionToXml();
    // QtInstallerCreator를 사용하여 설치 파일 생성
    createInstaller();
}

// 프로그램 이름이나 버전이 변경될 때 설치 경로를 자동으로 업데이트하는 함수
void MainWindow::updateInstallPath() {
    // 프로그램 이름과 버전 값을 가져오고, 빈 경우 기본값을 설정
    QString programName = ui->programNameInput->text().isEmpty() ? "ProgramName" : ui->programNameInput->text();
    QString version = ui->versionInput->text().isEmpty() ? "1.0.0" : ui->versionInput->text();

    // 경로를 "D:/qt/프로그램명/v버전명" 형식으로 설정
    QString defaultPath = QString("D:/qt/%1/v%2").arg(programName).arg(version);

    // 설정된 경로를 설치 경로 입력란에 표시
    ui->installPathInput->setText(defaultPath);
}

// Save 버튼 클릭 시 XML 파일과 디렉터리 구조를 생성하는 함수
void MainWindow::saveVersionToXml() {
    QString programName = ui->programNameInput->text();
    QString version = ui->versionInput->text();
    QString installPath = ui->installPathInput->text();

    fileManager = new InstallerFileManager(programName, version, installPath);

    if (!fileManager->createDirectoryStructure() ||
        !fileManager->createConfigXml() ||
        !fileManager->createPackageXml() ||
        !fileManager->createLicenseFile()) {
        ui->resultLabel->setText("Failed to create files or directory structure.");
        return;
    }

    // 압축 파일을 data 경로로 이동하고 임시 파일 삭제
    QString tempArchivePath = QCoreApplication::applicationDirPath() + "/" + QFileInfo(selectedFilePath).fileName() + ".zip";
    QString finalArchivePath = installPath + "/packages/com.mycompany." + programName.toLower().replace(" ", "") + "/data/" + QFileInfo(selectedFilePath).fileName() + ".zip";

    if (QFile::exists(finalArchivePath)) {
        QFile::remove(finalArchivePath); // 기존 파일 삭제
    }

    QFile::rename(tempArchivePath, finalArchivePath); // 임시 파일 이동

    ui->resultLabel->setText("Files and directory structure created and compressed file moved successfully!");
}

// 파일 선택 버튼 클릭 시 폴더를 선택하는 함수
void MainWindow::selectFile() {
    // 폴더 선택 대화 상자 열기
    selectedFilePath = QFileDialog::getExistingDirectory(this, "Select Folder to Compress");

    // 선택한 폴더 경로를 UI(select realese directory)에 표시
    if (!selectedFilePath.isEmpty()) {
        ui->filePathLabel->setText(selectedFilePath);
    }
}

// 압축 버튼 클릭 시 선택한 폴더 전체를 7z 형식으로 압축하는 함수
void MainWindow::compressSelectedFile() {
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No folder selected for compression.");
        return;
    }

    // 폴더 압축 수행
    if (fileManager && fileManager->compressFileToZip(selectedFilePath)) {
        ui->resultLabel->setText("Folder compressed successfully!");
    } else {
        ui->resultLabel->setText("Folder compression failed.");
    }
}

// 설치 파일 생성 함수
void MainWindow::createInstaller() {
    QString programName = ui->programNameInput->text();
    QString version = ui->versionInput->text();
    QString installPath = ui->installPathInput->text();

    QtInstallerCreator installerCreator(programName, version, installPath);

    if (!installerCreator.generateRepository()) {
        ui->resultLabel->setText("Failed to generate repository.");
        return;
    }

    if (!installerCreator.createInstallerExecutable()) {
        ui->resultLabel->setText("Failed to create installer executable.");
        return;
    }

    ui->resultLabel->setText("Installer executable created successfully!");
}


