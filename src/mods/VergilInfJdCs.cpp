#include "VergilInfJdCs.hpp"
#include "PlayerTracker.hpp"
uintptr_t VergilInfJdCs::jmp_ret{NULL};
uintptr_t VergilInfJdCs::cheaton{NULL};

// clang-format off
// only in clang/icl mode on x64, sorry

static naked void detour() {
	__asm {
        cmp byte ptr [PlayerTracker::playerid], 4 //change this to the char number obviously
        jne code
        push rax
        mov rax,[VergilInfJdCs::cheaton]
        cmp byte ptr [rax], 1
        pop rax
        je cheatcode
        jmp code

    cheatcode:
		jmp qword ptr [VergilInfJdCs::jmp_ret]

    code:
        inc byte ptr [rdi+000018E0h]
        jmp qword ptr [VergilInfJdCs::jmp_ret]
	}
}

// clang-format on

std::optional<std::string> VergilInfJdCs::on_initialize() {
  ischecked            = false;
  onpage               = vergilpage;
  full_name_string     = "Infinite Just Judgement Cuts";
  author_string        = "SSSiyan";
  description_string   = "Remove the consecutive Just Judgement Cut limit.";
  VergilInfJdCs::cheaton = (uintptr_t)&ischecked;
  auto base = g_framework->get_module().as<HMODULE>(); // note HMODULE
  auto addr = utility::scan(base, "FF 87 E0 18 00 00");
  if (!addr) {
    return "Unable to find VergilInfJdCs pattern.";
  }

  if (!install_hook_absolute(addr.value(), m_function_hook, &detour, &jmp_ret, 6)) {
    //  return a error string in case something goes wrong
    spdlog::error("[{}] failed to initialize", get_name());
    return "Failed to initialize VergilInfJdCs";
  }
  return Mod::on_initialize();
}

void VergilInfJdCs::on_draw_ui() {
}
