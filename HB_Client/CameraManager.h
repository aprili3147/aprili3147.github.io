#pragma once

#include <pylon/PylonIncludes.h>

using namespace Pylon;
using namespace GenApi;
using namespace cv;

class CameraManager
{
public:
    CameraManager();
    ~CameraManager();

    bool Open();
    void Close();
    bool IsOpened() const;
    bool GetFrame(cv::Mat& output);
    bool m_bCameraStreaming = false;
    //카메라 설정
    bool SetExposure(double exposureTime); // 단위: 마이크로초
    bool SetGain(double gain);             // 단위: dB

private:
    CInstantCamera m_camera;
    CImageFormatConverter m_converter;
    CPylonImage m_pylonImage;
    bool m_isOpened;

    double m_exposureTime = 10000.0;
    double m_gain = 0.0;
};
