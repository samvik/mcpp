#include <mcpp/connection.h>

#include <sstream>

namespace mcpp {

Connection::Connection(std::string senderId, std::string pullAddress,
											 std::string pubAddress) :
	m_senderId(senderId), m_context(1), m_pull(m_context, ZMQ_PULL), m_pub(m_context, ZMQ_PUB)
{
	m_pull.connect(pullAddress.c_str());
	m_pub.connect(pubAddress.c_str());
	m_pub.setsockopt(ZMQ_IDENTITY, m_senderId.data(), m_senderId.length());
}

Connection::~Connection()
{
}

Request Connection::recv()
{
	Request request;
	recv(request, -1);
	return request;
}

bool Connection::recv(Request &request, long timeout)
{
	bool result = false;
	zmq::message_t msg;

	zmq::pollitem_t items [] = {
		{ m_pull, 0, ZMQ_POLLIN, 0 }
	};

	zmq::poll(&items[0], 1, timeout);

	if (items [0].revents & ZMQ_POLLIN) {
		m_pull.recv(&msg);
		request = Request(msg);
		result = true;
	}

	return result;
}

bool Connection::send(const std::string &data)
{
	zmq::message_t msg(data.length());
	std::memcpy(reinterpret_cast<char*>(msg.data()), data.data(), data.length());
	return m_pub.send(msg);
}

bool Connection::deliver(const std::string uuid,
												 const std::vector<unsigned> &ids,
												 const std::string &data)
{
	assert(ids.size() <= MAX_IDS);

	std::ostringstream idStream;
	bool first = true;
	for(const unsigned &id : ids) {
		if(!first) { idStream << " "; }
		else { first = false; }
		idStream << id;
	}
	std::string idString = idStream.str();

	std::ostringstream s;
	s << uuid << " "
		<< idString.length() << ":" << idString
		<< ", " << data;

	return send(s.str());
}

bool Connection::reply(const Request &request, const std::string &data)
{
	std::ostringstream s;
	std::string id = std::to_string(request.connectionId());
	s << request.sender() << " " << id.size() << ":" << id << ", " << data;

	return send(s.str());
}

bool Connection::replyHttp(const Request &request, const std::string &content,
													 uint16_t code, const std::string &status,
													 const Json::Value &headers)
{
	std::ostringstream r;
	r << "HTTP/1.1 " << code << " " << status << "\r\n";
	r << "Content-Length: " << content.length() << "\r\n";

	 for( Json::ValueIterator it = headers.begin() ; it != headers.end() ; it++ ) {
		r << it.key().asString() << ": " << it->asString() << "\r\n";
	}

	r << "\r\n" << content;

	return reply(request, r.str());
}

bool Connection::replyHttp(const Request &request, const std::string &content,
													 http::StatusCode code, const Json::Value &headers)
{
	return replyHttp(request, content, static_cast<uint16_t>(code),
									 http::statusCodeMessage(code), headers);
}

bool Connection::replyHttp(const Request &request, http::StatusCode code, const Json::Value &headers)
{
	return replyHttp(request, http::statusCodeMessage(code), static_cast<uint16_t>(code),
									 http::statusCodeMessage(code), headers);
}

bool Connection::replyHttpJson(const Request &request, const Json::Value &json,
															 http::StatusCode code, const Json::Value &headers)
{
	Json::StyledWriter writer;
	std::string content = writer.write( json );
	return replyHttp(request, content, code, headers);
}

bool Connection::acceptWebsocket(const Request &request, Json::Value headers)
{
	headers["Upgrade"] = "websocket";
	headers["Connection"] = "Upgrade";
	headers["Sec-WebSocket-Accept"] = request.body();

	return replyHttp(request, "", mcpp::http::StatusCode::SWITCHING_PROTOCOLS,
									 headers);
}

bool Connection::rejectWebsocket(const Request &request, Json::Value headers)
{
	return replyHttp(request, "", mcpp::http::StatusCode::METHOD_NOT_ALLOWED,
									 headers);
}

bool Connection::replyWebsocket(const Request &request, const std::string &data,
																websocket::OpCode opcode, char rsvd)
{
	std::ostringstream header;
	websocket::encodeHeader(header, data.length(), opcode, rsvd);
	return reply(request, header.str() + data);
}

bool Connection::deliverWebsocket(const std::string &uuid,
																	const std::vector<unsigned> ids,
																	const std::string &data,
																	websocket::OpCode opcode, char rsvd)
{
	std::ostringstream header;
	websocket::encodeHeader(header, data.length(), opcode, rsvd);
	return deliver(uuid, ids, header.str() + data);
}

bool Connection::deliverWebsocketJson(const std::string &uuid,
																			const std::vector<unsigned> ids,
																			const Json::Value &json,
																			websocket::OpCode opcode, char rsvd)
{
	Json::StyledWriter writer;
	std::string content = writer.write( json );
	return deliverWebsocket(uuid, ids, content, opcode, rsvd);
}

bool Connection::closeWebsocket(const Request &request,
																websocket::StatusCode code, std::string reason)
{
	std::ostringstream s;
	s.write(reinterpret_cast<const char*>(&code), 2);
	s << reason;
	return replyWebsocket(request, s.str(), websocket::OpCode::CLOSE);
}

bool Connection::closeWebsocket(const std::string &uuid, const std::vector<unsigned> ids, websocket::StatusCode code, std::string reason)
{
	std::ostringstream s;
	s.write(reinterpret_cast<const char*>(&code), 2);
	s << reason;
	return deliverWebsocket(uuid, ids, s.str(), websocket::OpCode::CLOSE);
}

} // namespace mcpp

