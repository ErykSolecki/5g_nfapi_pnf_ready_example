import socket
import sctp
import struct


def parseNFapiHeader(nfapi_header_data):
    (
        segment_length,
        segment_number,
        sequence_number,
        transmit_timestamp,
    ) = struct.unpack("H2BI", nfapi_header_data)
    more = segment_number & 0b10000000
    segment_number = segment_number & 0b01111111

    print("----nFapi Header----")
    print(f"segment_length: {segment_length}")
    print(f"more: {more} segment_number: {segment_number}")
    print(f"sequence_number: {sequence_number} ")
    print(f"transmit_timestamp: {transmit_timestamp}")

    return segment_length


def parseNFapiMsgHeader(nfapi_msg_header_data):
    sru_termination_type, phy_id, message_id, length = struct.unpack(
        "2BHI",
        nfapi_msg_header_data,
    )
    print("----nFapi Msg Header----")

    print(f"sru_termination_type: {hex(sru_termination_type)}")
    print(f"phy_id: {phy_id}")
    print(f"message_id: {hex(message_id)} ")
    print(f"length: {length}")

    return length, message_id


def parseNfapiPnfReadyIndication(nfapi_msg_body_data):
    version_info = struct.unpack("I", nfapi_msg_body_data)[0]
    print("----nFapi Msg Body----")
    print(f"Version_info: {version_info}")


def parseNfapiData(data):
    nfapi_header_size = 8

    if len(data) >= nfapi_header_size:
        nfapi_header = data[:nfapi_header_size]
        segment_length = parseNFapiHeader(nfapi_header)

        if len(data) < segment_length:
            print("Failed to recieve full segment")
            return False

        nfapi_msg_header_size = 8
        nfapi_msg_header = data[
            nfapi_header_size : nfapi_header_size + nfapi_msg_header_size
        ]
        length, message_id = parseNFapiMsgHeader(nfapi_msg_header)

        if message_id != 0x109:
            print("Unknown message type")
            return False

        msg_body_offset = nfapi_header_size + nfapi_msg_header_size
        msg_body_data = data[msg_body_offset : msg_body_offset + length]
        parseNfapiPnfReadyIndication(msg_body_data)
        return True

    else:
        print()
        return False


def main():
    host = "127.0.0.1"
    port = 2222

    sock = sctp.sctpsocket_tcp(socket.AF_INET)
    sock.bind((host, port))
    sock.listen(1)

    while True:
        print("Waiting for a connection")
        connection, client_address = sock.accept()

        try:
            host, port = client_address
            print(f"Connection from {host}:{port}")

            while True:
                data = connection.recv(999)
                if data:
                    print(f"Received {len(data)} bytes")
                    if not parseNfapiData(data):
                        break
        finally:
            connection.close()


if __name__ == "__main__":
    main()
