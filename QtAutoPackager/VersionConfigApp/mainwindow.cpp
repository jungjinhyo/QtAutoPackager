#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "installerfilemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // UI 설정

    // Save 버튼 클릭 시 saveVersionToXml 함수 호출 연결
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveVersionToXml);
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
    // 프로그램 이름, 버전, 설치 경로를 UI에서 가져옴
    QString programName = ui->programNameInput->text();
    QString version = ui->versionInput->text();
    QString installPath = ui->installPathInput->text();

    // InstallerFileManager 객체 생성
    InstallerFileManager fileManager(programName, version, installPath);

    // 디렉터리 구조 및 XML 파일 생성
    if (!fileManager.createDirectoryStructure() ||
        !fileManager.createConfigXml() ||
        !fileManager.createPackageXml() ||
        !fileManager.createLicenseFile()) {
        // 실패 시 에러 메시지 표시
        ui->resultLabel->setText("Failed to create files or directory structure.");
        return;
    }

    // 성공 시 완료 메시지 표시
    ui->resultLabel->setText("Files and directory structure created successfully!");
}
