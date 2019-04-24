/*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 *
 * Description:
 * Defines the serial communication, using the ROCK iodriver
 */

#include "SerialPort.hpp"
using namespace pcde;

SerialPort::SerialPort(int max_packet_size)
    : Driver(max_packet_size)
{
    if (max_packet_size <= 0)
    {
        std::runtime_error("SerialPort: max_packet_size cannot be smaller or equal to 0!");
    }
}

SerialPort::~SerialPort()
{
    if(isRunning())
    {
        closePort();
    }
}

void SerialPort::setBaudrate(const int baudrate)
{
    if(m_isRunning)
    {
        std::runtime_error("SerialPort: Can not set baudrate while port is open!");
    }
    m_baudrate=baudrate;
}

void SerialPort::setReadTimeout(const int timeout_ms)
{
    if(m_isRunning)
    {
        std::runtime_error("SerialPort: Can not set timeout while port is open!");
    }
    if (timeout_ms < 0)
    {
        std::runtime_error("SerialPort: Read timeout cannot be smaller than 0!");
    }
    Driver::setReadTimeout(base::Time::fromMilliseconds(timeout_ms));
}

void SerialPort::setWriteTimeout(const int timeout_ms)
{
    if(m_isRunning)
    {
        std::runtime_error("SerialPort: Can not set timeout while port is open!");
    }
    if (timeout_ms < 0)
    {
        std::runtime_error("SerialPort: Write timeout cannot be smaller than 0!");
    }
    Driver::setWriteTimeout(base::Time::fromMilliseconds(timeout_ms));
}

bool SerialPort::openPort(std::string const& filename)
{
    if(m_isRunning)
    {
        std::runtime_error("SerialPort: Port is already open!");
    }
    if (Driver::openSerial(filename, m_baudrate))
    {
        m_isRunning=true;
        return true;
    }else
    {
        return false;
    }
}

void SerialPort::closePort(void)
{
    if (isValid())
    close();
}

bool SerialPort::isRunning()
{
    return m_isRunning;
}

int SerialPort::sendCommand(CommandBase command)
{
    uint8_t *request_msg = strtoui8t(command.m_request_msg);
    uint8_t response_msg[command.m_max_response_msg_length];

    // Send request, errors are handled by underlying code
    writePacket(request_msg, sizeof(request_msg) / sizeof(request_msg[0]) - 1);

    // Read response, errors are handled by underlying
    command.m_response_msg_length=readPacket(response_msg, sizeof(response_msg));

    command.m_response_msg = response_msg;

    return 0;
}

int SerialPort::extractPacket(uint8_t const *buffer, size_t buffer_size) const
{
    if (buffer[buffer_size - 1] == '\n')
    {
        // Packet is complete when line break is sent
        return buffer_size;
    }
    else
    {
        // Packet is not complete, go on reading
        return 0;
    }
}