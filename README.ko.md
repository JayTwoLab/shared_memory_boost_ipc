# 공유 메모리 Boost IPC

[English README](README.md)

이 프로젝트는 [Boost.Interprocess](https://www.boost.org/doc/libs/release/doc/html/interprocess.html)를 활용한 공유 메모리 기반의 프로세스 간 통신(IPC) 예제를 제공합니다. 여러 프로세스가 공유 메모리를 통해 데이터를 효율적으로 교환하는 방법을 리더와 라이터 예제를 통해 보여줍니다.

## 주요 특징
- Boost.Interprocess를 이용한 공유 메모리 통신
- 라이터 및 리더 예제 프로그램 제공
- CMake 기반 빌드 시스템
- 손쉬운 통합을 위한 헤더 전용 인터페이스

## 프로젝트 구조
```
shared_memory_boost_ipc/
├── include/
│   └── shared_ipc.hpp      # 공유 메모리 IPC 인터페이스
├── reader/
│   ├── reader.cpp          # 리더 예제
│   └── CMakeLists.txt      # 리더 빌드 설정
├── writer/
│   ├── writer.cpp          # 라이터 예제
│   └── CMakeLists.txt      # 라이터 빌드 설정
├── CMakeLists.txt          # 최상위 빌드 설정
├── LICENSE                 # 라이선스 정보
├── README.md               # 프로젝트 문서
```

## 필요 환경
- C++17 이상
- [Boost](https://www.boost.org/) 라이브러리 (Interprocess 모듈)
- CMake 3.10+

## 빌드 방법
1. Boost를 설치하세요 (`boost_system` 및 `boost_interprocess` 모듈 필요).
2. 저장소를 클론합니다:
   ```sh
   git clone https://github.com/JayTwoLab/shared_memory_boost_ipc.git
   cd shared_memory_boost_ipc
   ```
3. 빌드 디렉터리를 만들고 CMake를 실행합니다:
   ```sh
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
    - For fast search boost 
    ```sh
    cmake -S . -B build -DBOOST_ROOT=/usr -DBOOST_INCLUDEDIR=/usr/include -DBOOST_LIBRARYDIR=/usr/lib64   -DBoost_NO_SYSTEM_PATHS=ON -DBoost_DEBUG=ON
    ```   
4. 리더와 라이터 실행 파일이 각 폴더에 생성됩니다.

## 사용 방법
1. 라이터 프로세스를 실행하여 공유 메모리에 데이터를 작성합니다:
   ```sh
   ./writer/writer
   ```
2. 리더 프로세스를 실행하여 공유 메모리에서 데이터를 읽습니다:
   ```sh
   ./reader/reader
   ```

## 라이선스
이 프로젝트는 저장소의 LICENSE 파일에 명시된 조건에 따라 라이선스가 적용됩니다.

## 참고 자료
- [Boost.Interprocess 공식 문서](https://www.boost.org/doc/libs/release/doc/html/interprocess.html)
