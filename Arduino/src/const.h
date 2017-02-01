/*
** pinning arduino
*/
// moteur
#define pinA_0 3
#define pinA_1 4
#define pinA_2 5
#define pinA_3 6
#define pinB_0 7
#define pinB_1 8
#define pinB_2 9
#define pinB_3 10
// capteur distance
#define dist_trigger 11
#define dist_reader 12
/*
** controle moteur
*/
// etats
#define HALT     1
#define HALTING  2
#define ROTATE   3
#define DRIVE    4
#define HALT_INV 5
// allure
#define FULL    1
#define HALF    2
#define QUARTER 3
// sens
#define FORWARD  1
#define BACKWARD 2
// rotation
#define RIGHT    1
#define LEFT     2
/*
** mode autonome
*/
#define AUTO_NORM    1
#define AUTO_ROTATE  2
#define AUTO_DISABLE 3
