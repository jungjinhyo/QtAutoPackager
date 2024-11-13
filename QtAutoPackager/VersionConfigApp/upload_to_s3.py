import os
import boto3
import sys

def upload_file_to_s3(file_path, bucket_name, object_key, credentials_file_path=None):
    # aws_credentials.txt 파일 경로 설정
    if credentials_file_path is None:
        credentials_file_path = os.path.join(os.path.dirname(__file__), "key/aws_credentials.txt")

    # 자격 증명 파일에서 액세스 키와 시크릿 키 읽기
    with open(credentials_file_path, 'r') as file:
        lines = file.readlines()
        access_key = lines[0].strip().split('=')[1]
        secret_key = lines[1].strip().split('=')[1]

    # boto3 세션 생성
    session = boto3.Session(
        aws_access_key_id=access_key,
        aws_secret_access_key=secret_key
    )
    s3 = session.client('s3')

    # S3에 파일 업로드
    try:
        s3.upload_file(file_path, bucket_name, object_key)
        print(f"Upload successful: {file_path} to {bucket_name}/{object_key}")
    except Exception as e:
        print(f"Upload failed: {e}")

if __name__ == "__main__":
    file_path = sys.argv[1]      # 업로드할 파일 경로
    bucket_name = sys.argv[2]    # S3 버킷 이름
    object_key = sys.argv[3]     # S3 내 업로드 경로

    upload_file_to_s3(file_path, bucket_name, object_key)
