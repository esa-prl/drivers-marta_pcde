/*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 */

#ifndef _PCDE_HPP_
#define _PCDE_HPP_

/** Rock library for driver functionality **/
#include <iodrivers_base/Driver.hpp>

/** Definition of possible commands for the PCDE **/
#include <pcde/Command.hpp>

#include <string>

namespace pcde{

    class PCDE{
        public:
            PCDE();
            ~PCDE();

            // void setup(std::string const& uri);
            // void read();
            // void close();


            /**
             * @brief Reads the requested voltage current measurement of the PCDE
             *
             * @param channel Voltage/current channel that is requested
             * @param voltage Value given in Volts
             * @param current VAlue given in Ampere
             */
            int getVA(const VA_Request::CHANNEL channel, float& voltage, float& current);

            /**
             * @brief Get the Status of the MCS
             *
             * @param status True when MCS is turned on, false if turned off
             */
            int getMCSStatus(bool& status);

            /**
             * @brief Set the status of the MCS
             *
             * @param status Send True to turn On and false to turn Off
             */
            int setMCSStatus(const bool status);

            /**
             * @brief Get the percentage of the battery, or check whether one is present.
             *
             * @param percentage Charge of battery in %, returns -1 if no battery is connected
             */
            int getBatteryPercentage(int& percentage);



            int sendCommand(CommandBase command);

        private:

            /**
             * @brief COM function to interpret packages
             *
             * @param buffer
             * @param buffer_size
             * @return int
             */

            uint8_t* strtoui8t(std::string input);
            float ui8tof(uint8_t *input);

    };

}

#endif