# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.
add_custom_command(
  OUTPUT getting_report_u.h getting_report_u.c getting_report_args.h
  DEPENDS ${CMAKE_SOURCE_DIR}/getting_report.edl
  COMMAND
    openenclave::oeedger8r --untrusted ${CMAKE_SOURCE_DIR}/getting_report.edl
    --search-path ${OE_INCLUDEDIR} --search-path
    ${OE_INCLUDEDIR}/openenclave/edl/sgx)

add_executable(getting_report_host host.c
                               ${CMAKE_CURRENT_BINARY_DIR}/getting_report_u.c)

if (WIN32)
  copy_oedebugrt_target(getting_report_host_oedebugrt)
  add_dependencies(getting_report_host getting_report_host_oedebugrt)
endif ()

target_include_directories(
  getting_report_host PRIVATE # Needed for the generated file getting_report_u.h
                          ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(getting_report_host openenclave::oehost)
