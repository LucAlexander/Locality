#include <QtWidgets>
#include <QString>

class LinkLabel;
class ProjectManager;

void compileSelectedProject(void*);

class Editor : public QWidget{
	public:
		Editor(QWidget* parent = nullptr);
		void addProjectLink(QString name);
		void setSelected(LinkLabel* projectLink);
		void openProject();
		void closeProject();
	private:
		QScrollArea* projectlist;
		QWidget* projectcontainer;
		QVBoxLayout* vlayout;
		QScrollArea* actions;
		LinkLabel* selected;
		ProjectManager* manager;
};
