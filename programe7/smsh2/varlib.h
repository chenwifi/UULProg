int VLstore(char *name,char *value);
char *VLlookup(char *name);
int VLexport(char *name);
void VLlist();
char **VLtable2environ();
int VLenviron2table(char *env[]);
