#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>  // XML 관련 클래스 포함

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Qt의 Signal과 Slot 매커니즘을 사용하기 위해 필요

public:
    explicit MainWindow(QWidget *parent = nullptr);  // 생성자
    ~MainWindow();  // 소멸자

private slots:
    void saveVersionToXml();  // Save 버튼 클릭 시 호출되는 함수
    void updateInstallPath();  // 프로그램 이름 또는 버전 변경 시 설치 경로 업데이트

private:
    Ui::MainWindow *ui;  // UI 객체

    // XML 파일 생성 및 저장 함수
    bool createConfigXml(const QString& programName, const QString& version, const QString& baseDir);
    bool createPackageXml(const QString& programName, const QString& version, const QString& baseDir);
    bool createLicenseFile(const QString& baseDir);
    QDomElement createElement(QDomDocument& doc, const QString& tag, const QString& value);
    bool createDirectoryStructure(const QString& baseDir);
    bool saveXmlToFile(const QDomDocument& doc, const QString& filePath);
};

#endif // MAINWINDOW_H
