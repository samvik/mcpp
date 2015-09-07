#include <mcpp/http.h>

namespace mcpp {
namespace http {

std::string statusCodeMessage(mcpp::http::StatusCode code)
{
	std::string msg;
	switch(code) {
		case http::StatusCode::CONTINUE: msg = "Continue"; break;
		case http::StatusCode::SWITCHING_PROTOCOLS: msg = "Switching Protocols"; break;

		case http::StatusCode::OK: msg = "OK"; break;
		case http::StatusCode::CREATED: msg = "Created"; break;
		case http::StatusCode::ACCEPTED: msg = "Accepted"; break;
		case http::StatusCode::NON_AUTHORITATIVE_INFORMATION: msg = "Non-Authoritative Information"; break;
		case http::StatusCode::NO_CONTENT: msg = "No Content"; break;
		case http::StatusCode::RESET_CONTENT: msg = "Reset Content"; break;
		case http::StatusCode::PARTIAL_CONTENT: msg = "Partial Content"; break;

		case http::StatusCode::MULTIPLE_CHOICES: msg = "Multiple Choices"; break;
		case http::StatusCode::MOVED_PERMANENTLY: msg = "Moved Permanently"; break;
		case http::StatusCode::FOUND: msg = "Found"; break;
		case http::StatusCode::SEE_OTHER: msg = "See Other"; break;
		case http::StatusCode::NOT_MODIFIED: msg = "Not Modified"; break;
		case http::StatusCode::USE_PROXY: msg = "Use Proxy"; break;
		case http::StatusCode::TEMPORARY_REDIRECT: msg = "Temporary Redirect"; break;

		case http::StatusCode::BAD_REQUEST: msg = "Bad Request"; break;
		case http::StatusCode::UNAUTHPROZED: msg = "Unauthorized"; break;
		case http::StatusCode::PAYMENT_REQUIRED: msg = "Payment Required"; break;
		case http::StatusCode::FORBIDDEN: msg = "Forbidden"; break;
		case http::StatusCode::NOT_FOUND: msg = "Not Found"; break;
		case http::StatusCode::METHOD_NOT_ALLOWED: msg = "Method Not Allowed"; break;
		case http::StatusCode::NOT_ACCEPTABLE: msg = "Not Acceptable"; break;
		case http::StatusCode::PROXY_AUTHENTICATION_REQUIRED: msg = "Proxy Authentication Required"; break;
		case http::StatusCode::REQUEST_TIMEOUT: msg = "Request Timeout"; break;
		case http::StatusCode::CONFLICT: msg = "Conflict"; break;
		case http::StatusCode::GONE: msg = "Gone"; break;
		case http::StatusCode::LENGTH_REQUIRED: msg = "Length Required"; break;
		case http::StatusCode::PRECONDITION_FAILED: msg = "Precondition Failed"; break;
		case http::StatusCode::REQUEST_ENTITY_TOO_LARGE: msg = "Request Entity Too Large"; break;
		case http::StatusCode::REQUEST_URI_TOO_LONG: msg = "Request-URI Too Long"; break;
		case http::StatusCode::UNSUPPORTED_MEDIA_TYPE: msg = "Unsupported Media Type"; break;
		case http::StatusCode::REQUESTED_RANGE_NOT_SATISFIABLE: msg = "Requested Range Not Satisfiable"; break;
		case http::StatusCode::EXPECTATION_FAILED: msg = "Expectation Failed"; break;

		case http::StatusCode::INTERNAL_SERVER_ERROR: msg = "Internal Server Error"; break;
		case http::StatusCode::NOT_IMPLEMENTED: msg = "Not Implemented"; break;
		case http::StatusCode::BAD_GATEWAY: msg = "Bad Gateway"; break;
		case http::StatusCode::SERVICE_UNAVAILABLE: msg = "Service Unavailable"; break;
		case http::StatusCode::GATEWAY_TIMEOUT: msg = "Gateway Timeout"; break;
		case http::StatusCode::HTTP_VERSION_NOT_SUPPORTED: msg = "HTTP Version Not Supported"; break;
	}
	return msg;
}

}
} // namespace mcpp

