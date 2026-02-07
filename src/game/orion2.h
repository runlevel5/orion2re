typedef unsigned char undefined;
typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;

typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined5;
typedef unsigned long long    undefined6;
typedef unsigned long long    undefined7;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef enum ECON {
    ECON_FOOD=0,
    ECON_INDUSTRY=1,
    ECON_RESEARCH=2,
    ECON_BC=3
} ECON;

typedef enum FIELD_TYPE {
    FIELD_TYPE_BUTTON=0,
    FIELD_TYPE_RADIO=1,
    FIELD_TYPE_LOCKED=2,
    FIELD_TYPE_MULTI_BUTTON=3,
    FIELD_TYPE_4=4,
    FIELD_TYPE_5=5,
    FIELD_TYPE_SCROLL=6,
    FIELD_TYPE_HIDDEN=7,
    FIELD_TYPE_MULTI_HOTKEY=8,
    FIELD_TYPE_9=9,
    FIELD_TYPE_10=10,
    FIELD_TYPE_CONTINUOUS_INPUT=11,
    FIELD_TYPE_GRID=12,
    FIELD_TYPE_IRREGULAR_INPUT=13
} FIELD_TYPE;

typedef enum HESTRNGS {
    HESTR_001_TITAN_CONSTRUCTION_TEC=1,
    HESTR_002_DOOM_STAR_CONSTRUCTION=2,
    HESTR_003_YOU_NEED_TO_BUILD_A_ST=3,
    HESTR_004_YOU_NEED_TO_BUILD_A_ST=4,
    HESTR_005_YOU_NEED_TO_BUILD_A_ST=5,
    HESTR_006_YOU_NEED_TO_BUILD_A_ST=6,
    HESTR_007_YOUR_CURRENT_DESIGN_IS=7,
    HESTR_008_YOUR_CURRENT_DESIGN_IS=8,
    HESTR_009_YOUR_CURRENT_DESIGN_IS=9,
    HESTR_00A_YOUR_CURRENT_DESIGN_IS=10,
    HESTR_00B_YOUR_CURRENT_DESIGN_IS=11,
    HESTR_00C_YOUR_CURRENT_DESIGN_IS=12,
    HESTR_00D_YOU_MAY_NOT_CHANGE_HUL=13,
    HESTR_00E_SINCE_S_CANNOT_BUILD_S=14,
    HESTR_00F_THE_BLUEWHITE_STARS_AR=15,
    HESTR_010_WHITE_STARS_HAVE_A_FAI=16,
    HESTR_011_YELLOW_STARS_ARE_THE_S=17,
    HESTR_012_ORANGE_STARS_WILL_FREQ=18,
    HESTR_013_RED_STARS_ARE_THE_DIMM=19,
    HESTR_014_BROWN_STARS_ARE_REALLY=20,
    HESTR_015_BLACK_HOLES_ARE_ANCIEN=21,
    HESTR_016_THE_HIGH_COUNCIL_HAS_F=22,
    HESTR_017_ALTHOUGH_YOUR_SCORE_IS=23,
    HESTR_018_NO_NEW_WEAPONS_CAN_BE_=24,
    HESTR_019_NO_NEW_SPECIAL_SYSTEMS=25,
    HESTR_01A_NOT_ENOUGH_SPACE_TO_AD=26,
    HESTR_01B_NOT_ENOUGH_SPACE_TO_AD=27,
    HESTR_01C_HAS_YOUR_OPPONENT_PRES=28,
    HESTR_01D_PLAY_MASTER_OF_ORION_I=29,
    HESTR_01E_COMBATTING_COMPUTERIZE=30,
    HESTR_01F_THIS_WILL_GUIDE_YOU_TH=31,
    HESTR_020_OPTION_DISABLED_DURING=32,
    HESTR_021_A_BLACK_HOLE_BLOCKS_MO=33,
    HESTR_022_ONE_OR_MORE_SHIPS_DOES=34,
    HESTR_023_ONE_OR_MORE_SHIPS_CHOS=35,
    HESTR_024_SHIP_MOVEMENT_IS_NOT_A=36,
    HESTR_025_AFTER_DESTROYING_THE_G=37,
    HESTR_026_AFTER_DESTROYING_THE_G=38,
    HESTR_027_MAGNIFICIENT_YOU_HAVE_=39,
    HESTR_028_WELL_DONE_S_IS_TRULY_O=40,
    HESTR_029_COMPUTER_TYPE=41,
    HESTR_02A_BONUS_TO_HIT=42,
    HESTR_02B_COST=43,
    HESTR_02C_INIT_BONUS=44,
    HESTR_02D_NO_COMPUTER=45,
    HESTR_02E_WEAPON_NAME=46,
    HESTR_02F_DAM=47,
    HESTR_030_SPACE=48,
    HESTR_031_NOTES=49,
    HESTR_032_NO_WEAPON=50,
    HESTR_033_SHIELD_TYPE=51,
    HESTR_034_BASE_COST=52,
    HESTR_035_BASE_SPACE=53,
    HESTR_036_NO_SHIELD=54,
    HESTR_037_SYSTEM=55,
    HESTR_038_DESCRIPTION=56,
    HESTR_039_NO_SPECIAL=57,
    HESTR_03A_INSUFFICIENT_SPACE_TO_=58,
    HESTR_03B_D_MORE_MINIATURIZATION=59,
    HESTR_03C_D_MORE_MINIATURIZATION=60,
    HESTR_03D_HEAVY_MOUNT_MODIFICATI=61,
    HESTR_03E_POINT_DEFENSE_MODIFICA=62,
    HESTR_03F_EMMISIONS_GUIDANCE_TEC=63,
    HESTR_040_S_IS_NOT_ALLOWED_ON_S=64,
    HESTR_041_S_MAY_ONLY_BE_ADDED_ON=65,
    HESTR_042_YOU_MAY_HAVE_ONLY_ONE_=66,
    HESTR_043_YOU_CANNOT_PLACE_S_S_A=67,
    HESTR_044_BEAM_WEAPONS_MUST_HAVE=68,
    HESTR_045_INSUFFICIENT_SPACE_TO_=69,
    HESTR_046_FULL=70,
    HESTR_047_FORWARD_EXTENDED=71,
    HESTR_048_BACK_EXTENDED=72,
    HESTR_049_REAR=73,
    HESTR_04A_S_360=74,
    HESTR_04B_INSUFFICIENT_SPACE_TO_=75,
    HESTR_04C_INSUFFICIENT_SPACE_TO_=76,
    HESTR_04D_NOT_ENOUGH_ROOM_IN_CUR=77,
    HESTR_04E_YOU_HAVE_NOT_CHOSEN_A_=78,
    HESTR_04F_SS_HAVE_BUILTIN_360_AR=79,
    HESTR_050_YOU_MAY_NOT_UPGRADE_TH=80,
    HESTR_051_YOU_MAY_NOT_PLACE_SHIE=81,
    HESTR_052_YOU_MAY_NOT_UPGRADE_TH=82,
    HESTR_053_REMOVE_S_FROM_SHIP_DES=83,
    HESTR_054_D_COMBAT_SPEED=84,
    HESTR_055_D_STRUCTURE_POINTS=85,
    HESTR_056_D_ARMOR_POINTS=86,
    HESTR_057_NO_STRENGTH=87,
    HESTR_058_NO_DAMAGE_POINTS_BLOCK=88,
    HESTR_059_D_STRENGTH=89,
    HESTR_05A_D_DAMAGE_POINTS_BLOCKE=90,
    HESTR_05B_D_BEAM_ATTACK=91,
    HESTR_05C_BEAM_DEFENSE=92,
    HESTR_05D_MISSILE_EVASION=93,
    HESTR_05E_WEAPON_TYPE=94,
    HESTR_05F_DAMAGE=95,
    HESTR_060_ARC=96,
    HESTR_061_MODIFICATIONS=97,
    HESTR_062_SPECIAL_SYSTEM=98,
    HESTR_063_SPACE_AVAILABLE=99,
    HESTR_064_NO_MODIFICATIONS=100,
    HESTR_065_SPECIAL=101,
    HESTR_066_S_FLEET=102,
    HESTR_067_ORBITING_S=103,
    HESTR_068_DESTINATION_ANTARES=104,
    HESTR_069_D_TURN_TO_S=105,
    HESTR_06A_D_TURNS_TO_S=106,
    HESTR_06B_ETA_D_TURN=107,
    HESTR_06C_ETA_D_TURNS=108,
    HESTR_06D_BLACK_HOLE_INBETWEEN=109,
    HESTR_06E_IMMOBILE_FLEET=110,
    HESTR_06F_D_PARSEC_TO_S=111,
    HESTR_070_D_PARSECS_TO_S=112,
    HESTR_071_BLACK_HOLE_BLOCKS_RANG=113,
    HESTR_072_D_PARSECS=114,
    HESTR_073_HYPERSPACE_FLUX=115,
    HESTR_074_AFTER_SCRAPPING_S_YOU_=116,
    HESTR_075_S_CANNOT_RECEIVE_ORDER=117,
    HESTR_076_ALL_STAR_RELOCATIONS_H=118,
    HESTR_077_ALL_SHIPS_WILL_BE_RELO=119,
    HESTR_078_YOU_MAY_NOT_SEND_A_FLE=120,
    HESTR_079_YOU_HAVE_NO_OFFICERS_T=121,
    HESTR_07A_THIS_SHIP_IS_NOT_AT_A_=122,
    HESTR_07B_THIS_SHIP_IS_NOT_AT_AN=123,
    HESTR_07C_THESE_SHIPS_ARE_NOT_AT=124,
    HESTR_07D_THESE_SHIPS_ARE_NOT_AT=125,
    HESTR_07E_IF_YOU_SCRAP_S_S_WILL_=126,
    HESTR_07F_SCRAPPING_SHIPS_ABORTE=127,
    HESTR_080_SCRAPPING_THIS_SHIP_WI=128,
    HESTR_081_SCRAPPING_THESE_SHIPS_=129,
    HESTR_082_A_BLACK_HOLE_BLOCKS_NO=130,
    HESTR_083_YOU_MAY_NOT_RELOCATE_S=131,
    HESTR_084_YOU_MAY_NOT_RELOCATE_S=132,
    HESTR_085_YOU_MAY_NOT_RELOCATE_S=133,
    HESTR_086_YOU_MAY_NOT_RELOCATE_S=134,
    HESTR_087_S_IS_GUARDED_BY_A_SPAC=135,
    HESTR_088_YOU_DO_NOT_HAVE_A_COLO=136,
    HESTR_089_SHIP_RELOCATION_FROM_S=137,
    HESTR_08A_GREEN_CREW=138,
    HESTR_08B_REGULAR_CREW=139,
    HESTR_08C_VETERAN_CREW=140,
    HESTR_08D_ELITE_CREW=141,
    HESTR_08E_ULTRA_ELITE_CREW=142,
    HESTR_08F_TOO_MANY_SHIPS_IN_LIST=143,
    HESTR_090_S_IS_OUT_OF_RANGE=144,
    HESTR_091_THAT_SYSTEM_IS_OUT_OF_=145,
    HESTR_092_S_S_D_TURN=146,
    HESTR_093_S_S_D_TURNS=147,
    HESTR_094_UNEXPLORED=148,
    HESTR_095_FRONT=149,
    HESTR_096_RIGHT=150,
    HESTR_097_BACK=151,
    HESTR_098_LEFT=152,
    HESTR_099_BEAM_OCV=153,
    HESTR_09A_BEAM_DCV=154,
    HESTR_09B_DESTINATION_S=155,
    HESTR_09C_DESTINATION_UNEXPLORED=156,
    HESTR_09D_WEAPONS=157,
    HESTR_09E_SPECIALS=158,
    HESTR_09F_NONE=159,
    HESTR_0A0_PRIME=160,
    HESTR_0A1_PLACING_HOME_WORLDS=161,
    HESTR_0A2_START_THE_GAME_WITH_ON=162,
    HESTR_0A3_START_THE_GAME_WITH_D_=163,
    HESTR_0A4_PLAYER=164,
    HESTR_0A5_PLAYERS=165,
    HESTR_0A6_END_OF_TURN_SUMMARY=166,
    HESTR_0A7_END_OF_TURN_WAIT=167,
    HESTR_0A8_ENEMY_MOVES=168,
    HESTR_0A9_EXPANDING_HELP=169,
    HESTR_0AA_AUTO_SELECT_SHIPS=170,
    HESTR_0AB_ANIMATIONS=171,
    HESTR_0AC_AUTO_SELECT_COLONY=172,
    HESTR_0AD_SHOW_RELOCATION_LINES=173,
    HESTR_0AE_SHOW_GNN_REPORT=174,
    HESTR_0AF_AUTO_DELETE_TRADE_GOOD=175,
    HESTR_0B0_AUTO_SAVE_GAME=176,
    HESTR_0B1_SHOW_ONLY_SERIOUS_TURN=177,
    HESTR_0B2_SAVEDGAM_DOES_NOT_EXIS=178,
    HESTR_0B3_SAVEDGAM_IS_INVALID=179,
    HESTR_0B4_SAVEDGAM_IS_A_MULTIPLA=180,
    HESTR_0B5_YOU_MAY_NOT_LOAD_MULTI=181,
    HESTR_0B6_LOADED_SAVEDGAM=182,
    HESTR_0B7_SAVEDGAM_WAS_NOT_LOADE=183,
    HESTR_0B8_GAME_D_WAS_NOT_LOADED=184,
    HESTR_0B9_SAVE_GAME_FUNCTION_DIS=185,
    HESTR_0BA_DO_YOU_WISH_TO_ABANDON=186,
    HESTR_0BB_DO_YOU_WISH_TO_LEAVE_M=187,
    HESTR_0BC_COLONY=188,
    HESTR_0BD_COLONIES=189,
    HESTR_0BE_JAN=190,
    HESTR_0BF_FEB=191,
    HESTR_0C0_MAR=192,
    HESTR_0C1_APR=193,
    HESTR_0C2_MAY=194,
    HESTR_0C3_JUN=195,
    HESTR_0C4_JUL=196,
    HESTR_0C5_AUG=197,
    HESTR_0C6_SEP=198,
    HESTR_0C7_OCT=199,
    HESTR_0C8_NOV=200,
    HESTR_0C9_DEC=201,
    HESTR_0CA_STARDATE=202,
    HESTR_0CB_SAVE_D=203,
    HESTR_0CC_SAVE_GAME_D_S=204,
    HESTR_0CD_LOAD_GAME_D_S=205,
    HESTR_0CE_FILE_ANIMATION_NOT_OPE=206,
    HESTR_0CF_YOU_HAVE_TRIED_TO_CONT=207,
    HESTR_0D0_YOU_HAVE_TRIED_TO_LOAD=208,
    HESTR_0D1_YOU_HAVE_SET_ONLY_ONE_=209,
    HESTR_0D2_YOU_HAVE_NOT_SET_ANY_S=210,
    HESTR_0D3_GAME_SAVED=211,
    HESTR_0D4_SAVE_GAME_FUNCTION_DIS=212,
    HESTR_0D5_USE_THE_FORCE_LUKE_OMN=213,
    HESTR_0D6_YOU_HAVE_CHOSEN_POORLY=214,
    HESTR_0D7_S_HAS_BEEN_GIVEN_A_GIF=215,
    HESTR_0D8_S_WILL_BE_KNOWN=216,
    HESTR_0D9_S_IS_AN_OUTPOST_PLANET=217,
    HESTR_0DA_STARDATE_DD=218,
    HESTR_0DB_S_S_RULER_SEEKS_AN_AUD=219,
    HESTR_0DC_D_NEW_TECHNOLOGY_APPLI=220,
    HESTR_0DD_ALL_NEW_SHIPS_BUILT_WI=221,
    HESTR_0DE_MASTER_OF_ORION_BIDS_Y=222,
    HESTR_0DF_YOU_DO_NOT_HAVE_ANY_FR=223,
    HESTR_0E0_SPACE_FLUX=224,
    HESTR_0E1_D_TURN=225,
    HESTR_0E2_D_TURNS=226,
    HESTR_0E3_NO_TRANSPORT_SHIPS_SEL=227,
    HESTR_0E4_SEND_D_SHIP_TO_ATTACK_=228,
    HESTR_0E5_SEND_D_SHIPS_TO_ATTACK=229,
    HESTR_0E6_NO_SHIPS_SELECTED=230,
    HESTR_0E7_ON=231,
    HESTR_0E8_OFF=232,
    HESTR_0E9_S_STARDATE_DD=233,
    HESTR_0EA_BLOCKED_BY_BLACK_HOLE=234,
    HESTR_0EB_D_PARSECS=235,
    HESTR_0EC_ALL_FREIGHTERS_ARE_CUR=236,
    HESTR_0ED_CHOOSE_THE_NUMBER_OF_F=237,
    HESTR_0EE_YOU_HAVE_D_SURPLUS_S_S=238,
    HESTR_0EF_SCRAPPING_D_S=239,
    HESTR_0F0_FREIGHTER=240,
    HESTR_0F1_FREIGHTERS=241,
    HESTR_0F2_GENERATING_UNIVERSE=242,
    HESTR_0F3_GALAXY_MAP=243,
    HESTR_0F4_NO_MULTIPLAYER_SAVED_G=244,
    HESTR_0F5_READY_TO_START_A_NETWO=245,
    HESTR_0F6_AUTO_DETECT=246,
    HESTR_0F7_TONE=247,
    HESTR_0F8_PULSE=248,
    HESTR_0F9_D_HUMAN_HAS_JOINED_THE=249,
    HESTR_0FA_D_HUMANS_HAVE_JOINED_T=250,
    HESTR_0FB_D_OF_D_HUMANS_COMPLETE=251,
    HESTR_0FC_S_S_SCORE_S=252,
    HESTR_0FD_SAVE_D_IS_NOT_A_VALID_=253,
    HESTR_0FE_ENTER_NET_GAME_NAME=254,
    HESTR_0FF_S_IS_ALREADY_A_GAME_NA=255,
    HESTR_100_ENTER_STAR_NAME=256,
    HESTR_101_ENTER_HOME_STAR_NAME=257,
    HESTR_102_D_OUTPOST=258,
    HESTR_103_D_OUTPOSTS=259,
    HESTR_104_D_TRANSPORT=260,
    HESTR_105_D_TRANSPORTS=261,
    HESTR_106_D_COLONY_SHIP=262,
    HESTR_107_D_COLONY_SHIPS=263,
    HESTR_108_TO_HIRE=264,
    HESTR_109_MAINT=265,
    HESTR_10A_NO=266,
    HESTR_10B_OFFICER_POOL=267,
    HESTR_10C_UNASSIGNED=268,
    HESTR_10D_FOR_HIRE_D=269,
    HESTR_10E_FOR_HIRE=270,
    HESTR_10F_OFFICER_QUEUE_FULL_FOR=271,
    HESTR_110_THE=272,
    HESTR_111_S_IS_IN_THE_OFFICER_PO=273,
    HESTR_112_S_DOES_NOT_HAVE_A_LOCA=274,
    HESTR_113_S_IS_LOCATED_ON_S_WHIC=275,
    HESTR_114_S_CANNOT_BE_LOCATED=276,
    HESTR_115_TRANSFERING_DISPLAY_TO=277,
    HESTR_116_TRANSFERING_DISPLAY_TO=278,
    HESTR_117_YOU_MAY_NOT_ASSIGN_S_T=279,
    HESTR_118_IT_WILL_TAKE_D_TURNS_F=280,
    HESTR_119_ASSIGN_S_TO_S=281,
    HESTR_11A_S_IN_ROUTE_TO_S_WILL_B=282,
    HESTR_11B_S_CURRENTLY_ON_S_WILL_=283,
    HESTR_11C_CHOOSE_HERO_TO_ASSIGN_=284,
    HESTR_11D_S_IS_ALREADY_IN_USE=285,
    HESTR_11E_YOU_CANNOT_HAVE_S_AS_Y=286,
    HESTR_11F_YOU_CANNOT_HAVE_S_AS_Y=287,
    HESTR_120_S_IS_IN_SS_HIRING_QUEU=288,
    HESTR_121_D_BCS_AND_D_BC_TURN=289,
    HESTR_122_S_IS_ALREADY_IN_YOUR_S=290,
    HESTR_123_YOU_HAVE_D_BC_IN_YOUR_=291,
    HESTR_124_S_WILL_COST_D_BCS_INIT=292,
    HESTR_125_S_WILL_COST_D_BCS_INIT=293,
    HESTR_126_DISMISS_S=294,
    HESTR_127_S_MUST_BE_HIRED_BEFORE=295,
    HESTR_128_YOU_CANNOT_ASSIGN_S_TO=296,
    HESTR_129_D_BCS_NO_MAINTENANCE=297,
    HESTR_12A_SKILL=298,
    HESTR_12B_YOU_MAY_NOT_PLACE_S_IN=299,
    HESTR_12C_YOU_MAY_NOT_PLACE_LEAD=300,
    HESTR_12D_S_CANNOT_BE_PUT_INTO_S=301,
    HESTR_12E_COLONIES_ARE_NOT_ACCES=302,
    HESTR_12F_ETA_D=303,
    HESTR_130_SPACE_S=304,
    HESTR_131_S_FLEET=305,
    HESTR_132_MAKE_S_AVAILABLE_TO_S=306,
    HESTR_133_ETA_D=307,
    HESTR_134_YOU_ALREADY_HAVE_ONE_C=308,
    HESTR_135_YOU_ALREADY_HAVE_ONE_O=309,
    HESTR_136_COLONY=310,
    HESTR_137_OUTPOST=311,
    HESTR_138_S_ALREADY_HAS_A_COURSE=312,
    HESTR_139_S_HAS_SET_A_COURSE_FOR=313,
    HESTR_13A_S_HAS_SET_A_COURSE_SHO=314,
    HESTR_13B_S_IS_GUARDED_BY_A_S_DO=315,
    HESTR_13C_S_IS_GUARDED_BY_A_S_DO=316,
    HESTR_13D_S_HAS_A_COURSE_LAID_FO=317,
    HESTR_13E_S_HAS_A_COURSE_LAID_FO=318,
    HESTR_13F_SHIP_MOVEMENT_IS_NOT_A=319,
    HESTR_140_ARTIFACTS=320,
    HESTR_141_D_FOOD=321,
    HESTR_142_D_PROD=322,
    HESTR_143_D_MAX_POP=323,
    HESTR_144_BLACK_HOLE_BLOCKS_TRAV=324,
    HESTR_145_BLACK_HOLE=325,
    HESTR_146_CANNOT_REACH=326,
    HESTR_147_STAR_SYSTEM_UNREACHABL=327,
    HESTR_148_OUT_OF_RANGE=328,
    HESTR_149_YOU_ARE_UNABLE_TO_UNDO=329,
    HESTR_14A_YOU_ARE_UNABLE_TO_UNDO=330,
    HESTR_14B_ETA_D_TURN=331,
    HESTR_14C_ETA_D_TURNS=332,
    HESTR_14D_COLONY_SHIP_HEADING_TO=333,
    HESTR_14E_OUTPOST_SHIP_HEADING_T=334,
    HESTR_14F_NO_PLANETS_IN_THE_S_ST=335,
    HESTR_150_D_TURN_TO=336,
    HESTR_151_D_TURNS_TO=337,
    HESTR_152_D_PROD_WORKER=338,
    HESTR_153_DEFEATING_ANTARAN_HOME=339,
    HESTR_154_D_CAPTURED_COLONIST=340,
    HESTR_155_D_CAPTURED_COLONISTS=341,
    HESTR_156_WON_COUNCIL_VOTE=342,
    HESTR_157_TOTAL_SCORE=343,
    HESTR_158_PLAYERS_ELIMINATED=344,
    HESTR_159_D_PLAYER_ELIMINATED=345,
    HESTR_15A_D_PLAYERS_ELIMINATED=346,
    HESTR_15B_CAPTURED_ORION=347,
    HESTR_15C_D_POPULATION=348,
    HESTR_15D_TECHNOLOGY=349,
    HESTR_15E_TIME_D_TURNS=350,
    HESTR_15F_GAS_GIANT_UNINHABITABL=351,
    HESTR_160_ASTEROID_UNINHABITABLE=352,
    HESTR_161_OUTPOST=353,
    HESTR_162_D_MAX_POP=354,
    HESTR_163_S_FLEET=355,
    HESTR_164_S_2D_COLONY_SHIP=356,
    HESTR_165_S_2D_COLONY_SHIPS=357,
    HESTR_166_S_2D_OUTPOST_SHIP=358,
    HESTR_167_S_2D_OUTPOST_SHIPS=359,
    HESTR_168_S_2D_TRANSPORT_SHIP=360,
    HESTR_169_S_2D_TRANSPORT_SHIPS=361,
    HESTR_16A_STAR_SYSTEM_S=362,
    HESTR_16B_STAR_SYSTEM_UNEXPLORED=363,
    HESTR_16C_WORMHOLE_LINKS_S=364,
    HESTR_16D_STABLE_WORMHOLE=365,
    HESTR_16E_BONUS=366,
    HESTR_16F_PENALTY=367,
    HESTR_170_MAX_POPULATION=368,
    HESTR_171_FOOD_PER_FARMER=369,
    HESTR_172_INDUSTRY_PER_WORKER=370,
    HESTR_173_RESEARCH_PER_SCIENTIST=371,
    HESTR_174_NO_MAINTENANCE_BONUS=372,
    HESTR_175_MAINTENANCE=373,
    HESTR_176_NO_WORKER_BONUS=374,
    HESTR_177_WORKER=375,
    HESTR_178_COLONY_INFO_FOR_S=376,
    HESTR_179_STAR_GATE=377,
    HESTR_17A_JUMP_GATE=378,
    HESTR_17B_ARTEMIS_NET=379,
    HESTR_17C_DIMENSIONAL_PORTAL=380,
    HESTR_17D_D_S_SHIPS_D_NONCOMBAT=381,
    HESTR_17E_D_PARSECS_TURN=382,
    HESTR_17F_ADD_NEW_WEAPON=383,
    HESTR_180_ADD_NEW_SPECIAL_SYSTEM=384,
    HESTR_181_D_D_POP=385,
    HESTR_182_THE=386,
    HESTR_183_BREAKTHROUGH=387,
    HESTR_184_EMPTY_SLOT=388,
    HESTR_185_COST=389,
    HESTR_186_S=390,
    HESTR_187_SHIP_INITIATIVE=391,
    HESTR_188_NONE=392,
    HESTR_189_MAINTENANCE_BONUS=393,
    HESTR_18A_MAINTENANCE_PENALTY=394,
    HESTR_18B_WORKER_BONUS=395,
    HESTR_18C_WORKER_PENALTY=396
} HESTRNGS;

