/* Have a look at the LICENSE file for copyright and license details. */

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))


void setdir(const char * dir);
void die(const char *errstr, ...);


/* config.h globals */
extern char *Start_Dir;
