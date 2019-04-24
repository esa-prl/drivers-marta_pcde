/*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 *
 * Description:
 * Description of possible commands that can be used with the PCDE
 */

#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include <string>
#include <string.h>

namespace pcde
{

class CommandBase
{
  public:
    CommandBase();

    // Serial Msg to be sent
    std::string m_request_msg;

    // Received serial msg
    uint8_t *m_response_msg;
    // Actual length of serial response msg
    int m_response_msg_length;
    // Max Length of serial response msg
    int m_max_response_msg_length;
};

class VA_Request : public CommandBase
{
  public:
    /**
         * Voltage/Current channels of the PCDE
         */
    enum CHANNEL
    {
        INVALID,
        BATTERY_INPUT,
        EXTERNAL_INPUT,
        OUT_24VDC,
        OUT_12VOBC,
        OUT_12V,
        OUT_5V,
        PTU,
        MCS
    };

    VA_Request(VA_Request::CHANNEL channel);

  private:
    //VA channel this command is corresponding to
    VA_Request::CHANNEL m_channel;
};

class MCS_Get_Status_Request : public CommandBase
{
  public:
    MCS_Get_Status_Request();
};

class MCS_Set_Status_Request : public CommandBase
{
  public:
    MCS_Set_Status_Request(const bool status);
};

class Battery_Get_Status_Request : public CommandBase
{
  public:
    Battery_Get_Status_Request();
};

uint8_t* strtoui8t(std::string input);

float ui8tof(uint8_t *input);

} // namespace pcde
#endif
