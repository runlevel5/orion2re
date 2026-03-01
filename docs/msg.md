Mapping between MSG.msg_type and used structure type.

 0 = MSG_TYPE_COLONY
     base: s_MSG_colony        (u.colony)
     subtype (u.colony.subtype):
         0:  s_MSG_colony_scrap_building     (u.colony.c.scrap)
         1:  s_MSG_colony_build_info         (u.colony.c.build)
         2:  s_MSG_colony_assimilation       (u.colony.c.assimilation)
         3:  s_MSG_colony_assimilation       (u.colony.c.assimilation)
         4:  s_MSG_colony_starvation         (u.colony.c.starvation)
         5:  s_MSG_colony_settlers           (u.colony.c.settlers)
         6:  s_MSG_colony                    (only planet_idx/subtype used; u.colony.c not read)
         7:  s_MSG_colony                    (only planet_idx/subtype used; u.colony.c not read)
         8:  s_MSG_colony                    (only planet_idx/subtype used; u.colony.c not read)
         9:  s_MSG_colony_rebellion          (u.colony.c.rebels)
         10: s_MSG_colony                    (only planet_idx/subtype used; u.colony.c not read)
         11: s_MSG_colony_android_killed     (u.colony.c.android_killed)

 1 = MSG_TYPE_DECLARE_ALLIANCE
     s_MSG_players          (u.players)

 2 = MSG_TYPE_DECLARE_WAR
     s_MSG_players          (u.players)

 3 = (no case in sprintf_msg_; not mapped/used here)

 4 = MSG_TYPE_FINANCIAL
     s_MSG_financial        (u.financial)

 5 = MSG_TYPE_LOST_SHIP
     s_MSG_lost_ship        (u.lost_ship)

 6 = MSG_TYPE_SCRAP_SHIP
     s_MSG_scrap_ship       (u.scrap_ship)

 7 = MSG_TYPE_LOST_COLONY
     s_MSG_lost_colony      (u.lost_colony)

 8 = MSG_TYPE_HYPERSPACE_FLUX
     no s_MSG_union member used (only the type/priority; message is a fixed string)

 9 = MSG_TYPE_HYPERSPACE_BEAST
     s_MSG_hyperspace_beast (u.hyperspace_beast)

 10 = MSG_TYPE_LOST_ATTACKING_SPY
      s_MSG_lost_attacking_spy (u.lost_attacking_spy)

 11 = MSG_TYPE_LOST_DEFENDING_SPY
      s_MSG_lost_defending_spy (u.lost_defending_spy)

 12 = MSG_TYPE_ARTEMIS
      s_MSG_artemis         (u.artemis)

 13 = MSG_TYPE_ARTEMIS_LEADER
      s_MSG_artemis_leader  (u.artemis_leader)

 14 = MSG_TYPE_FAILED_RETREAT
      s_MSG_failed_retreat  (u.retreat)

 15 = MSG_TYPE_SPY_ASSASSINATION
      s_MSG_spy_assasination (u.spy_assasination)

 16 = MSG_TYPE_ANOMALY
      s_MSG_anomaly         (u.anomaly)

 17 = MSG_TYPE_DIPLOMACY
      s_MSG_diplomacy       (u.diplomacy)

 18 = MSG_TYPE_EEL_SPAWNED
      s_MSG_eel             (u.eel)

 19 = MSG_TYPE_EEL_VANISHED
      s_MSG_eel             (u.eel)

 20 = MSG_TYPE_ARTIFACTS
      s_MSG_artifacts       (u.artifacts)

 21 = MSG_TYPE_APRIL1
      no s_MSG_union member used (fixed string only)

 22 = MSG_TYPE_SCRAP_LEADER
      s_MSG_scrap_leader    (u.scrap_leader)
