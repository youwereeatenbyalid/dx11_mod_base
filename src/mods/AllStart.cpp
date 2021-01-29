#include "AllStart.hpp"
#include "PlayerTracker.hpp"
uintptr_t AllStart::jmp_ret{NULL};
bool AllStart::cheaton{NULL};
uintptr_t AllStart::jmp_initial{NULL};

bool enemystepcancels;

// clang-format off
// only in clang/icl mode on x64, sorry

static naked void detour() {
	__asm {
        cmp byte ptr [AllStart::cheaton], 1
        je cheatcode
        jmp code

    cheatcode:
        cmp [PlayerTracker::playerid], 0
        je nerocancels
        cmp [PlayerTracker::playerid], 1
        je dantecancels
        cmp [PlayerTracker::playerid], 2
        je vancels
        cmp [PlayerTracker::playerid], 4
        je vergilancels
        jmp code

    nerocancels:
        cmp dword ptr [PlayerTracker::playermoveid], 53C0000h // Nero Enemy Step
        je jccheck
        jmp code

    dantecancels:
        cmp dword ptr [PlayerTracker::playermoveid], 53C0000h // Dante Enemy Step
        je dantejccheck
        jmp code

    vancels:
        cmp dword ptr [PlayerTracker::playermoveid], 53C0000h // V Enemy Step (probably)
        je jccheck
        jmp code

    vergilancels:
        cmp dword ptr [PlayerTracker::playermoveid], 53C0000h // Vergil Enemy Step (probably)
        je jccheck
        jmp code

//_____________________________________________________________________________________


    jccheck:
        cmp byte ptr [enemystepcancels], 1
        je cancellable
        jmp code

    dantejccheck:
        cmp byte ptr [enemystepcancels], 1
        je dantejcweaponcheck
        jmp code

    dantejcweaponcheck:
        cmp byte ptr [PlayerTracker::danteweapon], 5 // Cavaliere
        je code
        cmp byte ptr [PlayerTracker::danteweapon], 6 // Cavailiere R
        je code
        jmp cancellable

    cancellable:
        mov word ptr [rdi+5Eh], 0100h
		jmp qword ptr [AllStart::jmp_ret]

    code:
        mov word ptr [rdi+5Eh], 0000h
        jmp qword ptr [AllStart::jmp_ret]
	}
}

// clang-format on

void AllStart::init_check_box_info() {
  m_check_box_name = m_prefix_check_box_name + std::string(get_name());
  m_hot_key_name   = m_prefix_hot_key_name + std::string(get_name());
}

std::optional<std::string> AllStart::on_initialize() {
  init_check_box_info();

  ischecked          = &AllStart::cheaton;
  onpage             = enemystep;

  full_name_string   = "AllStart";
  author_string      = "SSSiyan";
  description_string = "Allows you to cancel out of a selection of moves with any other move.";

  auto base = g_framework->get_module().as<HMODULE>(); // note HMODULE
  auto addr      = utility::scan(base, "66 C7 47 5E 00 00");
  if (!addr) {
    return "Unable to find AllStart pattern.";
  }
  AllStart::jmp_initial = addr.value();

  if (!install_hook_absolute(addr.value(), m_function_hook, &detour, &jmp_ret, 6)) {
    //  return a error string in case something goes wrong
    spdlog::error("[{}] failed to initialize", get_name());
    return "Failed to initialize AllStart";
  }
  return Mod::on_initialize();
}

void AllStart::on_config_load(const utility::Config& cfg) {
  enemystepcancels = cfg.get<bool>("enemy_step_cancels").value_or(true);
}
void AllStart::on_config_save(utility::Config& cfg) {
  cfg.set<bool>("enemy_step_cancels", enemystepcancels);
}

void AllStart::on_draw_ui() {
  ImGui::Checkbox("Enemy Step Cancels", &enemystepcancels);
}
