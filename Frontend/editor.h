#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets>
#include <QString>
#include <vector>

class LinkLabel;
class ProjectManager;
class Editor;

void compileSelectedProject(void*);
void createNewProject(void*);
void deleteSelectedProject(void*);

struct deleteParameter{
	ProjectManager* manager;
	Editor* editor;
};

class Editor : public QWidget{
	public:
		Editor(QWidget* parent = nullptr);
		void addProjectLink(QString name);
		void setSelected(LinkLabel* projectLink);
		void openProject();
		void closeProject();
		std::vector<LinkLabel*> getChildLinks();
		LinkLabel* selected;
		deleteParameter* delpar;
	private:
		QScrollArea* projectlist;
		QWidget* projectcontainer;
		QVBoxLayout* vlayout;
		QScrollArea* actions;
		ProjectManager* manager;
		std::vector<LinkLabel*> links;

};


#endif
