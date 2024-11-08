
# QtAutoPackager

### Overview

이 프로젝트는 Qt 기반으로 개발된 설치 파일 관리 애플리케이션으로, 프로그램 버전 관리, 설치 경로 설정, 디렉터리 구조 생성 및 파일 압축 기능을 제공합니다. `InstallerFileManager` 클래스는 디렉터리와 파일을 관리하고, `MainWindow` 클래스는 사용자 인터페이스 및 버튼 이벤트 처리를 담당합니다.

---

## Requirements

- **Qt 5 또는 6** 버전 필요 (필요 모듈: `QtCore`, `QtGui`, `QtWidgets`, `QtXml`)
- **PowerShell** (Windows에서 압축 기능 사용 시 필요)
- Windows OS 호환

---

## Project Structure

- **main.cpp**: 프로그램의 진입점이며, 언어 설정 및 메인 창을 초기화합니다.
- **mainwindow.h / mainwindow.cpp**: `MainWindow` 클래스를 구현하여 UI를 구성하고 버튼 클릭 이벤트를 처리합니다.
- **installerfilemanager.h / installerfilemanager.cpp**: `InstallerFileManager` 클래스를 구현하여 디렉터리와 파일 생성, XML 파일 저장, 파일 압축 등의 기능을 제공합니다.

---

## How to Build

1. **Qt 설치**: 최신 Qt 버전을 설치합니다.
2. **저장소 클론**:
   ```bash
   git clone https://github.com/username/version-config-app.git
   ```
3. **Qt Creator에서 열기**:
   - **Qt Creator**에서 `version-config-app.pro` 파일을 엽니다.
4. **빌드 및 실행**:
   - 배포용 **Release** 구성 또는 테스트용 **Debug** 구성을 사용하여 빌드합니다.
   - **Qt Creator**에서 프로젝트를 실행할 수 있습니다.

---

## Usage

### 애플리케이션 실행

애플리케이션을 실행한 후, 프로그램 이름과 버전, 설치 경로를 설정할 수 있습니다.

1. **디렉터리 생성**: `Save` 버튼을 클릭하여 디렉터리 구조와 파일을 생성하고, 버전에 맞는 설정 파일(config.xml, package.xml, license.txt)을 저장합니다.
2. **파일 압축**: `Compress` 버튼을 클릭하여 선택된 폴더를 압축하고, 생성된 압축 파일을 지정된 경로에 저장합니다.
  
### 주요 기능

- **createDirectoryStructure()**: 설정된 설치 경로에 필요한 디렉터리 구조를 생성합니다.
- **createConfigXml()**: 설치 설정을 위한 config.xml 파일을 생성합니다.
- **createPackageXml()**: 설치 패키지 정보를 담은 package.xml 파일을 생성합니다.
- **createLicenseFile()**: 라이선스 파일을 생성합니다.
- **compressFileToZip()**: 선택된 폴더를 7z 형식으로 압축하여 저장합니다.

### 예제

1. 프로그램 이름: `RockPaperScissors`
2. 버전: `v1.2.2`
3. 설치 경로: `D:/qt/RockPaperScissors/v1.2.2`

위 정보를 입력한 후 `Save` 버튼을 누르면, 해당 설치 경로에 필요한 디렉터리와 파일이 생성됩니다.

---

## Known Issues

- **PowerShell Compression Error**: Windows에서 압축 기능 사용 시 PowerShell이 필요하며, PowerShell 경로 설정이 잘못되었을 경우 압축 오류가 발생할 수 있습니다.
- **파일 경로 권한 문제**: 지정된 설치 경로에 쓰기 권한이 없을 경우 디렉터리 생성 및 파일 저장에 실패할 수 있습니다.

---

## License

이 프로젝트는 RGBLAB 라이선스 하에 제공됩니다.

---

### Author

개발자: Jung jinhyo

--- 

