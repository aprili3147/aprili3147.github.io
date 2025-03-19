# 3주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 3월 19일 ~ 2025년 3월 25일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

- **연구 목표:**
  - OpenCV 기반 패턴 매칭 및 거리 측정 기능 개발
  - TCP/IP 통신을 활용한 클라이언트-서버 데이터 송수신
  - MySQL을 이용한 데이터 관리 및 로깅 시스템 구축
  - MFC 기반 GUI 개발 및 사용자 인터페이스 개선

---

## 2. 연구 진행 내용

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

## 3. 문제점 및 해결 방안

- **TCP 통신 연결 지연 발생** → 패킷 최적화 및 비동기 방식 적용 검토
- **이미지 처리 속도 문제** → 멀티스레딩 도입 및 OpenCV 최적화 연구
- **데이터베이스 중복 데이터 문제** → 정규화를 통한 구조 개선 진행

---

## 4. 향후 연구 계획

- TCP/IP 통신 구현 완료 및 데이터 송수신 테스트
- OpenCV 이미지 처리 알고리즘 개선 및 최적화
- MySQL 데이터 연동 및 저장 기능 개발
- GUI 화면 개선 및 기능 연결 작업

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

