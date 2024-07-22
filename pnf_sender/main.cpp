#include <optional>
#include <iostream>

#include <netinet/in.h>

#include "sctp.hpp"
#include "nfapi.hpp"

const char *ip4_address_str = "127.0.0.1";
uint16_t sender_port = 1111;
uint16_t receiver_port = 2222;

std::optional<in_addr> ValidateIpAddress(const char *ip4_address)
{
    in_addr address;

    if (inet_aton(ip4_address, &address) == 0)
    {
        return {};
    }
    return address;
}

int main(int, char **)
{
    auto ip4_address = ValidateIpAddress(ip4_address_str);
    if (not ip4_address)
    {
        std::cerr << "Invalid ip4 adress\n";
        return 1;
    }

    Sctp sctp;
    if (!sctp.Create())
    {
        std::cerr << "Failed creating socket\n";
        return 2;
    }
    if (!sctp.SetLocalAddress(ip4_address.value(), sender_port))
    {
        std::cerr << "Failed binding address'n";
        return 3;
    }

    if (!sctp.Connect(ip4_address.value(), receiver_port))
    {
        std::cerr << "Failed connecting\n";
        return 4;
    }

    NfapiPnfReadyIndicationMsg ready_indication_msg = {
        .header = {
            .segment_length = sizeof(NfapiMsgHeader) + sizeof(NfapiPnfReadyIndicationBody),
            .more = 0,
            .segment_number = 0,
            .sequence_number = 0,
            .transmit_timestamp = 0,
        },
        .msg_header = {
            .sru_termination_type = SruType::P5P7Message,
            .phy_id = 0,
            .message_id = static_cast<uint16_t>(NfapiDedicatedMessage::PnfReadyIndication),
            .length = sizeof(NfapiPnfReadyIndicationBody),
        },
        .body = {.version_info = 1},
    };

    int sent_bytes = sctp.SendMsg(&ready_indication_msg, sizeof(NfapiPnfReadyIndicationMsg));

    std::cout << "Sent: " << sent_bytes << " bytes to " << ip4_address_str << ":" << receiver_port << std::endl;
    
    return 0;
}
