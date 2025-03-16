# 캡스톤 디자인 프로젝트 개발계획서

## 1. 프로젝트 개요

- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)
- **개발 기간:** 2025년 3월 ~ 2025년 6월
- **사용 기술:**
  - **백엔드:** C++(MFC)
  - **프론트엔드:** MFC GUI
  - **데이터베이스:** MySQL
  - **버전 관리:** Git, GitHub
  - **기타** OpenCV, TCP/IP 통신, Basler 등

---

## 2. 프로젝트 목표

- 클라이언트 기능

  - 카메라를 통해 이미지를 획득

  - 특정 패턴을 인식하여 서버로 전송

  - OpenCV 기반 패턴 매칭 및 거리 측정 알고리즘 구현

  - Masking 처리 및 Multi-pattern 인식 지원

  - 카메라 제어(노출, Gain, 조명 설정) 기능 제공

  - GUI 기반으로 패턴 등록 및 시뮬레이션 기능 지원

- 서버 기능

  - 수신한 이미지 데이터를 분석하여 Critical Dimension(CD) 측정 수행

  - TCP/IP 통신 프로토콜을 통해 클라이언트와 데이터 교환

  - 이미지 재편성 및 센터링 후 CD 측정 진행

  - 검사 결과 로그 관리 및 저장

  - 통신 및 Motion 실패 시 알람 처리 기능 제공

## 3. 주요 기능

3.1 클라이언트 기능

  - 이미지 획득 및 전처리

  - OpenCV 기반 패턴 매칭 및 거리 측정

  - 패턴 등록 및 시뮬레이션 기능 제공

  - 카메라 제어(노출, Gain, 조명 설정)

  - Masking 및 Multi-pattern 인식 지원

3.2 서버 기능

  - TCP/IP 통신을 통한 클라이언트와 데이터 송수신

  - 이미지 처리 및 CD 측정 알고리즘 구현

  - 검사 결과 로깅 및 데이터베이스 저장

  - 시스템 장애 및 Motion 실패 시 알람 처리

---

## 4. 일정 계획

| 단계  | 작업 내용 | 기간 |
|------|----------------------------------------------|
| 1단계  | 프로젝트 기획 및 개발계획서 작성 | 2주 |
| 2단계  | TCP 통신 프로토콜 설계 | 2주 |
| 3단계  | DB 설계 | 2주 |
| 4단계  | Server 모듈 제작 | 3주 |
| 5단계  | Client 모듈 제작 | 3주 |
| 6단계  | DB + OpenCV(Server 연동) | 4주 |
| 7단계  | Camera(Basler +Client 연동) | 4주 |
| 8단계  | 알고리즘(이미지처리 속도 향상) | 4주 |
| 9단계  | 추가 기능 및 테스트	 | 2주 |
| 10단계 | 발표 | 2주 |
| 11단계 | 보고서 작성 | 2주 |

---

## 5. 기대 효과

 - OpenCV 기반의 정밀한 패턴 매칭 및 거리 측정을 통해 품질 검사 자동화

 - 서버-클라이언트 구조를 활용한 실시간 데이터 처리 및 분석 가능

 - TCP/IP 통신을 통한 유연한 확장성과 다중 클라이언트 지원

 - 직관적인 GUI 제공으로 사용자 편의성 향상

 - 프로젝트를 통해 기업 연계를 강화하고 산업 현장 적용 가능성 검토



---

## 6. 참고 자료

- [GitHub 협업 가이드](https://docs.github.com/en/github)
- [Basler 공식 Tool 튜토리얼 매뉴얼](https://www.baslerweb.com/ko-kr/learning/tutorials/)
- [OpenCV Forum C++ 카테고리](https://forum.opencv.org/c/c/9)
- [OpenCV 공식 문서](https://docs.opencv.org/)
- [MFC 공식 문서](https://learn.microsoft.com/ko-kr/cpp/mfc/mfc-concepts?view=msvc-170)
- [MySQL 공식 문서](https://dev.mysql.com/doc/)
- HBS Solution 제공 프로젝트 PPT


---

이 문서는 초기 개발계획서로 이후 진행 상황에 따라 수정될 수 있습니다.
