#include "editor.h"
#include "linkLabel.h"
#include "projectManager.h"
#include "functionalButton.h"

#include <QPalette>
#include <QColor>
#include <QLabel>
#include <QScrollArea>

void Editor::addProjectLink(QString name){
	auto* link = new LinkLabel(this);
	link->setScaledContents(true);
	link->setText(name);
	QObject::connect(
		link, &LinkLabel::clicked,
		link, &LinkLabel::printData
	);
	vlayout->addWidget(link);
}

void Editor::setSelected(LinkLabel* projectLink){
	selected = projectLink;
	manager->SelectProject(selected->text());//TODO this is a test
}

void Editor::openProject(){
	if (selected != nullptr){
		manager->SelectProject(selected->text());
	}
}

void Editor::closeProject(){
	manager->DeselectProject();
	selected = nullptr;
}

void compileSelectedProject(void* t){
	ProjectManager* manager = (ProjectManager*)t;
	manager->CompileProject();
}

Editor::Editor(QWidget* parent):
	QWidget(parent),
	projectlist(new QScrollArea(this)),
	projectcontainer(new QWidget(this)),
	vlayout(new QVBoxLayout(projectcontainer)),
	actions(new QScrollArea(this)),
	selected(nullptr),
	manager(new ProjectManager())
{
	QPalette windowPalette;
	QPalette contentPalette;

	QColor backgroundColor(28, 27, 25);
	QColor contentColor(252, 232, 195);

	windowPalette.setColor(QPalette::Window, backgroundColor);
	contentPalette.setColor(QPalette::WindowText, contentColor);

	setAutoFillBackground(true);
	setPalette(windowPalette);

	projectlist->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	projectlist->setPalette(contentPalette);
	projectlist->setWidgetResizable(true);
	projectlist->setContentsMargins(0, 0, 0, 0);
		
	manager->SeekProjects(this);

	projectlist->setWidget(projectcontainer);
	projectlist->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	actions->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	actions->setPalette(contentPalette);
	actions->setWidgetResizable(true);
	actions->setContentsMargins(0, 0, 0, 0);

	QWidget* actioncontainer = new QWidget(this);
	auto* actionlayout = new QVBoxLayout(actioncontainer);
	actions->setWidget(actioncontainer);
	actions->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	FButton* compile = new FButton("Compile", this);
	compile->setArg(manager);
	compile->setFunction(compileSelectedProject);
	actionlayout->addWidget(compile);

	auto* grid = new QGridLayout(this);
	grid->addWidget(actions, 0, 0, 1, 1);
	grid->addWidget(projectlist, 0, 1, 1, 1);

	setLayout(grid);

	resize(960, 720);
	setWindowTitle("Editor");
}

