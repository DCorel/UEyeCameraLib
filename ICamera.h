#ifndef ICAMERA_H
#define ICAMERA_H
#include <string>
#include <vector>

using namespace std;

enum CAM_REGISTER
{
    CAM_GAIN=0x00,
    CAM_EXPOSURE=0x01
};

enum CAM_TYPE
{
    CAM_FLOAT, CAM_INT, CAM_BOOL
};

struct CAM_VALUE
{
    CAM_TYPE value;
    double d_value;
    int i_val;
    bool b_val;

};

struct CameraParameters
{
    double ExposureTime;
    int HardwareGain;
};


class ICamera
{
public:

    virtual void Connect()=0;
    virtual void Disconnect()=0;
    virtual void SetParameters()=0;//CAM_REGISTER reg, CAM_VALUE value)=0;
    virtual void SetLUT(unsigned char* UserDefinedLUT, int size)=0;
    virtual void GetLUT()=0;
    virtual void GetConnectedCameras()=0;
    virtual void ImageCapture()=0;
    virtual void GetParameters()=0;
    virtual void AddBufferToSequence()=0;
    virtual void SaveImage(string FILEPATH, string FILENAME, string ImageFormat, int ImageQuality)=0;
    virtual void AllocateMemory()=0;
    virtual void ReleaseMemory()=0;
    virtual void SetTriggerMode(bool ExternalTriggerMode)=0;
private:
    //virtual void LoadSettings(string xmlFilename)=0;
    //virtual void WriteSettings(string xmlFileName)=0;

};

#endif // ICAMERA_H
