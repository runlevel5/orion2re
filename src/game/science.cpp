#include "pch.h"

namespace SCIENCE {
    void __cdecl Show_Off_Stolen_Tech_(int16_t tech_idx, int16_t other_idx) {
        char message_text[0x100];
        void* animation_block;
        char* spy_anim;

        allocate::Push_Block_(MOX::_screen_seg);
        animation_block = allocate::Allocate_First_Block_(MOX::_screen_seg, 0x7C830);
        file_ani::Set_File_Animation_Buffer_(animation_block);
        JIM::Get_Text_Message_(s_billtext::lbx_0055135c, 2, message_text, 0x100);
        JIM::Decode_Text_Tech_And_Other_Name_(message_text, MOX::_PLAYER_NUM, tech_idx, other_idx, 0x100);
        spy_anim = BILL::Cache_Player_Spy_Anim_();
        SCIENCE::Science_Room_(spy_anim, 1, &tech_idx, message_text);
        file_ani::Reset_File_Animation_Buffer_();
        allocate::Pop_Block_(MOX::_screen_seg);
        COLONY::Reset_Mouse_Picture_();
        JIM::Play_Background_Music_();
    }

    void __cdecl Show_Off_Researched_Tech_(int16_t player_idx) {
        int16_t researched_tech_ids[4];
        char science_room_lbx[64];
        char decoded_text[256];
        short found_count = 0;

        MOX::_cur_plyr_ptr = &MOX::_player[player_idx];

        if (MOX::_cur_plyr_ptr->current_research_field != 0) {
            const uint8_t research_field = static_cast<uint8_t>(MOX::_cur_plyr_ptr->current_research_field);

            TECH::Technology_Fields_Name_(research_field);

            const uint16_t* field_tech_ids = &SCIENCE::_technology_fields[research_field].tech1;
            for (int tech_slot = 0; tech_slot < 4; ++tech_slot) {
                const uint16_t tech_id = field_tech_ids[tech_slot];
                if (tech_id > 0 && MOX::_cur_plyr_ptr->tech_applications[tech_id] == 3) {
                    researched_tech_ids[found_count++] = tech_id;
                }
            }

            if (found_count != 0) {
                JIM::Get_Text_Message_(SCIENCE::s_billtext_lbx_0055135c, 1, decoded_text, 0xFA);
                JIM::Decode_Text_Field_(decoded_text, research_field, 0xFA);

                _sprintf(
                    science_room_lbx,
                    SCIENCE::s_SR_R_x_SC_LBX_005512b0,
                    static_cast<unsigned int>(static_cast<uint8_t>(MOX::_player[player_idx].race)));

                SCIENCE::Science_Room_(science_room_lbx, found_count, researched_tech_ids, decoded_text);
                return;
            }
        }

        decoded_text[0] = '\0';
        fields::Clear_Fields_();
        ERIC::Fast_Fade_Out_();
    }
}