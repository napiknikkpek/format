include(ExternalProject)

ExternalProject_Add(Pprintf EXCLUDE_FROM_ALL 1
  GIT_REPOSITORY https://github.com/tfc/pprintpp.git
  TIMEOUT 120
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dependencies/pprintf"
  CONFIGURE_COMMAND "" # Disable configure step
  BUILD_COMMAND ""     # Disable build step
  INSTALL_COMMAND ""   # Disable install step
  TEST_COMMAND ""      # Disable test step
  UPDATE_COMMAND ""    # Disable source work-tree update
)
ExternalProject_Get_Property(Pprintf SOURCE_DIR)
set(Pprintf_INCLUDE_DIR ${SOURCE_DIR}/include)
