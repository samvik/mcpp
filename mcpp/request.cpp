#include <mcpp/request.h>
#include <mcpp/utility.h>

#include <sstream>
#include <zmq.hpp>

namespace mcpp {

Request::Request() : m_valid(false)
{

}

Request::Request(const zmq::message_t& message) : m_valid(false)
{
	std::string in(static_cast<const char*>(message.data()), message.size());
	std::istringstream stream(in);

	std::string path;
	std::string headers;

	// Decode message
	stream >> m_sender >> m_connectionId;
	stream >> path;
	headers = utility::readNetString(stream);
	m_body = utility::readNetString(stream);

	// Decode path
	if(!utility::decodeUrl(path, m_path)) {
		// TODO: Throw exception
		m_path = path;
	}

	// Parse http header
    m_headers = json::parse(headers);

	// Parse json body
    if(m_headers.count("METHOD") && m_headers["METHOD"] == "JSON") {
        m_jsonBody = json::parse(m_body);
	}

	// Parse query parameters
    std::string query = m_headers.value("QUERY", std::string());
	if(!query.empty()) {
		std::string decodedQuery;
		if(utility::decodeUrl(query, decodedQuery)) {
			utility::parseQuery(decodedQuery, m_query);
		}
		else {
			// TODO: Throw exception
		}
	}

	m_valid = true;
}

Request::~Request()
{

}

bool Request::isValid() const
{
	return m_valid;
}

bool Request::isDisconnect() const
{
	bool result = false;

    if(m_jsonBody.count("type") && m_jsonBody["type"] == "disconnect") {
		result = true;
	}

	return result;
}

const std::string &Request::sender() const
{
	return m_sender;
}

unsigned Request::connectionId() const
{
	return m_connectionId;
}

const std::string &Request::path() const
{
	return m_path;
}

const json &Request::headers() const
{
	return m_headers;
}

const std::string &Request::body() const
{
	return m_body;
}

std::string Request::basePath() const
{
    return m_headers.value("PATTERN", std::string());
}

std::string Request::methodString() const
{
    return m_headers.value("METHOD", std::string());
}

http::Method Request::method() const
{
	return http::getMethod(methodString());
}

const Query& Request::query() const
{
	return m_query;
}

} // namespace mcpp

