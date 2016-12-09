#include <ueyecamera.h>
#include <ueyecameraopencv.h>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    UEyeCamera Camera;
    UEyeCameraOpenCV OCV;

    string cameraSerialNumber = "4002718268";
    bool result = false;
    int cameraID;
    CAM_VALUE Parameters;
    SENSORINFO sensorInfo;
    int triggerCounter = 0;
    int triggerStatus;
    Parameters.d_value = 9.5;       //Setting the Exposure time [ms]
    Parameters.i_val = 50;          //Setting the Harware gain (Range: 0 - 100)
    bool initFailed = false;
    string fileName;
    string filePath = "/home/stagiairpc/Desktop/";
    wchar_t* = L"/home/stagiairpc/Desktop/";
    string imageFormat = "BMP"; //"BMP" or "PNG" or "JPEG"
    int imageQuality = 75;
    int ImageNr = 0;
    stringstream s;
    s << ImageNr;
    fileName = "ImageSobel" + s.str();

    try
    {
        Camera.GetCameraID(cameraSerialNumber, result, cameraID);
        if(result == true)
        {
            Camera.set_CameraHandle(cameraID);
            Camera.Connect();

            Camera.AllocateMemory();
            Camera.AddBufferToSequence();
            Camera.SetParameters(CAM_GAIN, Parameters);
            Camera.SetParameters(CAM_EXPOSURE, Parameters);
            Camera.SetTriggerMode(true);
            Camera.EnableEvent(IS_SET_EVENT_FRAME);
            Camera.ContinuousImageCapture();
        }else
        {
            cout << "Failed to connect.."<< endl;
        }

    }catch(UEyeCameraException i)
    {
        initFailed = true;
        const char* ch = i.ConvertError(i);
        cout << "Error! : " << ch << endl;
    }


    if(initFailed == false)
    {
        while(1)
        {
            //Main loop for checking of the triggers.

            Camera.WaitOnEvent(IS_SET_EVENT_FRAME, -1);
            cout << "Triggered" << endl;

            //Camera.SaveImage(filePath, fileName, imageFormat, imageQuality);



            OCV.SobelFilter();

            ImageNr++;
            s << ImageNr;
            fileName = "Image" + s.str();
        }
    }
}
