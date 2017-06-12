#include <iostream>
#include <sstream>

#include <mcpp/connection.h>

int main(int, char **)
{

    mcpp::Connection connection("tcp://127.0.0.1:9999", "tcp://127.0.0.1:9998");

    std::cout << "Waiting for requests..." << std::endl;
    for(;;)
    {
        // Wait for a request
        mcpp::Request req = connection.receive();
        if(req.isDisconnect())
        {
            std::cout << "Client disconnected." << std::endl;
            continue;
        }

        const std::string &method = req.methodString();
        std::cout << "Got a " << method << " request for " << req.path()
                  << std::endl;

        if(req.methodString() == "WEBSOCKET_HANDSHAKE")
        {
            connection.acceptWebsocket(req);
        }
        else if(req.methodString() == "WEBSOCKET")
        {
            mcpp::websocket::Flags flags =
                mcpp::websocket::decodeFlags(req.headers().value("FLAGS", ""));

            switch(flags.opCode)
            {
                case mcpp::websocket::OpCode::BIN:
                case mcpp::websocket::OpCode::TEXT:
                    connection.replyWebsocket(req, req.body(),
                                              mcpp::websocket::OpCode::TEXT);
                    connection.replyWebsocket(req, "Hello cruel world!!",
                                              mcpp::websocket::OpCode::TEXT);
                    break;
                case mcpp::websocket::OpCode::CLOSE:
                    connection.closeWebsocket(
                        req, mcpp::websocket::StatusCode::CLOSE_NORMAL,
                        "Normal closure.");
                    break;
                case mcpp::websocket::OpCode::PING:
                    connection.replyWebsocket(req, req.body(),
                                              mcpp::websocket::OpCode::PONG);
                    break;
                case mcpp::websocket::OpCode::PONG:
                    break;
                case mcpp::websocket::OpCode::CONT:
                    connection.closeWebsocket(
                        req, mcpp::websocket::StatusCode::CLOSE_NORMAL,
                        "Normal closure.");
                    break;
            }
        }
        else
        {
            connection.replyHttp(req,
                                 mcpp::http::StatusCode::METHOD_NOT_ALLOWED);
        }
    }
}
