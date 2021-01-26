
#include "HeavyDay.hpp"
#include "mods/PlayerTracker.hpp"
uintptr_t HeavyDay::enemystep_jmp_ret{NULL};
uintptr_t HeavyDay::enemystep_je_ret{NULL};
uintptr_t HeavyDay::lockon_jmp_ret{NULL};
uintptr_t HeavyDay::targetswitch_jmp_ret{NULL};
uintptr_t HeavyDay::damageall_jmp_ret{NULL};
uintptr_t HeavyDay::pvp1_jmp_ret{NULL};
uintptr_t HeavyDay::pvp2_jmp_ret{NULL};
uintptr_t HeavyDay::danteclientside_jmp_ret{NULL};
uintptr_t HeavyDay::dtenable_jmp_ret{NULL};
uintptr_t HeavyDay::dtenable_ja_ret{NULL};
uintptr_t HeavyDay::rgenable_jmp_ret{NULL};
uintptr_t HeavyDay::rgmod_jmp_ret{NULL};
bool HeavyDay::cheaton{NULL};


float shadowJCradius = 2.0;
float shadowJCyoffset = 0.5;
float griffonJCradius = 1.0;
float nightmareJCradius = 3;
float nightmareJCyoffset = 1.5;
float playerJCradius     = 2.0;
float playerJCyoffset    = 0.8;
float registerreset      = 0.0;

float rgmult = 10.0;
    // clang-format off
// only in clang/icl mode on x64, sorry

