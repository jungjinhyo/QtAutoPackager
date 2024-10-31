#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void saveVersionToXml();  // Save 버튼 클릭 시 호출되는 함수

private:
    Ui::MainWindow *ui;

    // 디렉터리 구조 생성 함수
    bool createDirectoryStructure(const QString& baseDir);

    // XML 파일 생성 함수
    bool createConfigXml(const QString& version, const QString& baseDir);
    bool createPackageXml(const QString& version, const QString& baseDir);

    // License 파일 생성 함수
    bool createLicenseFile(const QString& baseDir);

    // XML 요소 생성 및 파일 저장 함수
    QDomElement createElement(QDomDocument& doc, const QString& tag, const QString& value);
    bool saveXmlToFile(const QDomDocument& doc, const QString& filePath);
};

#endif // MAINWINDOW_H
