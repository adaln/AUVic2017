#include <serial/serial.h>
#include <string>
#include <iostream>
#include <vector>

/********************************************************************
 * Global Variables
/********************************************************************/
serial::Serial * serial_conn = nullptr; //shared between service calls so needs to global
typedef unsigned char BYTE;
 
int main(int argc, char ** argv){

	std::string port = "/dev/ttyUSB0";
	u_int32_t baud = 38400;
	serial_conn = new serial::Serial((std::string)"/dev/ttyUSB0", (u_int32_t) 38400, serial::Timeout::simpleTimeout(1000));
	serial_conn = new serial::Serial(port, baud, serial::Timeout::simpleTimeout(1000));
	
	
	BYTE val = 0x02;
	
	serial_conn->write(&val,sizeof(val)); 

	const size_t size = 23;
	BYTE buffer[size] = {0}; // store recieved bites
	size_t read = 0;
	
	read = serial_conn->read(buffer, size);
	std::cout<<buffer<<"\n";

}