static naked void enemystep_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code

        cheatcode:
            test ecx,ecx
            je code

            push r8
            push r9
            push r10

            mov r8, [rbx+0x1F0]

            //stick a grounded mem comparison here
            cmp [PlayerTracker::isgrounded], 0
            jne xmmclear

        shadowcheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            //if shadow is active and has coordinates, check to see if she's in range
            mov r9, [PlayerTracker::shadowtransform]
            mov r10, [PlayerTracker::shadowcontroller]
            test r9, r9
            jne shadowcomparison

        griffoncheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            //if griffon is active and has coordinates, check to see if he's in range
            mov r9, [PlayerTracker::griffontransform]
            mov r10, [PlayerTracker::griffoncontroller]
            test r9, r9
            jne griffoncomparison

       nightmarecheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            //if nightmare is active and has coordinates, check to see if he's in range
            mov r9, [PlayerTracker::nightmaretransform]
            mov r10, [PlayerTracker::nightmarecontroller]
            test r9, r9
            jne nightmarecomparison

        nerocheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            mov r9, [PlayerTracker::nerotransform]
            test r9, r9
            jne nerocomparison

        dantecheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            mov r9, [PlayerTracker::dantetransform]
            test r9, r9
            jne dantecomparison


         vcheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            mov r9, [PlayerTracker::vtransform]
            test r9, r9
            jne vcomparison

        vergilcheck:
            //get player coordinates from transform
            movss xmm13, [r8+0x30]
            movss xmm14, [r8+0x34]
            movss xmm15, [r8+0x38]

            mov r9, [PlayerTracker::vergiltransform]
            test r9, r9
            jne vergilcomparison

            jmp xmmclear

        shadowcomparison:
            //check entity state of shadow
            cmp byte ptr [r10+0x64], 0
            je griffoncheck

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            //subtract an additional 0.5 from y distance to raise shadow's JC sphere
            subss xmm14, [shadowJCyoffset]

            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [shadowJCradius]
            ja griffoncheck

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret

        griffoncomparison:
            //check entity state of griffon
            cmp byte ptr [r10+0x64], 0
            je nightmarecheck

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [griffonJCradius]
            ja nightmarecheck

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret


        nightmarecomparison:
            //check entity state of nightmare
            cmp byte ptr [r10+0x64], 0
            je nerocheck

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            //subtract an additional 0.5 from y distance to raise shadow's JC sphere
            subss xmm14, [nightmareJCyoffset]

            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [nightmareJCradius]
            ja nerocheck

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret


        nerocomparison:
            //check if playing as nero
            cmp r8, r9
            je dantecheck

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            //subtract an additional 0.5 from y distance to raise Nero's JC sphere
            subss xmm14, [playerJCyoffset]

            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [playerJCradius]
            ja dantecheck

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret



        dantecomparison:
            //check if playing as dante
            cmp r8, r9
            je vcheck

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            //subtract an additional 0.5 from y distance to raise Dante's JC sphere
            subss xmm14, [playerJCyoffset]

            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [playerJCradius]
            ja vcheck

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret


        vcomparison:
            //check if playing as v
            cmp r8, r9
            je xmmclear

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            //subtract an additional 0.5 from y distance to raise V's JC sphere
            subss xmm14, [playerJCyoffset]

            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [playerJCradius]
            comiss xmm15, [playerJCradius]
            ja vergilcheck

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret

        vergilcomparison:
            //check if playing as vergil
            cmp r8, r9
            je xmmclear

            //get differences between x y z
            subss xmm13, [r9+0x30]
            subss xmm14, [r9+0x34]
            //subtract an additional 0.5 from y distance to raise V's JC sphere
            subss xmm14, [playerJCyoffset]

            subss xmm15, [r9+0x38]
            //square differences
            mulss xmm13, xmm13
            mulss xmm14, xmm14
            mulss xmm15, xmm15
            //sum differences into xmm13
            addss xmm13, xmm14
            addss xmm13, xmm15

            //square root of differences into xmm15
            sqrtss xmm15, xmm13
            comiss xmm15, [playerJCradius]
            ja xmmclear

            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
            jmp jmp_ret

        xmmclear:
            movss xmm13, [registerreset]
            movss xmm14, [registerreset]
            movss xmm15, [registerreset]
            pop r10
            pop r9
            pop r8
         code:
            test ecx,ecx
            je je_ret
            jmp jmp_ret
         je_ret:
            jmp qword ptr [HeavyDay::enemystep_je_ret]       
        jmp_ret:
            jmp qword ptr [HeavyDay::enemystep_jmp_ret]        
	}
}
static naked void lockon_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code

        cheatcode:
            cmp byte ptr [rdx+0x000000F4],2
            je jmp_ret
            //so vergil doesn't auto lock the doppelganger
            cmp byte ptr [rdx+0x000000F4], 0
            je jmp_ret
            //if base matches a summoned enemy, jump to the specific comparisons
            cmp rdx, [PlayerTracker::shadowentity]
            je shadowcheck

            cmp rdx, [PlayerTracker::griffonentity]
            je griffoncheck

            cmp rdx, [PlayerTracker::nightmareentity]
            je nightmarecheck
            //otherwise enemy player, jumping forces not equal which allows lock-on
            mov byte ptr [rdx+0x108], 1
            jmp jmp_ret
        code:
            cmp dword ptr [rdx+0x00000108],eax
        jmp_ret:
            jmp qword ptr [HeavyDay::lockon_jmp_ret]    
        shadowcheck:
            push r8
            //move master lock-on to valid in case we aren't V.
            mov byte ptr [rdx+0x1D2], 0
            mov byte ptr [rdx+0x108], 1
            mov r8, [PlayerTracker::shadowcontroller]
            cmp byte ptr [r8+0x64], 0
            pop r8
            jmp jmp_ret

        griffoncheck:
            push r8
            //move master lock-on to valid in case we aren't V.
            mov byte ptr [rdx+0x1D2], 0
            mov byte ptr [rdx+0x108], 1
            mov r8, [PlayerTracker::griffoncontroller]
            cmp byte ptr [r8+0x64], 0
            pop r8
            jmp jmp_ret

        nightmarecheck:
            push r8
            //move master lock-on to valid in case we aren't V.
            mov byte ptr [rdx+0x1D2], 0
            mov byte ptr [rdx+0x108], 1
            mov r8, [PlayerTracker::nightmarecontroller]
            cmp byte ptr [r8+0x64], 0
            pop r8
            jmp jmp_ret
	}
}
static naked void targetswitch_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code
        code:
            test [rdi+0x00000090],r12d
            jmp qword ptr [HeavyDay::targetswitch_jmp_ret]        
        cheatcode:
            test rdi, rdi
            jmp qword ptr [HeavyDay::targetswitch_jmp_ret]
	}
}
static naked void damageall_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp originalcode
        cheatcode:
            cmp r10, 0x1A
            je originalcode
            cmp r10, 0x19
            je originalcode
            cmp r10, 0x16
            je originalcode
            cmp r10, 0x15
            je originalcode
            cmp r10, 0x14
            je originalcode
            cmp r10, 0x13
            je originalcode
            cmp r10, 0xB
            je originalcode
            cmp r10, 0xA
            je originalcode
            cmp r10, 0x1
            je originalcode
            mov eax, 2
            cmp ecx,01
            jmp qword ptr [HeavyDay::damageall_jmp_ret]   
        originalcode:
            mov eax,[r8+0x20]
            cmp ecx,01
            jmp qword ptr [HeavyDay::damageall_jmp_ret]   

	}
}
static naked void pvp1_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code
        code:
            cmp dword ptr [rbx+0x000000F4],01
            jmp qword ptr [HeavyDay::pvp1_jmp_ret]        
        cheatcode:
            jmp qword ptr [HeavyDay::pvp1_jmp_ret]
	}
}
static naked void pvp2_detour() {
	__asm {
        validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code
        code:
            mov eax,[r14+0x000000F4]
            jmp qword ptr [HeavyDay::pvp2_jmp_ret]        
        cheatcode:
            mov eax,[r14+0x000000F4]
            //if the target is a coop dummy, mark it as a player to force hits
            cmp eax, 3
            je move2
            //if the target is a player, mark it as a coop dummy. This prevents V from hitting himself with his own summons.
            cmp eax, 2
            je move3
            jmp qword ptr [HeavyDay::pvp2_jmp_ret]
        move2:
            mov eax, 2
            jmp qword ptr [HeavyDay::pvp2_jmp_ret]
        move3:
            mov eax, 3
            jmp qword ptr [HeavyDay::pvp2_jmp_ret]
	}
}
static naked void danteclientside_detour() {
	__asm {
        validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code
        code:
            mov eax,[rdx+0x5C]
            cmp eax,0x10
            jmp qword ptr [HeavyDay::danteclientside_jmp_ret]        
        cheatcode:
            mov eax, 0x10
            cmp eax, 0x10
            jmp qword ptr [HeavyDay::danteclientside_jmp_ret]
	}
}
static naked void dtenable_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code
        code:
            test eax, 0x200
            ja ja_ret
            jmp qword ptr [HeavyDay::dtenable_jmp_ret]        
        ja_ret:
            jmp qword ptr [HeavyDay::dtenable_ja_ret]        
        cheatcode:
            jmp qword ptr [HeavyDay::dtenable_jmp_ret]
	}
}
static naked void rgenable_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 0
            jne cheatcode
            jmp code
        code:
            cmp byte ptr [r8+0x00000ECA],00
            jmp qword ptr [HeavyDay::rgenable_jmp_ret]        
        cheatcode:
            jmp qword ptr [HeavyDay::rgenable_jmp_ret]
	}
}
static naked void rgmod_detour() {
	__asm {
	    validation:
            cmp byte ptr [HeavyDay::cheaton], 1
            je cheatcode
            jmp code
        code:
            movss xmm1,[rdx+0x28]
            jmp qword ptr [HeavyDay::rgmod_jmp_ret]        
        cheatcode:
            movss xmm1,[rdx+0x28]
            mulss xmm2, [rgmult]
            jmp qword ptr [HeavyDay::rgmod_jmp_ret]
	}
}


