#include <functional>
#include <iostream>
#include <sstream>

#include <mcpp/connection.h>
#include <mcpp/dispatcher.h>

using namespace std::placeholders;

void handler1(mcpp::Connection &connection, const mcpp::Request &req,
              const std::smatch &match)
{
    std::cout << "Got a " << req.methodString() << " request for " << req.path()
              << std::endl;

    std::ostringstream stream;
    stream << "Hello cruel world!";
    connection.replyHttp(req, stream.str());
}

void handler2(mcpp::Connection &connection, const mcpp::Request &req,
              const std::smatch &match)
{
    std::string id = match[0].str();
    std::cout << "Got a " << req.methodString() << " request for " << req.path()
              << " with id " << id << std::endl;

    std::ostringstream stream;
    stream << "The id is: " << id;
    connection.replyHttp(req, stream.str());
}

int main(int, char **)
{

    mcpp::Connection connection("tcp://127.0.0.1:9999", "tcp://127.0.0.1:9998");

    mcpp::Dispatcher dispatcher(connection);

    // Bind paths to handlers.
    dispatcher.assign("/api/?", &handler1);
    dispatcher.assign("/api/([^/]+)/?", &handler2);

    std::cout << "Waiting for requests..." << std::endl;
    for(;;)
    {
        // Wait for a request.
        mcpp::Request req = connection.receive();

        if(!req.isDisconnect())
        {
            // Dispatch the request.
            if(!dispatcher.dispatch(req))
            {
                connection.replyHttp(req, mcpp::http::StatusCode::NOT_FOUND);
            }
        }
    }
}
