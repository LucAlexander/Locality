#include "projectManager.h"
#include "editor.h"
#include "linkLabel.h"

#include <QTextStream>
#include <QStringList>

#include <cstdlib>

ProjectManager::ProjectManager():
	ProjectManager("./projects")
{}

ProjectManager::ProjectManager(QString projdir):
	project(),
	projectDir(projdir),
	dirManager("../"),
	projectStub("_proj"),
	projectFileStub("project.c"),
	componentFileStub("projectComponents.h")
{
	CreateProjectDirectory();
}

void ProjectManager::CreateProjectDirectory(){
	if (!dirManager.exists(projectDir)){
		dirManager.mkdir(projectDir);
	}
	dirManager.cd(projectDir);
}

void ProjectManager::CreateProject(QString name){
	QString dirname = name + projectStub;
	if (dirManager.exists(dirname)){
		int count = 0;
		while (dirManager.exists(dirname+"("+QString::number(count)+")")){
			count ++;
		}
		dirname+= ("(" + QString::number(count) + ")");
	}
	dirManager.mkdir(dirname);
	dirManager.cd(dirname);
	QFile projectFile(dirManager.path()+"/"+ projectFileStub);
	QFile componentFile(dirManager.path()+"/"+ componentFileStub);
	CreateProjectBaseFile(&projectFile,"//This is your projects entry point file\n#include<stdio.h>\n#include\"Locality.h\"\n#include\"LocGUI.h\"\n\nvoid project_config(Project_config* config){\n\t// Configuration code\n}\n\nvoid project_systems(){\n\t// Initialize and register project systems\n}\n\nvoid project(){\n\t// Project initialization and setup\n\tprintf(\"Hello World!\");\n}\n");
	CreateProjectBaseFile(&componentFile,"//This is where your components go\n#ifndef PROJECT_COMPONENTS_H\n#define PROJECT_COMPONENTS_H \\\n\tCOMPONENT_NAME_C\n\n#define PROJECT_COMPONENT_SIZES \\\n\tsizeof(void*)\n#endif\n");
	dirManager.cdUp();
}

void ProjectManager::CreateProjectBaseFile(QFile* file, const QString message){
	if (file->open(QIODevice::ReadWrite)){
		QTextStream stream(file);
		stream << message << Qt::endl;
	}
}

void ProjectManager::DeleteProject(QString name){
	if (!dirManager.exists(name)){
		name += projectStub;
	}
	if (!dirManager.exists(name)){
		return;
	}
	dirManager.cd(name);
	if (!dirManager.exists()){
		dirManager.cdUp();
		return;
	}
	dirManager.removeRecursively();
	dirManager.cdUp();
}

void ProjectManager::SelectProject(QString name){
	DeselectProject();
	project = ActiveProject(name, &dirManager);
	if (project.good()){
		dirManager.cd(name);
	}
}

void ProjectManager::DeselectProject(){
	if (project.state != ACTIVE_PROJECT_UNINITIALIZED){
		dirManager.cdUp();
		project.state = ACTIVE_PROJECT_UNINITIALIZED;
	}
}

void ProjectManager::CompileProject(){
	if (!project.good()){
		return;
	}
	project.compile();
}

void ProjectManager::SeekProjects(Editor* menu){
	if (project.state != ACTIVE_PROJECT_UNINITIALIZED){
		return;
	}
	QStringList projectFolders = dirManager.entryList(QDir::Dirs);
	menu->clearvbox();
	for (QString dir : projectFolders){
		if (dir.contains(projectStub)){
			menu->addProjectLink(dir);
		}
	}
}

ProjectManager::ActiveProject::ActiveProject():
	projectName(""),
	state(ACTIVE_PROJECT_UNINITIALIZED),
	parentManager(nullptr)
{}

ProjectManager::ActiveProject::ActiveProject(QString pn, QDir* parent):
	projectName(pn),
	state(ACTIVE_PROJECT_UNINITIALIZED),
	parentManager(parent)
{
	if (parentManager==nullptr || !parentManager->exists()){
		return;
	}
	if (parentManager->exists(projectName)){
		state = ACTIVE_PROJECT_INITIALIZED;
	}
}

bool ProjectManager::ActiveProject::good(){
	return state == ACTIVE_PROJECT_INITIALIZED;
}

void ProjectManager::ActiveProject::compile(){
	QStringList sources = parentManager->entryList(QDir::Files);
	CopyProjFiles(&sources);
	ConstructMakefile(&sources);
	system("cd .. && make debug");
	DestroyFiles(&sources);
}

void ProjectManager::ActiveProject::DestroyFiles(QStringList* files){
	for (QString file : *files){
		parentManager->remove("../../"+file);
	}
}

void ProjectManager::ActiveProject::CopyProjFiles(QStringList* source){
	QString sourcePath = parentManager->path() + "/";
	QString destPath = QDir::currentPath() + "/../";
	for (QString file : *source){
		QFile::copy(sourcePath+file, destPath+file);
	}
}

void ProjectManager::ActiveProject::ConstructMakefile(QStringList* source){
	QString data = ReadMakefile();
	uint32_t linePos = data.indexOf("PROJFILE=")+strlen("PROJFILE")+1;
	uint32_t i = 0;
	while (data[i+linePos]!='\n' && data[i+linePos] != EOF){
		i++;
	}
	data.remove(linePos, i);
	QString inputfile = "";
	for (QString token : *source){
		inputfile += token + " ";
	}
	data.insert(linePos, inputfile);
	WriteMakefile(data);
}

QString ProjectManager::ActiveProject::ReadMakefile(){
	QFile file("../Makefile");
	if (!file.open(QIODevice::ReadOnly)){
		return "";
	}
	QTextStream in(&file);
	QString line = in.readAll();
	file.close();
	return line;
}

void ProjectManager::ActiveProject::WriteMakefile(QString data){
	QFile file("../Makefile");
	if (!file.open(QIODevice::WriteOnly)){
		return;
	}
	QTextStream out(&file);
	out << data;
	file.close();
}


