project(STRUCK)

cmake_minimum_required(VERSION 2.6)

find_package(OpenCV REQUIRED)
MESSAGE( STATUS "OpenCV version: " ${OpenCV_VERSION} )
MESSAGE( STATUS "OpenCV libs: " ${OpenCV_LIBS} )

find_package(CUDA REQUIRED)
MESSAGE( STATUS "CUDA version: " ${CUDA_VERSION} )
MESSAGE( STATUS "CUDA libs: " ${CUDA_LIBS} )

# building a release type would make program faster.
set(CMAKE_BUILD_TYPE Release)

INCLUDE_DIRECTORIES ( src src/GraphUtils /usr/local/include/eigen3 ${OpenCV_INCLUDE_DIRS} )
LINK_DIRECTORIES ( /usr/local/cuda/lib64/ )

add_executable(STRUCK
    src/Config.cpp
    src/Features.cpp
    src/HaarFeature.cpp
    src/HaarFeatures.cpp
    src/HistogramFeatures.cpp
    src/ImageRep.cpp
    src/LaRank.cpp
    src/main.cpp
    src/MultiFeatures.cpp
    src/RawFeatures.cpp
    src/Sampler.cpp
    src/Tracker.cpp
    src/GraphUtils/GraphUtils.cpp
    )
target_link_libraries(STRUCK ${OpenCV_LIBS})

add_executable(struck_tracker
    src/Config.cpp
    src/Features.cpp
    src/HaarFeature.cpp
    src/HaarFeatures.cpp
    src/HistogramFeatures.cpp
    src/ImageRep.cpp
    src/LaRank.cpp
    src/struck_tracking.cpp
    src/MultiFeatures.cpp
    src/RawFeatures.cpp
    src/Sampler.cpp
    src/Tracker.cpp
    src/GraphUtils/GraphUtils.cpp
    )
target_link_libraries(struck_tracker ${OpenCV_LIBS})
