/********************************************************************
 * @file /src/imu_node.cpp
 * @brief The program that interfaces with the imu
 * @date October 2017
/********************************************************************/

/********************************************************************
 * Includes
/********************************************************************/
#include <ros/ros.h>
#include <string>
#include <serial/serial.h>

/********************************************************************
 * Defines
/********************************************************************/


/********************************************************************
 * Global Variables
/********************************************************************/
serial::Serial * serial_conn = nullptr; //shared between service calls so needs to global



/********************************************************************
 * Implementation [Helper Functions]
/********************************************************************/

/********************************************************************
 * @Name     setupSerialConnection
 * @Args     fd File Descriptor or port of serial device
 * @Args     baud_rate baud_rate of serial device. defaults to 38400
 * @Args     timeout Time before we auto-disconnect during comm.
 * @function Maps the serial_conn object to a specific port so all
 *           we have to do serial_conn->write or read to communicate
/********************************************************************/
void setupSerialConnection(std::string fd, int baud_rate = 38400, int timeout = 1000)
{
    if (serial_conn != nullptr)
    {
        serial_conn->close();
        delete serial_conn;
    }

    ROS_INFO("Creating serial connection on port: %s, Baud Rate: %d with timeout time: %d", fd.c_str(), baud_rate, timeout);
    serial_conn = new serial::Serial(fd, (u_int32_t) baud_rate, serial::Timeout::simpleTimeout(timeout));
}

/********************************************************************
 * @Name     serial_out
 * @Args     serial_out_string is the string to transfer
 * @Args     expect_response tells the function whether to read for a response
 * @function writes to the serial device and returns the response
/********************************************************************/
std::string serial_out(std::string serial_out_string, bool expect_response = true)
{
    serial_conn->write(serial_out_string); // dont really care about size of the message, let the firmware deal with that

    if (!expect_response)
    {
        return "";
    }

    return serial_conn->readline(65536ul,"\r\n");
}

/********************************************************************
 * @Name     parseSerialResponse
 * @Args     response is the string returned from the serial device
 * @Args     expect_response tells the function whether a response exists
 * @function parses the response from the device and determines if
 *           the command was successful
/********************************************************************/
inline bool parseSerialResponse(const std::string & response, 
								bool expect_response) {
    return (bool) !(expect_response && response == "ERR");
}

/********************************************************************
 * Implementation [Service Calls]
 * @Args     req is the request message sent by the service calller
 * @Args     res is the reponse that we generate depending on the outcome
/********************************************************************/

/********************************************************************
 * @Name     setMotorForward [Service Call]
 * @function Sets motor forward speed
/********************************************************************/
bool getAccMagVector() {
    
    /* Create UART string and send to serial device */
    /* String: M${X}R${Arg}\n */
    /*std::string serial_out_str = (unsigned char) "STX";

    /* Sending serial output */
    ROS_INFO("Sending serial Output: %s", serial_out_str.c_str());
    res.imu_response = serial_out(serial_out_str, false); //****
    res.imu_success = parseSerialResponse(res.motor_response, false); //***

return true;          
}


/********************************************************************
 * Implementation [Main]
 * @Args     argc is the number of command-line arguments provided
 * @Args     argv is a pointer to the argument strings
/********************************************************************/
int main(int argc, char ** argv)
{
    ros::init(argc, argv, "imu_con");
    ros::NodeHandle nh("~");

    // From System Manager, get the FD
    //ros::ServiceClient getDeviceName = nh.serviceClient<system_manager::device_manager>("/device_manager/getDeviceFd");
    //std::string sys_name;
    //nh.getParam("system_name", sys_name);
    //system_manager::device_manager srv;
    //srv.request.device = sys_name;
    //getDeviceName.call(srv);
    std::string fd;
    fd = (std::string)"ttyUSB0";
    nh.getParam("imu_port", fd);
    setupSerialConnection(fd);
	std::string imu_output;
	imu_output = serial_out("STX",true);
	ROS_INFO("%s",imu_output.c_str());
    /* Setup all the Different services/commands which we  can call. Each service does its own error handling */
    //ros::ServiceServer getAccMagVectorService = nh.advertiseService("getAccMagVector", getAccMagVector);

    /* Wait for callbacks */
    ros::spin();

    if (serial_conn != nullptr)
    {
        ROS_INFO("Closing connection on port %s", serial_conn->getPort().c_str());
        serial_conn->close();
        delete serial_conn;
    }

    return 0;
}
