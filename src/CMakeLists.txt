add_executable(
    ChatServer server/certificate.cpp server/room.cpp server/server.cpp
               server/websocket_session.cpp server/main.cpp)

target_compile_definitions(
    ChatServer PRIVATE BOOST_ASIO_NO_DEPRECATED
                       PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(ChatServer BEFORE PRIVATE -Wall -Wextra -Wpedantic
                                                     -fsanitize=address)
    target_link_options(ChatServer PRIVATE -fsanitize=address)
endif()
target_link_libraries(
    ChatServer
    PRIVATE spdlog::spdlog Boost::system CLI11::CLI11
            nlohmann_json::nlohmann_json OpenSSL::SSL OpenSSL::Crypto)
