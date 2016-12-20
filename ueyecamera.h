#pragma once;

#include "ueyecamera_global.h"
#include <ICamera.h>
#include "uEye.h"
#include <ueye_deprecated.h>
#include <cmath>
#include <vector>
#include <QList>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <qlist.h>


using namespace std;

enum UEyeException
{
    CONNECT_ERROR=0x00,
    DISCONNECT_ERROR=0x01,
    SET_PARAMETERS_ERROR=0x02,
    GET_PARAMETERS_ERROR=0x03,
    SET_LUT_ERROR=0x04,
    GET_LUT_ERROR=0x05,
    CAPTURE_IMAGE_ERROR=0x06,
    SET_EXTERNAL_TRIGGER_ERROR=0x07,
    ALLOCATE_MEMORY_ERROR=0x08,
    SAVE_IMAGE_ERROR=0x09,
    IMAGE_FORMAT_INPUT_ERROR=0x10,
    GET_TRIGGER_AMMOUNT_ERROR=0x11,
    IMAGE_CAPTURE_ERROR=0x12,
    ENABLE_EVENT_ERROR=0x13,
    WAIT_FOR_EVENT_ERROR=0x14,
    ADD_BUFFER_ERROR=0x15,
    GET_SENSOR_INFO=0x16,
    FREE_IMAGE_MEM_ERROR=0x17
};

class UEyeCameraException: public exception
{
public:

    UEyeCameraException(UEyeException Exception) throw() {

        m_ExceptionMessage = Exception;
    }
    ~UEyeCameraException() throw() {}

  int what() throw() // my call to the std exception class function (doesn't nessasarily have to be virtual).
  {
    return m_ExceptionMessage;
  }
  const char* ConvertError(UEyeCameraException i)
  {
      stringstream s;
      s << i.what();
      string str = "Error! : UEyeException " + s.str();
      const char* ch = str.c_str();
      return (ch);
  }

private:
    UEyeException m_ExceptionMessage;
};


class UEYECAMERASHARED_EXPORT UEyeCamera: public ICamera
{
public:
    /*Constructor(s)/Destructor(s)*/
    UEyeCamera();
    ~UEyeCamera();
public:    
    /*ICamera interface functions*/
    void Connect();
    void Disconnect();
    void UpdateParameters();
    void SetParameters();
    void SetLUT(unsigned char *userDefinedLUT, int size);
    void UpdateLUT();
    void GetConnectedCameras();
    void SingleImageCapture();
    void SequentialImageCapture();
    void SetTriggerMode(bool ExternalTriggerMode);
    void AllocateMemory();
    void ReleaseMemory();
    void AddBufferToSequence();

public:
    /*Get/Set Member vars functions*/
    IplImage * GetIplImage();
    char *GetImageMemoryPointer();
    void SetCameraHandle();
    void GetCameraHandle(int& CameraHandle);
    void GetImageParam(SENSORINFO& sensorInfo);
    void GetCameraParameters(CameraParameters &camParameters);
    void SetCameraParameters(int gain, double exposure);
    void GetImageMemoryID(int &MemoryID);
    void SetIplImage(int width, int height);
    void SetCameraBuffer(int BitsPerPixel);
    void GetAvailableCameraIdentifiers(vector<UEYE_CAMERA_INFO>& camera_identifier_list);
    void GetCameraID(string &expected_camera_serial, bool &result);

public:
    /*UEye-only functions*/
    void GetUEyeCameraAmountOfTriggers(int &ammountOfTriggers);
    void GetUEyeCameraTriggerInputStatus(int &TriggerStatus);
    void EnableEvent(INT EventID);
    void WaitOnEvent(INT EventID, INT TimeOut);
    void ConvertImageFromBufferToIplImage();
    void SaveIplImageList(int ListSize, QList<IplImage *> ImageList, string SavePath);
    void SaveImage(string FILEPATH, string ImageFormat, int ImageQuality);

public:
/*@DC: not reviewed/tested properly yet*/
    void ThrowException();
    void SetFrameRateCamera(double framerate);

private:
    INT m_MEM_ID;
    HIDS m_CameraHandle;
    CAM_VALUE m_CameraParameters;
    UEYE_CAMERA_LIST* m_CameraList;
    IS_LUT_CONFIGURATION_64 m_LUT;

    IplImage * m_img;
    CameraParameters m_CamParam;

    int m_CameraID;
    int m_img_width;
    int m_img_height;
    int m_img_bpp;
    char* m_ImageData;
    double m_ExposureTime;


};

