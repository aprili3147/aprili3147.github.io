# 7주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 4월 15일 ~ 2025년 4월 21일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

---

## 2. 구현 내용
- **Edge 검출에 회전 변화에 대응 가능한 템플릿 매칭 기법**
    - 템플릿을 -30도부터 +30도까지 5도 간격으로 회전하며 엣지 기반 외곽선 매칭 수행

- **이미지 매칭 방법을 클라이언트 폼에 적용**
    - 기존 연구 기반으로 이미지 매칭 방식 2가지를 클라이언트 폼에 적용

- **YOLO 모델을 사용한 CNN 기반 객체 탐지 알고리즘 구현 및 성능 개선**
    - 데이터 수집 및 전처리 후 YOLO 모델을 Google Colab에서 학습
    - 학습된 모델을 테스트 이미지에 적용하고, 신뢰도와 함께 결과 시각화

- **클라이언트에서도 템플릿 이미지 업로드 가능하도록 구현하고, 해당 이미지 히스토그램 출력 기능 추가**
    - 업로드 버튼 구현하여 로컬 이미지 업로드 후 Mat 객체로 저장, DisplayHistogram() 함수로 히스토그램 출력

---

## **3. 문제점 분석**
- **템플릿 및 입력 이미지 크기 변화**  
  - 템플릿 및 입력 이미지 크기 변화에는 미대응, 회전 매칭으로 인한 실행 시간 증가

- **OpenCV 지식 부족**  
  - OpenCV에 익숙하지 않아 필요한 기능 선별에 시간 소요

- **데이터셋 부족**  
  - 데이터셋 부족으로 확대/축소 이미지에서 객체 인식 정확도 저하

---

## **4. 향후 개선 방안**

- **데이터셋 라벨링**  
  - YOLO 모델 데이터셋 리벨링 및 증강을 통해 크기 변화에도 대응 가능한 모델 학습 진행 예정

- **YOLO 개선 및 알고리즘 개발**  
  - 바운딩 박스 대신 drawContours 방식으로 탐지 결과 시각화 예정
  - 데이터셋에 확대/축소 및 밝기 변화 반영하여 ORB 기반 매칭 알고리즘 개발

- **클라이언트 구현 및 최적화**  
  - 추가 매칭 방식 개발 시 클라이언트 폼에 구현 및 최적화 진행
  - 서버로부터 전달받은 템플릿 이미지 처리 로직 구현

- **카메라 히스토그램 조정**  
  - 카메라 영상의 히스토그램이 템플릿 이미지와 유사한 밝기 분포가 되도록 조정 예정

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
