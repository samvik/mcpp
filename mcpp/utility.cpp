#include <mcpp/utility.h>

#include <regex>
#include <sstream>

namespace mcpp
{
namespace utility
{

bool decodeUrl(const std::string &in, std::string &out)
{
    bool result = true;

    out.clear();
    out.reserve(in.size());

    for(std::size_t i = 0; i < in.size(); ++i)
    {
        if(in[i] == '%')
        {
            if(i + 3 <= in.size())
            {
                std::istringstream stream(in.substr(i + 1, 2));
                int value;
                if(stream >> std::hex >> value)
                {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else
                {
                    result = false;
                    break;
                }
            }
            else
            {
                result = false;
                break;
            }
        }
        else if(in[i] == '+')
        {
            out += ' ';
        }
        else
        {
            out += in[i];
        }
    }

    return result;
}

std::string readNetString(std::istringstream &stream)
{
    uint64_t length;
    stream >> length;
    stream.ignore(); // Ignore ':'

    std::string data;
    data.reserve(length);

    std::copy_n(std::istreambuf_iterator<char>(stream), length,
                std::back_inserter(data));
    stream.ignore(2); // Last caracter and ending ','

    return data;
}

void parseQuery(const std::string &query, Query &parameters)
{
    std::regex separator("&");

    std::sregex_token_iterator it(query.begin(), query.end(), separator, -1);
    std::sregex_token_iterator end;

    for(; it != end; ++it)
    {
        const std::string &token = *it;

        std::size_t pos = token.find('=');

        std::string key, value;
        if(pos != std::string::npos)
        {
            key = token.substr(0, pos);
            value = token.substr(pos + 1);
        }
        else
        {
            key = token;
        }

        parameters[key] = value;
    }
}
}
} // namespace mcpp
