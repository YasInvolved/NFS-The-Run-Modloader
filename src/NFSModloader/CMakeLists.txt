add_library(NFSModloader SHARED "hook.cpp" "framework.h" "pch.h" "pch.cpp" "utils.h"  "loader/Loader.h" "loader/Loader.cpp" "loader/ThreadPool.h" "loader/ThreadPool.cpp" "loader/DllObserver.cpp" "loader/DllObserver.h" "loader/offsets.h")
set_target_properties(NFSModloader PROPERTIES OUTPUT_NAME "dxgi")

target_precompile_headers(NFSModloader PRIVATE "pch.h")
target_compile_definitions(NFSModloader PRIVATE $<$<CONFIG:Debug>:NFS_MODLOADER_DEBUG>)
target_link_libraries(NFSModloader PRIVATE fmt::fmt dxguid.lib)

target_include_directories(NFSModloader PRIVATE "${NFS_MODLOADER_PATH}/src")