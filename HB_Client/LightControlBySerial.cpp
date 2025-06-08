#include "pch.h"
#include <opencv2/opencv.hpp>    
#include <windows.h>             
#include <iostream>     


using namespace std;
using namespace cv;

// 시리얼 포트를 열고 핸들을 반환하는 함수
HANDLE OpenSerialPort(const wstring& portName = L"COM3") {
    // COM3 포트를 열기. GENERIC_WRITE는 쓰기 전용
    HANDLE hSerial = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        DWORD errCode = GetLastError();

        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        wcerr << L"serial error code: " << errCode << L", message: " << (LPCWSTR)lpMsgBuf << endl;
        LocalFree(lpMsgBuf);

        return NULL;
    }

    // 포트 속성 설정
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb); // 현재 설정 불러오기
    dcb.BaudRate = CBR_9600;     // 보레이트 설정
    dcb.ByteSize = 8;            // 데이터 비트
    dcb.StopBits = ONESTOPBIT;  // 정지 비트
    dcb.Parity = NOPARITY;      // 패리티 없음
    SetCommState(hSerial, &dcb); // 설정 반영

    return hSerial;
}

// 조명 밝기 명령을 전송하는 함수 (3바이트 명령)
void SetBrightness(HANDLE hSerial, int brightness) {
    if (!hSerial) return;

    BYTE cmd[3] = { 0x44, 0x31, static_cast<BYTE>(brightness) }; // 명령 형식: 0x44 0x31 밝기값
    DWORD written;
    WriteFile(hSerial, cmd, 3, &written, NULL); // Write to serial
    Sleep(300); // 조명 장치가 밝기 변경을 반영할 시간 대기
}

// 입력 영상의 히스토그램을 계산해서 반환
Mat ComputeHistogram(const Mat& gray) {
    Mat hist;
    int histSize = 256;                      // 밝기값 구간 0~255
    float range[] = { 0, 256 };              // 범위 설정
    const float* histRange = { range };
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange); // 히스토그램 계산
    normalize(hist, hist, 0, 1, NORM_MINMAX); // 정규화 (0~1 사이로)
    return hist;
}


bool GetCurrentBrightness(HANDLE hSerial, int& outBrightness) {
    if (!hSerial) return false;

    // 요청 명령: 0x52 '1' 'D' = R 1 D
    BYTE cmd[3] = { 0x52, 0x31, 0x44 };
    DWORD written = 0;
    WriteFile(hSerial, cmd, 3, &written, NULL);

    // 응답 받기 (3바이트)
    BYTE response[3] = { 0 };
    DWORD bytesRead = 0;
    if (!ReadFile(hSerial, response, 3, &bytesRead, NULL) || bytesRead != 3) {
        cerr << "밝기 응답 수신 실패" << endl;
        return false;
    }

    if (response[0] == 0x52 && response[1] == 0x31) {
        outBrightness = response[2]; // 밝기 값
        return true;
    }

    cerr << "잘못된 응답 수신됨" << endl;
    return false;
}
int FindBestBrightness(
    HANDLE hSerial,
    const Mat& templateHist,
    std::function<bool(Mat&)> getFrameFunc,
    std::function<void(const std::wstring&)> logFunc = nullptr
) {
    double maxSimilarity = -1.0;
    int bestBrightness = 0;
    const double dropThreshold = 0.1;

    for (int br = 0; br <= 255; ++br) {
        SetBrightness(hSerial, br);
        Sleep(10);

        if (logFunc) logFunc(L"currunt brightness: " + std::to_wstring(br));

        Mat frame;
        if (!getFrameFunc(frame) || frame.empty()) continue;

        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Mat hist = ComputeHistogram(gray);
        double sim = compareHist(templateHist, hist, HISTCMP_CORREL);

        if (logFunc) logFunc(L"similarity: " + std::to_wstring(sim));

        if (maxSimilarity > 0 && sim < maxSimilarity - dropThreshold) {
            break;
        }

        if (sim > maxSimilarity) {
            maxSimilarity = sim;
            bestBrightness = br;
            if (logFunc) logFunc(L"best brightness: " + std::to_wstring(bestBrightness));
        }
    }

    return bestBrightness;
}
