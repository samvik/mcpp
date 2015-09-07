#ifndef MCPP_DISPATCHER_H
#define MCPP_DISPATCHER_H

#include <string>
#include <functional>

#include "request.h"

namespace mcpp {

/**
 * @brief Request dispatcher.
 *
 * Used to map request path patterns to handler functions.
 */
class Dispatcher
{
	public:
		Dispatcher();
		~Dispatcher();

		/**
		 * @brief Function to handle requests with 0 parameters.
		 */
		typedef std::function<void(const Request &request)> Handle0;

		/**
		 * @brief Function to handle requests with 1 parameter.
		 */
		typedef std::function<void(const Request &request, std::string)> Handle1;

		/**
		 * @brief Function to handle requests with 2 parameters.
		 */
		typedef std::function<void(const Request &request, std::string, std::string)> Handle2;

		/**
		 * @brief Function to handle requests with 3 parameters.
		 */
		typedef std::function<void(const Request &request, std::string, std::string, std::string)> Handle3;

		/**
		 * @brief Function to handle requests with 4 parameters.
		 */
		typedef std::function<void(const Request &request, std::string, std::string, std::string, std::string)> Handle4;

		/**
		 * @brief Assign a handle.
		 *
		 * Example
		 *
		 * dispatcher.assign("/items/?", handle);
		 *
		 * @note The number of captures in the regex should be greater or equal to
		 * the number of parameters in the handle.
		 *
		 * @param regex Regex to match.
		 * @param handle Handle to be called.
		 */
		void assign(std::string regex, Handle0 handle);

		/**
		 * @brief Assign a handle.
		 *
		 * Example
		 *
		 * dispatcher.assign("/([0-9]{4})/?", handle);
		 *
		 * @note The number of captures in the regex should be greater or equal to
		 * the number of parameters in the handle.
		 *
		 * @param regex Regex to match.
		 * @param handle Handle to be called.
		 */
		void assign(std::string regex, Handle1 handle);

		/**
		 * @brief Assign a handle.
		 *
		 * @note The number of captures in the regex should be greater or equal to
		 * the number of parameters in the handle.
		 *
		 * @param regex Regex to match.
		 * @param handle Handle to be called.
		 */
		void assign(std::string regex, Handle2 handle);

		/**
		 * @brief Assign a handle.
		 *
		 * @note The number of captures in the regex should be greater or equal to
		 * the number of parameters in the handle.
		 *
		 * @param regex Regex to match.
		 * @param handle Handle to be called.
		 */
		void assign(std::string regex, Handle3 handle);

		/**
		 * @brief Assign a handle.
		 *
		 * @note The number of captures in the regex should be greater or equal to
		 * the number of parameters in the handle.
		 *
		 * @param regex Regex to match.
		 * @param handle Handle to be called.
		 */
		void assign(std::string regex, Handle4 handle);

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
