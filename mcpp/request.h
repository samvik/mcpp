#ifndef MCPP_REQUEST_H
#define MCPP_REQUEST_H

#include <map>
#include <json/json.h>

#include <mcpp/query.h>

namespace zmq {
	class message_t;
}

namespace mcpp {

/**
 * @brief A request received from mongrel on behalf of a client.
 */
class Request
{
	public:
		/**
		 * @brief Construct an invalid request.
		 */
		Request();

		/**
		 * @brief Construct a request from a zeromq message.
		 *
		 * @param message Zeromq message.
		 */
		Request(const zmq::message_t &message);
		~Request();

		/**
		 * @brief Returns true if this is a valid request.
		 * @return True if valid, false otherwise.
		 */
		bool isValid() const;

		/**
		 * @brief Get sender uuid.
		 *
		 * This corresponds to the mongrel instance that sent the request.
		 *
		 * @return The sender uuid.
		 */
		const std::string& sender() const;

		/**
		 * @brief Client connection id.
		 *
		 * Assigned client connection id.
		 *
		 * @return Connection id.
		 */
		unsigned connectionId() const;

		/**
		 * @brief Get request path.
		 * @return Request path.
		 */
		const std::string& path() const;

		/**
		 * @brief Get request headers.
		 * @return Request headers.
		 */
		const Json::Value& headers() const;

		/**
		 * @brief Get request body.
		 * @return Request body.
		 */
		const std::string& body() const;

		/**
		 * @brief Get base path.
		 *
		 * Mongrel calls this PATTERN, which is the pattern the path matched to be
		 * sent to this handler.
		 *
		 * This is a convenience function. The same data can be accessd through the
		 * headers.
		 *
		 * @return Base path.
		 */
		const std::string basePath() const;

		/**
		 * @brief Get method.
		 *
		 * This is a convenience function. The same data can be retreivedfrom the
		 * headers.
		 *
		 * @return Request method.
		 */
		const std::string method() const;

		/**
		 * @brief Get query parameters.
		 *
		 * The raw query parameters can be retreived from the headers.
		 *
		 * @return Query parameters.
		 */
		const Query& query() const;

	private:
		bool m_valid;

		std::string m_sender;
		unsigned m_connectionId;
		Json::Value m_headers;
		std::string m_body;

		std::string m_path;

		Query m_query;
};

} // namespace mcpp

#endif // MCPP_REQUEST_H
