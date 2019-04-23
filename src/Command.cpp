 /*
 * Author: Maximilian Ehrhardt
 * Mail: maximilian.ehrhardt@essa.int
 * Date of Creation: April 2019
 *
 * Description:
 * Description of possible commands that can be used with the PCDE
 */

#include "Command.hpp"

#include <string>

using namespace pcde;

CommandBase::CommandBase(int request_msg_length, int max_response_msg_length)
    : m_request_msg_length(request_msg_length)
    , m_max_response_msg_length(max_response_msg_length)
{
}

VA_Request::VA_Request(VA_Request::CHANNEL channel): CommandBase(3, 15)
{
    m_request_msg = "VA" + std::to_string(channel);
}

MCS_Get_Status_Request::MCS_Get_Status_Request(): CommandBase(2, 5)
{
    m_request_msg = "St";
}

MCS_Set_Status_Request::MCS_Set_Status_Request(const bool status): CommandBase(2, 4)
{
    if (status)
    {
        m_request_msg="Up";
    }else
    {
        m_request_msg="Sh";
    }

}

Battery_Get_Status_Request::Battery_Get_Status_Request(): CommandBase(2,6)
{
    m_request_msg = "Bt";
} 

uint8_t* pcde::strtoui8t(std::string input)
{
    uint8_t *output;
    output = new uint8_t[input.length() + 1];
    memcpy(output, input.c_str(), input.length() + 1);
    return output;
}

float pcde::ui8tof(uint8_t *input)
{
    std::string s;
    s.assign(input, input + sizeof(input));
    return std::stof(s);
}