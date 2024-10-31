#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "installerfilemanager.h"  // InstallerFileManager 클래스 포함

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
    void saveVersionToXml();  // Save 버튼 클릭 시 호출되는 함수
    void updateInstallPath();  // 프로그램 이름이나 버전 변경 시 설치 경로 자동 업데이트

private:
    Ui::MainWindow *ui;  // UI 객체
};

#endif // MAINWINDOW_H
