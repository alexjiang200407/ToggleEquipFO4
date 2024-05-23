include("${CMAKE_CURRENT_LIST_DIR}/CommonLibF4-targets.cmake")
include(CMakeFindDependencyMacro)

find_dependency(mmio CONFIG)
find_dependency(spdlog CONFIG)
