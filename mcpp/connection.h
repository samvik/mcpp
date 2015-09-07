#ifndef MCPP_CONNECTION_H
#define MCPP_CONNECTION_H

#include <string>
#include <zmq.hpp>

#include <json/json.h>

#include <mcpp/request.h>
#include <mcpp/http.h>
#include <mcpp/websocket.h>

namespace mcpp {

/**
 * @brief Represents a connection to mongrel.
 *
 * Implements functions to receive requests and respond to them in different
 * ways.
 */
class Connection
{
public:
		/**
		 * @brief Open a connection
		 * @param pullAddress Mongrel2 pull connection string.
		 * @param pubAddress Mongrel2 publish connection string.
		 */
		Connection(std::string pullAddress, std::string pubAddress);

		/**
		 * @brief Open a connection
		 *
		 * The instance uuid is used by zeromq see ZMQ_IDENTITY
		 * (http://api.zeromq.org/2-1:zmq-setsockopt#toc6)
		 *
		 * @param uuid Instance uuid of this connection.
		 * @param pullAddress Mongrel2 pull connection string.
		 * @param pubAddress Mongrel2 publish connection string.
		 */
		Connection(std::string uuid, std::string pullAddress,
               std::string pubAddress);

		virtual ~Connection();

		/**
		 * @brief Wait for a request indefinitely.
		 * @return Received request.
		 */
		Request recv();

		/**
		 * @brief Receive a request.
		 * @param request The request will be written to this reference.
		 * @param timeout Number of micro seconds to wait. A value of -1 will wait indefinitely.
		 * @return True if a request was received, false otherwise.
		 */
		bool recv(Request &request, long timeout);


	protected:
		/**
		 * @brief Send data to mongrel.
		 *
		 * @param data Data to send.
		 * @return True if the data was successfully sent to mongrel, false otherwise.
		 */
    bool send(const std::string &data);

	public:
		/**
		 * @brief Deliver data to given ids.
		 * @param uuid Mongrel instance uuid.
		 * @param ids Vector of recipient ids.
		 * @param data Data to deliver.
		 * @return True if the data was successfully delivered to mongrel, false otherwise.
		 */
		bool deliver(const std::string &uuid, const std::vector<unsigned> &ids,
                 const std::string &data );

		/**
		 * @brief Reply to a request.
		 * @param request Request to reply to.
		 * @param data Data to reply with.
		 * @return True if successfull, false otherwise.
		 */
		bool reply(const Request &request, const std::string &data);

		/**
		 * @brief Reply to a request with a http message.
		 * @param request Request to reply to.
		 * @param content Http content.
		 * @param code Http status code.
		 * @param status Http status message.
		 * @param headers Http headers.
		 * @return True if successfull, false otherwise.
		 */
    bool replyHttp(const Request &request, const std::string &content,
                   uint16_t code, const std::string &status,
									 const Json::Value &headers = Json::Value());

		/**
		 * @brief Reply to a request with a http message.
		 *
		 * This version will fill in the http status message automatically based
		 * on the status code.
		 *
		 * @param request Request to reply to.
		 * @param content Http content.
		 * @param code Http status code.
		 * @param headers Http headers.
		 * @return True if successfull, false otherwise.
		 */
    bool replyHttp(const Request &request, const std::string &content,
                   http::StatusCode code = http::StatusCode::OK,
									 const Json::Value &headers = Json::Value());

		/**
		 * @brief Reply to a request with a http message.
		 *
		 * This version till reply without content.
		 *
		 * @param request Request to reply to.
		 * @param code Http status code.
		 * @param headers Http headers.
		 * @return True if successfull, false otherwise.
		 */
		bool replyHttp(const Request &request,
									 http::StatusCode code = http::StatusCode::OK,
									 const Json::Value &headers = Json::Value());

