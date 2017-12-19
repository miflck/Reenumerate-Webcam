#include "ofApp.h"

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>


#define USE_ASYNC_IO    //Comment this line out if you want to use
//synchronous calls for reads and writes
#define kTestMessage        "Bulk I/O Test"
#define k8051_USBCS         0x7f92
#define kOurVendorID        1351    //Vendor ID of the USB device
#define kOurProductID           8193    //Product ID of device BEFORE it
//is programmed (raw device)
#define kOurProductIDBulkTest   4098    //Product ID of device AFTER it is
//programmed (bulk test device)





//--------------------------------------------------------------
void ofApp::setup(){

    cout<<"Setup"<<endl;
    
    mach_port_t             masterPort;
    CFMutableDictionaryRef  matchingDict;
    CFRunLoopSourceRef      runLoopSource;
    kern_return_t           kr;
    SInt32                  usbVendor =0x046d;
    SInt32                  usbProduct = 0x0825;
    
    // to get vendorid and productid: type system_profiler SPUSBDataType in terminal
   // system_profiler SPUSBDataType
    

    //Create a master port for communication with the I/O Kit
    kr = IOMasterPort(MACH_PORT_NULL, &masterPort);
    if (kr || !masterPort)
    {
        cout<<"ERR: CouldnÕt create a master I/O Kit port(%08x)\n"<<endl;
        return -1;
    }
    //Set up matching dictionary for class IOUSBDevice and its subclasses
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (!matchingDict)
    {
        cout<<"CouldnÕt create a USB matching dictionary\n"<<endl;
        mach_port_deallocate(mach_task_self(), masterPort);
        return -1;
    }
    
    // Only get the Device you are looking for:
    //Add the vendor and product IDs to the matching dictionary.
    //This is the second key in the table of device-matching keys of the
    //USB Common Class Specification
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
        cout<<"fail"<<endl;
        return -1;// fail

    }
   
    
    kern_return_t               krr;
    io_service_t                usbDevice;
    IOCFPlugInInterface         **plugInInterface = NULL;
   // IOUSBDeviceInterface        **dev = NULL;
    IOUSBDeviceInterface187		**deviceInterface;
    HRESULT						res;
  //  HRESULT                     result;
    SInt32                      score;
    //UInt16                      vendor;
   // UInt16                      product;
   // UInt16                      release;
    //kern_return_t                     r;
    IOReturn					kkr;




    while (usbDevice = IOIteratorNext(iterator))
    {
        //Create an intermediate plug-in
        krr = IOCreatePlugInInterfaceForService(usbDevice,
                                               kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID,
                                               &plugInInterface, &score);
        //DonÕt need the device object after intermediate plug-in is created
        krr = IOObjectRelease(usbDevice);
        
        //Now create the device interface
     //   result = (*plugInInterface)->QueryInterface(plugInInterface,CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID),(LPVOID *)&dev);
        
        //DonÕt need the intermediate plug-in after device interface
        //is created
        (*plugInInterface)->Release(plugInInterface);
        
        //Now create the device interface
        res = (*plugInInterface)->QueryInterface(plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID187), (LPVOID *)&deviceInterface);
        // Now done with the plugin interface.
        (*plugInInterface)->Release(plugInInterface);
        
        kkr = (*deviceInterface)->USBDeviceOpen(deviceInterface);
        if(kkr == kIOReturnSuccess)
        {
            kkr = (*deviceInterface)->USBDeviceReEnumerate(deviceInterface, 0);
        }
    }
    
    /*
    io_service_t usbDevice;

    //Iterate for USB devices
    
    while (usbDevice = IOIteratorNext (iter))
        
    {
     
        io_name_t       deviceName;
        kr = IORegistryEntryGetName(usbDevice, deviceName);
        if (KERN_SUCCESS != kr) {
            deviceName[0] = '\0';
        }
        
        cout<<deviceName<<endl;
        printf("\ndeviceName:%s",deviceName);
        
        IOObjectRelease(usbDevice);
    }*/
    
    
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
