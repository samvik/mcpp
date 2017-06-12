#include <iostream>
#include <sstream>

#include <mcpp/connection.h>

int main(int, char **)
{

    mcpp::Connection connection("tcp://127.0.0.1:9999", "tcp://127.0.0.1:9998");

    int count = 0;

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

        std::cout << "Got a " << req.methodString() << " request for "
                  << req.path() << std::endl;

        // Prepare and send response.
        if(req.method() == mcpp::http::Method::GET)
        {
            std::ostringstream stream;
            stream << "Hello cruel world #" << ++count << "!";

            connection.replyHttp(req, stream.str());
        }
        else
        {
            connection.replyHttp(req,
                                 mcpp::http::StatusCode::METHOD_NOT_ALLOWED);
        }
    }
}
