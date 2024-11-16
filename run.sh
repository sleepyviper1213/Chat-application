set -e # Exit early if any commands fail
(
  cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
  cmake -B build/Debug -S .
  cmake --build ./build/Debug
)

exec ./build/Debug/src/ChatServer -p 8080 "$@"
