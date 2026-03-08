Mapping between MSG.msg\_type and used structure type. Code must use `s\_MSG` base type and then differentiate access

by union field type provided by `u`.



0 = MSG\_TYPE\_COLONY
base: s\_MSG\_colony        (u.colony)
subtype (u.colony.subtype):
0:  s\_MSG\_colony\_scrap\_building     (u.colony.c.scrap)
1:  s\_MSG\_colony\_build\_info         (u.colony.c.build)
2:  s\_MSG\_colony\_assimilation       (u.colony.c.assimilation)
3:  s\_MSG\_colony\_assimilation       (u.colony.c.assimilation)
4:  s\_MSG\_colony\_starvation         (u.colony.c.starvation)
5:  s\_MSG\_colony\_settlers           (u.colony.c.settlers)
6:  s\_MSG\_colony                    (only planet\_idx/subtype used; u.colony.c not read)
7:  s\_MSG\_colony                    (only planet\_idx/subtype used; u.colony.c not read)
8:  s\_MSG\_colony                    (only planet\_idx/subtype used; u.colony.c not read)
9:  s\_MSG\_colony\_rebellion          (u.colony.c.rebels)
10: s\_MSG\_colony                    (only planet\_idx/subtype used; u.colony.c not read)
11: s\_MSG\_colony\_android\_killed     (u.colony.c.android\_killed)

1 = MSG\_TYPE\_DECLARE\_ALLIANCE
s\_MSG\_players          (u.players)

2 = MSG\_TYPE\_DECLARE\_WAR
s\_MSG\_players          (u.players)

3 = (no case in sprintf\_msg\_; not mapped/used here)

4 = MSG\_TYPE\_FINANCIAL
s\_MSG\_financial        (u.financial)

5 = MSG\_TYPE\_LOST\_SHIP
s\_MSG\_lost\_ship        (u.lost\_ship)

6 = MSG\_TYPE\_SCRAP\_SHIP
s\_MSG\_scrap\_ship       (u.scrap\_ship)

7 = MSG\_TYPE\_LOST\_COLONY
s\_MSG\_lost\_colony      (u.lost\_colony)

8 = MSG\_TYPE\_HYPERSPACE\_FLUX
no s\_MSG\_union member used (only the type/priority; message is a fixed string)

9 = MSG\_TYPE\_HYPERSPACE\_BEAST
s\_MSG\_hyperspace\_beast (u.hyperspace\_beast)

10 = MSG\_TYPE\_LOST\_ATTACKING\_SPY
s\_MSG\_lost\_attacking\_spy (u.lost\_attacking\_spy)

11 = MSG\_TYPE\_LOST\_DEFENDING\_SPY
s\_MSG\_lost\_defending\_spy (u.lost\_defending\_spy)

12 = MSG\_TYPE\_ARTEMIS
s\_MSG\_artemis         (u.artemis)

13 = MSG\_TYPE\_ARTEMIS\_LEADER
s\_MSG\_artemis\_leader  (u.artemis\_leader)

14 = MSG\_TYPE\_FAILED\_RETREAT
s\_MSG\_failed\_retreat  (u.retreat)

15 = MSG\_TYPE\_SPY\_ASSASSINATION
s\_MSG\_spy\_assasination (u.spy\_assasination)

16 = MSG\_TYPE\_ANOMALY
s\_MSG\_anomaly         (u.anomaly)

17 = MSG\_TYPE\_DIPLOMACY
s\_MSG\_diplomacy       (u.diplomacy)

18 = MSG\_TYPE\_EEL\_SPAWNED
s\_MSG\_eel             (u.eel)

19 = MSG\_TYPE\_EEL\_VANISHED
s\_MSG\_eel             (u.eel)

20 = MSG\_TYPE\_ARTIFACTS
s\_MSG\_artifacts       (u.artifacts)

21 = MSG\_TYPE\_APRIL1
no s\_MSG\_union member used (fixed string only)

22 = MSG\_TYPE\_SCRAP\_LEADER
s\_MSG\_scrap\_leader    (u.scrap\_leader)



Complete s\_MSG structure list:

struct s\_MSG\_financial {

&nbsp;   int16\_t a;

&nbsp;   int16\_t b;

};



struct s\_MSG\_colony\_scrap\_building {

&nbsp;   int16\_t building\_id;

&nbsp;   int16\_t bc;

};



struct s\_MSG\_lost\_attacking\_spy {

&nbsp;   int16\_t player\_idx;

};



struct s\_MSG\_colony\_settlers {

&nbsp;   int8\_t settlers;

&nbsp;   int8\_t over\_limit;

&nbsp;   bool blockaded;

};



struct s\_MSG\_players {

&nbsp;   int16\_t player\_a\_idx;

&nbsp;   int16\_t player\_b\_idx;

};



struct s\_MSG\_artemis {

&nbsp;   int16\_t star\_idx;

};



struct s\_MSG\_artifacts {

&nbsp;   uint8\_t star\_idx;

&nbsp;   int8\_t tech1;

&nbsp;   int8\_t tech2;

};



struct s\_MSG\_scrap\_leader {

&nbsp;   int8\_t leader\_idx;

};



struct s\_MSG\_failed\_retreat {

&nbsp;   int8\_t star\_idx;

&nbsp;   int8\_t reason;

&nbsp;   int16\_t ships\_lost;

&nbsp;   int8\_t leader\_count;

};