// clang-format on

std::optional<std::string> HeavyDay::on_initialize() {
  auto base = g_framework->get_module().as<HMODULE>(); // note HMODULE
  ischecked = &HeavyDay::cheaton;
  onpage    = commonpage;

  full_name_string     = "PVP";
  author_string        = "The HitchHiker, SSSiyan, Dr. Penguin";
  description_string   = "Enables PVP Combat between players."
      "All players must have the cheat enabled for the mod to work correctly.";

  auto enemystep_addr = utility::scan(base, "85 C9 0F 84 AA 00 00 00 F3 0F 10 8B");
  auto lockon_addr = utility::scan(base, "39 82 08 01 00 00 0F");
  auto targetswitch_addr = utility::scan(base, "44 85 A7 90 00 00 00");
  auto damageall_addr = utility::scan(base, "41 8B 40 20 83 F9 01");
  auto pvp1_addr = utility::scan(base, "83 BB F4 00 00 00 01");
  auto pvp2_addr         = utility::scan(base, "41 8B 86 F4 00 00 00");
  auto danteclientside_addr = utility::scan(base, "8B 42 5C 83 F8 10 74 36");
  auto dtenable_addr = utility::scan(base, "0F 87 60 01 00 00 A9");
  auto rgenable_addr = utility::scan(base, "41 80 B8 CA 0E 00 00 00 74 58");
  auto rgmod_addr = utility::scan(base, "4D F3 0F 10 4A 28");

  if (!enemystep_addr) {
    return "Unable to find Enemy Step pattern.";
  }
  if (!lockon_addr) {
    return "Unable to find Lock On pattern.";
  }
  if (!targetswitch_addr) {
    return "Unable to find Target Switch pattern.";
  }
  if (!damageall_addr) {
    return "Unable to find Damage All pattern.";
  }
  if (!pvp1_addr) {
    return "Unable to find PVP 1 pattern.";
  }
  if (!pvp2_addr) {
    return "Unable to find PVP 2 pattern.";
  }
  if (!danteclientside_addr) {
    return "Unable to find Dante Clientside Damage pattern.";
  }
  if (!dtenable_addr) {
    return "Unable to find Devil Trigger Enable pattern.";
  }
  if (!rgenable_addr) {
    return "Unable to find Royal Guard Enable pattern.";
  }
  if (!rgmod_addr) {
    return "Unable to find Royalg Guard Mod pattern.";
  }
    if (!install_hook_absolute(enemystep_addr.value(), m_enemystep_hook,
                                &enemystep_detour, &enemystep_jmp_ret, 8)) {
    //  return a error string in case something goes wrong
    spdlog::error("[{}] failed to initialize", get_name());
    return "Failed to initialize enemystep";
    }
    if (!install_hook_absolute(lockon_addr.value(), m_lockon_hook,
                               &lockon_detour, &lockon_jmp_ret, 6)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize lockon";
    }
    if (!install_hook_absolute(targetswitch_addr.value(), m_targetswitch_hook,
                               &targetswitch_detour, &targetswitch_jmp_ret, 7)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize targetswitch";
    }
    if (!install_hook_absolute(damageall_addr.value(), m_damageall_hook,
                               &damageall_detour, &damageall_jmp_ret, 7)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize damageall";
    }
    if (!install_hook_absolute(pvp1_addr.value(), m_pvp1_hook,
                               &pvp1_detour, &pvp1_jmp_ret, 7)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize pvp1";
    }
    if (!install_hook_absolute(pvp2_addr.value(), m_pvp2_hook,
                               &pvp2_detour, &pvp2_jmp_ret, 7)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize pvp2";
    }
    if (!install_hook_absolute(danteclientside_addr.value(), m_danteclientside_hook,
                               &danteclientside_detour, &danteclientside_jmp_ret, 6)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize danteclientside";
    }
    if (!install_hook_absolute(dtenable_addr.value(), m_dtenable_hook,
                               &dtenable_detour, &dtenable_jmp_ret, 6)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize dtenable";
    }
    if (!install_hook_absolute(rgenable_addr.value(), m_rgenable_hook,
                               &rgenable_detour, &rgenable_jmp_ret, 8)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize rgenable";
    }
    if (!install_hook_absolute(rgmod_addr.value()+1, m_rgmod_hook,
                               &rgmod_detour, &rgmod_jmp_ret, 5)) {
      //  return a error string in case something goes wrong
      spdlog::error("[{}] failed to initialize", get_name());
      return "Failed to initialize rgmod";
    }
    
    HeavyDay::enemystep_je_ret = enemystep_addr.value() + 0xB2;
    HeavyDay::dtenable_ja_ret  = dtenable_addr.value() + 0x166;
  return Mod::on_initialize();
}

// during load
//void HeavyDay::on_config_load(const utility::Config &cfg) {}
// during save
//void HeavyDay::on_config_save(utility::Config &cfg) {}
// do something every frame
//void HeavyDay::on_frame() {}
// will show up in debug window, dump ImGui widgets you want here
//void HeavyDay::on_draw_debug_ui() {}
// will show up in main window, dump ImGui widgets you want here
//void HeavyDay::on_draw_ui() {}
