# Specify the minimum required version of CMake.
# This ensures that the version of CMake being used supports the required features.
cmake_minimum_required(VERSION 3.10)

# Define the name of the project.
# "RealmWeaver" is the name of your project. You can change this to anything you like.
project(RealmWeaver)

# Set the C++ standard to use (C++11 in this case).
# This ensures that your compiler will use the correct version of C++.

set(CMAKE_CXX_STANDARD 17)  # Set C++ standard to C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Require this standard
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# Include the header files from the "include" directory.
# This tells the compiler where to find header files during compilation.
include_directories(
    ${CMAKE_SOURCE_DIR}/include             #General include dir
    ${CMAKE_SOURCE_DIR}/src/gemini          # Gemini module headers
    ${CMAKE_SOURCE_DIR}/src/core            #Core game engine headers
    ${CMAKE_SOURCE_DIR}/src/stats           # Stats module headers
    ${CMAKE_SOURCE_DIR}/src/item       #Equipment module headers
    ${CMAKE_SOURCE_DIR}/src/event           #Event module headers
    ${CMAKE_SOURCE_DIR}/src/interactions    #core game engine headers
    ${CMAKE_SOURCE_DIR}/src/player          # Player module headers
    ${CMAKE_SOURCE_DIR}/src/npc             # NPC module headers
    ${CMAKE_SOURCE_DIR}/src/quest           # Quest module headers
    ${CMAKE_SOURCE_DIR}/src/inventory       # Inventory module headers
    ${CMAKE_SOURCE_DIR}/src/location        # Location module headers
    )

# Find the CURL library. This searches for the required CURL package on your system.
# REQUIRED means that the CMake process will fail if CURL is not found.
find_package(CURL REQUIRED)

# Define the source files for your project.
# This variable "SOURCES" contains the path to your source files (in this case, "src/main.cpp").
set(GEMINI_SOURCES
    src/gemini/Gemini.cpp
)

set(CORE_SOURCES
    src/core/GameEngine.cpp
)

set(PLAYER_SOURCES
    src/player/Player.cpp
)

set(STATS_SOURCES
    src/stats/Stats.cpp
)

set(NPC_SOURCES
    src/npc/NPC.cpp
)

set(QUEST_SOURCES
    src/quest/Quest.cpp
)

set(LOCATION_SOURCES
    src/location/Location.cpp
)

set(INVENTORY_SOURCES
    src/inventory/Inventory.cpp
)

set(ITEM_SOURCES
    src/item/Item.cpp
)

set(EVENT_SOURCES
    src/event/EventManager.cpp
)

set(INTERACTIONS_SOURCES
    src/interactions/InteractionSystem.cpp
)


# Aggregate all source files into one variable.
# Add all the source files for the different modules to this list.
set(SOURCES
    ${GEMINI_SOURCES}
    ${CORE_SOURCES}
    ${PLAYER_SOURCES}
    ${STATS_SOURCES}
    ${NPC_SOURCES}
    ${QUEST_SOURCES}
    ${LOCATION_SOURCES}
    ${INVENTORY_SOURCES}
    ${ITEM_SOURCES}
    ${EVENT_SOURCES}
    ${INTERACTIONS_SOURCES}
)

# Add an executable target. This tells CMake to generate an executable named "main"
# using the source files specified in the SOURCES variable.
add_executable(RealmWeaver ${SOURCES})

# Link against the CURL library.
# This ensures that the compiled program will be linked with the CURL libraries, which are required for using libcurl functions.
target_link_libraries(RealmWeaver ${CURL_LIBRARIES})
