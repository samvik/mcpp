#include <iostream>
#include <sstream>

#include <mcpp/connection.h>

int main(int, char **) {

	mcpp::Connection connection("tcp://127.0.0.1:9999",
															"tcp://127.0.0.1:9998");
	
	int count = 0;
	
	std::cout << "Waiting for requests..." << std::endl;
	for(;;) {

		// Wait for a request
		mcpp::Request req = connection.recv();
		
		std::cout << "Got a " << req.method() << " request for " << req.path() << std::endl;

		// Prepare and send response.
		if(req.method() == "GET") {
			std::ostringstream stream;
			stream << "Hello cruel world #" << ++count << "!";

			connection.replyHttp(req, stream.str());
		}
		else {
			connection.replyHttp(req, mcpp::http::StatusCode::METHOD_NOT_ALLOWED);
		}
	}
	
}
