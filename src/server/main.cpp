#include "server.hpp"

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <boost/asio/signal_set.hpp>

struct Options {
	boost::asio::ip::port_type port;
	bool verbose;
};

int main(int argc, char **argv) {
	CLI::App app;
	Options options;

	app.add_option("-p,--port", options.port, "Port")
		->check(CLI::PositiveNumber)
		->required();
	app.add_flag("-v,--verbose", options.verbose, "Verbose output");

	// Parse arguments
	CLI11_PARSE(app, argc, argv);

	try {
		boost::asio::io_context context;
		// Capture SIGINT and SIGTERM to perform a clean shutdown
		boost::asio::signal_set signals(context, SIGINT, SIGTERM);
		signals.async_wait(
			[&](const boost::system::error_code &error, unsigned long) {
				if (error) return;
				// A signal occurred, stop the `io_context`. This will cause
				// `run()` to return immediately, eventually destroying the
				// `io_context` and all of the sockets in it.
				context.stop();
			});
		chat::Server s(context, options.port);
		s.start();
		context.run();
	} catch (const boost::system::system_error &e) {
		spdlog::critical("{}", e.what());
	}
}
