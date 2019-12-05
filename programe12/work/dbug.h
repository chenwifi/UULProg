#define dabort() \
{printf("Aborting in line %d in source file %s\n",__LINE__,__FILE__);abort();}
