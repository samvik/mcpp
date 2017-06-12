#ifndef MCPP_DISPATCHER_H
#define MCPP_DISPATCHER_H

#include <functional>
#include <regex>
#include <string>

#include <mcpp/request.h>

namespace mcpp
{

class Connection;

/**
 * @brief Request dispatcher.
 *
 * Used to map request path patterns to handler functions.
 */
class Dispatcher
{
public:
    Dispatcher(Connection &connection);
    ~Dispatcher();

    typedef std::vector<std::string> Arguments;
    typedef std::function<void(Connection &connection, const Request &request,
                               std::smatch match)>
        Handler;

    /**
     * @brief Assign a handler.
     *
     * Example
     *
     * dispatcher.assign("/items/?", handler);
     *
     * @param regex Regex to match.
     * @param handle Handler to be called.
     */
    void assign(std::string regex, Handler handler);

    /**
     * @brief Dispatch a request.
     *
     * This will call the first matching handle assigend.
     *
     * @param request Request to dispatch.
     * @return True if request was dispatched, false otherwise.
     */
    bool dispatch(const Request &request);

private:
    struct Impl;
    Impl *m_impl;
};

} // namespace mcpp

#endif // MCPP_DISPATCHER_H
