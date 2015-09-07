#include "request.h"

#include <zmq.hpp>
#include <json/json.h>

#include <sstream>

#include "utility.h"

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
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(headers, m_headers);
	if ( !parsingSuccessful )
	{
		// TODO: Throw exception
	}

	// Parse query parameters
	std::string query = m_headers.get("QUERY", std::string()).asString();
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

const Json::Value &Request::headers() const
{
	return m_headers;
}

const std::string &Request::body() const
{
	return m_body;
}

const std::string Request::basePath() const
{
	return m_headers.get("PATTERN", std::string()).asString();
}

const std::string Request::method() const
{
	return m_headers.get("METHOD", std::string()).asString();
}

const Query& Request::query() const
{
	return m_query;
}

} // namespace mcpp

