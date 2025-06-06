add_library(NFSModloader SHARED "hook.cpp" "framework.h" "pch.h" "pch.cpp" "utils.h" "DllMain.cpp" "Loader.h" "Loader.cpp" "ThreadPool.h" "ThreadPool.cpp")
set_target_properties(NFSModloader PROPERTIES OUTPUT_NAME "dinput8")

target_precompile_headers(NFSModloader PRIVATE "pch.h")
target_compile_definitions(NFSModloader PRIVATE $<$<CONFIG:Debug>:NFS_MODLOADER_DEBUG>)
target_link_libraries(NFSModloader PRIVATE fmt::fmt)