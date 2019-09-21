#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVARS 200

struct var{
	char *str;
	int global;
};

static struct var tab[MAXVARS];

char *new_string(char *name,char *value){
	char *retval;
	retval = (char *)malloc(strlen(name)+strlen(value)+2);
	if(retval != NULL){
		sprintf(retval,"%s=%s",name,value);
	}

	return retval;
}

struct var *find_item(char *name,int first_blank){
	int i;
	int len = strlen(name);

	for(i = 0;i < MAXVARS && tab[i].str != NULL;i++){
		if(strncmp(tab[i].str,name,len) == 0 && tab[i].str[len] == '='){
			return &tab[i];
		}
	}

	if(i < MAXVARS && first_blank){
		return &tab[i];
	}

	return NULL;
}

int VLstore(char *name,char *value){
	struct var *itemp;
	char *s;
	int rv = 1;

	if((itemp = find_item(name,1)) != NULL && (s = new_string(name,value)) != NULL){
		if(itemp->str){
			free(itemp->str);
		}
		itemp->str = s;
		itemp->global = 0;
		rv = 0;
	}

	return rv;
}

char *VLlookup(char *name){
	struct var *itemp;

	if((itemp = find_item(name,0)) != NULL){
		return itemp->str + 1 + strlen(name);
	}

	return "";
}

int VLexport(char *name){
	struct var *itemp;
	int rv = 1;

	if((itemp = find_item(name,0)) != NULL){
		itemp->global = 1;
		rv = 0;
	}else if(VLstore(name,"") == 0){
		rv = VLexport(name);
	}

	return rv;
}

void VLlist(){
	int i;
	
	for(i = 0;i < MAXVARS && tab[i].str != NULL;i++){
		if(tab[i].global){
			printf(" * %s\n",tab[i].str);
		}else{
			printf("  %s\n",tab[i].str);
		}
	}
}

int VLenviron2table(char *env[]){
	int i;
	char *newstring;

	for(i = 0;env[i] != NULL; i++){
		if(i >= MAXVARS){
			return 0;
		}

		newstring = (char *)malloc(strlen(env[i]) + 1);
		if(newstring == NULL){
			return 0;
		}

		strcpy(newstring,env[i]);
		tab[i].str = newstring;
		tab[i].global = 1;
	}

	return 1;
}

char **VLtable2environ(){
	int i,j;
	int n = 0;
	char **envtab;

	for(i = 0;i < MAXVARS && tab[i].str != NULL;i++){
		if(tab[i].global == 1){
			n++;
		}

		envtab = (char **)malloc(n * sizeof(char *));
		if(envtab == NULL){
			return NULL;
		}
	}

	for(i = 0,j = 0;i < MAXVARS && tab[i].str != NULL;i++){
		if(tab[i].global == 1){
			//envtab[j++] = (char *) malloc (strlen(tab[i].str) + 1);
			envtab[j++] = tab[i].str;
		}
	}

	envtab[j] = NULL;

	return envtab;
}
