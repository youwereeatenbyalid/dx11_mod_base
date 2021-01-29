
#include "DisableAutoAssist.hpp"
#include "mods/PlayerTracker.hpp"
uintptr_t DisableAutoAssist::jmp_ret{NULL};
bool DisableAutoAssist::cheaton{NULL};
// clang-format off
// only in clang/icl mode on x64, sorry

static naked void detour() {
	__asm {
	    validation:
            cmp byte ptr [DisableAutoAssist::cheaton], 1
            je cheatcode
            jmp code
        code:
            mov [rax+0x14], ecx
            mov rbx, [rsp+0x30]
            jmp qword ptr [DisableAutoAssist::jmp_ret]        
        cheatcode:
		    mov rbx, [rsp+0x30]
            jmp qword ptr [DisableAutoAssist::jmp_ret]
	}
}

// clang-format on
std::optional<std::string> DisableAutoAssist::on_initialize() {
  auto base = g_framework->get_module().as<HMODULE>(); // note HMODULE
  ischecked = &DisableAutoAssist::cheaton;
  onpage    = qol;
  full_name_string     = "Disable Auto Assist";
  author_string        = "The Hitchhiker";
  description_string   = "Prevents Auto Assist from being activated.";

  auto addr = utility::scan(base, "BE 89 48 14 48 8B 5C 24 30");
  if (!addr) {
    return "Unable to find DisableAutoAssist pattern.";
  }
  if (!install_hook_absolute(addr.value()+1, m_function_hook, &detour, &jmp_ret, 8)) {
  //  return a error string in case something goes wrong
    spdlog::error("[{}] failed to initialize", get_name());
    return "Failed to initialize DisableAutoAssist";
  }
  return Mod::on_initialize();
}

// during load
void DisableAutoAssist::on_config_load(const utility::Config &cfg) {}
// during save
void DisableAutoAssist::on_config_save(utility::Config &cfg) {}
// do something every frame
void DisableAutoAssist::on_frame() {}
// will show up in debug window, dump ImGui widgets you want here
void DisableAutoAssist::on_draw_debug_ui() {}
// will show up in main window, dump ImGui widgets you want here
void DisableAutoAssist::on_draw_ui() {}
