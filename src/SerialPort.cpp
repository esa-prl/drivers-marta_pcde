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

SerialPort::SerialPort(int max_packet_size, int baudrate, int timeout_ms)
    : Driver(max_packet_size)
    , m_timeout(base::Time::fromMilliseconds(timeout_ms))
    , m_baudrate(baudrate)
{
    // if(MAX_BUFFER_SIZE <= 0)
    // {
    //     std::runtime_error("SerialPort: max_packet_size cannot be smaller or equal to 0!");
    // }

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
