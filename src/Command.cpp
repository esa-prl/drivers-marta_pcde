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
#include <iostream>

using namespace pcde;

CommandBase::CommandBase()
{
}

CommandBase::~CommandBase()
{
}

VA_Request::VA_Request(VA_Request::CHANNEL channel) : CommandBase()
{
    m_request_msg = "VA" + std::to_string(channel);
    m_max_response_msg_length = 15;
}

MCS_Get_Status_Request::MCS_Get_Status_Request() : CommandBase()
{
    m_request_msg = "St";
    m_max_response_msg_length = 5;
}

MCS_Set_Status_Request::MCS_Set_Status_Request(const bool status) : CommandBase()
{
    if (status)
    {
        m_request_msg = "Up";
    }
    else
    {
        m_request_msg = "Sh";
    }
    m_max_response_msg_length = 4;
}

Battery_Get_Status_Request::Battery_Get_Status_Request() : CommandBase()
{
    m_request_msg = "Bt";
    m_max_response_msg_length = 6;
}

uint8_t *pcde::strtoui8t(std::string input)
{
    uint8_t *output;
    output = new uint8_t[input.length() + 1];
    memcpy(output, input.c_str(), input.length() + 1);
    return output;
}

float pcde::ui8tof(const uint8_t *input)
{
    std::string s(input, input + sizeof(input) );
    return std::stof(s);
}

float pcde::ui8tof(std::vector<uint8_t> input)
{
    std::string s(input.begin(), input.end());
    return std::stof(s);
}

float pcde::ui8toi(std::vector<uint8_t> input)
{
    std::string s(input.begin(), input.end());
    return std::stoi(s);
}