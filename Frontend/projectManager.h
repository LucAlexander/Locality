#ifndef LOCALITY_PROJECT_MANAGER_H
#define LOCALITY_PROJECT_MANAGER_H

#include <QString>
#include <QDir>
#include <QFile>

class Editor;

class ProjectManager{
	public:
		ProjectManager();
		ProjectManager(QString projectdir);
		void CreateProject(QString name);
		void DeleteProject(QString name);
		void SelectProject(QString name);
		void DeselectProject();
		void CompileProject();
		void SeekProjects(Editor* menu);
		bool projectSelected();
		QString getSelectedProjectFile();
	private:
		enum ACTIVE_PROJECT_STATE{
			ACTIVE_PROJECT_UNINITIALIZED,
			ACTIVE_PROJECT_INITIALIZED
		};
		struct ActiveProject{
			ActiveProject();
			ActiveProject(QString projectName, QDir* parent);
			bool good();
			void compile();
			void CopyProjFiles(QStringList* source);
			void ConstructMakefile(QStringList* source);
			void DestroyFiles(QStringList* source);
			QString ReadMakefile();
			void WriteMakefile(QString data);
			QString projectName;
			ACTIVE_PROJECT_STATE state;
			QDir* parentManager;
		};
		ActiveProject project;
		void CreateProjectDirectory();
		void CreateProjectBaseFile(QFile* file, const QString message);
		const QString projectDir;
		QDir dirManager;
		const QString projectStub;
		const QString projectFileStub;
		const QString componentFileStub;
};

#endif
