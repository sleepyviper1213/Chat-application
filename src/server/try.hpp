#pragma once
#include <expected>

namespace chat {
// Use GCC and Clang extension called statement expressions.
#define TRY(expr)                                                              \
	({                                                                         \
		const auto result = (expr);                                            \
		if (!result.has_value()) [[unlikely]]                                  \
			return std::unexpected<decltype(result)::error_type>{              \
				std::in_place,                                                 \
				result.error()};                                               \
		result.value();                                                        \
	})

#define LET(expr, callback)                                                    \
	({                                                                         \
		const auto result = (expr);                                            \
		if (result.has_value()) [[likely]]                                     \
			(callback);                                                        \
		result.value();                                                        \
	})
} // namespace chat