typedef enum MAIN_MENU_BTN {
    MAIN_MENU_BTN_CONTINUE=0,
    MAIN_MENU_BTN_LOAD=1,
    MAIN_MENU_BTN_NEW=2,
    MAIN_MENU_BTN_MULTIPLAY=3,
    MAIN_MENU_BTN_HELP=4,
    MAIN_MENU_BTN_QUIT=5
} MAIN_MENU_BTN;

typedef enum MINERAL_RESOURCE {
    MINERAL_RESOURCE_ULTRA_POOR=0,
    MINERAL_RESOURCE_POOR=1,
    MINERAL_RESOURCE_ABUNDANT=2,
    MINERAL_RESOURCE_RICH=3,
    MINERAL_RESOURCE_ULTRA_RICH=4
} MINERAL_RESOURCE;

typedef enum PLANET_CLIMATE {
    PLANET_CLIMAGE_TOXIC=0,
    PLANET_CLIMAGE_RADIATED=1,
    PLANET_CLIMAGE_BARREN=2,
    PLANET_CLIMAGE_DESERT=3,
    PLANET_CLIMAGE_TUNDRA=4,
    PLANET_CLIMAGE_OCEAN=5,
    PLANET_CLIMAGE_SWAMP=6,
    PLANET_CLIMAGE_ARID=7,
    PLANET_CLIMAGE_TERRAN=8,
    PLANET_CLIMAGE_GAIA=9
} PLANET_CLIMATE;

