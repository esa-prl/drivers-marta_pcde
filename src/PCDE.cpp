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

#include <string.h>
#include <string>

#include <memory>

using namespace pcde;

PCDE::PCDE() : m_serialPort() {}

PCDE::~PCDE() {}

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

void PCDE::getVA(VA_Request::CHANNEL channel, float& voltage, float& current)
{
    VA_Request va_request(channel);

    m_serialPort.sendCommand(va_request);
    extractVA(va_request.m_response_msg, va_request.m_response_msg_length, voltage, current);
}

void PCDE::extractVA(const std::vector<uint8_t> message,
                     const int message_length,
                     float& voltage,
                     float& current)
{
    // Message example: 0.16A0029.85V00
    // From Specification of PCDE voltage and current have 2 decimal places
    // With a point as decimal delimiter, 6 characters would be sufficient
    // for values up to 999.99 A/V
    // uint8_t cur_msg[6];
    // uint8_t vol_msg[6];
    std::vector<uint8_t> cur_msg;
    std::vector<uint8_t> vol_msg;

    int value_delimiter_index;

    bool currentValid = false;
    bool voltageValid = false;

    for (int i = 0; i < message_length; i++)
    {
        if (message[i] == 'A')
        {
            // Current message should not be longer than this
            if (i > 6)
            {
                throw std::runtime_error("Invalid response to VA request!");
            }
            // Extract current value without current unit charecter
            // This way of copying arrays is not the best, but it worked
            for (size_t k = 0; k < i; k++)
            {
                // cur_msg[k] = message[k];
                cur_msg.push_back(message[k]);
            }

            value_delimiter_index = i;

            currentValid = true;
        }
        if (message[i] == 'V')
        {
            // Current is expected to be read first, so check that
            if (!currentValid) throw std::runtime_error("Invalid response to VA request!");

            // Be sure voltage message is not longer than 6 characters
            int vol_message_begin = value_delimiter_index + 2;
            int length = i - vol_message_begin;

            if (length > 6) vol_message_begin = vol_message_begin + (length - 6);

            // Extract voltage value without unit character
            // Again a complicated way of copying, maybe there are better ways
            for (size_t k = vol_message_begin; k < i; k++)
            {
                // vol_msg[k - vol_message_begin] = message[k];
                vol_msg.push_back(message[k]);
            }
            voltageValid = true;
            break;
        }
    }

    // If current and voltage are reveived, convert them to float
    if (voltageValid && currentValid)
    {
        current = ui8tof(cur_msg);
        voltage = ui8tof(vol_msg);
    }
    else
    {
        // It is expected to get always a full message, maybe an exception is not the best way here
        throw std::runtime_error("No full VA response delivered.");
    }
}

void PCDE::getMCSStatus(bool& status)
{
    MCS_Get_Status_Request mcs_status_request;

    m_serialPort.sendCommand(mcs_status_request);

    if (mcs_status_request.m_response_msg[0] == 'O' && mcs_status_request.m_response_msg[1] == 'N')
    {
        status = true;
        return;
    }
    else if (mcs_status_request.m_response_msg[0] == 'O'
             && mcs_status_request.m_response_msg[1] == 'F'
             && mcs_status_request.m_response_msg[2] == 'F')
    {
        status = false;
        return;
    }
    else
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

void PCDE::getBatteryPercentage(int& percentage)
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

    extractPercentage(battery_status_request.m_response_msg,
                      battery_status_request.m_response_msg_length,
                      percentage);
}

void PCDE::extractPercentage(const std::vector<uint8_t> message,
                             const int message_length,
                             int& percentage)
{
    std::vector<uint8_t> perc_msg;

    for (int i = 0; i < message_length; i++)
    {
        if (message[i] == '%')
        {
            if (i > 4) std::runtime_error("Invalid response to battery status request!");

            for (size_t k = 0; k < i; k++)
            {
                perc_msg.push_back(message[k]);
            }

            break;
        }
    }

    percentage = ui8tof(perc_msg);
    printf("percentage: %i\n", percentage);
}