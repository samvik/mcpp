#include <iostream>
#include <sstream>
#include <functional>

#include <mcpp/connection.h>
#include <mcpp/dispatcher.h>

using namespace std::placeholders;

void handler1(mcpp::Connection &connection, const mcpp::Request &req) {
	std::cout << "Got a " << req.methodString() << " request for " << req.path() << std::endl;

	std::ostringstream stream;
	stream << "Hello cruel world!";
	connection.replyHttp(req, stream.str());
}

void handler2(mcpp::Connection &connection, const mcpp::Request &req, std::string id) {
	std::cout << "Got a " << req.methodString() << " request for " << req.path() << " with id " << id << std::endl;

	std::ostringstream stream;
	stream << "The id is: " << id;
	connection.replyHttp(req, stream.str());
}

int main(int, char **) {

	mcpp::Connection connection("tcp://127.0.0.1:9999", "tcp://127.0.0.1:9998");

	mcpp::Dispatcher dispatcher;

	// Bind paths to handlers.
	auto f1 = std::bind(handler1, std::ref(connection), _1);
	dispatcher.assign("/api/?", static_cast<mcpp::Dispatcher::Handle0>(f1));

	auto f2 = std::bind(handler2, std::ref(connection), _1, _2);
	dispatcher.assign("/api/([^/]+)/?", static_cast<mcpp::Dispatcher::Handle1>(f2));

	std::cout << "Waiting for requests..." << std::endl;
	for(;;) {
		// Wait for a request.
		mcpp::Request req = connection.receive();

		if(!req.isDisconnect()) {
			// Dispatch the request.
			if(!dispatcher.dispatch(req)) {
				connection.replyHttp(req, mcpp::http::StatusCode::NOT_FOUND);
			}
		}
	}
	
}