typedef enum PLANET_GRAVITY {
    PLANET_GRAVITY_LOW_G=0,
    PLANET_GRAVITY_NORMAL_G=1,
    PLANET_GRAVITY_HEAVY_G=2
} PLANET_GRAVITY;

typedef enum PLANET_SIZE {
    PLANET_SIZE_TINY=0,
    PLANET_SIZE_SMALL=1,
    PLANET_SIZE_MEDIUM=2,
    PLANET_SIZE_LARGE=3,
    PLANET_SIZE_HUGE=4
} PLANET_SIZE;

typedef enum PLANET_TYPE {
    PLANET_TYPE_NOT_USED0=0,
    PLANET_TYPE_ASTEROID=1,
    PLANET_TYPE_GAS_GIANT=2,
    PLANET_TYPE_PLANET=3,
    PLANET_TYPE_NOT_USED4=4,
    PLANET_TYPE_NOT_USED5=5,
    PLANET_TYPE_NOT_USED6=6,
    PLANET_TYPE_NOT_USED7=7,
    PLANET_TYPE_NOT_USED8=8,
    PLANET_TYPE_NOT_USED9=9
} PLANET_TYPE;

typedef struct s_animation_header s_animation_header, *Ps_animation_header;

typedef short int16_t;

typedef uchar uint8_t;

typedef uint uint32_t;

struct s_animation_header {
    int16_t width;
    int16_t height;
    int16_t current_frame;
    int16_t frame_count;
    uint8_t loop_frame;
    uint8_t field5_0x9;
    uint8_t field6_0xa;
    uint8_t flags;
    uint32_t frame_offsets[1];
};

typedef struct s_antaran s_antaran, *Ps_antaran;

struct s_antaran {
    undefined4 _antarans;
    undefined2 field1_0x4;
    undefined2 field2_0x6;
    undefined2 field3_0x8;
    undefined field4_0xa;
    undefined field5_0xb;
    undefined field6_0xc;
    undefined field7_0xd;
    undefined2 field8_0xe;
    undefined field9_0x10;
    undefined field10_0x11;
    undefined field11_0x12;
    undefined field12_0x13;
    undefined field13_0x14;
    undefined field14_0x15;
    undefined field15_0x16;
    undefined field16_0x17;
    undefined2 field17_0x18;
    undefined2 field18_0x1a;
    undefined field19_0x1c;
    undefined field20_0x1d;
    undefined field21_0x1e;
    undefined field22_0x1f;
    undefined field23_0x20;
    undefined field24_0x21;
    undefined field25_0x22;
    undefined field26_0x23;
    undefined field27_0x24;
    undefined field28_0x25;
    undefined2 field29_0x26;
    undefined2 field30_0x28;
    undefined2 field31_0x2a;
    undefined2 field32_0x2c;
    undefined field33_0x2e;
    undefined field34_0x2f;
    undefined field35_0x30;
    undefined field36_0x31;
    undefined field37_0x32;
    undefined field38_0x33;
    undefined field39_0x34;
    undefined field40_0x35;
    undefined field41_0x36;
    undefined field42_0x37;
    undefined2 field43_0x38;
    undefined2 field44_0x3a;
    undefined1 field45_0x3c;
    undefined4 field46_0x3d;
    undefined1 field47_0x41;
};

typedef struct s_bomb s_bomb, *Ps_bomb;

typedef char sbyte;

struct s_bomb {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    sbyte field10_0xa;
    short field11_0xb;
    undefined field12_0xd;
    undefined field13_0xe;
};

typedef struct s_class_group s_class_group, *Ps_class_group;

struct s_class_group {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
};

typedef struct s_colony s_colony, *Ps_colony;


/* WARNING! conflicting data type names: /rpcndr.h/byte - /byte */

typedef struct s_pop_info s_pop_info, *Ps_pop_info;

struct s_pop_info {
    uint race:4;
    uint loyalty:3;
    uint job:2;
    uint conquered:1;
    uint unknown1:4;
    uint starve_priority:2;
    uint unknown2:15;
    uint employed:1;
};

struct s_colony {
    char owner;
    char allocated_to;
    short planet;
    short officer;
    byte outpost_flag;
    char morale;
    short pollution;
    char n_pops;
    char specialty;
    struct s_pop_info pop[42];
    short pop_roundoff[10];
    short pop_growth[10];
    char n_turns_existed;
    char food2_per_farmer;
    char industry_per_worker;
    char research_per_scientist;
    char max_farms;
    char max_population;
    char climate;
    short ground_strength;
    short space_strength;
    short production[4];
    char maintenance[4];
    short imports[4];
    char n_industry_recyclers;
    char food2_needed_for_our_empire;
    char food2_needed_for_assimilated;
    char food2_needed_for_conquered;
    char food2_needed_for_natives;
    char industry2_needed_for_our_empire;
    char industry2_needed_for_androids;
    char industry2_needed_for_assimilated;
    char industry2_needed_for_conquered;
    char food2_needed_for_empire[8];
    char industry2_needed_for_empire[8];
    char n_food_replicated;
    short producing[7];
    short just_produced;
    short production_spent;
    short n_industry_taxed;
    char auto_building;
    short production_surplus;
    short bought_outright;
    char occupation_points;
    char occupation_policy;
    short military[2];
    char tank_roundoff;
    char infantry_roundoff;
    byte buildings[49];
    short last_turn_building_destroyed;
};

typedef struct s_colony_bldg s_colony_bldg, *Ps_colony_bldg;

struct s_colony_bldg {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
};

typedef struct s_colors s_colors, *Ps_colors;

struct s_colors {
    uint8_t colors[8];
};

typedef struct s_combat_data s_combat_data, *Ps_combat_data;


struct s_combat_data {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    sbyte owner;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    sbyte field36_0x24;
    undefined field37_0x25;
    sbyte field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
    undefined field52_0x34;
    undefined field53_0x35;
    undefined field54_0x36;
    undefined field55_0x37;
    undefined field56_0x38;
    undefined field57_0x39;
    undefined field58_0x3a;
    undefined field59_0x3b;
    undefined field60_0x3c;
    undefined field61_0x3d;
    undefined field62_0x3e;
    undefined field63_0x3f;
    undefined field64_0x40;
    undefined field65_0x41;
    undefined field66_0x42;
    undefined field67_0x43;
    undefined field68_0x44;
    undefined field69_0x45;
    undefined field70_0x46;
    undefined field71_0x47;
    undefined field72_0x48;
    undefined field73_0x49;
    int8_t in_stasis;
    undefined field75_0x4b;
    undefined field76_0x4c;
    undefined field77_0x4d;
    undefined field78_0x4e;
    undefined field79_0x4f;
    undefined field80_0x50;
    undefined field81_0x51;
    undefined field82_0x52;
    undefined field83_0x53;
    undefined field84_0x54;
    undefined field85_0x55;
    undefined field86_0x56;
    undefined field87_0x57;
    undefined field88_0x58;
    undefined field89_0x59;
    undefined field90_0x5a;
    undefined field91_0x5b;
    undefined field92_0x5c;
    undefined field93_0x5d;
    undefined field94_0x5e;
    undefined field95_0x5f;
    undefined field96_0x60;
    undefined field97_0x61;
    undefined field98_0x62;
    undefined field99_0x63;
    undefined field100_0x64;
    undefined field101_0x65;
    undefined field102_0x66;
    undefined field103_0x67;
    undefined field104_0x68;
    undefined field105_0x69;
    undefined field106_0x6a;
    undefined field107_0x6b;
    undefined field108_0x6c;
    undefined field109_0x6d;
    undefined field110_0x6e;
    undefined field111_0x6f;
    undefined field112_0x70;
    undefined field113_0x71;
    undefined field114_0x72;
    undefined field115_0x73;
    undefined field116_0x74;
    undefined field117_0x75;
    undefined field118_0x76;
    undefined field119_0x77;
    undefined field120_0x78;
    undefined field121_0x79;
    undefined field122_0x7a;
    undefined field123_0x7b;
    undefined field124_0x7c;
    undefined field125_0x7d;
    undefined field126_0x7e;
    undefined field127_0x7f;
    undefined field128_0x80;
    undefined field129_0x81;
    undefined field130_0x82;
    undefined field131_0x83;
    undefined field132_0x84;
    undefined field133_0x85;
    undefined field134_0x86;
    undefined field135_0x87;
    undefined field136_0x88;
    undefined field137_0x89;
    undefined field138_0x8a;
    undefined field139_0x8b;
    undefined field140_0x8c;
    undefined field141_0x8d;
    undefined field142_0x8e;
    undefined field143_0x8f;
    undefined field144_0x90;
    undefined field145_0x91;
    undefined field146_0x92;
    undefined field147_0x93;
    undefined field148_0x94;
    undefined field149_0x95;
    undefined field150_0x96;
    undefined field151_0x97;
    undefined field152_0x98;
    undefined field153_0x99;
    undefined field154_0x9a;
    undefined field155_0x9b;
    undefined field156_0x9c;
    undefined field157_0x9d;
    undefined field158_0x9e;
    undefined field159_0x9f;
    undefined field160_0xa0;
    undefined field161_0xa1;
    undefined field162_0xa2;
    undefined field163_0xa3;
    undefined field164_0xa4;
    undefined field165_0xa5;
    undefined field166_0xa6;
    undefined field167_0xa7;
    undefined field168_0xa8;
    undefined field169_0xa9;
    undefined field170_0xaa;
    undefined field171_0xab;
    undefined field172_0xac;
    undefined field173_0xad;
    undefined field174_0xae;
    undefined field175_0xaf;
    undefined field176_0xb0;
    undefined field177_0xb1;
    undefined field178_0xb2;
    undefined field179_0xb3;
    undefined field180_0xb4;
    undefined field181_0xb5;
    undefined field182_0xb6;
    undefined field183_0xb7;
    undefined field184_0xb8;
    undefined field185_0xb9;
    undefined field186_0xba;
    undefined field187_0xbb;
    undefined field188_0xbc;
    undefined field189_0xbd;
    undefined field190_0xbe;
    undefined field191_0xbf;
    undefined field192_0xc0;
    undefined field193_0xc1;
    undefined field194_0xc2;
    undefined field195_0xc3;
    undefined field196_0xc4;
    undefined field197_0xc5;
    undefined field198_0xc6;
    undefined field199_0xc7;
    undefined field200_0xc8;
    undefined field201_0xc9;
    undefined field202_0xca;
    undefined field203_0xcb;
    undefined field204_0xcc;
    undefined field205_0xcd;
    undefined field206_0xce;
    undefined field207_0xcf;
    undefined field208_0xd0;
    undefined field209_0xd1;
    undefined field210_0xd2;
    undefined field211_0xd3;
    undefined field212_0xd4;
    undefined field213_0xd5;
    undefined field214_0xd6;
    undefined field215_0xd7;
    undefined field216_0xd8;
    undefined field217_0xd9;
    undefined field218_0xda;
    undefined field219_0xdb;
    undefined field220_0xdc;
    undefined field221_0xdd;
    undefined field222_0xde;
    undefined field223_0xdf;
    undefined field224_0xe0;
    undefined field225_0xe1;
    undefined field226_0xe2;
    undefined field227_0xe3;
    undefined field228_0xe4;
    undefined field229_0xe5;
    undefined field230_0xe6;
    undefined field231_0xe7;
    undefined field232_0xe8;
    undefined field233_0xe9;
    undefined field234_0xea;
    undefined field235_0xeb;
    undefined field236_0xec;
    undefined field237_0xed;
    undefined field238_0xee;
    undefined field239_0xef;
    undefined field240_0xf0;
    undefined field241_0xf1;
    undefined field242_0xf2;
    undefined field243_0xf3;
    undefined field244_0xf4;
    undefined field245_0xf5;
    undefined field246_0xf6;
    undefined field247_0xf7;
    undefined field248_0xf8;
    undefined field249_0xf9;
    undefined field250_0xfa;
    undefined field251_0xfb;
    undefined field252_0xfc;
    undefined field253_0xfd;
    undefined field254_0xfe;
    undefined field255_0xff;
    undefined field256_0x100;
    undefined field257_0x101;
    undefined field258_0x102;
    undefined field259_0x103;
    undefined field260_0x104;
    undefined field261_0x105;
    undefined field262_0x106;
    undefined field263_0x107;
    undefined field264_0x108;
    undefined field265_0x109;
    undefined field266_0x10a;
    undefined field267_0x10b;
    undefined field268_0x10c;
    undefined field269_0x10d;
    undefined field270_0x10e;
    undefined field271_0x10f;
    undefined field272_0x110;
    undefined field273_0x111;
    undefined field274_0x112;
    undefined field275_0x113;
    undefined field276_0x114;
    undefined field277_0x115;
    undefined field278_0x116;
    undefined field279_0x117;
    undefined field280_0x118;
    undefined field281_0x119;
    undefined field282_0x11a;
    undefined field283_0x11b;
    undefined field284_0x11c;
    undefined field285_0x11d;
    undefined field286_0x11e;
    undefined field287_0x11f;
    undefined field288_0x120;
    undefined field289_0x121;
    undefined field290_0x122;
    undefined field291_0x123;
    undefined field292_0x124;
    undefined field293_0x125;
    undefined field294_0x126;
    undefined field295_0x127;
    undefined field296_0x128;
    undefined field297_0x129;
    undefined field298_0x12a;
    undefined field299_0x12b;
    undefined field300_0x12c;
    undefined field301_0x12d;
    undefined field302_0x12e;
    undefined field303_0x12f;
    undefined field304_0x130;
    undefined field305_0x131;
    undefined field306_0x132;
    undefined field307_0x133;
    undefined field308_0x134;
    undefined field309_0x135;
    undefined field310_0x136;
    undefined field311_0x137;
    undefined field312_0x138;
};

