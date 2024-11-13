# QtAutoPackager

Qt Auto Packager는 Qt 기반으로 제작된 자동 설치 프로그램 생성 도구입니다. 이 프로그램은 프로젝트를 자동으로 설치 가능한 형식으로 패키징하고, 생성된 설치 파일을 AWS S3 버킷에 업로드할 수 있도록 도와줍니다..

---

## 최종 문서 업데이트 날짜 : 2024.11.13

---

# 설치 방법

## 1. 개발 환경 설정  
`bin/` 폴더에는 `binarycreator`, `installerbase`, `repogen` 파일이 포함되어 있으며, 이는 **Qt Installer Framework 4.8** 버전에 기반합니다. 따라서 사용자가 이 버전을 다운로드하여 설치했는지 확인해야 합니다.

## 2. 필요한 라이브러리 설치  
AWS S3 업로드를 위해 필요한 `boto3` 라이브러리를 설치합니다. 

```bash
pip install boto3
```

---

# 프로젝트 구조 및 파일 설명

프로젝트의 주요 파일 및 디렉토리는 다음과 같습니다.

```
|QtAutoPackager/
|-- bin/
|   |-- binarycreator.exe
|   |-- installerbase
|   |-- repogen.exe
|-- key/
|   |-- aws_credentials.txt
|-- installerfilemanager.cpp
|-- QtInstallerCreator.cpp
|-- mainwindow.cpp
|-- upload_to_s3.py
```

---

## 기능

- **디렉터리 구조 및 XML 파일 생성**: 설치 파일 생성을 위해 필요한 디렉터리 구조와 설정 파일(config.xml, package.xml, license.txt)을 자동으로 생성합니다.
- **설치 파일 생성**: 사용자 입력에 따라 `repogen` 및 `binarycreator` 명령을 통해 설치 파일을 생성합니다.
- **S3 업로드**: 생성된 설치 파일을 Amazon S3에 업로드하여, 팀원이나 사용자가 웹을 통해 설치 파일에 접근할 수 있도록 합니다.

---

## 설치 파일 구조

다음은 자동으로 생성되는 디렉터리 구조입니다.

```
[installPath]/
├── config/                 # config.xml 파일을 포함하는 폴더
├── packages/               # 설치 패키지 정보 (data, meta 포함)
│   └── com.mycompany.[프로그램명]/
│       ├── meta/           # package.xml, license.txt 파일 포함
│       └── data/           # 압축된 프로그램 파일
└── repository/             # repogen 명령어를 통해 생성된 설치 파일 저장소
```

---

## 주요 파일 및 폴더 설명

- **bin/**:
  - `repogen.exe`, `binarycreator.exe`, `installerbase.exe`가 포함되어 있습니다.
  - Qt Installer Framework 4.8에서 제공된 도구들이며, 설치 파일 생성에 사용됩니다.
  
- **key/aws_credentials.txt**:
  - S3 업로드 권한이 있는 IAM 액세스 키와 시크릿 키가 저장된 파일입니다.
  - 형식 예시:
    ```
    aws_access_key_id=YOUR_ACCESS_KEY
    aws_secret_access_key=YOUR_SECRET_KEY
    ```

---

## 사용 방법

### 1. 설치 파일 생성

1. 프로그램 이름과 버전, 설치 경로를 입력합니다.
2. **Save** 버튼을 클릭하여 디렉터리 구조와 필요한 XML 파일을 생성합니다.
3. XML 생성 후 설치 파일이 자동으로 생성됩니다. 결과는 지정한 `installPath`에 저장됩니다.

### 2. S3 업로드

1. **bucketName**을 입력합니다. S3에 올라간 버킷 이름과 다를 경우 업로드되지 않습니다.
2. **S3 Upload** 버튼을 클릭하여 `installPath` 경로에 생성된 설치 파일을 S3에 업로드합니다.
3. 업로드할 S3 버킷 및 경로는 Python 스크립트(`upload_to_s3.py`)를 통해 설정됩니다.

---

## 주요 파일 설명

### 1. `bin/binarycreator.exe`, `bin/repogen.exe`  
이 두 파일은 **Qt Installer Framework**에서 제공하는 실행 파일들로, 자동 설치 프로그램 생성에 필수적입니다. `binarycreator`는 설치 파일을 생성하고, `repogen`은 설치 패키지를 생성하는 데 사용됩니다.

### 2. `key/aws_credentials.txt`  
AWS S3 버킷에 파일을 업로드할 수 있는 자격 증명을 포함한 파일입니다. 이 파일에는 **AWS Access Key**와 **Secret Key**가 저장되어 있어야 하며, 각각의 라인에 `aws_access_key_id=YOUR_ACCESS_KEY`와 `aws_secret_access_key=YOUR_SECRET_KEY` 형식으로 작성됩니다.

### 3. `scripts/upload_to_s3.py`  
S3에 설치 파일을 업로드하기 위한 스크립트 파일입니다. 이 파일은 지정된 경로에 있는 설치 파일을 `bucket_name` 및 `object_key`로 S3에 업로드합니다.

---

# 실행 방법

## 1. 패키지 저장소 및 설치 파일 생성  
버튼을 클릭하여 저장소 생성 및 설치 파일을 생성할 수 있습니다. Qt Installer Framework 경로가 올바르게 설정되어 있는지 확인해야 합니다.

```cpp
QtInstallerCreator installerCreator(programName, version, installPath);
installerCreator.generateRepository();
installerCreator.createInstallerExecutable();
```
---