		/**
		 * @brief Reply to a request with a json http message.
		 * @param request Request to reply to.
		 * @param json Json content.
		 * @param code Http status code.
		 * @param headers Http headers.
		 * @return True if successfull, false otherwise.
		 */
		bool replyHttpJson(const Request &request, const Json::Value &json,
                       http::StatusCode code = http::StatusCode::OK,
											 const Json::Value &headers = Json::Value());


		/**
		 * @brief Accept a websocket connection.
		 *
		 * Used to accept a websocket connection when a request with the
		 * method "WEBSOCKET_HANDSHAKE" is received.
		 *
		 * @param request The request to reply to.
		 * @param headers Additional http headers.
		 * @return True if successfull, false otherwise.
		 */
    bool acceptWebsocket(const Request &request,
												 Json::Value headers = Json::Value());

		/**
		 * @brief Reject a websocket connection.
		 *
		 * Used to reject a websocket connection when a request with the
		 * method "WEBSOCKET_HANDSHAKE" is received.
		 *
		 * @param request Request to reply to.
		 * @param headers Additional http headers.
		 * @return True if successfull, false otherwise.
		 */
    bool rejectWebsocket(const Request &request,
												 Json::Value headers = Json::Value());

		/**
		 * @brief Reply to a websocket request.
		 *
		 * Used to reply to a request with the method "WEBSOCKET".
		 *
		 * @param request The request to reply to.
		 * @param data Data to reply with.
		 * @param opcode Websocket operation code. See mcpp::websocket::OpCode.
		 * @param rsvd Websocket extension, should be 0 if not used.
		 * @return True if successfull, false otherwise.
		 */
    bool replyWebsocket(const Request &request, const std::string &data,
                        websocket::OpCode opcode, char rsvd = 0x00);

		/**
		 * @brief Deliver data to websocket clients.
		 *
		 * Can be executed at any time, provided that the client ids are known.
		 *
		 * @param uuid Mongrel instance uuid.
		 * @param ids Vector of recipient ids.
		 * @param data Data to deliver.
		 * @param opcode Websocker operation code. See mcpp::websocket::OpCode.
		 * @param rsvd Websocket extension, should be 0 if not used.
		 * @return True if successfull, false otherwise.
		 */
    bool deliverWebsocket(const std::string &uuid,
                          const std::vector<unsigned> ids,
                          const std::string &data,
                          websocket::OpCode opcode, char rsvd = 0x00);

		/**
		 * @brief  Deliver json to websocket clients.
		 *
		 * Can be executed at any time, provided that the client ids are known.
		 *
		 * @param uuid Mongrel instance uuid.
		 * @param ids Vector of recipient ids.
		 * @param json Json to deliver.
		 * @param opcode Websocker operation code. See mcpp::websocket::OpCode.
		 * @param rsvd Websocket extension, should be 0 if not used.
		 * @return True if successfull, false otherwise.
		 */
    bool deliverWebsocketJson(const std::string &uuid,
                              const std::vector<unsigned> ids,
															const Json::Value &json,
                              websocket::OpCode opcode, char rsvd = 0x00);

		/**
		 * @brief Close a websocket connection.
		 * @param request Request to reply to.
		 * @param code Websocket status code.
		 * @param reason Descriptive reason to why the connection was closed.
		 * @return True if successfull, false otherwise.
		 */
    bool closeWebsocket(const Request &request, websocket::StatusCode code,
                        std::string reason);

		/**
		 * @brief Close websocket connections.
		 * @param uuid Mongrel instance uuid.
		 * @param ids Connection ids to close.
		 * @param code Websocket status code.
		 * @param reason Descriptive reason to why the connection was closed.
		 * @return True if successfull, false otherwise.
		 */
		bool closeWebsocket(const std::string &uuid,
												const std::vector<unsigned> ids,
												websocket::StatusCode code,
												std::string reason);

		/**
		 * @brief Max number of ids to deliver to with one call.
		 */
    static const unsigned MAX_IDS = 128;

private:
    zmq::context_t m_context;
    zmq::socket_t m_pull;
    zmq::socket_t m_pub;
};

} // namespace mcpp

#endif // MCPP_CONNECTION_H
