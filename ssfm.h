/* Have a look at the LICENSE file for copyright and license details. */

/* Macros */
#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))


/* Structs */
typedef struct {
    WINDOW *menu;
    WINDOW *dir;
    WINDOW *cmd;
} Windows;

typedef struct {
    ITEM **item;
    int count;
} Items;

typedef struct {
    MENU *dmenu;
    int size;
    int rows;
    int cols;
    bool showhidden;
} Menu;

/* Prototypes */
void startfm(void);
void allwinrefresh(Windows win);
int getkey();
void setdir(const char *dir);
int isdir(const char *dpath);
void getdiritems(Items *items, char *dpath, bool showhidden);
void execsh(char *cmd);
void die(const char *errstr, ...);
Windows draw(int termrows, int termcols);
void draw_menu(Items *items, Menu *menu, Windows win, char *pwd);
bool open_file_or_chdir(Menu *menu, char *pwd);
void freeall(Menu *menu, Items *item, char *pwd);


/* config.h globals */
extern char *Start_Dir;
extern char *Opener;
extern char *Marker;
extern char *Shell;
extern bool ShowHidden;
