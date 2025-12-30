cmake_minimum_required(VERSION 3.15)
project(DeviceTrustShim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only library - install headers
install(DIRECTORY include/ DESTINATION include)

# Examples
add_executable(radiology_example examples/radiology_device_example.cpp)
target_include_directories(radiology_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(infusion_pump_example examples/infusion_pump_example.cpp)
target_include_directories(infusion_pump_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(dicom_adapter_example examples/dicom_adapter_example.cpp)
target_include_directories(dicom_adapter_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(clinical_trial_adapter_example examples/clinical_trial_adapter_example.cpp)
target_include_directories(clinical_trial_adapter_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(industrial_adapter_example examples/industrial_adapter_example.cpp)
target_include_directories(industrial_adapter_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(building_automation_example examples/building_automation_example.cpp)
target_include_directories(building_automation_example PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Tests
add_executable(test_audit_chain tests/test_audit_chain.cpp)
target_include_directories(test_audit_chain PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Enable testing
enable_testing()
add_test(NAME AuditChainTests COMMAND test_audit_chain)

# Installation
install(TARGETS radiology_example infusion_pump_example 
              dicom_adapter_example clinical_trial_adapter_example
              industrial_adapter_example building_automation_example
        DESTINATION bin)

# Package configuration
set(ConfigPackageLocation lib/cmake/DeviceTrustShim)
install(EXPORT DeviceTrustShimTargets
    FILE DeviceTrustShimTargets.cmake
    NAMESPACE dts::
    DESTINATION ${ConfigPackageLocation})

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

