int utmp_open(char *filename);
struct utmp* utmp_next();
int utmp_reload();
void utmp_close();
