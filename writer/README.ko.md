# Writer 모듈

[English README](README.md)

이 디렉터리에는 Boost.Interprocess를 사용한 공유 메모리 writer의 구현이 포함되어 있습니다. writer는 공유 메모리 세그먼트를 생성하고 데이터를 기록하는 역할을 하며, 이 데이터는 reader 모듈 등 다른 프로세스에서 접근할 수 있습니다.

## 파일
- `writer.cpp`: Boost.Interprocess를 사용하여 공유 메모리를 생성하고 기록하는 방법을 보여주는 주요 소스 파일입니다.

## 주요 기능
- 공유 메모리 세그먼트 생성
- 공유 메모리에 데이터 기록
- Boost 라이브러리를 활용한 프로세스 간 통신(IPC) 예제

## 사용법
이 모듈은 일반적으로 공유 메모리에 기록된 데이터를 읽는 reader 모듈과 함께 사용됩니다. writer를 빌드하고 실행하려면:

1. 시스템에 Boost가 설치되어 있는지 확인하세요.
2. CMake를 사용해 프로젝트를 빌드하세요:
   ```sh
   cd writer
   cmake .
   cmake --build .
   ```

3. writer 실행 파일을 실행하세요:
   ```sh
   ./writer
   ```

### 공유 메모리 세그먼트 삭제

writer 실행 파일은 `--remove` 인자를 지원합니다. 이 옵션을 사용하면 시스템에서 공유 메모리 세그먼트를 삭제할 수 있습니다. 리소스 정리나 공유 메모리 상태 초기화가 필요할 때 유용합니다. 사용 예시는 다음과 같습니다:

```sh
./writer --remove
```
세그먼트가 존재하면 삭제되고, 결과 메시지가 출력됩니다.

## 요구 사항
- C++11 이상을 지원하는 컴파일러
- Boost.Interprocess 라이브러리

## 참고
- [../reader](../reader): 공유 메모리에서 데이터를 읽는 reader 모듈
- [../include/shared_ipc.hpp](../include/shared_ipc.hpp): IPC 로직을 위한 공용 헤더
