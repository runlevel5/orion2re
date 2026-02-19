#include "deps.h"

namespace npcdiplo {
    /**
     * @address 004e9f80
     */
    void __cdecl NPC_Diplomacy_() {
        diplomac::NPC_Sneak_Attacks_();
        NPC_Diplomacy_Buildup_Penalties_();
        NPC_Threats_();
        NPC_To_NPC_Treaty_Negotiations_();
        NoOp3();
        NPC_Declarations_Of_War_();
        NPC_Peace_Negotiations_();
        NPC_Tech_Exchange_();
    }
}