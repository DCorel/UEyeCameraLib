#pragma once;

#include "ueyecamera_global.h"
#include <ICamera.h>
#include "uEye.h"
#include <cmath>
#include <vector>
#include <QList>
#include <string>
#include <iostream>
#include <sstream>


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
    GET_SENSOR_INFO=0x16
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
    UEyeCamera();
public:
    void Connect();
    void Disconnect();
    void SetParameters(CAM_REGISTER register, CAM_VALUE value);
    void GetParameters(CAM_REGISTER register);
    void SetLUT(unsigned char *userDefinedLUT, int size);
    void GetLUT();
    void GetConnectedCameras();
    void ImageCapture();
    void SetTriggerMode(bool ExternalTriggerMode);
    void AllocateMemory();
    void AddBufferToSequence();
    void SaveImage(string FILEPATH, string FILENAME, string ImageFormat, int ImageQuality);

    /*Get/set functions*/
    void set_CameraHandle(int CameraHandle);
    void get_CameraHandle(int& CameraHandle);
    void get_ImageParam(SENSORINFO& sensorInfo);


public:
/*@DC: not reviewed yet*/
    void GetAvailableCameraIdentifiers(vector<UEYE_CAMERA_INFO>& camera_identifier_list);
    void GetCameraID(string &expected_camera_serial,bool &result,int &cameraID);
    void ThrowException();
    void GetUEyeCameraAmountOfTriggers(int &ammountOfTriggers);
    void GetUEyeCameraTriggerInputStatus(int &TriggerStatus);
    void EnableEvent(INT EventID);
    void WaitOnEvent(INT EventID, INT TimeOut);
    void ContinuousImageCapture();

private:
    HIDS m_CameraHandle;
    char* m_ImageData;
    INT m_MEM_ID;
    IS_LUT_CONFIGURATION_64 m_LUT;
    UEYE_CAMERA_LIST* m_CameraList;
    CameraParameters CamParam;
    //CAM_REGISTER cam_reg;


};