typedef struct s_current_design s_current_design, *Ps_current_design;

struct s_current_design {
    char name[16];
    undefined field1_0x10;
    short shield;
    short shield_cost;
    short shield_space;
    undefined field5_0x17;
    undefined field6_0x18;
    undefined field7_0x19;
    undefined field8_0x1a;
    short ftl_type;
    short drive_speed;
    short drive_cost;
    short base_space; /* Created by Rename Structure Field action */
    undefined field13_0x23;
    undefined field14_0x24;
    short base_combat_speed; /* Created by Rename Structure Field action */
    undefined field16_0x27;
    undefined field17_0x28;
    short field18_0x29;
    short computer_type;
    short computer_cost;
    short fuel_type;
    short armor_type;
    short armor_cost;
    short weapon_type[8];
    short weapon_count[8];
    char weapon_firing_arc[8]; /* Created by Rename Structure Field action */
    short weapon_specials[8]; /* Created by Rename Structure Field action */
    int weapon_cost[8];
    int weapon_space[8];
    char weapon_ammo[8]; /* Created by Rename Structure Field action */
    short special_devices[8];
    short special_device_cost[8];
    short special_device_space[8];
    short ship_size;
    short picture_type;
    int hull_space;
    int space_used;
    int total_cost;
    int8_t show_help;
    short help1;
    short help2;
    undefined field42_0xfa;
    undefined field43_0xfb;
    undefined field44_0xfc;
    undefined field45_0xfd;
    undefined field46_0xfe;
    undefined field47_0xff;
    undefined field48_0x100;
    undefined field49_0x101;
    undefined field50_0x102;
    undefined field51_0x103;
    undefined field52_0x104;
    undefined field53_0x105;
    undefined field54_0x106;
    undefined field55_0x107;
    undefined field56_0x108;
    undefined field57_0x109;
    undefined field58_0x10a;
    undefined field59_0x10b;
    undefined field60_0x10c;
    undefined field61_0x10d;
    undefined field62_0x10e;
    undefined field63_0x10f;
    undefined field64_0x110;
    undefined field65_0x111;
    undefined field66_0x112;
    undefined field67_0x113;
    undefined field68_0x114;
    undefined field69_0x115;
    undefined field70_0x116;
    undefined field71_0x117;
    undefined field72_0x118;
    undefined field73_0x119;
    undefined field74_0x11a;
    undefined field75_0x11b;
    undefined field76_0x11c;
    undefined field77_0x11d;
    undefined field78_0x11e;
    undefined field79_0x11f;
    undefined field80_0x120;
    undefined field81_0x121;
    undefined field82_0x122;
    short field83_0x123;
};

typedef struct s_event_data s_event_data, *Ps_event_data;

struct s_event_data {
    sbyte field0_0x0;
    sbyte field1_0x1;
    sbyte field2_0x2;
    short event_type;
    short field4_0x5;
    short field5_0x7;
};

typedef struct s_field s_field, *Ps_field;

typedef union s_field_detail s_field_detail, *Ps_field_detail;

typedef struct s_field_button s_field_button, *Ps_field_button;

typedef struct s_field_grid s_field_grid, *Ps_field_grid;

typedef struct s_field_radio s_field_radio, *Ps_field_radio;

typedef struct s_field_scroll s_field_scroll, *Ps_field_scroll;

typedef struct s_field_cont_input s_field_cont_input, *Ps_field_cont_input;

struct s_field_grid {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    short step_x;
    undefined field6_0xe;
    undefined field7_0xf;
    short step_y;
    undefined field9_0x12;
    undefined field10_0x13;
    short *grid_x_ptr;
    short *grid_y_ptr;
    struct s_animation_header *pic;
    short field14_0x20;
    undefined field15_0x22;
    undefined field16_0x23;
};

struct s_field_scroll {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    short field4_0x8;
    undefined field5_0xa;
    undefined field6_0xb;
    short field7_0xc;
    undefined field8_0xe;
    undefined field9_0xf;
    short *value;
    short field11_0x14;
    undefined field12_0x16;
    undefined field13_0x17;
    short field14_0x18;
    undefined field15_0x1a;
    undefined field16_0x1b;
    short field17_0x1c;
    undefined field18_0x1e;
    undefined field19_0x1f;
    short field20_0x20;
    undefined field21_0x22;
    undefined field22_0x23;
};

struct s_field_button {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    short help_start;
    undefined field6_0xe;
    undefined field7_0xf;
    short help_end;
    undefined field9_0x12;
    undefined field10_0x13;
    short full_store_flag;
    undefined field12_0x16;
    undefined field13_0x17;
    int field14_0x18;
    struct s_animation_header *pic;
    short field16_0x20;
    undefined field17_0x22;
    undefined field18_0x23;
};

struct s_field_cont_input {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    int field5_0xc;
    short help_end;
    undefined field7_0x12;
    undefined field8_0x13;
    short full_store_flag;
    undefined field10_0x16;
    undefined field11_0x17;
    int field12_0x18;
    short field13_0x1c;
    undefined field14_0x1e;
    undefined field15_0x1f;
    short field16_0x20;
    undefined field17_0x22;
    undefined field18_0x23;
};

struct s_field_radio {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    short help_start;
    undefined field6_0xe;
    undefined field7_0xf;
    char *selection;
    short field9_0x14;
    undefined field10_0x16;
    undefined field11_0x17;
    int field12_0x18;
    struct s_animation_header *pic;
    short field14_0x20;
    undefined field15_0x22;
    undefined field16_0x23;
};

union s_field_detail {
    struct s_field_button button;
    struct s_field_grid grid;
    struct s_field_radio radio;
    struct s_field_scroll scroll;
    struct s_field_cont_input cont_input;
};

struct s_field {
    short x;
    short y;
    short x_end;
    short y_end;
    short field_type;
    short font_style;
    struct s_colors *font_color;
    union s_field_detail detail;
    char hotkey;
    short sound;
};

typedef struct s_font s_font, *Ps_font;

typedef ushort uint16_t;

struct s_font {
    byte selected_colors[8];
    short font_style_height;
    short font_style;
    byte normal_colors[8];
    byte highlight_colors[8];
    byte special_colors[8];
    uint16_t current_height;
    uint16_t line_height;
    uint16_t field24_0x28;
    short font_style_spacing;
    char font_style_widths[256];
    uint32_t font_style_offsets[256];
    short font_heights[8];
    short font_spacing[8];
    short field31_0x54c[8];
    byte font_glyph_widths[6][256];
    uint32_t font_glyph_offsets[6][256];
};

typedef struct s_font_color s_font_color, *Ps_font_color;

struct s_font_color {
    uint8_t field1;
    uint32_t field2;
    uint16_t field6;
    uint8_t field7;
};

typedef struct s_game_anims s_game_anims, *Ps_game_anims;

struct s_game_anims {
    struct s_animation_header *pictures[25];
    int8_t field1_0x64;
};

typedef struct s_game_popup_fields s_game_popup_fields, *Ps_game_popup_fields;

typedef int int32_t;

