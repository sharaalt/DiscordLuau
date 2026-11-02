# Basic Configuration
cmake_minimum_required(VERSION 3.28)
project(DiscordLuau
	VERSION 0.1.0
	LANGUAGES CXX
)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

# CXX Configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Vcpkg dependencies
find_package(unofficial-luau CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

# Source Files
file(GLOB_RECURSE DISCORD_SOURCE CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_executable(DiscordLuau
	${DISCORD_SOURCE}
)

target_include_directories(DiscordLuau
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link vcpkg libraries
target_link_libraries(DiscordLuau PRIVATE
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
    unofficial::luau::Luau.VM
    unofficial::luau::Luau.Compiler
    unofficial::luau::Luau.Ast
    unofficial::luau::Luau.Require
)