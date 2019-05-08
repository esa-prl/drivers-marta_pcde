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

#ifndef _PCDE_HPP_
#define _PCDE_HPP_

/** Rock library for driver functionality **/
#include <iodrivers_base/Driver.hpp>

/** Definition of possible commands for the PCDE **/
#include <pcde/Command.hpp>
#include <pcde/SerialPort.hpp>

#include <string>
// Used for smart pointers
#include <memory>
#include <vector>

namespace pcde{
    struct SerialConfig
    {
        std::string port="";
        int baudrate=19200;
        int write_timeout_ms=0;
        int read_timeout_ms=0;

    };

    class PCDE{
        public:
            PCDE(void);
            ~PCDE(void);

            /**
             * @brief SerialPort object to manage all the serial communication
             *
             */
            SerialPort m_serialPort;

            /**
             * @brief Sets the configuration of the serial communication and starts it
             *
             * @param config Struct containing the config values
             */
            void setupSerial(const SerialConfig config);

            /**
             * @brief Sets the configuration of a mock serial communication and starts it
             *
             * @param config Struct containing the config values
             */
            void setupTestSerial(const SerialConfig config);

            /**
             * @brief Reads the requested voltage current measurement of the PCDE
             *
             * @param channel Voltage/current channel that is requested
             * @param voltage Value given in Volts
             * @param current VAlue given in Ampere
             */
            void getVA(const VA_Request::CHANNEL channel, float& voltage, float& current);

            /**
             * @brief Get the Status of the MCS
             *
             * @param status True when MCS is turned on, false if turned off
             */
            void getMCSStatus(bool& status);

            /**
             * @brief Set the status of the MCS
             *
             * @param status Send True to turn On and false to turn Off
             */
            void setMCSStatus(const bool status);

            /**
             * @brief Get the percentage of the battery, or check whether one is present.
             *
             * @param percentage Charge of battery in %, returns -1 if no battery is connected
             */
            void getBatteryPercentage(int& percentage);

        private:

            /**
             * @brief Extracts the voltage and current value from the serial message
             *
             * @param message Serial message
             * @param message_length Length of the serial message
             * @param voltage Returned voltage value
             * @param current Returned current value
             */
            void extractVA(const std::vector<uint8_t> message, const int message_length, float& voltage, float& current);

            /**
             * @brief Extracts the battery percentage value from the serial message
             *
             * @param message Serial message
             * @param message_length Length of the serial message
             * @param percentage Returned percentage value
             */
            void extractPercentage(const std::vector<uint8_t> message, const int message_length, int& percentage);

    };

}

#endif