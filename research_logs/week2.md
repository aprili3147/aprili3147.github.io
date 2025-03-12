title: "2주차 연구일지"
date: 2025-03-16
categories: 개발일지

📌 2주차 연구일지

📅 기간: 2025년 3월 10일 ~ 3월 16일

✅ 진행한 작업

📌 기술 스택 확정

백엔드: Spring Boot (REST API, JPA, Spring Security)

프론트엔드: Vue.js

데이터베이스: MySQL

배포 환경: Docker, AWS (EC2, RDS, S3)

기타: OpenAI API, JWT 인증, Redis(캐싱)

🛠 개발 환경 세팅

Spring Boot 프로젝트 초기화 및 Gradle 설정

JPA 및 데이터베이스(MySQL) 연동

application.yml 설정 (DB 접속 정보, JWT 설정 등)

GitHub 레포지토리 생성 및 버전 관리 시작

🗂 데이터베이스 설계 및 구축

주요 테이블 설계 (users, questions, answers, likes, tags, question_tags)

MySQL에 테이블 생성 및 초기 데이터 삽입

ER 다이어그램 작성

🔍 문제 및 해결

🚨 Spring Boot & MySQL 연결 문제

HikariCP 커넥션 풀 오류 발생 → application.yml에서 spring.datasource.hikari.maximum-pool-size 설정 조정하여 해결

💡 ERD 설계 시 고민했던 점

질문과 답변에 대한 좋아요(Like) 관계 설정 → likes 테이블에서 question_id와 answer_id를 각각 NULLABLE로 두어 해결

📝 다음 주 계획 (3주차)

ERD 기반 JPA 엔티티 클래스 생성

회원가입 및 로그인 기능 구현 (JWT 인증 적용)

REST API 기본 구조 설계 및 컨트롤러 개발 시작

단위 테스트 진행 (회원 관련 API 검증)

💡 느낀 점

처음부터 ERD를 체계적으로 설계하는 것이 중요함을 깨달았다.

Spring Boot의 설정이 많아 익숙해지는 데 시간이 필요하지만, 점점 이해도가 올라가고 있다.

팀원들과 GitHub 협업을 원활하게 하기 위해 브랜치 전략을 잘 세워야겠다.

