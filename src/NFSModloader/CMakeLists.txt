add_library(NFSModloader SHARED "hook.cpp" "framework.h" "pch.h" "pch.cpp" "utils.h")
set_target_properties(NFSModloader PROPERTIES OUTPUT_NAME "dinput8")

target_precompile_headers(NFSModloader PRIVATE "pch.h")

target_link_libraries(NFSModloader PRIVATE fmt::fmt)