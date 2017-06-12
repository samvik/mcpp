#ifndef MCPP_HTTPSTATUSCODE_H
#define MCPP_HTTPSTATUSCODE_H

#include <cinttypes>
#include <string>

namespace mcpp
{
namespace http
{

/**
 * @brief Standard http methods.
 */
enum class Method
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    OPTIONS,
    CONNECT,
    PATCH,
    CUSTOM
};

/**
 * @brief Http status codes.
 */
enum class StatusCode : uint16_t
{
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,

    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,

    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,

    BAD_REQUEST = 400,
    UNAUTHPROZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

/**
 * @brief Get http message associated with a given status code.
 * @param code Status code.
 * @return Http message.
 */
std::string statusCodeMessage(StatusCode code);

/**
 * @brief Get the method as an enum.
 * @param method Method as string.
 * @return Method as enum.
 */
Method getMethod(std::string method);
}
} // namespace mcpp

#endif // MCPP_HTTPSTATUSCODE_H
