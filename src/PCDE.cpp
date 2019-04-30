/*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 *
 * Description:
 * Driver for the Power Distribution and Control Electronics (PCDE) of the MaRTA rover.
 * Communication is based on a serial connection to send request messages for:
 * - Battery status
 * - Voltage and current levels of different channels
 * - Motor Control Subsystem (MCS) status
 */

/** PCDE Library header **/
#include "PCDE.hpp"

#include <string>
#include <string.h>

#include <memory>

using namespace pcde;

PCDE::PCDE():m_serialPort()
{
}

PCDE::~PCDE()
{
}

void PCDE::setupSerial(const SerialConfig config)
{
    m_serialPort.closePort();

    m_serialPort.setBaudrate(config.baudrate);
    m_serialPort.setWriteTimeout(config.write_timeout_ms);
    m_serialPort.setReadTimeout(config.read_timeout_ms);

    m_serialPort.openPort(config.port);
}

void PCDE::setupTestSerial(const SerialConfig config)
{
    m_serialPort.closePort();
    
    m_serialPort.setBaudrate(config.baudrate);
    m_serialPort.setWriteTimeout(config.write_timeout_ms);
    m_serialPort.setReadTimeout(config.read_timeout_ms);

    m_serialPort.openTestPort();
}

void PCDE::getVA(VA_Request::CHANNEL channel, float &voltage, float &current)
{
    VA_Request va_request(channel);

    m_serialPort.sendCommand(va_request);
    extractVA(va_request.m_response_msg, va_request.m_response_msg_length, voltage, current);
}

void PCDE::extractVA(const uint8_t* message, const int message_length, float& voltage, float& current)
{
    // Message example: 0.16A0029.85V00
    // From Specification of PCDE voltage and current have 2 decimal places
    // With a point as decimal delimiter, 6 characters would be sufficient
    // for values up to 999.99 A/V
    uint8_t vol_msg[6];
    uint8_t cur_msg[6];

    int value_delimiter_index;

    bool currentValid=false;

    for(int i=0; i<message_length; i++)
    {
        if(message[i]=='A')
        {
            // Avoid writing to wrong memory
            if (i>6)
            {
                throw std::runtime_error("Invalid response to VA request!");
            }
            // Extract current value without unit char
            std::copy(message, message+i-1, cur_msg);
            value_delimiter_index=i;

            currentValid=true;
        }
        if(message[i]=='V')
        {
            //Current is expected to be read first
            if (!currentValid)
                throw std::runtime_error("Invalid response to VA request!");

            // Be sure voltage message is not longer than 6 characters
            int vol_message_begin=value_delimiter_index+1;
            int length = i-vol_message_begin;

            // Minimum of leading digit, decimal point, 2 decimal digits and unit char
            if (length<5)
                throw std::runtime_error("Invalid response to VA request!");

            if(length>6)
                vol_message_begin=vol_message_begin+(length-6);

            // Extract voltage value without unit char
            std::copy(message+vol_message_begin, message+i-1, vol_msg);
            break;
        }
    }

    // Convert the messages to float
    current=ui8tof(cur_msg);
    voltage=ui8tof(vol_msg);
}

void PCDE::getMCSStatus(bool &status)
{
    MCS_Get_Status_Request mcs_status_request;

    m_serialPort.sendCommand(mcs_status_request);

    if(mcs_status_request.m_response_msg[0]=='O' && mcs_status_request.m_response_msg[1]=='N')
    {
        status = true;
        return;
    }else if(mcs_status_request.m_response_msg[0]=='O' && mcs_status_request.m_response_msg[1]=='F' && mcs_status_request.m_response_msg[2]=='F')
    {
        status = false;
        return;
    }else
    {
        throw std::runtime_error("Invalid response to MCS status request!");
    }
}

void PCDE::setMCSStatus(const bool status)
{
    MCS_Set_Status_Request mcs_set_status_request(status);

    m_serialPort.sendCommand(mcs_set_status_request);

    return;
}

void PCDE::getBatteryPercentage(int &percentage)
{
    Battery_Get_Status_Request battery_status_request;

    // Try send command and catch TimeOut error
    try
    {
        m_serialPort.sendCommand(battery_status_request);
    }
    catch (iodrivers_base::TimeoutError e)
    {
        // This behavior is expected due to no response, if no battery is connected
        percentage = -1;
        return;
    }

    percentage = ui8tof(battery_status_request.m_response_msg);
}

void PCDE::extractPercentage(const uint8_t* message, const int message_length, float& percentage)
{
    uint8_t perc_msg[3];

    for(int i = 0; i < message_length; i++)
    {
        if (message[i]=='%')
        {
            if (i>4)
                std::runtime_error("Invalid response to battery status request!");

            std::copy(message, message+i-1, perc_msg);
            break;
        }
    }

    percentage = ui8tof(perc_msg);
}