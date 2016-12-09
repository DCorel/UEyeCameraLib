#include "ueyecamera.h"

UEyeCamera::UEyeCamera()
{
}

/*Get and Set functions*/
void UEyeCamera::set_CameraHandle(int CameraHandle)
{
    m_CameraHandle = CameraHandle;
}

void UEyeCamera::get_CameraHandle(int &CameraHandle)
{
    CameraHandle = m_CameraHandle;
}

char* UEyeCamera::get_ImageMemoryPointer()
{
    //char ImageMemoryPointer = m_ImageData;
    return (m_ImageData);
}

void UEyeCamera::get_ImageMemoryID(int &MemoryID)
{
    MemoryID = m_MEM_ID;
}

void UEyeCamera::get_CameraParameters(CameraParameters &camParameters)
{
    camParameters.ExposureTime = m_CamParam.ExposureTime;
    camParameters.HardwareGain = m_CamParam.HardwareGain;
}


void UEyeCamera::GetCameraID(string &expected_camera_serial,bool &result,int &cameraID)
{
    result = false;
    INT pnNumCams;
    if(is_GetNumberOfCameras (&pnNumCams) != IS_SUCCESS)
    {
        UEyeCameraException e(CONNECT_ERROR);
        throw e;
    }

    m_CameraList = (UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + pnNumCams * sizeof (UEYE_CAMERA_INFO)];
    m_CameraList->dwCount = pnNumCams;

    //Retrieve camera info
    if (is_GetCameraList(m_CameraList) != IS_SUCCESS)
    {
        UEyeCameraException e(CONNECT_ERROR);
        throw e;
    }

    string test = string(m_CameraList[0].uci[0].SerNo).c_str();

    for(int i=0;i<pnNumCams;i++)
    {
        //if(strcmp(expected_camera_serial.c_str(), string(m_CameraList[i].uci[0].SerNo).c_str())!=0)
        if(expected_camera_serial.c_str() == test)
        {
            cameraID = m_CameraList[i].uci[0].dwCameraID;
            result = true;
        }
    }
}

/*Above function GetCameraID makes this function obsolete*/
void UEyeCamera::GetAvailableCameraIdentifiers(vector<UEYE_CAMERA_INFO> &camera_identifier_list)
{

    camera_identifier_list.clear();
    INT pnNumCams;
    if(is_GetNumberOfCameras (&pnNumCams) != IS_SUCCESS)
    {
        UEyeCameraException e(CONNECT_ERROR);
        throw e;
    }

    m_CameraList = (UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + pnNumCams * sizeof (UEYE_CAMERA_INFO)];
    m_CameraList->dwCount = pnNumCams;

    //Retrieve camera info
    if (is_GetCameraList(m_CameraList) != IS_SUCCESS)
    {
        UEyeCameraException e(CONNECT_ERROR);
        throw e;
    }

    UEYE_CAMERA_INFO element;
    for(int i=0;i<pnNumCams;i++)
    {
        element = m_CameraList[i].uci[0];
        camera_identifier_list.push_back(element);
    }
}
/********************************************************************************************************************/

void UEyeCamera::Connect()
{

    if(is_InitCamera(&m_CameraHandle, NULL) != IS_SUCCESS)
    {
        UEyeCameraException e(CONNECT_ERROR);
        throw e;
    }

}

void UEyeCamera::Disconnect()
{

    if(is_ExitCamera(m_CameraHandle) != IS_SUCCESS)
    {
        UEyeCameraException e(DISCONNECT_ERROR);
        throw e;
    }

}

void UEyeCamera::SetParameters(CAM_REGISTER cam_reg, CAM_VALUE value)
{
    if(cam_reg == CAM_GAIN)
    {
        if(value.i_val < 0 || value.i_val > 100){
            throw "Not a valid gain value!";
        }else
        {
            if(is_SetHardwareGain (m_CameraHandle, value.i_val, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER) != IS_SUCCESS)
            {
                UEyeCameraException e(SET_PARAMETERS_ERROR);
                throw e;
            }
        }
    }
    if(cam_reg == CAM_EXPOSURE)
    {
        double *time_ms = &value.d_value;
        if(is_Exposure(m_CameraHandle, IS_EXPOSURE_CMD_SET_EXPOSURE, time_ms, sizeof(double)) != IS_SUCCESS)
        {
            UEyeCameraException e(SET_PARAMETERS_ERROR);
            throw e;
        }
    }
}