struct s_game_popup_fields {
    int32_t field0_0x0[10];
    short field_l;
    short field_s;
    short field_q;
    short field_n;
    short field_o;
    short field_esc;
    undefined field7_0x34;
    undefined field8_0x35;
    undefined field9_0x36;
    undefined field10_0x37;
    short saved_game_fields[10];
    short field_d;
    undefined field13_0x4e;
    undefined field14_0x4f;
    undefined field15_0x50;
    undefined field16_0x51;
    short option_names[13];
    short field_A;
    short field19_0x6e;
    short field_esc_2;
    short field_space;
    short music_level;
    short sound_fx_level;
    short music_on;
    ushort sound_fx_on;
    short save_slot_fields[10];
    ushort options[13];
    int8_t saved_games[10];
    undefined field29_0xb4;
    undefined field30_0xb5;
    undefined field31_0xb6;
    undefined field32_0xb7;
    undefined field33_0xb8;
    undefined field34_0xb9;
    undefined field35_0xba;
    undefined field36_0xbb;
    undefined field37_0xbc;
    undefined field38_0xbd;
    undefined field39_0xbe;
    undefined field40_0xbf;
    undefined field41_0xc0;
    undefined field42_0xc1;
    undefined field43_0xc2;
    undefined field44_0xc3;
    undefined field45_0xc4;
    undefined field46_0xc5;
    undefined field47_0xc6;
    undefined field48_0xc7;
    undefined field49_0xc8;
    undefined field50_0xc9;
    undefined field51_0xca;
    undefined field52_0xcb;
    undefined field53_0xcc;
    undefined field54_0xcd;
    undefined field55_0xce;
    undefined field56_0xcf;
    undefined field57_0xd0;
    undefined field58_0xd1;
    undefined field59_0xd2;
    undefined field60_0xd3;
    undefined field61_0xd4;
    undefined field62_0xd5;
    undefined field63_0xd6;
    undefined field64_0xd7;
    undefined field65_0xd8;
    undefined field66_0xd9;
    undefined field67_0xda;
    undefined field68_0xdb;
    undefined field69_0xdc;
    undefined field70_0xdd;
    undefined field71_0xde;
    undefined field72_0xdf;
    undefined field73_0xe0;
    undefined field74_0xe1;
    undefined field75_0xe2;
    undefined field76_0xe3;
    undefined field77_0xe4;
    undefined field78_0xe5;
    undefined field79_0xe6;
    undefined field80_0xe7;
    undefined field81_0xe8;
    undefined field82_0xe9;
    undefined field83_0xea;
    undefined field84_0xeb;
    undefined field85_0xec;
    undefined field86_0xed;
    undefined field87_0xee;
    undefined field88_0xef;
    undefined field89_0xf0;
    undefined field90_0xf1;
    undefined field91_0xf2;
    undefined field92_0xf3;
    undefined field93_0xf4;
    undefined field94_0xf5;
    undefined field95_0xf6;
    undefined field96_0xf7;
    undefined field97_0xf8;
    undefined field98_0xf9;
    undefined field99_0xfa;
    undefined field100_0xfb;
    undefined field101_0xfc;
    undefined field102_0xfd;
    undefined field103_0xfe;
    undefined field104_0xff;
    undefined field105_0x100;
    undefined field106_0x101;
    undefined field107_0x102;
    undefined field108_0x103;
    undefined field109_0x104;
    undefined field110_0x105;
    undefined field111_0x106;
    undefined field112_0x107;
    undefined field113_0x108;
    undefined field114_0x109;
    undefined field115_0x10a;
    undefined field116_0x10b;
    undefined field117_0x10c;
    undefined field118_0x10d;
    undefined field119_0x10e;
    undefined field120_0x10f;
    undefined field121_0x110;
    undefined field122_0x111;
    undefined field123_0x112;
    undefined field124_0x113;
    undefined field125_0x114;
    undefined field126_0x115;
    undefined field127_0x116;
    undefined field128_0x117;
    undefined field129_0x118;
    undefined field130_0x119;
    undefined field131_0x11a;
    undefined field132_0x11b;
    undefined field133_0x11c;
    undefined field134_0x11d;
    undefined field135_0x11e;
    undefined field136_0x11f;
    undefined field137_0x120;
    undefined field138_0x121;
    undefined field139_0x122;
    undefined field140_0x123;
    undefined field141_0x124;
    undefined field142_0x125;
    undefined field143_0x126;
    undefined field144_0x127;
    undefined field145_0x128;
    undefined field146_0x129;
    undefined field147_0x12a;
    undefined field148_0x12b;
    undefined field149_0x12c;
    undefined field150_0x12d;
    undefined field151_0x12e;
    undefined field152_0x12f;
    undefined field153_0x130;
    undefined field154_0x131;
    undefined field155_0x132;
    undefined field156_0x133;
    undefined field157_0x134;
    undefined field158_0x135;
    undefined field159_0x136;
    undefined field160_0x137;
    undefined field161_0x138;
    undefined field162_0x139;
    undefined field163_0x13a;
    undefined field164_0x13b;
    undefined field165_0x13c;
    undefined field166_0x13d;
    undefined field167_0x13e;
    undefined field168_0x13f;
    undefined field169_0x140;
    undefined field170_0x141;
    undefined field171_0x142;
    undefined field172_0x143;
    undefined field173_0x144;
    undefined field174_0x145;
    undefined field175_0x146;
    undefined field176_0x147;
    undefined field177_0x148;
    undefined field178_0x149;
    undefined field179_0x14a;
    undefined field180_0x14b;
    undefined field181_0x14c;
    undefined field182_0x14d;
    undefined field183_0x14e;
    undefined field184_0x14f;
    undefined field185_0x150;
    undefined field186_0x151;
    undefined field187_0x152;
    undefined field188_0x153;
    undefined field189_0x154;
    undefined field190_0x155;
    undefined field191_0x156;
    undefined field192_0x157;
    undefined field193_0x158;
    undefined field194_0x159;
    undefined field195_0x15a;
    undefined field196_0x15b;
    undefined field197_0x15c;
    undefined field198_0x15d;
    undefined field199_0x15e;
    undefined field200_0x15f;
    undefined field201_0x160;
    undefined field202_0x161;
    undefined field203_0x162;
    undefined field204_0x163;
    undefined field205_0x164;
    undefined field206_0x165;
    undefined field207_0x166;
    undefined field208_0x167;
    undefined field209_0x168;
    undefined field210_0x169;
    undefined field211_0x16a;
    undefined field212_0x16b;
    undefined field213_0x16c;
    undefined field214_0x16d;
    undefined field215_0x16e;
    undefined field216_0x16f;
    undefined field217_0x170;
    undefined field218_0x171;
    undefined field219_0x172;
    undefined field220_0x173;
    undefined field221_0x174;
    undefined field222_0x175;
    undefined field223_0x176;
    undefined field224_0x177;
    undefined field225_0x178;
    undefined field226_0x179;
    undefined field227_0x17a;
    undefined field228_0x17b;
    undefined field229_0x17c;
    undefined field230_0x17d;
    undefined field231_0x17e;
    undefined field232_0x17f;
    undefined field233_0x180;
    undefined field234_0x181;
    undefined field235_0x182;
    undefined field236_0x183;
    undefined field237_0x184;
    undefined field238_0x185;
    undefined field239_0x186;
    undefined field240_0x187;
    undefined field241_0x188;
    undefined field242_0x189;
    undefined field243_0x18a;
    undefined field244_0x18b;
    undefined field245_0x18c;
    undefined field246_0x18d;
    undefined field247_0x18e;
    undefined field248_0x18f;
    undefined field249_0x190;
    undefined field250_0x191;
    undefined field251_0x192;
    undefined field252_0x193;
    undefined field253_0x194;
    undefined field254_0x195;
    undefined field255_0x196;
    undefined field256_0x197;
    undefined field257_0x198;
    undefined field258_0x199;
    undefined field259_0x19a;
    undefined field260_0x19b;
    undefined field261_0x19c;
    undefined field262_0x19d;
    undefined field263_0x19e;
    undefined field264_0x19f;
    undefined field265_0x1a0;
    undefined field266_0x1a1;
    undefined field267_0x1a2;
    undefined field268_0x1a3;
    undefined field269_0x1a4;
    undefined field270_0x1a5;
    undefined field271_0x1a6;
    undefined field272_0x1a7;
    undefined field273_0x1a8;
    undefined field274_0x1a9;
    undefined field275_0x1aa;
    undefined field276_0x1ab;
    undefined field277_0x1ac;
    undefined field278_0x1ad;
    undefined field279_0x1ae;
    undefined field280_0x1af;
    undefined field281_0x1b0;
    undefined field282_0x1b1;
    undefined field283_0x1b2;
    undefined field284_0x1b3;
    undefined field285_0x1b4;
    undefined field286_0x1b5;
    undefined field287_0x1b6;
    undefined field288_0x1b7;
    undefined field289_0x1b8;
    undefined field290_0x1b9;
    undefined field291_0x1ba;
    undefined field292_0x1bb;
    undefined field293_0x1bc;
    undefined field294_0x1bd;
    undefined field295_0x1be;
    undefined field296_0x1bf;
    undefined field297_0x1c0;
    undefined field298_0x1c1;
    undefined field299_0x1c2;
    undefined field300_0x1c3;
    undefined field301_0x1c4;
    undefined field302_0x1c5;
    undefined field303_0x1c6;
    undefined field304_0x1c7;
    undefined field305_0x1c8;
    undefined field306_0x1c9;
    undefined field307_0x1ca;
    undefined field308_0x1cb;
    undefined field309_0x1cc;
    undefined field310_0x1cd;
    undefined field311_0x1ce;
    undefined field312_0x1cf;
    undefined field313_0x1d0;
    undefined field314_0x1d1;
    undefined field315_0x1d2;
    undefined field316_0x1d3;
    undefined field317_0x1d4;
    undefined field318_0x1d5;
    undefined field319_0x1d6;
    undefined field320_0x1d7;
    undefined field321_0x1d8;
    undefined field322_0x1d9;
    undefined field323_0x1da;
    undefined field324_0x1db;
    undefined field325_0x1dc;
    undefined field326_0x1dd;
    undefined field327_0x1de;
    undefined field328_0x1df;
    undefined field329_0x1e0;
    undefined field330_0x1e1;
    undefined field331_0x1e2;
    undefined field332_0x1e3;
    undefined field333_0x1e4;
    undefined field334_0x1e5;
    undefined field335_0x1e6;
    undefined field336_0x1e7;
    undefined field337_0x1e8;
    undefined field338_0x1e9;
    undefined field339_0x1ea;
    undefined field340_0x1eb;
    undefined field341_0x1ec;
    undefined field342_0x1ed;
    undefined field343_0x1ee;
    undefined field344_0x1ef;
    undefined field345_0x1f0;
    undefined field346_0x1f1;
    undefined field347_0x1f2;
    undefined field348_0x1f3;
    undefined field349_0x1f4;
    undefined field350_0x1f5;
    undefined field351_0x1f6;
    undefined field352_0x1f7;
    undefined field353_0x1f8;
    undefined field354_0x1f9;
    undefined field355_0x1fa;
    undefined field356_0x1fb;
    undefined field357_0x1fc;
    undefined field358_0x1fd;
    undefined field359_0x1fe;
    undefined field360_0x1ff;
    undefined field361_0x200;
    undefined field362_0x201;
    undefined field363_0x202;
    undefined field364_0x203;
    undefined field365_0x204;
    undefined field366_0x205;
    undefined field367_0x206;
    undefined field368_0x207;
    undefined field369_0x208;
    undefined field370_0x209;
    undefined field371_0x20a;
    undefined field372_0x20b;
    undefined field373_0x20c;
    undefined field374_0x20d;
    undefined field375_0x20e;
    undefined field376_0x20f;
    undefined field377_0x210;
    undefined field378_0x211;
    undefined field379_0x212;
    undefined field380_0x213;
    undefined field381_0x214;
    undefined field382_0x215;
    undefined field383_0x216;
    undefined field384_0x217;
    undefined field385_0x218;
    undefined field386_0x219;
    undefined field387_0x21a;
    undefined field388_0x21b;
    undefined field389_0x21c;
    undefined field390_0x21d;
    undefined field391_0x21e;
    undefined field392_0x21f;
    undefined field393_0x220;
    undefined field394_0x221;
    undefined field395_0x222;
    undefined field396_0x223;
    undefined field397_0x224;
    undefined field398_0x225;
    int8_t saved_game_types[10];
    short field_l2; /* Created by Rename Structure Field action */
    short field_s2;
    struct s_colors popup_color1;
    struct s_colors popup_color2;
    short option_toggles[13];
    short language;
    undefined field406_0x260;
    undefined field407_0x261;
    undefined field408_0x262;
    undefined field409_0x263;
    undefined field410_0x264;
    undefined field411_0x265;
    undefined field412_0x266;
    undefined field413_0x267;
    undefined field414_0x268;
    undefined field415_0x269;
    undefined field416_0x26a;
    undefined field417_0x26b;
    undefined field418_0x26c;
    undefined field419_0x26d;
    undefined field420_0x26e;
    undefined field421_0x26f;
    undefined field422_0x270;
    undefined field423_0x271;
    undefined field424_0x272;
    undefined field425_0x273;
    undefined field426_0x274;
    undefined field427_0x275;
    undefined field428_0x276;
    undefined field429_0x277;
};

typedef struct s_good_star s_good_star, *Ps_good_star;

struct s_good_star {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    uint32_t star_distances[72];
    uint16_t star_count;
};

typedef struct s_lbx_header s_lbx_header, *Ps_lbx_header;

struct s_lbx_header {
    uint16_t entry_count;
    uint16_t magic;
    uint32_t unknown4;
    uint32_t offsets[510];
};

typedef struct s_leader_data s_leader_data, *Ps_leader_data;

struct s_leader_data {
    char name[15];
    char title[20];
    char type;
    short xp;
    uint32_t general_skills;
    uint32_t special_skills;
    byte tech_application[3];
    char pict_num;
    short skill_value;
    char level;
    short location;
    sbyte eta;
    char display_level_popup;
    sbyte status;
    char player_index;
};

typedef struct s_main_menu s_main_menu, *Ps_main_menu;

typedef struct s_main_menu_button s_main_menu_button, *Ps_main_menu_button;

struct s_main_menu_button {
    struct s_animation_header *field0_0x0;
    struct s_animation_header *field1_0x4;
    struct s_animation_header *field2_0x8;
    short field;
};

