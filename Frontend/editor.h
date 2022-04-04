#include <QtWidgets>
#include <QString>

class LinkLabel;

class Editor : public QWidget{
	public:
		Editor(QWidget* parent = nullptr);
		void addProjectLink(QString name);
		void setSelected(LinkLabel* projectLink);
	private:
		QScrollArea* projectlist;
		QWidget* projectcontainer;
		QVBoxLayout* vlayout;
		QScrollArea* actions;
		LinkLabel* selected;
};
