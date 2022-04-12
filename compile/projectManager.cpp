#include "projectManager.h"

#include <QTextStream>
#include <QStringList>

#include <cstdlib>

ProjectManager::ProjectManager():
	ProjectManager("./projects")
{}

ProjectManager::ProjectManager(QString projdir):
	project(),
	projectDir(projdir),
	dirManager("./"),
	projectStub("_proj"),
	projectFileStub("project.c"),
	componentFileStub("projectComponents.h"),
	makeTarget("make release")
{
	CreateProjectDirectory();
}

void ProjectManager::CreateProjectDirectory(){
	if (!dirManager.exists(projectDir)){
		dirManager.mkdir(projectDir);
	}
	dirManager.cd(projectDir);
}

void ProjectManager::CreateProjectBaseFile(QFile* file, const QString message){
	if (file->open(QIODevice::ReadWrite)){
		QTextStream stream(file);
		stream << message << Qt::endl;
	}
}

void ProjectManager::SelectProject(QString name){
	project = ActiveProject(name, &dirManager);
	if (project.good()){
		dirManager.cd(name);
	}
}

void ProjectManager::CompileProject(){
	if (!project.good()){
		return;
	}
	project.makeTarget = makeTarget;
	project.compile();
}

ProjectManager::ActiveProject::ActiveProject():
	projectName(""),
	state(ACTIVE_PROJECT_UNINITIALIZED),
	parentManager(nullptr),
	makeTarget("make release")
{}

ProjectManager::ActiveProject::ActiveProject(QString pn, QDir* parent):
	projectName(pn),
	state(ACTIVE_PROJECT_UNINITIALIZED),
	parentManager(parent),
	makeTarget("make release")
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
	system(makeTarget.c_str());
	DestroyFiles(&sources);
}

void ProjectManager::ActiveProject::DestroyFiles(QStringList* files){
	for (QString file : *files){
		parentManager->remove("../../"+file);
	}
}

void ProjectManager::ActiveProject::CopyProjFiles(QStringList* source){
	QString sourcePath = parentManager->path() + "/";
	QString destPath = QDir::currentPath() + "/";
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
	QFile file("./Makefile");
	if (!file.open(QIODevice::ReadOnly)){
		return "";
	}
	QTextStream in(&file);
	QString line = in.readAll();
	file.close();
	return line;
}

void ProjectManager::ActiveProject::WriteMakefile(QString data){
	QFile file("./Makefile");
	if (!file.open(QIODevice::WriteOnly)){
		return;
	}
	QTextStream out(&file);
	out << data;
	file.close();
}


