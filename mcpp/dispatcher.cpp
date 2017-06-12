#include <mcpp/dispatcher.h>

#include <regex>
#include <vector>

#include "connection.h"

namespace mcpp {

struct Route {
        std::string regex;
        Dispatcher::Handler handler;
};

struct Dispatcher::Impl {
        Impl(Connection &connection) : connection_(connection) {}
        Connection &connection_;
        std::vector<Route> m_routes;
};

Dispatcher::Dispatcher(Connection &connection) : m_impl(new Impl(connection))
{
}

Dispatcher::~Dispatcher()
{
    delete m_impl;
}

void Dispatcher::assign(std::string regex, Dispatcher::Handler handler)
{
    Route route = {regex, handler};
    m_impl->m_routes.push_back(route);
}

bool Dispatcher::dispatch(const Request &request)
{
	bool result = false;

	// Calculate dispatch path.
	std::string path = request.path().substr(request.basePath().length());

    for(Route &route : m_impl->m_routes) {
        std::regex regex (route.regex);

        std::smatch match;
        if (std::regex_match (path, match, regex)) {
            route.handler(m_impl->connection_, request, match);
            result = true;
            break;
        }
    }

	return result;
}

} // namespace mcpp

