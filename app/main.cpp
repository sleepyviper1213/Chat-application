#include "chat.hpp"

#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
	try {
		if (argc != 5) {
			spdlog::error("Usage: <listen_address> <listen_port> "
						  "<remote_address> <remote_port>",
						  argv[0]);
			return 1;
		}

		chat::ChatSession(argv[1], argv[2], argv[3], argv[4]);

	} catch (std::exception &e) { spdlog::error("Exception: {}", e.what()); }
}
