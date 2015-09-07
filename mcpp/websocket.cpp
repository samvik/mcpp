#include <mcpp/websocket.h>

namespace mcpp {
namespace websocket {

void encodeHeader(std::ostringstream &header,
									size_t dataLength,
									websocket::OpCode opcode, char rsvd)
{
	header.put(0x80 | static_cast<char>(opcode) | static_cast<char>(rsvd<<4));

	char dummyLength;
	size_t realLength = dataLength;
	if (realLength < 126) {
		dummyLength = static_cast<char>(realLength);
	} else if (realLength < 1<<16) {
		dummyLength = 126;
	} else {
		dummyLength = 127;
	}
	header.put(dummyLength);
	if (dummyLength == 127) {
		header.put(static_cast<char>(realLength >> 56 &0xff));
		header.put(static_cast<char>(realLength >> 48 &0xff));
		header.put(static_cast<char>(realLength >> 40 &0xff));
		header.put(static_cast<char>(realLength >> 32 &0xff));
		header.put(static_cast<char>(realLength >> 24 & 0xff));
		header.put(static_cast<char>(realLength >> 16 & 0xff));
	}
	if (dummyLength == 126 || dummyLength == 127) {
		header.put(static_cast<char>(realLength >> 8 & 0xff));
		header.put(static_cast<char>(realLength & 0xff));
	}
}

Flags decodeFlags(const std::string &flags)
{
	Flags result;

	uint8_t f = static_cast<uint8_t>(std::stoul(flags, 0, 16));

	result.finished = (f & 0x80);
	result.rsvp = (f & 0x70);
	result.opCode = static_cast<OpCode>(f&0x0f);

	return result;
}

}
} // namespace mcpp

