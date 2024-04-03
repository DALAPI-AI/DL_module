#ifdef __linux__
#include "process/comm.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
int configureSerial(char* serialPort,int baud_Rate){
    int serial_device;
    if ((serial_device = serialOpen(serialPort, baud_Rate)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return -1;
    }
    return serial_device;
}


int sendMessage(int channel,char* msg){
    serialPrintf(channel, msg);
    serialPrintf(channel, "\n");
    return 0;
}


int retrieveMessage(int channel,char* buffer){
    char data = 'a';
    int i=0;
    int nb = 0;
    while ((nb = serialDataAvail(channel)) > 0) {
        data = serialGetchar(channel);
        if (data =='\n') break;
        buffer[i++] = data;
    }
    buffer[i] = '\0';
    return i;
}

#endif