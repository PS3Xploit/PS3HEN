#ifndef __LV2_USB_H__
#define __LV2_USB_H__

#include <lv2/lv2.h>

#define USB_REQTYPE_DIR_BITS                    0x80
#define USB_REQTYPE_DIR_TO_DEVICE               0x00
#define USB_REQTYPE_DIR_TO_HOST                 0x80
#define USB_REQTYPE_TYPE_BITS                   0x60
#define USB_REQTYPE_TYPE_STANDARD               0x00
#define USB_REQTYPE_TYPE_CLASS                  0x20
#define USB_REQTYPE_TYPE_VENDOR                 0x40
#define USB_REQTYPE_TYPE_RESERVED               0x60
#define USB_REQTYPE_RECIP_BITS                  0x1f
#define USB_REQTYPE_RECIP_DEVICE                0x00
#define USB_REQTYPE_RECIP_INTERFACE             0x01
#define USB_REQTYPE_RECIP_ENDPOINT              0x02
#define USB_REQTYPE_RECIP_OTHER                 0x03

#define USB_REQUEST_GET_STATUS                  0x00
#define USB_REQUEST_CLEAR_FEATURE               0x01
#define USB_REQUEST_SET_FEATURE                 0x03
#define USB_REQUEST_SET_ADDRESS                 0x05
#define USB_REQUEST_GET_DESCRIPTOR              0x06
#define USB_REQUEST_SET_DESCRIPTOR              0x07
#define USB_REQUEST_GET_CONFIGURATION           0x08
#define USB_REQUEST_SET_CONFIGURATION           0x09
#define USB_REQUEST_GET_INTERFACE               0x0a
#define USB_REQUEST_SET_INTERFACE               0x0b
#define USB_REQUEST_SYNCH_FRAME                 0x0c

#define USB_DESCRIPTOR_TYPE_DEVICE              0x01
#define USB_DESCRIPTOR_TYPE_CONFIGURATION       0x02
#define USB_DESCRIPTOR_TYPE_STRING              0x03
#define USB_DESCRIPTOR_TYPE_INTERFACE           0x04
#define USB_DESCRIPTOR_TYPE_ENDPOINT            0x05
#define USB_DESCRIPTOR_TYPE_HID                 0x21
#define USB_DESCRIPTOR_TYPE_REPORT              0x22

#define USB_FEATURE_ENDPOINT_HALT               0x00
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP        0x01

#define USB_CLASS_PER_INTERFACE                 0x00
#define USB_CLASS_AUDIO                         0x01
#define USB_CLASS_COMMUNICATIONS                0x02
#define USB_CLASS_HID                           0x03
#define USB_CLASS_MONITOR                       0x04
#define USB_CLASS_PHYSICAL                      0x05
#define USB_CLASS_POWER                         0x06
#define USB_CLASS_PRINTER                       0x07
#define USB_CLASS_STORAGE                       0x08
#define USB_CLASS_HUB                           0x09
#define USB_CLASS_DATA                          0x0a
#define USB_CLASS_VENDOR_SPECIFIC               0xff

typedef struct CellUsbdLddOps 
{
	const char *name;
	int (*probe)(int dev_id);
	int (*attach)(int dev_id);
	int (*detach)(int dev_id);
} CellUsbdLddOps;

typedef struct UsbEndpointDescriptor 
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
} UsbEndpointDescriptor;

typedef struct UsbDeviceRequest 
{
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} UsbDeviceRequest;

typedef void (*CellUsbdDoneCallback)(int result, int count, void *arg);

LV2_EXPORT int cellUsbdRegisterLdd(CellUsbdLddOps *lddops);
LV2_EXPORT int cellUsbdUnregisterLdd(CellUsbdLddOps *lddops);
LV2_EXPORT void *cellUsbdScanStaticDescriptor(int dev_id, void *ptr, unsigned char type);
LV2_EXPORT int cellUsbdOpenPipe(int dev_id, UsbEndpointDescriptor *ed);
LV2_EXPORT int cellUsbdClosePipe(int pipe_id);
LV2_EXPORT int cellUsbdControlTransfer(int pipe_id, UsbDeviceRequest *dr, void *buf, CellUsbdDoneCallback done_cb, void *arg);
LV2_EXPORT int cellUsbdBulkTransfer(int pipe_id, void *buf, int len, CellUsbdDoneCallback done_cb, void *arg);


#endif /* __LV2_USB_H__ */

