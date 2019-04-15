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
#include <iodrivers_base/Driver.hpp>
#include <base/Time.hpp>

namespace pcde{
    class SerialPort: public iodrivers_base::Driver{
        private:

        protected:

            int m_baudrate;
            base::Time m_timeout;

        public:



            SerialPort(int max_packet_size, int baudrate, int timeout_ms);

            ~SerialPort(void);


            void Greeting(void);

            bool setBaudRate(int brate);

            bool openPort(std::string const& filename);

            int extractPacket(uint8_t const* buffer, size_t buffer_size) const;

            int printBuffer (void);

            int closePort (void);
    };

}

#endif