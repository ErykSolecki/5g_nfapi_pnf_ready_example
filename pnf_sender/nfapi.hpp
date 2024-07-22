#include <cstdint>

struct NfapiHeader
{
    uint16_t segment_length;
    uint8_t more : 1;
    uint8_t segment_number : 7;
    uint8_t sequence_number;
    uint32_t transmit_timestamp;
};

enum class SruType : uint8_t
{
    P5P7Message = 0x01,
    P19Dfe = 0x02,
    P19Rf = 0x03,
    P4 = 0x04,
};

enum class NfapiDedicatedMessage : uint16_t
{
    PnfParamRequest = 0x0100,
    PnfParamResponse = 0x0101,
    PnfConfigRequest = 0x0102,
    PnfConfigResponse = 0x0103,
    PnfStartRequest = 0x0104,
    PnfStartResponse = 0x0105,
    PnfStopRequest = 0x0106,
    PnfStopResponse = 0x0107,
    StartResponse = 0x0108,
    PnfReadyIndication = 0x0109,
    DlNodeSync = 0x0180,
    UlNodeSync = 0x0181,
    TimingInfo = 0x0182,
};

struct NfapiMsgHeader
{
    SruType sru_termination_type;
    uint8_t phy_id = 0;
    uint16_t message_id;
    uint32_t length;
};

struct NfapiPnfReadyIndicationBody
{
    uint32_t version_info;
};

struct NfapiPnfReadyIndicationMsg
{
    NfapiHeader header;
    NfapiMsgHeader msg_header;
    NfapiPnfReadyIndicationBody body;
};

unsigned int CalculatePadding(unsigned int preceding_message_body_length)
{
    return (4 - (preceding_message_body_length % 4)) % 4;
}