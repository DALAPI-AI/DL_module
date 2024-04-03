/**
 * @file comm.h
 * @author Dania OULKADI
 * @author Laurent LIN
 * @author Pierre FRANCK-PAPUCHON
 * @author Alicia BERROUANE
 * @brief Gère la communication entre la Raspberry et Arduino
 */
#ifdef __linux__
#ifndef COMM_H
#define COMM_H
#include <wiringPi.h>
#include <wiringSerial.h>
/**
 * @brief Configure the serial and return the file descriptor of the serial port
 * 
 * @param serialPort serial port name (/dev/tty* or COM)
 * @param baud_Rate baud rate to use 
 * @return int -1 if error | int file descriptor of the port
 */
int configureSerial(char* serialPort,int baud_Rate);
/**
 * @brief Send the message to the serial
 * 
 * @param channel serial port 
 * @param msg message
 * @return int nBytes sent | -1 error
 */
int sendMessage(int channel,char* msg);


/**
 * @brief Receive the message from the serial
 * 
 * @param channel serial port
 * @param buffer buffer to store the messsage
 * @return int nBytes read | -1 error
 */
int retrieveMessage(int channel,char* buffer);



#endif // COMM_H
#endif