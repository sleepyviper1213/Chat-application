#pragma once
#include "alias.hpp"

namespace chat {
template <typename T>
concept http_body = http::is_body<T>::value;
template <typename T>
concept http_fields = http::is_fields<T>::value;
} // namespace chat
