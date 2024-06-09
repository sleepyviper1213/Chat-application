#pragma once

#include <boost/asio.hpp>
#include <fmt/format.h>

template <typename InternetProtocol>
struct fmt::formatter<boost::asio::ip::basic_endpoint<InternetProtocol>>
	: formatter<std::string> {
	auto format(boost::asio::ip::basic_endpoint<InternetProtocol> &endpoint,
				format_context &ctx) const {
		boost::asio::ip::detail::endpoint tmp_ep(endpoint.address(),
												 endpoint.port());
		return formatter<std::string>::format(tmp_ep.to_string(), ctx);
	}
};