struct s\_MSG\_artemis\_leader {

&nbsp;   int8\_t star\_idx;

&nbsp;   int8\_t leader\_idx;

&nbsp;   int8\_t fate;

};



struct s\_MSG\_eel {

&nbsp;   uint8\_t star\_idx;

};



struct s\_MSG\_scrap\_ship {

&nbsp;   int16\_t where;

&nbsp;   int16\_t bc;

&nbsp;   int16\_t ship\_idx;

};



struct s\_MSG\_hyperspace\_beast {

&nbsp;   int16\_t ship\_idx;

&nbsp;   int8\_t star\_idx;

&nbsp;   int8\_t field2\_0x3;

&nbsp;   int8\_t leader\_idx;

&nbsp;   int8\_t fate;

&nbsp;   int8\_t your\_ship;

};



struct s\_MSG\_lost\_ship {

&nbsp;   int16\_t star\_idx;

&nbsp;   int16\_t ship\_idx;

&nbsp;   int8\_t leader\_idx;

&nbsp;   int8\_t fate;

};



struct s\_MSG\_colony\_android\_killed {

&nbsp;   int8\_t race\_idx;

};



struct s\_MSG\_colony\_starvation {

&nbsp;   int8\_t a;

&nbsp;   int8\_t b;

};



struct s\_MSG\_colony\_assimilation {

&nbsp;   uint8\_t val;

};



struct s\_MSG\_colony\_rebellion {

&nbsp;   int8\_t owner\_player;

&nbsp;   int8\_t rebels;

&nbsp;   int8\_t lost;

};



struct s\_MSG\_colony\_build\_info {

&nbsp;   int16\_t build\_id;

&nbsp;   int16\_t product\_id;

&nbsp;   bool is\_refit;

&nbsp;   int8\_t append\_fmt;

};



union s\_MSG\_colony\_union {

&nbsp;   struct s\_MSG\_colony\_scrap\_building scrap;

&nbsp;   struct s\_MSG\_colony\_build\_info build;

&nbsp;   struct s\_MSG\_colony\_assimilation assimilation;

&nbsp;   struct s\_MSG\_colony\_starvation starvation;

&nbsp;   struct s\_MSG\_colony\_settlers settlers;

&nbsp;   struct s\_MSG\_colony\_rebellion rebels;

&nbsp;   struct s\_MSG\_colony\_android\_killed android\_killed;

};



struct s\_MSG\_colony {

&nbsp;   uint8\_t planet\_idx;

&nbsp;   int8\_t subtype;

&nbsp;   union s\_MSG\_colony\_union c;

};



struct s\_MSG\_lost\_colony {

&nbsp;   int16\_t planet\_idx;

&nbsp;   int16\_t reason;

&nbsp;   int16\_t leader\_idx;

&nbsp;   int8\_t colony\_or\_outpost;

};



struct s\_MSG\_diplomacy {

&nbsp;   int8\_t diplomacy\_idx;

&nbsp;   int8\_t plr\_a;

&nbsp;   int8\_t plr\_b;

};



struct s\_MSG\_lost\_defending\_spy {

&nbsp;   int16\_t count;

};



struct s\_MSG\_spy\_assasination {

&nbsp;   int8\_t plr\_a;

&nbsp;   int8\_t plr\_b;

&nbsp;   int8\_t leader\_id;

&nbsp;   int8\_t victim\_id;

};



struct s\_MSG\_anomaly {

&nbsp;   uint8\_t star\_idx;

};



union s\_MSG\_union {

&nbsp;   struct s\_MSG\_colony colony;

&nbsp;   struct s\_MSG\_players players;

&nbsp;   struct s\_MSG\_financial financial;

&nbsp;   struct s\_MSG\_lost\_ship lost\_ship;

&nbsp;   struct s\_MSG\_scrap\_ship scrap\_ship;

&nbsp;   struct s\_MSG\_lost\_colony lost\_colony;

&nbsp;   struct s\_MSG\_hyperspace\_beast hyperspace\_beast;

&nbsp;   struct s\_MSG\_lost\_attacking\_spy lost\_attacking\_spy;

&nbsp;   struct s\_MSG\_lost\_defending\_spy lost\_defending\_spy;

&nbsp;   struct s\_MSG\_artemis artemis;

&nbsp;   struct s\_MSG\_artemis\_leader artemis\_leader;

&nbsp;   struct s\_MSG\_failed\_retreat retreat;

&nbsp;   struct s\_MSG\_spy\_assasination spy\_assasination;

&nbsp;   struct s\_MSG\_anomaly anomaly;

&nbsp;   struct s\_MSG\_diplomacy diplomacy;

&nbsp;   struct s\_MSG\_eel eel;

&nbsp;   struct s\_MSG\_artifacts artifacts;

&nbsp;   struct s\_MSG\_scrap\_leader scrap\_leader;

};



struct s\_MSG {

&nbsp;   int8\_t msg\_type;

&nbsp;   uint8\_t priority;

&nbsp;   union s\_MSG\_union u;

&nbsp;   undefined field3\_0xa;

&nbsp;   undefined field4\_0xb;

&nbsp;   undefined field5\_0xc;

&nbsp;   undefined field6\_0xd;

&nbsp;   undefined field7\_0xe;

&nbsp;   undefined field8\_0xf;

&nbsp;   undefined field9\_0x10;

&nbsp;   undefined field10\_0x11;

};

