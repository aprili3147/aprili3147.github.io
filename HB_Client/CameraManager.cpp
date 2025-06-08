//CameraManager.cpp

#include "pch.h"
#include "CameraManager.h"

CameraManager::CameraManager() : m_isOpened(false)
{
    PylonInitialize();
}

CameraManager::~CameraManager()
{
    Close();
    PylonTerminate();
}

bool CameraManager::Open()
{
    try {
        CTlFactory& factory = CTlFactory::GetInstance();
        m_camera.Attach(factory.CreateFirstDevice());
        m_camera.Open();

        //카메라 파라미터 조정
        //SetExposure(m_exposureTime); 
        //SetGain(m_gain);             

        m_camera.StartGrabbing(GrabStrategy_LatestImageOnly);
        m_converter.OutputPixelFormat = PixelType_BGR8packed;
        m_converter.OutputBitAlignment = OutputBitAlignment_MsbAligned;
        m_isOpened = true;
        return true;
    }
    catch (const GenericException& e) {
        AfxMessageBox(CString("camera open failed: ") + e.GetDescription());
        return false;
    }
}

void CameraManager::Close()
{
    if (m_camera.IsGrabbing())
        m_camera.StopGrabbing();
    if (m_camera.IsOpen())
        m_camera.Close();
    m_isOpened = false;
}

bool CameraManager::IsOpened() const
{
    return m_isOpened;
}

bool CameraManager::GetFrame(Mat& output)
{
    if (!m_camera.IsGrabbing()) return false;

    try {
        CGrabResultPtr ptrGrabResult;
        m_camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
        if (ptrGrabResult->GrabSucceeded()) {
            m_converter.Convert(m_pylonImage, ptrGrabResult);
            output = Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (void*)m_pylonImage.GetBuffer()).clone();
            return true;
        }
    }
    catch (...) {}
    return false;
}

bool CameraManager::SetExposure(double exposure)
{
    try {
        if (m_camera.IsOpen()) {
            INodeMap& nodemap = m_camera.GetNodeMap();
            CFloatPtr exposureTime = nodemap.GetNode("ExposureTime");
            if (IsWritable(exposureTime)) {
                exposureTime->SetValue(exposure);
                m_exposureTime = exposure;
                return true;
            }
        }
    }
    catch (...) {}
    return false;
}

bool CameraManager::SetGain(double gain)
{
    try {
        if (m_camera.IsOpen()) {
            INodeMap& nodemap = m_camera.GetNodeMap();
            CFloatPtr gainNode = nodemap.GetNode("Gain");
            if (IsWritable(gainNode)) {
                gainNode->SetValue(gain);
                m_gain = gain;
                return true;
            }
        }
    }
    catch (...) {}
    return false;
}