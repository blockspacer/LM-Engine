project(lmng)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_STANDARD 17)
else ()
  set(CMAKE_CXX_STANDARD 20)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})

find_package(lmlib REQUIRED)
find_package(cereal REQUIRED)
find_package(entt REQUIRED)
find_package(Bullet REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Catch2 REQUIRED)

add_library(
  lmng
  SHARED

  src/reflection.cpp
  src/serialisation.cpp
  src/bullet_physics/bullet_physics.cpp
  src/bullet_physics/rigid_body.cpp
  src/bullet_physics/character_controller.cpp
  src/transform.cpp
  src/name.cpp
)

target_include_directories(
  lmng

  PUBLIC
  include
)

target_link_libraries(
  lmng

  PUBLIC
  lmlib::lmlib
  cereal::cereal
  entt::entt
  Bullet::Bullet
  yaml-cpp::yaml-cpp
)

if (MSVC)
  target_compile_definitions(lmng PRIVATE _ENABLE_EXTENDED_ALIGNED_STORAGE)
endif()

add_executable(
  test_lmng
  src/test_lmng.cpp
  src/test_transform.cpp
)

target_link_libraries(
  test_lmng
  PRIVATE
  Catch2::Catch2
  lmng
)

add_test(NAME lmng COMMAND test_lmng)

if(MINGW)
  target_compile_options(lmng PRIVATE "-Wa,-mbig-obj")
endif()