void UEyeCamera::GetParameters(CAM_REGISTER cam_reg)
{
    if(cam_reg == CAM_GAIN)
    {
        m_CamParam.HardwareGain = is_SetHardwareGain (m_CameraHandle, IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    }
    if(cam_reg == CAM_EXPOSURE)
    {
        if(is_Exposure(m_CameraHandle, IS_EXPOSURE_CMD_GET_EXPOSURE, &m_CamParam.ExposureTime, sizeof(double)) != IS_SUCCESS)
        {
            UEyeCameraException e(GET_PARAMETERS_ERROR);
            throw e;
        }
    }

}

void UEyeCamera::AllocateMemory()
{
    SENSORINFO pInfo;


    if(is_GetSensorInfo (m_CameraHandle, &pInfo) != IS_SUCCESS)
    {
        UEyeCameraException e(GET_SENSOR_INFO);
        throw e;
    }

    if(is_AllocImageMem (m_CameraHandle, pInfo.nMaxWidth, pInfo.nMaxHeight, 8, &m_ImageData, &m_MEM_ID) != IS_SUCCESS)
    {
        UEyeCameraException e(ALLOCATE_MEMORY_ERROR);
        throw e;
    }

    if(is_SetImageMem (m_CameraHandle, m_ImageData, m_MEM_ID) != IS_SUCCESS)
    {
        UEyeCameraException e(ALLOCATE_MEMORY_ERROR);
        throw e;
    }
}

void UEyeCamera::get_ImageParam(SENSORINFO& sensorInfo)
{
    if(is_GetSensorInfo (m_CameraHandle, &sensorInfo) != IS_SUCCESS)
    {
        UEyeCameraException e(GET_SENSOR_INFO);
        throw e;
    }
}


void UEyeCamera::SaveImage(string FILEPATH, string FILENAME, string ImageFormat, int ImageQuality)
{
    IMAGE_FILE_PARAMS ImageFileParams;

    if(ImageFormat == "JPEG")
    {
        ImageFileParams.nFileType = IS_IMG_JPG;
    }else if(ImageFormat == "PNG")
    {
        ImageFileParams.nFileType = IS_IMG_PNG;
    }else if(ImageFormat == "BMP")
    {
        ImageFileParams.nFileType = IS_IMG_BMP;
    }else
    {
        UEyeCameraException e(IMAGE_FORMAT_INPUT_ERROR);
        throw e;
    }

    string FilePath = FILEPATH + FILENAME + ImageFormat;
    std::wstring widestr = std::wstring(FilePath.begin(), FilePath.end());
    wchar_t* pwchFileName = const_cast<wchar_t*>(widestr.c_str());

    UINT ImageID = UINT(m_MEM_ID);
    ImageFileParams.pwchFileName = pwchFileName;
    ImageFileParams.pnImageID = &ImageID;
    ImageFileParams.ppcImageMem = &m_ImageData;
    ImageFileParams.nQuality = ImageQuality;


    if(is_ImageFile(m_CameraHandle, IS_IMAGE_FILE_CMD_SAVE, &ImageFileParams, sizeof(ImageFileParams)) != IS_SUCCESS)
    {
        UEyeCameraException e(SAVE_IMAGE_ERROR);
        throw e;
    }
}

void UEyeCamera::SetLUT(unsigned char *userDefinedLUT, int size)
{
    IS_LUT_STATE lutState;
    bool Set_Enable = false;

    if(is_LUT(m_CameraHandle, IS_LUT_CMD_GET_STATE, (void*) &lutState, sizeof(lutState)) != IS_SUCCESS)
    {
        UEyeCameraException e(SET_LUT_ERROR);
        throw e;
    }

    if(lutState.bLUTEnabled == false)
    {
        IS_LUT_ENABLED_STATE nLutEnabled = IS_LUT_ENABLED;
        if(is_LUT(m_CameraHandle, IS_LUT_CMD_SET_ENABLED, (void*) &nLutEnabled, sizeof(nLutEnabled)) != IS_SUCCESS)
        {
            UEyeCameraException e(SET_LUT_ERROR);
            throw e;
        }else
        {
            Set_Enable = true;
        }
    }else if (lutState.nLUTStateID == IS_LUT_STATE_ID_NOT_SUPPORTED)
    {
        UEyeCameraException e(SET_LUT_ERROR);
        throw e;
    }else
    {
        Set_Enable = true;
    }

    if(Set_Enable == true)
    {
        const int size_out = 64;

        IS_LUT_CONFIGURATION_64 LUT;
        LUT.bAllChannelsAreEqual = true;

        int step = (size/(float)(size_out)+0.5);

        //check step > 0
        int j = 0;
        //int k = 0;
        for(int i=0;i<size_out;i++)
        {

            LUT.dblValues[0][i] = (1.0/255.0) * double (userDefinedLUT[j]);
            j+=step;
            //k += step;
        }

        if(is_LUT(m_CameraHandle, IS_LUT_CMD_SET_USER_LUT, (void*) &LUT, sizeof(LUT)) != IS_SUCCESS)
        {
            UEyeCameraException e(SET_LUT_ERROR);
            throw e;
        }

        wchar_t* pFilename2 = L"/home/stagiairpc/Desktop/lutFile2.xml";
        if(is_LUT(m_CameraHandle, IS_LUT_CMD_SAVE_FILE, (void*) pFilename2 , NULL) != IS_SUCCESS)
        {
            UEyeCameraException e(SET_LUT_ERROR);
            throw e;
        }
    }
}

void UEyeCamera::GetLUT(){

    if(is_LUT(m_CameraHandle, IS_LUT_CMD_GET_USER_LUT, (void*) &m_LUT, sizeof(m_LUT)) != IS_SUCCESS)
    {
        UEyeCameraException e(GET_LUT_ERROR);
        throw e;
    }

}

void UEyeCamera::GetConnectedCameras()
{
    INT pnNumCams;
    if(is_GetNumberOfCameras (&pnNumCams) != IS_SUCCESS)
    {
        throw "Getting the No of cameras failed";
    }else
    {
        if( pnNumCams >= 1 )
        {
          // Create new list with suitable size
          m_CameraList = (UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + pnNumCams * sizeof (UEYE_CAMERA_INFO)];
          m_CameraList->dwCount = pnNumCams;

          //Retrieve camera info
          if (is_GetCameraList(m_CameraList) != IS_SUCCESS)
          {
              UEyeCameraException e(CONNECT_ERROR);
              throw e;
          }
        }else
        {
            UEyeCameraException e(CONNECT_ERROR);
            throw e;
        }
    }
}

void UEyeCamera::ImageCapture()
{
    if(is_SetExternalTrigger(m_CameraHandle, IS_GET_EXTERNALTRIGGER) == IS_SET_TRIGGER_SOFTWARE)
    {
        UINT nMode = IO_FLASH_MODE_TRIGGER_HI_ACTIVE;
        if(is_IO(m_CameraHandle, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode)) != IS_SUCCESS)
        {
            UEyeCameraException e(CONNECT_ERROR);
            throw e;
        }
        if(is_FreezeVideo(m_CameraHandle, IS_WAIT) != IS_SUCCESS)
        {
            UEyeCameraException e(CONNECT_ERROR);
            throw e;
        }
    }else
    {

        UEyeCameraException e(IMAGE_CAPTURE_ERROR);
        throw e;

    }
}

