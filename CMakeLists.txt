cmake_minimum_required(VERSION 3.15)
project(DeviceTrustShim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the header-only library interface target
add_library(dts::DeviceTrustShim INTERFACE)
target_include_directories(dts::DeviceTrustShim INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)

# Installation of headers
install(TARGETS dts::DeviceTrustShim EXPORT DeviceTrustShimTargets
        DESTINATION include)

# Examples
add_executable(radiology_example examples/radiology_device_example.cpp)
target_link_libraries(radiology_example PRIVATE dts::DeviceTrustShim)

add_executable(infusion_pump_example examples/infusion_pump_example.cpp)
target_link_libraries(infusion_pump_example PRIVATE dts::DeviceTrustShim)

add_executable(dicom_adapter_example examples/dicom_adapter_example.cpp)
target_link_libraries(dicom_adapter_example PRIVATE dts::DeviceTrustShim)

add_executable(clinical_trial_adapter_example examples/clinical_trial_adapter_example.cpp)
target_link_libraries(clinical_trial_adapter_example PRIVATE dts::DeviceTrustShim)

add_executable(industrial_adapter_example examples/industrial_adapter_example.cpp)
target_link_libraries(industrial_adapter_example PRIVATE dts::DeviceTrustShim)

add_executable(building_automation_example examples/building_automation_example.cpp)
target_link_libraries(building_automation_example PRIVATE dts::DeviceTrustShim)

# Tests
add_executable(test_audit_chain tests/test_audit_chain.cpp)
target_link_libraries(test_audit_chain PRIVATE dts::DeviceTrustShim)

# Enable testing
enable_testing()
add_test(NAME AuditChainTests COMMAND test_audit_chain)

# Install executables
install(TARGETS radiology_example infusion_pump_example 
              dicom_adapter_example clinical_trial_adapter_example
              industrial_adapter_example building_automation_example
              test_audit_chain
        DESTINATION bin)

# Package configuration
set(ConfigPackageLocation lib/cmake/DeviceTrustShim)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/DeviceTrustShimConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/DeviceTrustShimConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/DeviceTrustShimConfig.cmake"
    INSTALL_DESTINATION ${ConfigPackageLocation})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/DeviceTrustShimConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/DeviceTrustShimConfigVersion.cmake"
    DESTINATION ${ConfigPackageLocation})

install(EXPORT DeviceTrustShimTargets
    FILE DeviceTrustShimTargets.cmake
    NAMESPACE dts::
    DESTINATION ${ConfigPackageLocation})