struct s_main_menu {
    struct s_main_menu_button fields[6];
    undefined field1_0x54;
    undefined field2_0x55;
    undefined field3_0x56;
    undefined field4_0x57;
    undefined field5_0x58;
    undefined field6_0x59;
    undefined field7_0x5a;
    undefined field8_0x5b;
    undefined field9_0x5c;
    undefined field10_0x5d;
    undefined field11_0x5e;
    undefined field12_0x5f;
    undefined field13_0x60;
    undefined field14_0x61;
    undefined field15_0x62;
    undefined field16_0x63;
    undefined field17_0x64;
    undefined field18_0x65;
    undefined field19_0x66;
    undefined field20_0x67;
    undefined field21_0x68;
    undefined field22_0x69;
    undefined field23_0x6a;
    undefined field24_0x6b;
    undefined field25_0x6c;
    undefined field26_0x6d;
    undefined field27_0x6e;
    undefined field28_0x6f;
    undefined field29_0x70;
    undefined field30_0x71;
    undefined field31_0x72;
    undefined field32_0x73;
    undefined field33_0x74;
    undefined field34_0x75;
    undefined field35_0x76;
    undefined field36_0x77;
    undefined field37_0x78;
    undefined field38_0x79;
    undefined field39_0x7a;
    undefined field40_0x7b;
    undefined field41_0x7c;
    undefined field42_0x7d;
    undefined field43_0x7e;
    undefined field44_0x7f;
    undefined field45_0x80;
    undefined field46_0x81;
    undefined field47_0x82;
    undefined field48_0x83;
    undefined field49_0x84;
    undefined field50_0x85;
    undefined field51_0x86;
    undefined field52_0x87;
    undefined field53_0x88;
    undefined field54_0x89;
    undefined field55_0x8a;
    undefined field56_0x8b;
    undefined field57_0x8c;
    undefined field58_0x8d;
    undefined field59_0x8e;
    undefined field60_0x8f;
    undefined field61_0x90;
    undefined field62_0x91;
    undefined field63_0x92;
    undefined field64_0x93;
    undefined field65_0x94;
    undefined field66_0x95;
    undefined field67_0x96;
    undefined field68_0x97;
    undefined field69_0x98;
    undefined field70_0x99;
    undefined field71_0x9a;
    undefined field72_0x9b;
    undefined field73_0x9c;
    undefined field74_0x9d;
    undefined field75_0x9e;
    undefined field76_0x9f;
    undefined field77_0xa0;
    undefined field78_0xa1;
    undefined field79_0xa2;
    undefined field80_0xa3;
    undefined field81_0xa4;
    undefined field82_0xa5;
    undefined field83_0xa6;
    undefined field84_0xa7;
    undefined field85_0xa8;
    undefined field86_0xa9;
    undefined field87_0xaa;
    undefined field88_0xab;
    undefined field89_0xac;
    undefined field90_0xad;
    undefined field91_0xae;
    undefined field92_0xaf;
    undefined field93_0xb0;
    undefined field94_0xb1;
    undefined field95_0xb2;
    undefined field96_0xb3;
    undefined field97_0xb4;
    undefined field98_0xb5;
    undefined field99_0xb6;
    undefined field100_0xb7;
    undefined field101_0xb8;
    undefined field102_0xb9;
    undefined field103_0xba;
    undefined field104_0xbb;
    undefined field105_0xbc;
    undefined field106_0xbd;
    undefined field107_0xbe;
    undefined field108_0xbf;
    undefined field109_0xc0;
    undefined field110_0xc1;
    undefined field111_0xc2;
    undefined field112_0xc3;
    undefined field113_0xc4;
    undefined field114_0xc5;
    undefined field115_0xc6;
    undefined field116_0xc7;
    undefined field117_0xc8;
    undefined field118_0xc9;
    undefined field119_0xca;
    undefined field120_0xcb;
    undefined field121_0xcc;
    undefined field122_0xcd;
    undefined field123_0xce;
    undefined field124_0xcf;
    undefined field125_0xd0;
    undefined field126_0xd1;
};

typedef struct s_missle s_missle, *Ps_missle;

struct s_missle {
    short type;
    short owner;
    short field2_0x4;
    short target_missle_idx;
    sbyte is_anti_missile_rocket;
    undefined field5_0x9;
    undefined field6_0xa;
    undefined field7_0xb;
    undefined field8_0xc;
    undefined field9_0xd;
    undefined field10_0xe;
    undefined field11_0xf;
    undefined field12_0x10;
    undefined field13_0x11;
    undefined field14_0x12;
    undefined field15_0x13;
    undefined field16_0x14;
    undefined field17_0x15;
    undefined field18_0x16;
    undefined field19_0x17;
    undefined field20_0x18;
    undefined field21_0x19;
};

typedef struct s_movable_box s_movable_box, *Ps_movable_box;

struct s_movable_box {
    uint16_t index;
    int16_t off_flag;
    undefined2 height;
    undefined2 width;
    int16_t x;
    int16_t y;
    undefined field6_0xc;
    undefined field7_0xd;
    short field8_0xe;
    short field9_0x10;
    undefined field10_0x12;
    undefined field11_0x13;
    int16_t field12_0x14;
    int16_t field13_0x16;
    undefined2 field14_0x18;
    undefined2 field15_0x1a;
};

typedef struct s_MSG s_MSG, *Ps_MSG;

struct s_MSG {
    undefined4 field0_0x0;
    undefined4 field1_0x4;
    dword field2_0x8;
    dword field3_0xc;
    word field4_0x10;
};

typedef struct s_nebula s_nebula, *Ps_nebula;

struct s_nebula {
    short x;
    short y; /* Created by Rename Structure Field action */
    int8_t type;
};

typedef struct s_planet_data s_planet_data, *Ps_planet_data;

struct s_planet_data {
    short colony_index;
    char star_index;
    char orbit;
    int8_t planet_type;
    int8_t size;
    char gravity_class;
    char group;
    char climate;
    char climate_bg_type;
    char mineral_class;
    uint8_t food_per_farmer;
    char n_terraforms;
    char max_farms;
    char max_population;
    char planet_special;
    char environmental_alterations;
};

typedef struct s_planet_max_farms s_planet_max_farms, *Ps_planet_max_farms;

struct s_planet_max_farms {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
};

typedef struct s_player s_player, *Ps_player;

typedef struct s_settler_info s_settler_info, *Ps_settler_info;

typedef struct s_ship_design s_ship_design, *Ps_ship_design;


/* WARNING! conflicting data type names: /types.h/ushort - /ushort */

typedef struct s_ship_weapons s_ship_weapons, *Ps_ship_weapons;

struct s_ship_weapons {
    short type;
    char count;
    char current_count;
    char firing_arc;
    short specials;
    char ammo;
};

struct s_ship_design {
    char name[16];
    char size;
    byte ship_type;
    char shield_type;
    char ftl_type;
    char speed;
    char computer_type;
    char armor_type;
    char special_device_flags[5];
    struct s_ship_weapons ship_weapon[8];
    sbyte picture_num;
    char previous_owner;
    short cost;
    char combat_speed;
    ushort date_of_design;
};

struct s_settler_info {
    byte source_colony:8;
    byte dest_planet:8;
    byte player:4;
    byte eta:4;
    byte job:2;
};

struct s_player {
    char picture;
    char name[20];
    char race_name[15];
    byte eliminated;
    char race;
    char color;
    char personality;
    byte objectives;
    short home_planet_id;
    short network_player_id;
    char player_done_flags;
    short dead_field;
    char research_breakthrough;
    char tax_rate;
    int bc;
    short n_freighters;
    short surplus_freighters;
    short command_points;
    short command_points_used;
    short food_freighted;
    short settlers_freighted;
    struct s_settler_info settlers[25];
    short total_pop;
    short food_produced;
    short industry_produced;
    short research_produced;
    short bc_produced;
    short surplus_food;
    short surplus_bc;
    int total_maintenance;
    short maintenance[6];
    byte tech_fields[83];
    byte tech_applications[212];
    int research_accumulated;
    char captured_orion;
    char captured_antares;
    char won_council_vote;
    short player_defeated[8];
    short captured_population;
    char last_attacker;
    char ship_design_theme;
    char ship_special_theme;
    char tech_apps_learned[4];
    char tech_modifiers[17];
    char hyper_advanced_tech[8];
    int total_known_tech_cost;
    char knows_racial_stats;
    int last_tech_report_date[8];
    char player_tech_applications[8][27];
    char current_research_field;
    char current_research_application;
    char got_new_app;
    short ship_range;
    struct s_ship_design ship_designs[6];
    short ships_built[6];
    sbyte contact[8];
    char just_established_contact;
    char n_times_established_contact[8];
    sbyte within_range[8];
    char do_not_research;
    char offensive_stagepoint;
    char best_ftl_drive;
    char ship_speed;
    char officer_for_hire;
    short n_trade_pops;
    short current_trade_agreement_level[8];
    short trade_agreement_goal[8];
    short n_research_pops;
    short current_research_agreement_level[8];
    short research_agreement_goal[8];
    char fighter_beam_type;
    char fighter_bomb_type;
    short total_research;
    short total_ships;
    int32_t total_ship_strength[8]; /* Type 'dword.conflict[8]' was deleted */
    short total_colonies;
    sbyte failed_to_expand;
    char first_contact[8];
    sbyte relations[8];
    char base_relations[8];
    sbyte treaty[8];
    char trade_treaty[8];
    char research_treaty[8];
    short tribute_treaty[8];
    char diplomacy_incident[8];
    char diplomacy_message[8];
    short diplomacy_severity[8];
    short diplomacy_value[8];
    short diplomacy_system[8];
    short treaty_modifier[8];
    short trade_modifier[8];
    short tech_exchange_modifier[8];
    short peace_modifier[8];
    sbyte last_bad_diplomatic_incident[8];
    char broken_treaties_modifier[8];
    char last_broken_treaty[8];
    char last_gift[8];
    short ship_war_losses[8];
    short colony_war_losses[8];
    char biological_weapons_flag[8];
    char time_since_last_attack[8];
    char threats[8];
    char dishonored_flag[8];
    char peace_duration[8];
    short last_turn_diplomacy_severity[8];
    char last_broken_treaty_type[8];
    sbyte last_turn_treaty[8];
    char diplomacy_proposal_request[8];
    char diplomacy_proposal_tribute_bribe[8];
    char diplomacy_proposal_tech_bribe1[8];
    char diplomacy_proposal_tech_bribe2[8];
    short diplomacy_proposal_gold_bribe[8];
    char diplomacy_proposal_war_player[8];
    int diplomacy_proposal_exchange_max_value[8];
    char diplomacy_proposal_exchange_tech[8];
    short diplomacy_proposal_system_bribe[8];
    short sneak_attack_planet;
    char sneak_attack_message;
    char sneak_attack_player;
    char council_threat;
    short reward_amount[8];
    char reward_tech[8];
    char reward_attack_player[8];
    short food_shortage_duration;
    char trust_worthiness[8];
    char trust_breaker_player[8];
    char trust_breaker_treaty[8];
    char stop_spying_duration[8];
    char stop_blockading_duration[8];
    char war_buildup_duration;
    short last_diplomacy_proposal_turn[8];
    short diplomacy_proposal_rejection[8];
    short opportunity_planet_num[8];
    short opportunity_planet_value[8];
    int opportunity_actual_attacker_ratio[8];
    short temp_diplomacy_dummy1[8];
    char diplomacy_demand_rejection_message[8];
    sbyte last_diplomacy_threat_turn[8];
    char delayed_diplomacy_orders[8];
    int8_t traits[31];
    int8_t evolutionary_upgrade_traits[31];
    char picked_evolutionary_upgrade;
    char applied_evolutionary_upgrade;
    char fleet_history[350];
    char tech_history[350];
    char population_history[350];
    char production_history[350];
    int8_t spies[8];
    char history_btns;
    char ignoring;
    char peace_flags[8];
    int8_t cheated;
    ushort stardate_last_offered_hero;
    char current_zoom_level;
    short cur_map_x;
    short cur_map_y;
    char council_voted_for;
    char surrender_to;
    short time_since_last_lucky_event;
    char has_galactic_lore;
    char warned_about_cheating_player;
    char cheated_this_turn;
    int tribute_income;
    char filler[45];
};

typedef struct s_player_info s_player_info, *Ps_player_info;

struct s_player_info {
    char current_government;
    char pop_growth_bonus;
    char food_bonus;
    char production_bonus;
    char research_bonus;
    char tax_bonus;
    char ship_defense;
    char ship_attack;
    char ground_combat_bonus;
    char spying_bonus;
    char low_g_world;
    char heavy_g_world;
    sbyte aquatic;
    sbyte subterranean;
    char large_home_world;
    char rich_home_world;
    char ancient_home_world;
    sbyte cybernetic;
    char eats_minerals;
    char repulsive;
    char charismatic;
    char uncreative;
    char creative;
    sbyte environment_immune;
    char fantastic_traders;
    sbyte telepathic;
    sbyte lucky;
    char omniscience;
    char stealthy_ships;
    sbyte trans_dimensional;
    char warlord;
};

typedef struct s_save_game_description s_save_game_description, *Ps_save_game_description;

struct s_save_game_description {
    char description[12];
    undefined field1_0xc;
    undefined field2_0xd;
    undefined field3_0xe;
    undefined field4_0xf;
    undefined field5_0x10;
    undefined field6_0x11;
    undefined field7_0x12;
    undefined field8_0x13;
    undefined field9_0x14;
    undefined field10_0x15;
    undefined field11_0x16;
    undefined field12_0x17;
    undefined field13_0x18;
    undefined field14_0x19;
    undefined field15_0x1a;
    undefined field16_0x1b;
    undefined field17_0x1c;
    undefined field18_0x1d;
    undefined field19_0x1e;
    undefined field20_0x1f;
    undefined field21_0x20;
    undefined field22_0x21;
    undefined field23_0x22;
    undefined field24_0x23;
    undefined field25_0x24;
};

