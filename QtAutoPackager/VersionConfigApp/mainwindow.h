#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "installerfilemanager.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);  // 생성자
    ~MainWindow();  // 소멸자

private slots:
    void updateInstallPath();            // 프로그램 이름이나 버전 변경 시 설치 경로 자동 업데이트
    void selectFile();                   // 파일 선택 버튼 클릭 시 파일 선택
    void handleSaveButtonClick();        // compressSelectedFile, saveVersionToXml 함수 실행
    void compressSelectedFile();         // 압축 버튼 클릭 시 파일 압축
    void saveVersionToXml();             // Save 버튼 클릭 시 XML 파일과 디렉터리 구조 생성

private:
    Ui::MainWindow *ui;                  // UI 객체
    InstallerFileManager *fileManager;   // 파일 관리 객체
    QString selectedFilePath;            // 선택한 파일 경로 저장
};

#endif // MAINWINDOW_H
