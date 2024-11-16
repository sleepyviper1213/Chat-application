#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket.hpp>

namespace chat {

namespace asio = boost::asio;
using tcp      = asio::ip::tcp;         // from <boost/asio/ip/tcp.hpp>

namespace beast     = boost::beast;     // from <boost/beast.hpp>
namespace http      = beast::http;      // from <boost/beast/http.hpp>
namespace ssl       = asio::ssl;        // from <boost/asio/ssl.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>

} // namespace chat
