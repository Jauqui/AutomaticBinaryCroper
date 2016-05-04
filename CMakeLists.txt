cmake_minimum_required(VERSION 2.8)

project(testCroper)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
endif()

INCLUDE(CTest)
ENABLE_TESTING()
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

# Add a simple test
ADD_TEST(CDashTest1 testCroper testCroper)
add_executable(testCroper MACOSX_BUNDLE main.cpp itkAutomaticCropImageFilter.cxx)

if( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(testCroper ITKReview ${ITK_LIBRARIES})
else( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(testCroper ${ITK_LIBRARIES})
endif( "${ITK_VERSION_MAJOR}" LESS 4 )
