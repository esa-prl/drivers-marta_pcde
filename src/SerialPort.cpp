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

SerialPort::SerialPort(int max_packet_size) : Driver(max_packet_size)
{
    if (max_packet_size <= 0)
    {
        std::runtime_error("SerialPort: max_packet_size cannot be smaller or equal to 0!");
    }
}

SerialPort::~SerialPort()
{
    if (isRunning())
    {
        closePort();
    }
}

void SerialPort::setBaudrate(const int baudrate)
{
    if (m_isRunning)
    {
        std::runtime_error("SerialPort: Can not set baudrate while port is open!");
    }
    m_baudrate = baudrate;
}

void SerialPort::setReadTimeout(const int timeout_ms)
{
    if (m_isRunning)
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
    if (m_isRunning)
    {
        std::runtime_error("SerialPort: Can not set timeout while port is open!");
    }
    if (timeout_ms < 0)
    {
        std::runtime_error("SerialPort: Write timeout cannot be smaller than 0!");
    }
    Driver::setWriteTimeout(base::Time::fromMilliseconds(timeout_ms));
}

void SerialPort::openPort(std::string const& filename)
{
    if (m_isRunning)
    {
        std::runtime_error("SerialPort: Port is already open, first close it!");
    }
    Driver::openSerial(filename, m_baudrate);
    m_isRunning = true;
}

void SerialPort::openTestPort()
{
    if (m_isRunning)
    {
        std::runtime_error("SerialPort: Port is already open, first close it!");
    }
    Driver::openURI("test://");
    m_isRunning = true;
}

void SerialPort::closePort(void)
{
    if (isValid()) close();
}

bool SerialPort::isRunning() { return m_isRunning; }

int SerialPort::sendCommand(CommandBase& command)
{
    uint8_t* request_msg = strtoui8t(command.m_request_msg);

    /** TODO:
     *  There should be a factory that manages all the possible commands and
     *  is able to keep track of the maximum length of response messages.
     *  This one then should be used to initialize the iodrivers_base.
     *  For now this quick fix helps as long there is no command added,
     *  that expects a response message longer than MAX_PACKET_SIZE
     */
    // uint8_t response_msg[command.m_max_response_msg_length];
    uint8_t response_msg[MAX_PACKET_SIZE];

    // Send request, errors are handled by underlying code
    writePacket(request_msg, sizeof(request_msg) / sizeof(request_msg[0]) - 1);

    // Read response, errors are handled by underlying code
    command.m_response_msg_length = readPacket(response_msg, sizeof(response_msg));

    for (size_t i = 0; i < command.m_response_msg_length; i++)
    {
        command.m_response_msg.push_back(response_msg[i]);
    }

    return 0;
}

int SerialPort::extractPacket(uint8_t const* buffer, size_t buffer_size) const
{
    /**
     * This is a really bad hack. The messages received from PCDE
     * are terminated by the hex value 0x00, but also the current and voltage values
     * are seperated by this value. To handle them as one packet, this hack with the unit character
     * was the easiest solution. There may be a lot better ones.
     * Best would be to implement a unique end character for messages send by the PCDE
     * microcontroller
     */
    if (buffer[buffer_size - 1] == 0 && buffer[buffer_size - 2] != 'A')
    {
        // Packet is complete when zero value is sent
        return buffer_size;
    }
    else
    {
        // Packet is not complete, go on reading
        return 0;
    }
}