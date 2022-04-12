#include "editor.h"
#include "linkLabel.h"
#include "projectManager.h"
#include "functionalButton.h"

#include <QPalette>
#include <QColor>
#include <QLabel>
#include <QScrollArea>
#include <QInputDialog>

#include "TextEditor/textEditorComponent.h"

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
	if (projectLink != nullptr){
		selected = projectLink;
		manager->SelectProject(selected->text());
	}
}

void Editor::openProject(){
	TextLauncher::OpenProject();
}

void Editor::closeProject(){
	manager->DeselectProject();
	selected = nullptr;
}

void openSelectedProject(void* t){
	deleteParameter* args = (deleteParameter*)t;
	Editor* edit = args->editor;
	edit->openProject();
}

void compileSelectedProject(void* t){
	ProjectManager* manager = (ProjectManager*)t;
	manager->CompileProject();
}

void createNewProject(void* t){
	deleteParameter* args = (deleteParameter*)t;
	ProjectManager* manager = args->manager;
	Editor* edit = args->editor;
	bool good;
	QString projectName = QInputDialog::getText(0, "Input Project Name", "Name: ", QLineEdit::Normal, "", &good);
	if (!good){
		return;
	}
	if (projectName.isEmpty()){
		projectName = "Unnamed-Project";
	}
	manager->CreateProject(projectName);
	manager->SeekProjects(edit);
}

void deleteSelectedProject(void* t){
	deleteParameter* args = (deleteParameter*)t;
	ProjectManager* manager = args->manager;
	Editor* edit = args->editor;
	if (edit->selected==nullptr){
		return;
	}
	QString targetProjectName = edit->selected->text();
	edit->selected = nullptr;
	manager->DeselectProject();
	manager->DeleteProject(targetProjectName);
	manager->SeekProjects(edit);
}


void Editor::clearvbox(){
	QLayoutItem* child;
	while(vlayout->count()!=0){
		child = vlayout->takeAt(0);
		if(child->widget() != 0){
			delete child->widget();
		}
		delete child;
	}
}

Editor::Editor(QWidget* parent):
	QWidget(parent),
	selected(nullptr),
	delpar(new deleteParameter()),
	projectlist(new QScrollArea(this)),
	projectcontainer(new QWidget(this)),
	vlayout(new QVBoxLayout(projectcontainer)),
	actions(new QScrollArea(this)),
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

	FButton* createButton = new FButton("Create Project", this);
	FButton* deleteButton = new FButton("Delete Project", this);
	FButton* openButton = new FButton("Open Project", this);
	delpar->manager = manager;
	delpar->editor = this;
	createButton->setArg(delpar);
	deleteButton->setArg(delpar);
	openButton->setArg(delpar);
	createButton->setFunction(createNewProject);
	deleteButton->setFunction(deleteSelectedProject);
	openButton->setFunction(openSelectedProject);
	actionlayout->addWidget(createButton);
	actionlayout->addWidget(deleteButton);
	actionlayout->addWidget(openButton);

	auto* grid = new QGridLayout(this);
	grid->addWidget(actions, 0, 0, 1, 1);
	grid->addWidget(projectlist, 0, 1, 1, 1);

	setLayout(grid);

	resize(960, 720);
	setWindowTitle("Editor");
}
