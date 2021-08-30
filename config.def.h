/* Have a look at the LICENSE file for copyright and license details. */

/*
 * To make ssfm start always at a particular location when
 * no location is passed as arguments set the value of 
 * Start_Dir to the location as a string. For eg to make ssfm
 * start always at home set Start_Dir = "/home/<username>";
 */
char *Start_Dir = NULL;

/*
 * Change the value of Marker below to a character or
 * a string to have it as file menu marker.
 */
char *Marker = " ";

/*
 * To override $SHELL and use different shell set the 
 * below Shell varaible. For eg Shell = "/bin/bash";
 */
char *Shell = NULL;

/*
 * To always show hidden files change the below 
 * option to true.
 */
bool ShowHidden = false;

char *Opener = "xdg-open";
