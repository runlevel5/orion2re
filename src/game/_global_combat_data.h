#pragma once
#include <cstdint>

namespace _global_combat_data {
  inline int16_t& design_idx =
      *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059BDAE});

  inline int8_t& shield_type =
      *reinterpret_cast<int8_t*>(std::uintptr_t{0x0059BDB6});
}