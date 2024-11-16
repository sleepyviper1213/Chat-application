#include "certificate.hpp"

#include <fstream>

namespace chat {
std::string read_file(const std::filesystem::path &path) {
	std::ifstream file(path);

	const std::string content{std::istreambuf_iterator<char>(file),
							  std::istreambuf_iterator<char>()};
	return content;
}

ssl::context load_server_cert() {
	ssl::context ctx{ssl::context::tlsv12};

	const auto cert = read_file(PROJECT_SOURCE_DIR "/src/cert/cert.pem");
	const auto key  = read_file(PROJECT_SOURCE_DIR "/src/cert/key.pem");
	const auto dh   = read_file(PROJECT_SOURCE_DIR "/src/cert/dh.pem");

	if (cert.empty() || dh.empty() || key.empty())
		throw boost::system::errc::no_such_file_or_directory;
	ctx.set_password_callback(
		[](size_t, ssl::context::password_purpose) { return "test"; });

	ctx.set_options(ssl::context::default_workarounds | ssl::context::no_sslv2 |
					ssl::context::single_dh_use);

	ctx.use_certificate_chain(asio::buffer(cert));

	ctx.use_private_key(asio::buffer(key), ssl::context::file_format::pem);

	ctx.use_tmp_dh(asio::buffer(dh));
	return ctx;
}
} // namespace chat
