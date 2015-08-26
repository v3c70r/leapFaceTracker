#include <frameReader.h>

/*
 * Read frame from file
 */
Frame FrameReader::readFrame()
{
    Frame output(FRAME_WIDTH, FRAME_HEIGHT);
    int usb_frame_size;
    int dataIdx=0;

    while(true)
    {
        fread(&usb_frame_size, sizeof(usb_frame_size), 1, f);
        fread(usbBuf, usb_frame_size, 1, f);

        //process usb frame
        //get frame info
        int bHeaderLen = usbBuf[0];
        int bmHeaderInfo = usbBuf[1];

        if (feof(f))
            break;

        for (int i = bHeaderLen; i < usb_frame_size ; i += 2) 
        {
            if (dataIdx >= FRAME_SIZE)
                break ;
            CvScalar l;
            CvScalar r;

            l.val[2] = usbBuf[i];
            l.val[1] = usbBuf[i];
            l.val[0] = usbBuf[i];

            r.val[2] = usbBuf[i+1];
            r.val[1] = usbBuf[i+1];
            r.val[0] = usbBuf[i+1];

            int x = dataIdx % FRAME_WIDTH;
            int y = dataIdx / FRAME_WIDTH;
            //std::cout<<y<<"\t"<<dataIdx<<std::endl;
            cvSet2D((output.leftFrame), 2 * y,  x, l);       //left frame
            cvSet2D((output.leftFrame), 2 * y+1,  x, l);       //left frame
            cvSet2D((output.rightFrame), 2 * y, x, r);       //right frame
            cvSet2D((output.rightFrame), 2 * y+1, x, r);       //right frame
            dataIdx++;
        }
        if (bmHeaderInfo & (1<<1)){
            break;
        }
    }
    return output;
}

Frame FrameReader::readFrameSDK()
{
    if (ctrl.isConnected())
    {
        Leap::Frame frame = ctrl.frame();
    }
}


