# 5주차 연구일지

## 1. 연구 개요

- **연구 기간:** 2025년 4월 1일 ~ 2025년 4월 7일
- **프로젝트 이름:** 서버 클라이언트 구조의 CD Measurer/Aligner 구현 (HBS Solution 기업 연계)

---

## 2. 구현 내용
- **CMake 환경 구성**
    - CMake Preset과 vcpkg를 이용하여 라이브러리 자동 연동 환경 구성  
    - 프로젝트 내 CMakeLists.txt와 CMakePresets.json 표준화  
    - Visual Studio 기반 개발 환경에서 빌드 반복 최소화  

---

- **템플릿 매칭 처리**
    - OpenCV의 `matchTemplate`을 사용한 템플릿 매칭 구현  
    - `minMaxLoc`으로 매칭 위치 탐색 후 중심 정렬 처리  
    - 기존 사각형(Rectangle) 방식 대신 `Contours` 기반 시각화 적용 (`findContours`, `drawContours`)  
    - 템플릿 결과를 이미지 중앙 기준으로 재배치하여 시각적 확인 용이  

- **Basler 카메라 처리**
    - 실시간 영상 스트리밍을 클라이언트에 출력하는 기능 구현  
    - Capture 버튼 클릭 시 현재 프레임 추출 및 OpenCV 이미지 처리 연동  
    - 카메라 설정 파라미터(Gain, Exposure Time, Frame Rate) 적용 가능하도록 설계  

- **서버-클라이언트 통신 및 DB 처리**
    - 클라이언트로부터 수신한 이미지 데이터를 서버가 저장하고 재조회  
    - 서버-클라이언트 간 메시지 및 이미지 송수신 기능 구축  
    - DB 관련 코드 클래스화 및 모듈화 시도 

---

## **3. 문제점 분석**
- **라이브러리 설정 이슈**  
  - vcpkg 설치 경로 및 구조에 대한 이해 부족 → `Config.cmake` 탐색 오류 발생

- **매칭 알고리즘 한계**  
  - `matchTemplate` 단일 알고리즘만 사용 중 → 잡음 및 유사 패턴에 오탐지 발생 가능성  

- **이미지 저장 처리 이슈**  
  - 이미지 DB 저장 시 포맷과 크기 오류 발생  
  - DB 코드 클래스 구조로 분리 후 각 탭 간 재사용 어려움  

- **카메라 화질 최적화 미흡**  
  - 실시간 영상의 품질 향상을 위한 파라미터 튜닝 부족  
  - 초기 설정값으로만 운영됨  


---

## **4. 향후 개선 방안**

- **템플릿 프로젝트 구성 및 공유**  
  - vcpkg 포함 환경 자동화 템플릿 프로젝트 제작 및 배포 예정  

- **알고리즘 고도화**  
  - 다양한 환경 대응을 위한 알고리즘 선택 기준 정립  
  - 정확도 향상을 위한 CNN 기반 매칭 알고리즘 실험 예정  

- **저장 및 로그 처리 개선**  
  - 매칭 결과 이미지의 크기 분석을 기반으로 저장 방식 개선  
  - 로그 기록 클래스 생성 → 운영 이력 관리 체계화  

- **카메라 품질 개선**  
  - Gain, Exposure Time, Frame Rate 등 고급 설정을 실시간 반영 가능하도록 구현  

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
