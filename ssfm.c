/* Have a look at the LICENSE file for copyright and license details. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <linux/limits.h>
#include <ncurses.h>
#include <menu.h>

#include "ssfm.h"
#include "config.h"


Windows
draw(int termrows, int termcols) {
  Windows win;
  win.menu  =  newwin(termrows - 2, termcols, 1, 1);
  win.cmd   =  newwin(1, termcols, termrows - 1, 0);
  win.dir   =  newwin(1, termcols, termrows - 2, 0);
  /* box(win.menu, 0, 0); */
  allwinrefresh(win);
  return win;
}

void
allwinrefresh(Windows win) {
  refresh();
  wrefresh(win.menu);
  wrefresh(win.cmd);
  wrefresh(win.dir);
}

void
draw_menu(Items *items, Menu *menu, Windows win, char *pwd) {
  
  getdiritems(items, pwd, menu->showhidden);

  menu->size = items->count;
  menu->dmenu = new_menu((ITEM **)items->item);
  menu_opts_off(menu->dmenu, O_ONEVALUE);
  menu_opts_off(menu->dmenu, O_SHOWDESC);
  set_menu_win(menu->dmenu, win.menu);
  set_menu_sub(menu->dmenu, win.menu);
  set_menu_format(menu->dmenu, menu->rows, menu->cols);
  set_menu_mark(menu->dmenu, Marker);
  post_menu(menu->dmenu);
  wprintw(win.dir, pwd);

  allwinrefresh(win);
}

void 
freeall(Menu *menu, Items *items, char* pwd) {
  unpost_menu(menu->dmenu);
  for (int i = 0; i < items->count; i++)
    free_item(items->item[i]);
  free_menu(menu->dmenu);
  if (pwd != NULL)
    free(pwd);
}
void 
execsh(char *cmd) {
  def_prog_mode();
  endwin();

  if (cmd == NULL)
    system("$SHELL");
  else 
    system(cmd);
  
  reset_prog_mode();
  refresh();
}

void
startfm(void) {
  char *pwd = getcwd(NULL, 0);
  int termrows, termcols;
  Windows win;
  Menu *menu = (Menu *) malloc(sizeof(MENU));
  Items *items = (Items *) malloc(sizeof(Items));
  
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);

  getmaxyx(stdscr, termrows, termcols);

  win = draw(termrows, termcols);
  menu->rows = termrows - 3;
  menu->cols = 1;
  menu->showhidden = ShowHidden;
  draw_menu(items, menu, win, pwd);

  int key;
  while ((key = getkey()) != 'q') {

    if (key == 'S')
      execsh(NULL);

    else if (key == KEY_DOWN || key == 'j')
      menu_driver(menu->dmenu, REQ_DOWN_ITEM);

    else if (key == KEY_UP || key == 'k')
      menu_driver(menu->dmenu, REQ_UP_ITEM);

    else if (key == KEY_LEFT || key == 'h') {
      if (strcmp(pwd, "/") != 0) {
        setdir("../");
        pwd = getcwd(NULL, 0);
        win = draw(termrows, termcols);
        draw_menu(items, menu, win, pwd);  
      }
    }
    else if (key == KEY_RIGHT || key == 'l') {
      if ( !open_file_or_chdir(menu, pwd) ) {
        freeall(menu, items, pwd);
        pwd = getcwd(NULL, 0);
        win = draw(termrows, termcols);
        draw_menu(items, menu, win, pwd);  
      }
    }
    else if (key == 'g')
      menu_driver(menu->dmenu, REQ_FIRST_ITEM);

    else if (key == 'G')
      menu_driver(menu->dmenu, REQ_LAST_ITEM);

    else if (key == 'H') {
      menu->showhidden = !menu->showhidden;
        freeall(menu, items, NULL);
        win = draw(termrows, termcols);
        draw_menu(items, menu, win, pwd);  
    }

    allwinrefresh(win);
  }
  
  freeall(menu, items, pwd);
  endwin();
}

bool
open_file_or_chdir(Menu *menu, char *pwd) {
  char namebuff[ PATH_MAX + NAME_MAX ];
  char *prompt = "2>/dev/null ; read -p \'Press enter key to continue...\' temp";

  sprintf(namebuff, "%s/%s", pwd,
      item_name(current_item(menu->dmenu)));

  if (isdir(namebuff)) {
    setdir(namebuff);
    return false;
  }
  
  char cmdbuff[ 256 + PATH_MAX + NAME_MAX ];
  if (Opener != NULL) {
    sprintf(cmdbuff, "%s %s %s ", Opener, namebuff, prompt);
    execsh(cmdbuff);
    return true;
  }

  /* Implement Code for opener with mimetype */
  return true;
}

int
getkey() {
  int key;
  while (1) {
    key = getch();
    if (key != KEY_MOUSE && key != KEY_RESIZE)
      return key;
  }
}

void 
setdir(const char *dir) {
    if ( chdir(dir) == -1 )
      die("Error: %s %s\n", dir, strerror(errno));
}

void
getdiritems(Items *items, char *dpath, bool showhidden) {
  int pathlen = strlen(dpath);
  struct dirent *de;
  DIR *dr = opendir(dpath);
  int ditems_count = 0;

  while ((de = readdir(dr)) != NULL)
    ditems_count++;
  
  rewinddir(dr);

  items->item = (ITEM **)calloc(ditems_count + 1, sizeof(ITEM *));

  int itr = 0;
  while ((de = readdir(dr)) != NULL) {

    if ( !strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") )
      continue;

    if ( !showhidden && de->d_name[0] == '.' )
      continue;

    int d_name_len = strlen(de->d_name);
    char fullpath[ pathlen + d_name_len + 1 ];
    sprintf(fullpath, "%s/%s", dpath, de->d_name);

    char *dname = (char *) malloc( d_name_len + 2 );

    if (isdir(fullpath)) {
        sprintf(dname, "%s/", de->d_name);
        items->item[itr] = new_item(dname, dname);
    }
    else
        items->item[itr] = new_item(de->d_name, de->d_name);
    itr++;
  }
  items->item[ditems_count] = (ITEM *)NULL;
  items->count = itr;
}

int
isdir(const char *dpath) {
  DIR *dir = opendir(dpath);
  if (!dir)
    return 0;
  closedir(dir);
  return  1;
}

void
die(const char *errstr, ...) {
  va_list ap;

  va_start(ap, errstr);
  vfprintf(stderr, errstr, ap);
  va_end(ap);
  exit(1);
}

int
main(int argc, const char *argv[])
{
  if (argc > 1)
    setdir(argv[1]);
  else if (Start_Dir != NULL)
    setdir(Start_Dir);

  startfm();
  
  return 0;
}
