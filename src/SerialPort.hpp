/*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 *
 * Description:
 * Defines the serial communication, using the ROCK iodriver
 */

#ifndef _PCDE_SERIAL_PORT_HPP_
#define _PCDE_SERIAL_PORT_HPP_

/** Rock Std types **/
#include <base/Time.hpp>
#include <iodrivers_base/Driver.hpp>

#include <pcde/Command.hpp>

namespace pcde
{
class SerialPort : public iodrivers_base::Driver
{
  private:
    base::Time m_timeout = base::Time::fromMilliseconds(0);
    int m_baudrate = Driver::SERIAL_19200;
    bool m_isRunning = false;

  public:
    /**
     * @brief Construct a new Serial Port object
     *
     * @param max_packet_size Maximum size a packet received can have.
     * @param timeout_ms Timeout for read and write communications. Value of 0 is non-blocking.
     */
    SerialPort(int max_packet_size = 25);

    ~SerialPort();

    /**
     * @brief Set the baudrate of the serial communication
     *
     * @param baudrate to use for the serial communication
     */
    void setBaudrate(const int baudrate);

    /**
     * @brief Set the timeout for read communication
     *
     * @param read_timeout_ms Value of 0 is non-blocking
     */
    void setReadTimeout(const int read_timeout_ms);

    /**
     * @brief Set the timeout for write communication
     *
     * @param write_timeout_ms Value of 0 is non-blocking
     */
    void setWriteTimeout(const int write_timeout_ms);

    /**
     * @brief Opens the port for serial communication
     *
     * @param filename Name of the port. Can be left blank when used in testMode
     *
     */
    void openPort(std::string const& filename);

    /**
     * @brief Opens a mock port to simulate serial connection for testing, which is represented by a
     * file stream
     *
     */
    void openTestPort();

    /**
     * @brief Returns whether port is already open
     *
     */
    bool isRunning();

    /**
     * @brief Closes the port if it is open
     *
     */
    void closePort(void);

    /**
     * @brief Definition of how a serial packet should be handled.
     * for further documentation, see base class.
     *
     */
    int extractPacket(uint8_t const* buffer, size_t buffer_size) const;

    /**
     * @brief Sends a command over serial port and
     *
     * @param command Predefined command containing message
     */
    int sendCommand(CommandBase& command);
};

}  // namespace pcde

#endif