typedef struct s_settings s_settings, *Ps_settings;

struct s_settings {
    sbyte end_of_turn_summary;
    sbyte end_of_turn_wait;
    sbyte random_events;
    sbyte enemy_moves;
    sbyte expanding_help;
    sbyte auto_select_ships;
    sbyte animations_on;
    sbyte auto_select_colony;
    sbyte show_relocation_lines;
    sbyte show_gnn_report;
    sbyte auto_delete_tg_housing;
    sbyte auto_saves;
    sbyte only_show_serious_turn_summary_reports;
    sbyte ship_initiative;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    sbyte sound_fx_on;
    sbyte sound_fx_level;
    sbyte music_on;
    int8_t music_level;
    char active_save_slot;
    byte multi_player_game_type;
    char net_game_name[9];
    byte modem_settings[131];
    byte modem_init_string[11];
    undefined field26_0xae;
    undefined field27_0xaf;
    undefined field28_0xb0;
    undefined field29_0xb1;
    undefined field30_0xb2;
    undefined field31_0xb3;
    char combat_speedx2_flag;
    char combat_legal_moves_flag;
    char combat_missile_impact_flag;
    char combat_shields_arc_flag;
    char combat_grid_flag;
    undefined field37_0xb9;
    undefined field38_0xba;
    undefined field39_0xbb;
    undefined field40_0xbc;
    undefined field41_0xbd;
    undefined field42_0xbe;
    undefined field43_0xbf;
    undefined field44_0xc0;
    undefined field45_0xc1;
    undefined field46_0xc2;
    undefined field47_0xc3;
    undefined field48_0xc4;
    undefined field49_0xc5;
    undefined field50_0xc6;
    undefined field51_0xc7;
    undefined field52_0xc8;
    undefined field53_0xc9;
    undefined field54_0xca;
    undefined field55_0xcb;
    undefined field56_0xcc;
    undefined field57_0xcd;
    undefined field58_0xce;
    undefined field59_0xcf;
    undefined field60_0xd0;
    undefined field61_0xd1;
    int8_t language;
    byte xenons_exist;
    byte game_difficulty;
    byte number_of_players;
    byte galaxy_size;
    byte galaxy_age;
    byte strategic_combat_flag;
    byte starting_civilization_level;
    uint32_t start_of_game_seed;
    short field71_0xde;
    short field73_0xe0;
    uint16_t version;
    undefined field74_0xe4;
    undefined field75_0xe5;
    undefined field76_0xe6;
    undefined field77_0xe7;
    undefined field78_0xe8;
    undefined field79_0xe9;
    undefined field80_0xea;
    undefined field81_0xeb;
    undefined field82_0xec;
    undefined field83_0xed;
    undefined field84_0xee;
    undefined field85_0xef;
    undefined field86_0xf0;
    undefined field87_0xf1;
    undefined field88_0xf2;
    undefined field89_0xf3;
    undefined field90_0xf4;
    undefined field91_0xf5;
    undefined field92_0xf6;
    undefined field93_0xf7;
    undefined field94_0xf8;
    undefined field95_0xf9;
    undefined field96_0xfa;
    undefined field97_0xfb;
    undefined field98_0xfc;
    undefined field99_0xfd;
    undefined field100_0xfe;
    undefined field101_0xff;
    undefined field102_0x100;
    undefined field103_0x101;
    undefined field104_0x102;
    undefined field105_0x103;
    undefined field106_0x104;
    undefined field107_0x105;
    undefined field108_0x106;
    undefined field109_0x107;
    undefined field110_0x108;
    undefined field111_0x109;
    undefined field112_0x10a;
    undefined field113_0x10b;
    undefined field114_0x10c;
    undefined field115_0x10d;
    undefined field116_0x10e;
    undefined field117_0x10f;
    undefined field118_0x110;
    undefined field119_0x111;
    undefined field120_0x112;
    undefined field121_0x113;
    undefined field122_0x114;
    undefined field123_0x115;
    undefined field124_0x116;
    undefined field125_0x117;
    undefined field126_0x118;
    undefined field127_0x119;
    undefined field128_0x11a;
    undefined field129_0x11b;
    undefined field130_0x11c;
    undefined field131_0x11d;
    undefined field132_0x11e;
    undefined field133_0x11f;
    undefined field134_0x120;
    undefined field135_0x121;
    undefined field136_0x122;
    undefined field137_0x123;
    undefined field138_0x124;
    undefined field139_0x125;
    undefined field140_0x126;
    undefined field141_0x127;
    undefined field142_0x128;
    undefined field143_0x129;
    undefined field144_0x12a;
    undefined field145_0x12b;
    undefined field146_0x12c;
    undefined field147_0x12d;
    undefined field148_0x12e;
    undefined field149_0x12f;
    undefined field150_0x130;
    undefined field151_0x131;
    undefined field152_0x132;
    undefined field153_0x133;
    undefined field154_0x134;
    undefined field155_0x135;
    undefined field156_0x136;
    undefined field157_0x137;
    undefined field158_0x138;
    undefined field159_0x139;
    undefined field160_0x13a;
    undefined field161_0x13b;
    undefined field162_0x13c;
    undefined field163_0x13d;
    undefined field164_0x13e;
    undefined field165_0x13f;
    undefined field166_0x140;
    undefined field167_0x141;
    undefined field168_0x142;
    undefined field169_0x143;
    undefined field170_0x144;
    undefined field171_0x145;
    undefined field172_0x146;
    undefined field173_0x147;
    undefined field174_0x148;
    undefined field175_0x149;
    undefined field176_0x14a;
    undefined field177_0x14b;
    undefined field178_0x14c;
    undefined field179_0x14d;
    undefined field180_0x14e;
    undefined field181_0x14f;
    undefined field182_0x150;
    undefined field183_0x151;
    undefined field184_0x152;
    undefined field185_0x153;
    undefined field186_0x154;
    undefined field187_0x155;
    undefined field188_0x156;
    undefined field189_0x157;
    undefined field190_0x158;
    undefined field191_0x159;
    undefined field192_0x15a;
    undefined field193_0x15b;
    undefined field194_0x15c;
    undefined field195_0x15d;
    undefined field196_0x15e;
    undefined field197_0x15f;
    undefined field198_0x160;
    undefined field199_0x161;
    undefined field200_0x162;
    undefined field201_0x163;
    undefined field202_0x164;
    undefined field203_0x165;
    undefined field204_0x166;
    undefined field205_0x167;
    undefined field206_0x168;
    undefined field207_0x169;
    undefined field208_0x16a;
    undefined field209_0x16b;
    undefined field210_0x16c;
    undefined field211_0x16d;
    undefined field212_0x16e;
    undefined field213_0x16f;
    undefined field214_0x170;
    undefined field215_0x171;
    undefined field216_0x172;
    undefined field217_0x173;
    undefined field218_0x174;
    undefined field219_0x175;
    undefined field220_0x176;
    undefined field221_0x177;
    undefined field222_0x178;
    undefined field223_0x179;
    undefined field224_0x17a;
    undefined field225_0x17b;
    undefined field226_0x17c;
    undefined field227_0x17d;
    undefined field228_0x17e;
    undefined field229_0x17f;
    undefined field230_0x180;
    undefined field231_0x181;
    undefined field232_0x182;
    undefined field233_0x183;
    undefined field234_0x184;
    undefined field235_0x185;
    undefined field236_0x186;
    undefined field237_0x187;
    undefined field238_0x188;
    undefined field239_0x189;
    undefined field240_0x18a;
    undefined field241_0x18b;
    undefined field242_0x18c;
    undefined field243_0x18d;
    undefined field244_0x18e;
    undefined field245_0x18f;
    undefined field246_0x190;
    undefined field247_0x191;
    undefined field248_0x192;
    undefined field249_0x193;
    undefined field250_0x194;
    undefined field251_0x195;
    undefined field252_0x196;
    undefined field253_0x197;
    undefined field254_0x198;
    undefined field255_0x199;
    undefined field256_0x19a;
    undefined field257_0x19b;
    undefined field258_0x19c;
    undefined field259_0x19d;
    undefined field260_0x19e;
    undefined field261_0x19f;
    undefined field262_0x1a0;
    undefined field263_0x1a1;
    undefined field264_0x1a2;
    undefined field265_0x1a3;
    undefined field266_0x1a4;
    undefined field267_0x1a5;
    undefined field268_0x1a6;
    undefined field269_0x1a7;
    undefined field270_0x1a8;
    undefined field271_0x1a9;
    undefined field272_0x1aa;
    undefined field273_0x1ab;
    undefined field274_0x1ac;
    undefined field275_0x1ad;
    undefined field276_0x1ae;
    undefined field277_0x1af;
    undefined field278_0x1b0;
    undefined field279_0x1b1;
    undefined field280_0x1b2;
    undefined field281_0x1b3;
    undefined field282_0x1b4;
    undefined field283_0x1b5;
    undefined field284_0x1b6;
    undefined field285_0x1b7;
    undefined field286_0x1b8;
    undefined field287_0x1b9;
    undefined field288_0x1ba;
    undefined field289_0x1bb;
    undefined field290_0x1bc;
    undefined field291_0x1bd;
    undefined field292_0x1be;
    undefined field293_0x1bf;
    undefined field294_0x1c0;
    undefined field295_0x1c1;
    undefined field296_0x1c2;
    undefined field297_0x1c3;
    undefined field298_0x1c4;
    undefined field299_0x1c5;
    undefined field300_0x1c6;
    undefined field301_0x1c7;
    undefined field302_0x1c8;
    undefined field303_0x1c9;
    undefined field304_0x1ca;
    undefined field305_0x1cb;
    undefined field306_0x1cc;
    undefined field307_0x1cd;
    undefined field308_0x1ce;
    undefined field309_0x1cf;
    undefined field310_0x1d0;
    undefined field311_0x1d1;
    undefined field312_0x1d2;
    undefined field313_0x1d3;
    undefined field314_0x1d4;
    undefined field315_0x1d5;
    undefined field316_0x1d6;
    undefined field317_0x1d7;
    undefined field318_0x1d8;
    undefined field319_0x1d9;
    undefined field320_0x1da;
    undefined field321_0x1db;
    undefined field322_0x1dc;
    undefined field323_0x1dd;
    undefined field324_0x1de;
    undefined field325_0x1df;
    undefined field326_0x1e0;
    undefined field327_0x1e1;
    undefined field328_0x1e2;
    undefined field329_0x1e3;
    undefined field330_0x1e4;
    undefined field331_0x1e5;
    undefined field332_0x1e6;
    undefined field333_0x1e7;
    undefined field334_0x1e8;
    undefined field335_0x1e9;
    undefined field336_0x1ea;
    undefined field337_0x1eb;
    undefined field338_0x1ec;
    undefined field339_0x1ed;
    undefined field340_0x1ee;
    undefined field341_0x1ef;
    undefined field342_0x1f0;
    undefined field343_0x1f1;
    undefined field344_0x1f2;
    undefined field345_0x1f3;
    undefined field346_0x1f4;
    undefined field347_0x1f5;
    undefined field348_0x1f6;
    undefined field349_0x1f7;
    undefined field350_0x1f8;
    undefined field351_0x1f9;
    undefined field352_0x1fa;
    undefined field353_0x1fb;
    undefined field354_0x1fc;
    undefined field355_0x1fd;
    undefined field356_0x1fe;
    undefined field357_0x1ff;
    undefined field358_0x200;
    undefined field359_0x201;
    undefined field360_0x202;
    undefined field361_0x203;
    undefined field362_0x204;
    undefined field363_0x205;
    undefined field364_0x206;
    undefined field365_0x207;
    undefined field366_0x208;
    undefined field367_0x209;
    undefined field368_0x20a;
    undefined field369_0x20b;
    undefined field370_0x20c;
    undefined field371_0x20d;
    undefined field372_0x20e;
    undefined field373_0x20f;
    undefined field374_0x210;
    undefined field375_0x211;
    undefined field376_0x212;
    undefined field377_0x213;
    undefined field378_0x214;
    undefined field379_0x215;
    undefined field380_0x216;
    undefined field381_0x217;
    undefined field382_0x218;
    undefined field383_0x219;
    undefined field384_0x21a;
    undefined field385_0x21b;
    undefined field386_0x21c;
    undefined field387_0x21d;
    undefined field388_0x21e;
    undefined field389_0x21f;
    undefined field390_0x220;
    undefined field391_0x221;
    undefined field392_0x222;
    undefined field393_0x223;
    undefined field394_0x224;
    undefined field395_0x225;
    undefined field396_0x226;
    undefined field397_0x227;
    undefined field398_0x228;
};

typedef struct s_ship_data s_ship_data, *Ps_ship_data;

