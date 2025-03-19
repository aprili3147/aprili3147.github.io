# 2주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 3월 10일 ~ 2025년 3월 18일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

- **연구 목표:**
  - MFC DB 서버 연결을 진행한다.
  - OpenCV를 Visual Studio MFC App과 연동하고, OpenCV를 C++ 언어로 사용하는 방법과 OpenCV에서 자주 사용되는 함수들에 대해 공부
  - Basler 카메라와 연동을 위한 Pylon SDK 환경설정을 하고, 카메라 제어 방법을 학습

---

## 2. 연구 계획 내용

- **TCP/IP 통신 프로토콜 설계**
  - 클라이언트-서버 간 데이터 송수신 방식 연구
  - 패킷 구조 및 오류 처리 방식 설계

- **데이터베이스 설계**
  - MySQL을 활용한 테이블 구조 설계
  - 검사 결과 및 로깅 시스템 구축 방안 논의

- **OpenCV 기반 이미지 처리**
  - 패턴 매칭 및 거리 측정 알고리즘 기초 구현
  - 이미지 전처리(그레이스케일 변환, 노이즈 제거) 테스트 수행

- **MFC GUI 개발**
  - 프로젝트 UI 구조 설계 및 초기 화면 구성
  - 패턴 등록 및 시뮬레이션 기능 개발 기획

---

## 3. 문제점 분석

- 실시간 이미지 처리에 대한 부분은 아직 익히지 못하여 향후 프로젝트 진행에 영향을 미칠 것으로 예상된다.
- C++ 문법의 숙련도와 MFC의 구조를 파악하는 것에 어려움이 있었다.
- Pylon SDK를 설치하고 Visual Studio와 연동하는 환경설정에서 링크 에러가 발생하였다.

## 4. 향후 연구 계획

- 프로젝트에 사용될 테이블과 ER 다이어그램을 만들고, MFC에 연동할 예정이다.
- OpenCV의 함수인 GrayScale, Edge-detected, Contour 등에 대한 추가적인 공부를 진행하고, 실시간 이미지 처리에 대해서도 공부할 예정이다.
- MFC 실습과 C++ 문법 공부를 병행하며, 프로젝트의 초기 기능을 개발할 예정이다.
- 카메라를 Visual Studio에서 제어가 가능하도록 하고, 카메라의 영상을 가져오도록 할 예정이다. 이후 파라미터 조정을 통해 영상 품질을 최적화할 계획이다.

---

## 4. 참고 자료

- [GitHub 협업 가이드](https://docs.github.com/en/github)
- [Basler 공식 Tool 튜토리얼 매뉴얼](https://www.baslerweb.com/ko-kr/learning/tutorials/)
- [OpenCV Forum C++ 카테고리](https://forum.opencv.org/c/c/9)
- [OpenCV 공식 문서](https://docs.opencv.org/)
- [MFC 공식 문서](https://learn.microsoft.com/ko-kr/cpp/mfc/mfc-concepts?view=msvc-170)
- [MySQL 공식 문서](https://dev.mysql.com/doc/)
- HBS Solution 제공 프로젝트 PPT

---

이 문서는 연구 진행 상황에 따라 업데이트될 수 있습니다.