void UEyeCamera::ContinuousImageCapture()
{
    if(is_CaptureVideo(m_CameraHandle, IS_DONT_WAIT) != IS_SUCCESS)
    {
        UEyeCameraException e(IMAGE_CAPTURE_ERROR);
        throw e;
    }
}

void UEyeCamera::SetTriggerMode(bool ExternalTriggerMode)
{
    if(ExternalTriggerMode == true)
    {
        is_SetExternalTrigger(m_CameraHandle, IS_SET_TRIGGER_LO_HI); //Sets the hardware trigger on the rising edge
    }else
    {
        is_SetExternalTrigger(m_CameraHandle, IS_SET_TRIGGER_SOFTWARE); //Sets the software trigger to be used. Image will be captured on ImageCapture();
    }
}

void UEyeCamera::GetUEyeCameraTriggerInputStatus(int &TriggerStatus)
{
    TriggerStatus = is_SetExternalTrigger(m_CameraHandle, IS_GET_TRIGGER_STATUS);
}

void UEyeCamera::GetUEyeCameraAmountOfTriggers(int &ammountOfTriggers)
{
    if(is_SetTriggerCounter (m_CameraHandle, ammountOfTriggers) != IS_SUCCESS)
    {
        UEyeCameraException e(GET_TRIGGER_AMMOUNT_ERROR);
        throw e;
    }
}

void UEyeCamera::EnableEvent(INT EventID)
{
    if(is_EnableEvent ( m_CameraHandle, EventID) != IS_SUCCESS)
    {
        UEyeCameraException e(ENABLE_EVENT_ERROR);
        throw e;
    }
}

//Function to for an event to trigger before moving on

void UEyeCamera::WaitOnEvent(INT EventID, INT TimeOut)
{
    if(TimeOut == -1)
    {
        if(is_WaitEvent (m_CameraHandle, EventID, INFINITE) != IS_SUCCESS)
        {
            UEyeCameraException e(ENABLE_EVENT_ERROR);
            throw e;
        }
    }else
    {
        if(is_WaitEvent (m_CameraHandle, EventID, TimeOut) != IS_SUCCESS)
        {
            UEyeCameraException e(ENABLE_EVENT_ERROR);
            throw e;
        }
    }

}

void UEyeCamera::AddBufferToSequence()
{
    if(is_AddToSequence (m_CameraHandle, m_ImageData, m_MEM_ID) != IS_SUCCESS)
    {
        UEyeCameraException e(ADD_BUFFER_ERROR);
        throw e;
    }
}
