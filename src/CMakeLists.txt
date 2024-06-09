add_library(Chat chat.cpp)

target_compile_options(Chat PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(Chat PUBLIC spdlog::spdlog fmt::fmt)
