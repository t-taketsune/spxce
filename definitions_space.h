/* Direction definitions. */
#define LEFT 'l'
#define RIGHT 'r'
#define UP 'u'
#define DOWN 'd'
#define NONE 'n'

/* State definitions. */
#define ALIVE 1
#define DEAD 0
#define DYING -1
#define IN_PROGRESS 1
#define LOST 0
#define WON -2

/* Entity type definitions. */
#define TANK 1
#define BARRIER 2
#define ALIEN 3
#define BOMB 4
#define SHOT 5
#define BOX 6
#define MOTHERSHIP 7

/* General game definitions. */
#define ROW 38
#define COL 100
#define MOB 55
#define N_BAR 4
#define MAX_SHOT 5
#define MAX_BOMB 20
#define CYCLE 400000
#define ALIEN_SCORE 3
#define MOTHERSHIP_SCORE 20

/* Score output definitions. */
#define SCORE_LENGTH 8
#define NAME_LENGTH 3
#define N_SCORES 5

/* String definitions for each entity's body. */
static const char TANK_BODY[] = " /^\\ MMMMM";
static const char BARRIER_BODY[] = " AMMMA AMMMMMAMM   MM";
static const char ALIEN_1_RIGHT[] = "/^\\|O|\\v/";
static const char ALIEN_1_LEFT[] = "\\^/|X|/v\\";
static const char ALIEN_2_RIGHT[] = " O ###|||";
static const char ALIEN_2_LEFT[] = " X ###~X~";
static const char ALIEN_3_RIGHT[] = "/M\\<O>/-\\";
static const char ALIEN_3_LEFT[] = "\\m/}X{\\-/";
static const char EXPLOSION[] = "\\|/- -/|\\";
static const char MOTHERSHIP_BODY[] = "//MoM\\\\<MoMoM>\\|'-'|/";
static const char MOTHERSHIP_EXPLOSION[] = "ASVDS";
static const char XSET_COMMAND[] = "xset r rate ";

/* List struct definitions. */
typedef struct t_item {
    char* body, dir;
    int row, col;
    int type, state;
} t_item;

typedef struct t_node {
    t_item* item;
    struct t_node *prev, *prox;
} t_node;

typedef struct t_list {
    t_node *ini;
    t_node *current;
    t_node *end;
    int size;
} t_list;

/* Tank struct is different because the direction is treated as an int, not as a char. */
typedef struct t_tank {
    int row, col;
    int type, dir;
    char *body;
} tank;

/* Game struct containing every element of the game. */
typedef struct t_game {
    t_list mob;
    t_list barriers;
    t_list shots;
    t_list bombs;
    t_item mothership;
    tank player;
    int score, alive_enemies, flip, state, n_wins, cycle, delay;
    char name[4];
} t_game;
