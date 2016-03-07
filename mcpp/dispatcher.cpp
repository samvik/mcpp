#include <mcpp/dispatcher.h>

#include <list>
#include <regex>

namespace mcpp {

class BaseHandleWrapper {
	public:
		virtual ~BaseHandleWrapper();
		virtual void operator()(const Request &request, std::smatch &sm) = 0;
};


BaseHandleWrapper::~BaseHandleWrapper()
{

}

class HandleWrapper0 : public BaseHandleWrapper {
	public:
		HandleWrapper0(Dispatcher::Handle0 handle) : m_handle(handle) {}
		Dispatcher::Handle0 m_handle;

		void operator()(const Request &request, std::smatch &);
};

void HandleWrapper0::operator()(const Request &request, std::smatch &) {
	m_handle(request);
}

class HandleWrapper1 : public BaseHandleWrapper {
	public:
		HandleWrapper1(Dispatcher::Handle1 handle) : m_handle(handle) {}
		Dispatcher::Handle1 m_handle;

		void operator()(const Request &request, std::smatch &sm);
};

void HandleWrapper1::operator()(const Request &request, std::smatch &sm) {
	m_handle(request, sm[1]);
}


class HandleWrapper2 : public BaseHandleWrapper {
	public:
		HandleWrapper2(Dispatcher::Handle2 handle) : m_handle(handle) {}
		Dispatcher::Handle2 m_handle;

		void operator()(const Request &request, std::smatch &sm);
};

void HandleWrapper2::operator()(const Request &request, std::smatch &sm) {
	m_handle(request, sm[1], sm[2]);
}

class HandleWrapper3 : public BaseHandleWrapper {
	public:
		HandleWrapper3(Dispatcher::Handle3 handle) : m_handle(handle) {}
		Dispatcher::Handle3 m_handle;

		void operator()(const Request &request, std::smatch &sm);
};

void HandleWrapper3::operator()(const Request &request, std::smatch &sm) {
	m_handle(request, sm[1], sm[2], sm[3]);
}

class HandleWrapper4 : public BaseHandleWrapper {
	public:
		HandleWrapper4(Dispatcher::Handle4 handle) : m_handle(handle) {}
		Dispatcher::Handle4 m_handle;

		void operator()(const Request &request, std::smatch &sm);
};

void HandleWrapper4::operator()(const Request &request, std::smatch &sm) {
	m_handle(request, sm[1], sm[2], sm[3], sm[4]);
}

struct Route {
		std::string regex;
		BaseHandleWrapper *handle;
};

struct Dispatcher::Impl {
		std::list<Route> m_routes;
};

Dispatcher::Dispatcher() : m_impl(new Impl())
{
}

Dispatcher::~Dispatcher()
{
	for(Route route : m_impl->m_routes) {
		delete route.handle;
	}

	delete m_impl;
}

void Dispatcher::assign(std::string regex, Dispatcher::Handle0 handle)
{
	Route route = {regex, new HandleWrapper0(handle)};
	m_impl->m_routes.push_back(route);
}

void Dispatcher::assign(std::string regex, Dispatcher::Handle1 handle)
{
	Route route = {regex, new HandleWrapper1(handle)};
	m_impl->m_routes.push_back(route);
}

void Dispatcher::assign(std::string regex, Dispatcher::Handle2 handle)
{
	Route route = {regex, new HandleWrapper2(handle)};
	m_impl->m_routes.push_back(route);
}

void Dispatcher::assign(std::string regex, Dispatcher::Handle3 handle)
{
	Route route = {regex, new HandleWrapper3(handle)};
	m_impl->m_routes.push_back(route);
}

void Dispatcher::assign(std::string regex, Dispatcher::Handle4 handle)
{
	Route route = {regex, new HandleWrapper4(handle)};
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
			(*(route.handle))(request, match);
			result = true;
			break;
		}
	}

	return result;
}

} // namespace mcpp

