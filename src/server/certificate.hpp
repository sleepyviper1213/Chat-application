#pragma once

#include "utilities/alias.hpp"

#include <filesystem>

namespace chat {
/**
 * \note Error handling is intentionally left umimplemented. Actually, I find it
 * pretty disastrous to tackle std::fstream or std::filesystem error. Just
 * assume it works as expected. Although an exception is thrown in a unhappy
 * path, but due to boost::asio::deferred, it will not be caught, and this
 * server still runs perfectly fine.
 */
std::string read_file(const std::filesystem::path &path);

ssl::context load_server_cert();

} // namespace chat
