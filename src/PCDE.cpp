/*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 */

/** PCDE Library header **/
#include "PCDE.hpp"

#include <string>
#include <string.h>

using namespace pcde;

PCDE::PCDE()
{
}

PCDE::~PCDE()
{
    if (isValid())
    {
        close();
    }
}

int PCDE::getVA(VA_Request::CHANNEL channel, float &voltage, float &current)
{
    VA_Request va_request(channel);

    sendCommand(va_request);

    // Check the response
    if (false)
    {
        throw std::runtime_error("Invalid response to VA request!");
    }

    return 0;
}

int PCDE::getMCSStatus(bool &status)
{
    MCS_Get_Status_Request mcs_status_request;

    sendCommand(mcs_status_request);

    if(mcs_status_request.m_response_msg[0]=='O' && mcs_status_request.m_response_msg[1]=='N')
    {
        status = true;
        return 0;
    }else if(mcs_status_request.m_response_msg[0]=='O' && mcs_status_request.m_response_msg[1]=='F' && mcs_status_request.m_response_msg[2]=='F')
    {
        status = false;
        return 0;
    }else
    {
        throw std::runtime_error("Invalid response to MCS status request!");
        return -1;
    }
}

int PCDE::setMCSStatus(const bool status)
{
    MCS_Set_Status_Request mcs_set_status_request(status);

    sendCommand(mcs_set_status_request);

    return 0;
}

int PCDE::getBatteryPercentage(int &percentage)
{
    Battery_Get_Status_Request battery_status_request;

    // Try send command and catch TimeOut error
    try
    {
        sendCommand(battery_status_request);
    }
    catch (iodrivers_base::TimeoutError e)
    {
        // This behavior is expected due to no response, if no battery is connected
        percentage = -1;
        return 0;
    }

    percentage = ui8tof(battery_status_request.m_response_msg);

    // Check the response
    // if (false)
    // {
    //     throw std::runtime_error("Invalid response to battery status request!");
    //     return -1;
    // }

    return 0;
}

int PCDE::sendCommand(CommandBase command)
{
    uint8_t *request_msg = strtoui8t(command.m_request_msg);
    uint8_t response_msg[command.m_max_response_msg_length];

    // Send request, errors are handled by underlying code
    writePacket(request_msg, sizeof(request_msg) / sizeof(request_msg[0]) - 1);

    // Read response, errors are handled by underlying
    readPacket(response_msg, sizeof(response_msg));

    command.m_response_msg = response_msg;

    return 0;
}



uint8_t* PCDE::strtoui8t(std::string input)
{
    uint8_t *output;
    output = new uint8_t[input.length() + 1];
    memcpy(output, input.c_str(), input.length() + 1);
    return output;
}

float PCDE::ui8tof(uint8_t *input) {
    std::string s;
    s.assign(input, input + sizeof(input));
    return std::stof(s);
}
