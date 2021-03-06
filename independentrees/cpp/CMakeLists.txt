project(latentrees)
cmake_policy(SET CMP0044 NEW)
cmake_minimum_required(VERSION 3.18)

include_directories(.
                    ../pcg-cpp/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/models/interfaces
                    ${CMAKE_CURRENT_SOURCE_DIR}/models/include)


set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(LAT_FILES generator.cpp)
set(MODELS models/uniformModel.cpp models/gausModel.cpp models/nbinModel.cpp models/model.cpp)

add_executable(lat
    main.cpp
    ${LAT_FILES}
    ${MODELS}
)

target_link_libraries(lat Threads::Threads)

add_subdirectory(tests)
