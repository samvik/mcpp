#ifndef MCPP_WEBSOCKET_H
#define MCPP_WEBSOCKET_H

#include <cinttypes>
#include <sstream>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define U16_TO_NETWORK_BYTE_ORDER(x)                                           \
    ((static_cast<uint16_t>(x) >> 8) | ((static_cast<uint16_t>(x) & 0xff) << 8))
#elif __BYTE_ORDER == __BIG_ENDIAN
#define U16_TO_NETWORK_BYTE_ORDER(x) x
#endif

namespace mcpp
{
namespace websocket
{

/**
 * @brief Websocket operation code.
 */
enum class OpCode : uint8_t
{
    CONT = 0,
    TEXT = 1,
    BIN = 2,
    CLOSE = 8,
    PING = 9,
    PONG = 10
};

/**
 * @brief Websocket status code.
 */
enum class StatusCode : uint16_t
{
    CLOSE_NORMAL = U16_TO_NETWORK_BYTE_ORDER(1000),
    CLOSE_GOING_AWAY = U16_TO_NETWORK_BYTE_ORDER(1001),
    CLOSE_PROTOCOL_ERROR = U16_TO_NETWORK_BYTE_ORDER(1002),
    CLOSE_BAD_TYPE = U16_TO_NETWORK_BYTE_ORDER(1003),
    CLOSE_BAD_DATA = U16_TO_NETWORK_BYTE_ORDER(1007),
    CLOSE_POLICY_VIOLATION = U16_TO_NETWORK_BYTE_ORDER(1008),
    CLOSE_TOO_BIG = U16_TO_NETWORK_BYTE_ORDER(1009),
    CLOSE_EXTENSION_NEEDED = U16_TO_NETWORK_BYTE_ORDER(1010),
    CLOSE_UNEXPECTED_CONDITION = U16_TO_NETWORK_BYTE_ORDER(1011)
};

/**
 * @brief Websocket flags.
 */
struct Flags
{
    bool finished;
    OpCode opCode;
    uint8_t rsvp;
};

/**
 * @brief Encode websocket header.
 * @param header Stream to write the header to.
 * @param dataLength Content length.
 * @param opcode Websocket operation code.
 * @param rsvd Websocket extension, should be 0 if not used.
 */
void encodeHeader(std::ostringstream &header, size_t dataLength,
                  websocket::OpCode opcode, char rsvd);

/**
 * @brief Decode websocket flags.
 * @param flags Data containing the flags.
 * @return Decoded flags.
 */
Flags decodeFlags(const std::string &flags);
}
} // namespace mcpp

#endif // MCPP_WEBSOCKET_H
