# 3주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 3월 26일 ~ 2025년 4월 1일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

- **연구 목표:**
  - CMake를 활용하여 다른 사용자도 자동으로 환경 설정이 완료된 상태에서 프로젝트를 사용할 수 있도록 구성
  - 서버와 클라이언트에서 이미지 및 문자열 데이터를 송수신
  - 서버에 접속할 수 있는 클라이언트를 제한하여 보안 강화
  - OpenCV의 matchTemplate 함수를 활용한 템플릿 매칭 수행
  - minMaxLoc 함수를 이용하여 매칭된 위치를 찾아 표시하고, 결과 이미지를 Picture Control에 출력
  - CMake를 적용하여 MFC, MySQL, OpenCV 라이브러리를 자동으로 설정하고 실행할 수 있는 환경 제공

---

## 2. 구현 내용

- **CMake 적용**
  - MFC, MySQL, OpenCV 라이브러리를 자동 설정하여 실행 환경 구성
  - Visual Studio의 내장된 CMake 기능을 활용하여 CMakePresets.json을 표준화하여 프로젝트 구성 관리

- **서버-클라이언트 통신**
  - 이미지 및 문자열 데이터를 송수신하는 기능 구현
  - 클라이언트 접속 제한 기능 추가
  - 3-way, 4-way handshake를 기반으로 메시지를 로그에 저장하는 기능 구현

- **템플릿 매칭(Image Processing)**
  - OpenCV의 matchTemplate 함수를 이용하여 템플릿 매칭 수행

  - minMaxLoc 함수를 사용하여 매칭된 위치 탐색 후 사각형으로 표시

  - 결과 이미지를 MFC의 Picture Control에 출력

  - 입력 이미지 내에서 템플릿 이미지의 유사한 영역을 찾아 위치 정보 제공

---

## **3. 문제점 분석**

- **MySQL 및 OpenCV 경로 문제** → CMake를 활용하여 경로를 자동으로 설정하는 방식으로 개선 필요
- **CSocket과 MFC UI 쓰레드 충돌** → 쓰레드 간 안전한 데이터 공유 방식 적용 필요
- **템플릿 매칭 정확도 문제(입력 이미지가 정렬되지 않거나 기울어진 경우 템플릿과의 위치·각도 불일치 발생)** →  이미지의 회전 또는 기울어짐에 대한 보정 필요

## 4. 향후 개선 방안

- 결과 이미지 개선 → 현재는 매칭된 위치를 사각형으로 표현하나, Edge-Detected 기법을 적용하여 출력할 예정
- Picture Control에서 템플릿이 중앙에 위치하도록 보정하는 방법 연구
- Visual Studio의 내장 CMake 기능을 활용하여 CMakePresets.json을 활용한 표준화 → MySQL 및 OpenCV의 경로 자동 설정 기능 추가
- 3-way, 4-way handshake 기반 메시지 로그 저장 기능 강화 → 클라이언트 인증 및 접속 제한 기능 개선

---

## 5. 참고 자료

- [GitHub 협업 가이드](https://docs.github.com/en/github)
- [Basler 공식 Tool 튜토리얼 매뉴얼](https://www.baslerweb.com/ko-kr/learning/tutorials/)
- [OpenCV Forum C++ 카테고리](https://forum.opencv.org/c/c/9)
- [OpenCV 공식 문서](https://docs.opencv.org/)
- [MFC 공식 문서](https://learn.microsoft.com/ko-kr/cpp/mfc/mfc-concepts?view=msvc-170)
- [MySQL 공식 문서](https://dev.mysql.com/doc/)
- HBS Solution 제공 프로젝트 PPT

---

이 문서는 연구 진행 상황에 따라 업데이트될 수 있습니다.
