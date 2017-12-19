#include "ofApp.h"

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>







//--------------------------------------------------------------
void ofApp::setup(){

    cout<<"REENUMERATE! "<<endl;
    mach_port_t             masterPort;
    CFMutableDictionaryRef  matchingDict;
    kern_return_t           kr;
    SInt32                  usbVendor =0x046d;
    SInt32                  usbProduct = 0x0825;
    
    io_service_t                usbDevice;
    IOCFPlugInInterface         **plugInInterface = NULL;
    IOUSBDeviceInterface187     **deviceInterface;
    HRESULT                     result;
    SInt32                      score;
    
    // to get vendorid and productid in termina: system_profiler SPUSBDataType
    
    //Create a master port for communication with the I/O Kit
    kr = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (kr || !masterPort)
    {
        return -1;
    }
    //Set up matching dictionary for class IOUSBDevice and its subclasses
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (!matchingDict)
    {
        mach_port_deallocate(mach_task_self(), masterPort);
        return -1;
    }
    // Only get the Device you are looking for:
    //Add the vendor and product IDs to the matching dictionary.
   CFDictionarySetValue(matchingDict, CFSTR(kUSBVendorName),
                         CFNumberCreate(kCFAllocatorDefault,
                                        kCFNumberSInt32Type, &usbVendor));
    CFDictionarySetValue(matchingDict, CFSTR(kUSBProductName),
                         CFNumberCreate(kCFAllocatorDefault,
                                        kCFNumberSInt32Type, &usbProduct));
    
    io_iterator_t iterator;
    /*Get an iterator.*/
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iterator);
    if (kr != KERN_SUCCESS)
    {
        return -1;// fail
    }
    

    while (usbDevice = IOIteratorNext(iterator))
    {
        io_name_t deviceName;
        IORegistryEntryGetName( usbDevice, deviceName );
        printf("\ndeviceName:%s",deviceName);
        
        //Create an intermediate plug-in
        kr = IOCreatePlugInInterfaceForService(usbDevice,
                                               kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID,
                                               &plugInInterface, &score);
        //DonÕt need the device object after intermediate plug-in is created
        kr = IOObjectRelease(usbDevice);
     
        if ((kIOReturnSuccess != kr) || !plugInInterface)
        {
            printf("Unable to create a plug-in (%08x)\n", kr);
            continue;
        }
        //Now create the device interface
        result = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID187), (LPVOID *)&deviceInterface);
        // Now done with the plugin interface.
        (*plugInInterface)->Release(plugInInterface);
        
      
        kr = (*deviceInterface)->USBDeviceOpen(deviceInterface);
        if(kr == kIOReturnSuccess)
        {
           kr = (*deviceInterface)->USBDeviceReEnumerate(deviceInterface, 0);
        }
        
        (*deviceInterface)->USBDeviceClose(deviceInterface);
        (*deviceInterface)->Release(deviceInterface);
    }
    IOObjectRelease(iterator);
}




//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
