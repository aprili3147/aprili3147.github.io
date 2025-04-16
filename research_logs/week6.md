# 6주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 4월 8일 ~ 2025년 4월 14일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

---

## 2. 구현 내용
- **이미지 증강 및 라벨링 자동화**
    - Albumentations 라이브러리를 이용하여 이미지 증강 적용
    - LabelImg를 활용하여 YOLO 포맷의 라벨링을 자동화

---

- **Contour 기반 객체 탐지**
    - matchShapes() 함수를 사용하여 Contour 간 유사도 계산
    - 유사도가 임계값 이하인 Contour를 탐지 대상으로 설정 및 시각적 표시

- **서버-클라이언트 로그 기록**
    - MFC에서 파일 저장 및 조회 방법을 연구하여 로그 파일 저장/조회 기능 구현
    - 생성일을 파일명으로 설정하여 로그 관리
    - 로그 데이터의 DB 저장 기능 추가

- **카메라 영상 히스토그램 출력**
    - OpenCV를 활용하여 클라이언트의 basler_camera_histogram 영역에 히스토그램 출력

---

## **3. 문제점 분석**
- **유사 이미지 과다 생성**  
  - 이미지 증강으로 인해 유사한 이미지가 과다 생성되어 일반화 성능 저하 가능

- **객체 탐지 정확도 문제**  
  - 단순한 형태의 객체는 높은 정확도를 보이지만, 복잡한 형태에서는 정확도가 낮음

- **클라이언트 CSocket 이해도 부족**  
  - 버그 수정 중 CSocket의 비동기 처리 과정에 대한 이해 부족 문제 발견
  - 추가 학습 후 예외 처리 로직 적용

- **카메라 히스토그램 성능 문제**  
  - 지속적인 히스토그램 출력으로 성능 저하 발생 가능
  - 현재 프레임 기준의 히스토그램 출력 방식 고려 중 

---

## **4. 향후 개선 방안**

- **증강 데이터 다양성 제어**  
  - 다양한 모델(YOLOv5, YOLOv8, SSD 등)에 증강 전략 적용 후 일반화 성능 비교 연구 수행

- **매칭 알고리즘 고도화**  
  - YOLO 모델을 추가하여 복잡한 형태의 객체 탐지 성능 향상

- **로그 시스템 개선**  
  - 팀원들이 구현한 기능을 통합하여 일괄적인 로그 기록 방식 적용

- **카메라 히스토그램 최적화**  
  - camera_picture_histogram에 기준 사진 히스토그램을 출력
  - 메뉴를 통해 카메라 영상 촬영 시작/종료 기능 구현 예정

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
