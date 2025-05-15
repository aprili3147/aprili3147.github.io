# 8주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 4월 22일 ~ 2025년 4월 29일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

---

## 2. 구현 내용
- **YOLO 학습용 데이터셋 구축을 위한 이미지 라벨링**

- **ORB 특징점 매칭 알고리즘을 활용하여 이미지 간 특징점 매칭 수행**
    - 다양한 환경 (회전, 스케일 변화, 조명 변화 등)에서 견고한 매칭 성능 확보

- **현재까지 구현된 매칭 알고리즘을 클라이언트 폼에 적용**
    
- **템플릿 사진의 히스토그램을 바탕으로 카메라 사진과 히스토그램 매칭 수행**
    - cameraPic의 이미지를 templatePic의 이미지 밝기와 비슷하도록 조정

---

## **3. 문제점 분석**
- **패턴에 다양한 노이즈, 회전, 밝기 조절을 적용하여 데이터 증강 후 라벨링 수행**  

- **OpenCV 라이브러리를 사용하여 ORB 특징점 검출 및 매칭 수행**  
  - BFMatcher 또는 FLANN 기반 매칭 사용, 결과 시각화

- **클라이언트 폼에 YOLO, ORB 특징점 기반 매칭 알고리즘 적용**  

- **두 이미지의 히스토그램 매칭 수행**
  - 누적분포함수(CDF) 기반 매핑 테이블 생성, 밝기 변환

---

## **4. 향후 개선 방안**

- **학습 데이터 확장**  
  - 다양한 각도, 배경, 해상도에서 생성된 패턴 포함하여 일반화 성능 향상

- **코드 최적화**  
  - 연산 병목 구간 최적화 및 코드 리팩토링

- **클라이언트 폼 통합**  
  - 클라이언트에 ORB 매칭 기능 통합

- **히스토그램 매칭 기능 개선**  
  - LightSetting 및 LightReset 메뉴 구현

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
