#ifndef MCPP_UTILITY_H
#define MCPP_UTILITY_H

#include <mcpp/query.h>
#include <string>

namespace mcpp
{
namespace utility
{

/**
 * @brief Decode an url.
 * @param in Url to decode.
 * @param out Decoded url.
 * @return True if successfull, false otherwise.
 */
bool decodeUrl(const std::string &in, std::string &out);

/**
 * @brief Read a netstring from given stream.
 * @param stream Stream to read from.
 * @return The read netstream as a normal string.
 */
std::string readNetString(std::istringstream &stream);

/**
 * @brief Parse the query parameters.
 * @param query Decoded query string.
 * @param parameters Object to store the parsed queries in.
 */
void parseQuery(const std::string &query, Query &parameters);
}
} // namespace mcpp

#endif // MCPP_UTILITY_H
