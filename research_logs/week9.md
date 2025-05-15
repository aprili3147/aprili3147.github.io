# 9주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 4월 30일 ~ 2025년 5월 6일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

---

## 2. 구현 내용
- **기존 YOLO 객체 탐지 모델 개선**
    - 확대, 축소, 회전 등 이미지 변형에서 탐지 정확도 향상

- **결과 이미지 출력 문제 해결**
    - 이미지 출력 지연 및 오류 파악 및 수정

- **Histogram Matching 실시간 출력**
    - 기존 정적 매칭에서 실시간 변환된 이미지 출력으로 개선

---

## **3. 문제점 분석**
- **데이터 증강으로 인한 과적합 위험**  
  - 특정 유형이 반복되면서 모델이 데이터에 과도하게 적응할 가능성

- **코드 이해 부족**  
  - Win32 방식과 MFC 방식 코드의 차이 미숙지

- **카메라 테스트 미진행**
  - 이미지 변환 지연 발생 가능성

---

## **4. 향후 개선 방안**

- **데이터 균형 조정**  
  - 특정 증강 유형(회전, 확대 등) 비중 편중 방지

- **기능 통합**  
  - 각자 개발한 기능을 하나의 프로젝트에 병합

- **카메라 테스트**  
  - 실시간 변화 테스트 및 로그 기록

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