struct s_ship_data {
    struct s_ship_design d;
    char owner;
    sbyte status;
    short location;
    short x;
    short y;
    char group_has_navigator;
    char travelling_speed;
    char turns_left;
    char shield_damage_percent;
    char drive_damage_percent;
    char computer_damage;
    char crew_quality;
    short crew_experience;
    short officer_index;
    byte special_device_damage_flags[5];
    short armor_damage;
    short structural_damage;
    char mission;
    char just_built;
};

typedef struct s_ship_drive_data s_ship_drive_data, *Ps_ship_drive_data;

struct s_ship_drive_data {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    short tech_application_idx;
    undefined field7_0x8;
    undefined field8_0x9;
    undefined field9_0xa;
    undefined field10_0xb;
    undefined field11_0xc;
    undefined field12_0xd;
    undefined field13_0xe;
    undefined field14_0xf;
    undefined field15_0x10;
    undefined field16_0x11;
    undefined field17_0x12;
    undefined field18_0x13;
    undefined field19_0x14;
    undefined field20_0x15;
    undefined field21_0x16;
    undefined field22_0x17;
    undefined field23_0x18;
    undefined field24_0x19;
    undefined field25_0x1a;
    undefined field26_0x1b;
    undefined field27_0x1c;
    undefined field28_0x1d;
    undefined field29_0x1e;
    undefined field30_0x1f;
    uint8_t combat_speed_modifier[12];
    short ftl_speed;
};

typedef struct s_ship_icon s_ship_icon, *Ps_ship_icon;

struct s_ship_icon {
    short field0_0x0;
    short node_idx;
    short field2_0x4;
    undefined field3_0x6;
    undefined field4_0x7;
    short x;
    short y;
};

typedef struct s_ship_node s_ship_node, *Ps_ship_node;

struct s_ship_node {
    short ship_idx;
    short field1_0x2;
    char field2_0x4;
};

typedef struct s_skill_data s_skill_data, *Ps_skill_data;

struct s_skill_data {
    byte num;
    uint skill_mask;
    byte unknown1;
    int16_t skill_bonus;
    byte unknown2;
    byte unknown3;
    char *name;
    char *format_str;
};

typedef struct s_star_data s_star_data, *Ps_star_data;

struct s_star_data {
    char name[15];
    short x;
    short y;
    char size;
    int8_t owner;
    char pict_type;
    int8_t spectral_class;
    int8_t last_planet_selected[8];
    char black_hole_blocks[9];
    int8_t system_special;
    int8_t wormhole_star_id;
    byte blockaded;
    char blockaded_by[8];
    int8_t visited;
    byte just_visited;
    byte ignore_colony_ships;
    byte ignore_combat_ships;
    int8_t colonize_player;
    byte has_colony;
    byte has_warp_field_interdictor;
    byte next_wfi_in_list;
    byte has_tachyon;
    byte has_subspace;
    byte has_stargate;
    byte has_jumpgate;
    byte has_artemis_net;
    byte has_dimensional_portal;
    byte is_stagepoint;
    char officer_index[8];
    short planet_index[5];
    short relocate_ship_to[8];
    byte not_used1;
    byte not_used2;
    byte not_used3;
    char surrender_to[8];
    byte in_nebula;
    byte artifacts_gave_app;
};

typedef struct s_system_display_ships s_system_display_ships, *Ps_system_display_ships;

struct s_system_display_ships {
    short field0_0x0;
    short field1_0x2;
    short button;
    undefined field3_0x6;
    undefined field4_0x7;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
};

typedef struct s_tech_app_data s_tech_app_data, *Ps_tech_app_data;

struct s_tech_app_data {
    char *name;
    ushort id;
    short tech_field_id;
    byte unk_class;
    byte ai_group;
    byte unknown1;
    byte in_strategic;
    byte type;
};

typedef struct s_tech_armor_data s_tech_armor_data, *Ps_tech_armor_data;

struct s_tech_armor_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    short tech_app_id; /* Type 'TECH_APP' was deleted */
    undefined field4_0x8;
    undefined field5_0x9;
    short armor_absorption_percent;
    short cost;
    undefined field8_0xe;
};

typedef struct s_tech_building_data s_tech_building_data, *Ps_tech_building_data;

struct s_tech_building_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    int cost;
    short maintenance;
    undefined field7_0xe;
    undefined field8_0xf;
    undefined field9_0x10;
    undefined field10_0x11;
    undefined field11_0x12;
};

typedef struct s_tech_computer_data s_tech_computer_data, *Ps_tech_computer_data;

struct s_tech_computer_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    short tech_app;
    short cost_by_ship_type[6];
    short field5_0x14;
};

typedef struct s_tech_drive_data s_tech_drive_data, *Ps_tech_drive_data;

struct s_tech_drive_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    short tech_app;
    short space_per_ship_type[6];
    short cost_per_ship_type[6];
    int8_t combat_speed_modifier[6];
    int8_t speed_modifier[6];
    short speed;
};

typedef struct s_tech_field_data s_tech_field_data, *Ps_tech_field_data;

struct s_tech_field_data {
    char *name;
    short id;
    short prev_field_id;
    short next_field_id;
    ushort tech1;
    ushort tech2;
    ushort tech3;
    ushort tech4;
    uint cost;
    byte ai_group;
};

typedef struct s_tech_fuel_data s_tech_fuel_data, *Ps_tech_fuel_data;

struct s_tech_fuel_data {
    char *name;
    short field4;
    short tech_app;
    short ship_range;
};

typedef struct s_tech_hull_data s_tech_hull_data, *Ps_tech_hull_data;

struct s_tech_hull_data {
    short hull_space;
    short hull_size;
    undefined field2_0x4;
    undefined field3_0x5;
    short armor_absorption;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
    undefined field9_0xc;
    undefined field10_0xd;
    undefined field11_0xe;
    undefined field12_0xf;
    short beam_weapon_count;
    short missle_weapon_count;
    short special_weapon_count;
    short bomb_weapon_count;
    undefined field17_0x18;
    undefined field18_0x19;
    undefined field19_0x1a;
    undefined field20_0x1b;
    char *name;
    char *size_name;
};

typedef struct s_tech_shield_data s_tech_shield_data, *Ps_tech_shield_data;

struct s_tech_shield_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    short tech_app_id;
    short space_per_ship_type[6];
    short cost_per_ship_type[6];
    short points_blocked;
    undefined field7_0x22;
    undefined field8_0x23;
    undefined field9_0x24;
    undefined field10_0x25;
    undefined field11_0x26;
    undefined field12_0x27;
    undefined field13_0x28;
    undefined field14_0x29;
    undefined field15_0x2a;
    undefined field16_0x2b;
    undefined field17_0x2c;
    undefined field18_0x2d;
    undefined field19_0x2e;
    undefined field20_0x2f;
    undefined field21_0x30;
    undefined field22_0x31;
    undefined field23_0x32;
    undefined field24_0x33;
    undefined field25_0x34;
    undefined field26_0x35;
    undefined field27_0x36;
    undefined field28_0x37;
    undefined field29_0x38;
    undefined field30_0x39;
    undefined field31_0x3a;
};

typedef struct s_tech_specials_data s_tech_specials_data, *Ps_tech_specials_data;

struct s_tech_specials_data {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    short tech_application_id;
    short space_per_ship_type[6];
    short cost_per_ship_type[6];
    undefined field9_0x20;
    undefined field10_0x21;
    undefined field11_0x22;
    undefined field12_0x23;
    undefined field13_0x24;
    undefined field14_0x25;
    undefined field15_0x26;
    undefined field16_0x27;
    undefined field17_0x28;
    undefined field18_0x29;
    undefined field19_0x2a;
    undefined field20_0x2b;
    undefined field21_0x2c;
    undefined field22_0x2d;
    undefined field23_0x2e;
};

typedef struct s_tech_weapon_data s_tech_weapon_data, *Ps_tech_weapon_data;

struct s_tech_weapon_data {
    char *name;
    char *name_plural;
    undefined field2_0x8;
    undefined field3_0x9;
    short tech_application_id;
    sbyte rack;
    undefined field6_0xd;
    short base_size;
    undefined field8_0x10;
    undefined field9_0x11;
    undefined field10_0x12;
    undefined field11_0x13;
    undefined field12_0x14;
    undefined field13_0x15;
    undefined field14_0x16;
    undefined field15_0x17;
    undefined field16_0x18;
    undefined field17_0x19;
    undefined field18_0x1a;
    undefined field19_0x1b;
};

typedef struct s_tech_weapon_mod_data s_tech_weapon_mod_data, *Ps_tech_weapon_mod_data;

struct s_tech_weapon_mod_data {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
};

typedef struct s_unit_data s_unit_data, *Ps_unit_data;

struct s_unit_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
};

typedef enum SCREEN {
    SCREEN_UNKNOWN=-1,
    SCREEN_MAIN=0,
    SCREEN_COLONY=1,
    SCREEN_DESIGN=3,
    SCREEN_FLEET=4,
    SCREEN_RACE=6,
    SCREEN_EXIT=7,
    SCREEN_GAME=8,
    SCREEN_INFO=9,
    SCREEN_MAIN_MENU=10,
    SCREEN_MAIN_NEXT_TURN=12,
    SCREEN_NEW_GAME=13,
    SCREEN_HALL_OF_FAME=14,
    SCREEN_MULTI_PLAYER=15,
    SCREEN_HOTSEAT=16,
    SCREEN_HOTSEAT_SELECT_PLAYER=17,
    SCREEN_PLANET_DATA=18,
    SCREEN_COLONY_SUMMARY=20,
    SCREEN_START_NET=21,
    SCREEN_JOIN_NET=22,
    SCREEN_QUEUE_POPUP=25,
    SCREEN_DISTANCE_BETWEEN_STARS=28,
    SCREEN_OFFICERS=29,
    SCREEN_COLONIZATION_IN_MAIN=30,
    SCREEN_BUILDING_OUTPOSTS_IN_MAIN=31,
    SCREEN_PLANET_SUMMARY=32,
    SCREEN_COLONY_LANDING=33,
    SCREEN_UNLOADING_TRANSPORTS_IN_MAIN=34,
    SCREEN_SCRAP_FREIGHTERS=35,
    SCREEN_TECH_CHANGE=36,
    SCREEN_NET_NEXT_TURN=37,
    SCREEN_REPORTS=39,
    SCREEN_TURN_SUMMARY=40,
    SCREEN_LOAD_NET=41,
    SCREEN_SHOW_COMMAND_POINTS=43
} SCREEN;

typedef enum SHIP_SIZE {
    SHIP_SIZE_SMALL=0,
    SHIP_SIZE_MEDIUM=1,
    SHIP_SIZE_LARGE=2,
    SHIP_SIZE_HUGE=3,
    SHIP_SIZE_TITAN=4,
    SHIP_SIZE_DOOM_STAR=5
} SHIP_SIZE;

typedef enum SHIP_TYPE {
    SHIP_TYPE_COMBAT=0,
    SHIP_TYPE_COLONY=1,
    SHIP_TYPE_TRANSPORT=2,
    SHIP_TYPE_UNUSED=3,
    SHIP_TYPE_OUTPOST=4
} SHIP_TYPE;

typedef enum STAR_CLASS {
    STAR_CLASS_B=0,
    STAR_CLASS_F=1,
    STAR_CLASS_G=2,
    STAR_CLASS_K=3,
    STAR_CLASS_M=4,
    STAR_CLASS_DWARF=5,
    STAR_CLASS_BLACK_HOLE=6
} STAR_CLASS;

typedef enum SYSTEM_SPECIAL {
    SYSTEM_SPECIAL_NO_SPECIAL=0,
    SYSTEM_SPECIAL_STABLE_WORMHOLE=1,
    SYSTEM_SPECIAL_SPACE_DEBRIS=2,
    SYSTEM_SPECIAL_PIRATE_CACHE=3,
    SYSTEM_SPECIAL_GOLD_DEPOSITS=4,
    SYSTEM_SPECIAL_GEM_DEPOSITS=5,
    SYSTEM_SPECIAL_NATIVES=6,
    SYSTEM_SPECIAL_SPLINTER_COLONY=7,
    SYSTEM_SPECIAL_MAROONED_HERO=8,
    SYSTEM_SPECIAL_SPACE_MONSTER=9,
    SYSTEM_SPECIAL_ANCIENT_ARTIFACTS=10,
    SYSTEM_SPECIAL_ORION_STAR=11
} SYSTEM_SPECIAL;

typedef enum WEAPON_MOD_FLAG {
    NO_MODS=1,
    HEAVY_MOUNT=2,
    POINT_DEFENCE=4,
    ARMOR_PIERCING=8,
    CONTINUOUS=16,
    NO_RANGE_DISSIPATION=32,
    SHIELD_PIERCING=64,
    AUTO_FIRE=128,
    ENVELOPING=256,
    MIRV=512,
    ECCM=1024,
    HEAVILY_ARMORED=2048,
    FAST=4096,
    EMISSION_GUIDANCE=8192,
    OVERLOADED=16384
} WEAPON_MOD_FLAG;

