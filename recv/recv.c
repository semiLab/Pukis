/*
 * Driver for USB receiver for camera...or an attempt thereof.
 *
 *
 * */
#include <usb.h>
#include <stdio.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>


#define VENDOR_ID 0x18ec
#define PRODUCT_ID 0x3280
#define INTERFACE 0

const static int req_EP1_len=64;
const static int req_EP2_len=1024;
const static int req_EP3_len=3072; //1024x3

const static int endpoint_EP1_in=0x81; /* endpoint 0x81 address for IN */
const static int endpoint_EP1_out=0x01; /* endpoint 1 address for OUT */
const static int endpoint_EP2_in=0x82; /* endpoint 0x81 address for IN */
const static int endpoint_EP3_in=0x83; /* endpoint 1 address for OUT */
const static int timeout=5000; /* timeout in ms */

#define NUM_ISO_PACKETS 8 
#define BUFFERSIZE NUM_ISO_PACKETS*3072
unsigned char buffer1[BUFFERSIZE];
//, buffer2[NUM_ISO_PACKETS*3072]; 


usb_dev_handle *find_dvb();



usb_dev_handle* setup_libusb_access() 
{
    usb_dev_handle *dvb;
    usb_set_debug(255);
    usb_init();
    usb_find_busses();
    usb_find_devices();


    if(!(dvb = find_dvb())) {
        printf("Couldn't find the USB device, Exiting\n");
        return NULL;
    }
    if (usb_set_configuration(dvb, 1) < 0) {
        printf("Could not set configuration 1\n");
        return NULL;
    }

    if (usb_claim_interface(dvb, INTERFACE) < 0) {
        printf("Could not claim interface\n");
        return NULL;
    }
    return dvb;
}

usb_dev_handle *find_dvb() 
{
    struct usb_bus *bus;
    struct usb_device *dev;
    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if (dev->descriptor.idVendor == VENDOR_ID && 
                    dev->descriptor.idProduct == PRODUCT_ID ) {
                usb_dev_handle *handle;
                printf("dvb with Vendor Id: %x and Product Id: %x found.\n", VENDOR_ID, PRODUCT_ID);
                if (!(handle = usb_open(dev))) {
                    printf("Could not open USB device\n");
                    return NULL;
                }
                return handle;
            }
        }
    }
    return NULL;
}

static void iso_transfer_callback(struct libusb_transfer *transfer)
{
    printf("Got data! Woohoo!\n");
    printf("%d %d %d\n", buffer1[0], buffer1[1], buffer1[2]);    
    printf("actual: %d\n", transfer->actual_length);    
    
    libusb_free_transfer(transfer);
}

void test_iso_transfer(libusb_device_handle *dev)
{
        struct libusb_transfer* usbTrans1;
        
        printf("Setting alt setting\n");
        if(libusb_set_interface_alt_setting(dev, 0, 1))
        {
            printf("wat\n");
        }
        printf("Alloc transfer\n");
        usbTrans1=libusb_alloc_transfer(NUM_ISO_PACKETS);
        //usbTrans2=libusb_alloc_transfer(NUM_ISO_PACKETS);
       printf("Fill transfer\n");
        printf("%d %d %d\n", buffer1[0], buffer1[1], buffer1[2]);    
        libusb_fill_iso_transfer(usbTrans1, dev, endpoint_EP3_in, buffer1, sizeof(buffer1), NUM_ISO_PACKETS, iso_transfer_callback, NULL, timeout);
        libusb_set_iso_packet_lengths(usbTrans1, 3072);
        printf("submit\n");
        libusb_submit_transfer(usbTrans1);
        printf("%d %d %d\n", buffer1[0], buffer1[1], buffer1[2]);    
        usleep(80000);
}

void test_bulk_transfer(usb_dev_handle *dev)
  {
    int r,i;
    unsigned char answer[req_EP3_len];
    while(1)
    {
        r = usb_bulk_read(dev, endpoint_EP3_in, answer, req_EP3_len, timeout);
        if( r != req_EP3_len )
        {
            printf("USB bulk read error, got %d\n", r);
            exit(-1);
        }
        for (i=0;i<req_EP3_len;i++) printf("%i, \n",answer[i]);
        //   usb_set_altinterface(dev, 0);
    }
    usb_release_interface(dev, 0);
 
  }

int main()
{
    //usb_dev_handle *dvb;
    libusb_device_handle *dvb;
    libusb_init(NULL);
    libusb_set_debug(NULL,3);
    dvb = libusb_open_device_with_vid_pid(NULL,VENDOR_ID, PRODUCT_ID);
    //libusb_set_configuration(dvb, USB_CONFIG); 
    libusb_claim_interface(dvb, 0);
    //if ((dvb = setup_libusb_access()) == NULL) {
    //    exit(-1);
    //} 
    //     test_control_transfer(lvr_winusb);
    //     test_interrupt_transfer(lvr_winusb);
    printf("Testing iso transfer\n");
    test_iso_transfer(dvb);
    //test_bulk_transfer(dvb);
    //
    libusb_handle_events_completed(NULL, NULL);
    libusb_release_interface(dvb,0);
    libusb_close(dvb);
    libusb_exit(NULL);
    return 0;
}
