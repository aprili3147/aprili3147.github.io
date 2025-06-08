#pragma once

#include <windows.h>
#include <string>


HANDLE OpenSerialPort(const std::wstring& portName = L"COM3");
void SetBrightness(HANDLE hSerial, int brightness);
bool GetCurrentBrightness(HANDLE hSerial, int& outBrightness);
//Mat CaptureCameraImage(VideoCapture& cap);
Mat ComputeHistogram(const Mat& gray);
bool GetCurrentBrightness(HANDLE hSerial, int& outBrightness);
int FindBestBrightness(
    HANDLE hSerial,
    const Mat& templateHist,
    std::function<bool(Mat&)> getFrameFunc,
    std::function<void(const std::wstring&)> logFunc = nullptr
);