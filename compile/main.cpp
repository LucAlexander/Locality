#include <iostream>
#include "projectManager.h"

int main(int argc, char** argv){
	if (argc < 1){return 0;}
	ProjectManager manager;
	if(strcmp(argv[1], "-g")==0){
		manager.makeTarget = "make debug";
		manager.SelectProject(argv[2]);
	}
	else if (strcmp(argv[1], "-w")==0){
		manager.makeTarget = "make release-windows";
		manager.SelectProject(argv[2]);
	}
	else{
		manager.SelectProject(argv[1]);
	}
	manager.CompileProject();
	return 0;
}
