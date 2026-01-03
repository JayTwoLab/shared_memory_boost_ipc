# shared_ipc.hpp 상세 설명

[English README](README.md)

이 헤더는 Boost.Interprocess 공유 메모리를 이용한 프로세스 간 통신(IPC)을 위한 핵심 정의와 유틸리티를 제공합니다. writer와 reader 모듈 모두에서 사용되어 프로세스 간 안전하고 효율적인 데이터 교환을 지원합니다.

## 주요 구성 요소

### 상수
- `k_segment_name`: 공유 메모리 세그먼트의 이름
- `k_object_name`: 세그먼트 내 공유 객체의 이름
- `k_segment_size`: 공유 메모리 세그먼트의 크기(기본값: 1MB). 데이터 크기에 맞게 조정해야 합니다.

### `shared_payload` 구조체
- 프로세스 간 교환되는 실제 데이터를 포함합니다.
- 필드:
  - `magic`: 공유 객체의 고유 식별자(기본값: 0xAABBCCDD)
  - `version`: 구조체 버전(호환성 유지 목적)
  - `sequence`: 갱신 시 증가하는 시퀀스 번호
  - `unix_ms`: 에포크 이후 밀리초 단위 타임스탬프
  - `message[256]`: 사용자 데이터 버퍼(크기 조정 가능)
  - `mutex`: 안전한 동시 접근을 위한 interprocess mutex
  - `cond`: 갱신 신호를 위한 condition variable

### 유틸리티 함수
- `copy_cstr`: 고정 크기 버퍼에 C-문자열을 안전하게 복사
- `now_unix_ms`: 현재 시간을 밀리초 단위로 반환
- `sleep_ms`: 지정한 밀리초 동안 현재 스레드를 대기

### `shutdown_flag` 클래스
- SIGINT/SIGTERM 신호를 통한 정상 종료 처리
- `is_set()`으로 종료 요청 여부 확인 가능

## 템플릿으로 활용 시 보완점
이 헤더는 공유 메모리 IPC의 기본 템플릿으로 적합하지만, 다음과 같은 부분을 보완할 수 있습니다:

- **데이터 구조 수정**: `shared_payload`를 애플리케이션에 맞게 필드 추가/변경, `message` 크기 조정 등
- **세그먼트/객체 이름 변경**: 여러 IPC 애플리케이션을 동시에 실행할 경우 이름 충돌 방지
- **세그먼트 크기 조정**: 데이터 구조가 커질 경우 `k_segment_size` 증가
- **동기화 확장**: 더 복잡한 동기화가 필요하다면 mutex/condition variable 추가 또는 Boost IPC의 다른 동기화 도구 활용
- **에러 처리 강화**: 실제 서비스 환경에서는 할당 실패, 예외 처리 등 견고한 에러 처리 필요
- **플랫폼별 신호 처리**: `shutdown_flag`는 POSIX 신호를 사용하므로, Windows 환경에서는 `SetConsoleCtrlHandler` 등으로 대체 필요

## 관련 파일
- 본 프로젝트의 writer, reader 모듈 모두에서 사용됩니다.
- 참고: [../writer/writer.cpp](../writer/writer.cpp), [../reader/reader.cpp](../reader/reader.cpp)

## 요구 사항
- C++11 이상
- Boost.Interprocess 라이브러리

---
이 파일은 공유 메모리 IPC 프로젝트의 재사용 가능한 템플릿을 목적으로 합니다. 필요에 따라 구조체와 유틸리티를 자유롭게 수정해 사용하세요.
