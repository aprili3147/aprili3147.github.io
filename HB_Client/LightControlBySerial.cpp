#include "pch.h"
#include <opencv2/opencv.hpp>    
#include <windows.h>             
#include <iostream>     


using namespace std;
using namespace cv;

// �ø��� ��Ʈ�� ���� �ڵ��� ��ȯ�ϴ� �Լ�
HANDLE OpenSerialPort(const wstring& portName = L"COM3") {
    // COM3 ��Ʈ�� ����. GENERIC_WRITE�� ���� ����
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

    // ��Ʈ �Ӽ� ����
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb); // ���� ���� �ҷ�����
    dcb.BaudRate = CBR_9600;     // ������Ʈ ����
    dcb.ByteSize = 8;            // ������ ��Ʈ
    dcb.StopBits = ONESTOPBIT;  // ���� ��Ʈ
    dcb.Parity = NOPARITY;      // �и�Ƽ ����
    SetCommState(hSerial, &dcb); // ���� �ݿ�

    return hSerial;
}

// ���� ��� ����� �����ϴ� �Լ� (3����Ʈ ���)
void SetBrightness(HANDLE hSerial, int brightness) {
    if (!hSerial) return;

    BYTE cmd[3] = { 0x44, 0x31, static_cast<BYTE>(brightness) }; // ��� ����: 0x44 0x31 ��Ⱚ
    DWORD written;
    WriteFile(hSerial, cmd, 3, &written, NULL); // Write to serial
    Sleep(300); // ���� ��ġ�� ��� ������ �ݿ��� �ð� ���
}

// �Է� ������ ������׷��� ����ؼ� ��ȯ
Mat ComputeHistogram(const Mat& gray) {
    Mat hist;
    int histSize = 256;                      // ��Ⱚ ���� 0~255
    float range[] = { 0, 256 };              // ���� ����
    const float* histRange = { range };
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange); // ������׷� ���
    normalize(hist, hist, 0, 1, NORM_MINMAX); // ����ȭ (0~1 ���̷�)
    return hist;
}


bool GetCurrentBrightness(HANDLE hSerial, int& outBrightness) {
    if (!hSerial) return false;

    // ��û ���: 0x52 '1' 'D' = R 1 D
    BYTE cmd[3] = { 0x52, 0x31, 0x44 };
    DWORD written = 0;
    WriteFile(hSerial, cmd, 3, &written, NULL);

    // ���� �ޱ� (3����Ʈ)
    BYTE response[3] = { 0 };
    DWORD bytesRead = 0;
    if (!ReadFile(hSerial, response, 3, &bytesRead, NULL) || bytesRead != 3) {
        cerr << "��� ���� ���� ����" << endl;
        return false;
    }

    if (response[0] == 0x52 && response[1] == 0x31) {
        outBrightness = response[2]; // ��� ��
        return true;
    }

    cerr << "�߸��� ���� ���ŵ�" << endl;
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
