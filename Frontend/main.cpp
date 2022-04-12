#include "projectManager.h"
#include "editor.h"

#include "TextEditor/textarea.h"
#include "TextEditor/filemanage.h"

#include <QApplication>
#include <QColumnView>
#include <QFont>

int main(int argc, char** argv){
	//Q_INIT_RESOURCE(application);
	QApplication app(argc, argv);

	QFont font;
	font.setPointSize(16);

	QApplication::setFont(font);

	Editor window;

		QPalette windowPalette;
		QPalette contentPalette;

		QColor backgroundColor(75, 71, 92);
		QColor contentColor(215, 222, 230);

		windowPalette.setColor(QPalette::Window, backgroundColor);
		contentPalette.setColor(QPalette::WindowText, contentColor);

		//******************************Frame
		QWidget *frame = new QWidget();
		frame->setPalette(windowPalette);
		frame->setWindowTitle("TextEditor");
		frame->setMinimumSize(800,600);
		QVBoxLayout *Vlayout = new QVBoxLayout();
		//Menu Bar******************************
		QMenuBar *MenuBar = new QMenuBar();
		MenuBar->setPalette(contentPalette);

		//MenuBar Item**************************
		QMenu *File = new QMenu("File");
		QMenu *Edit = new QMenu("Edit");
		QMenu *View =new QMenu("View");
		QMenu *Setting = new QMenu("Settings");

		//MenuItem to each Menu*****************

		File->addAction("Open File");
		File->addAction("Save File");
		File->addAction("Quit");
		//********
		Edit->addAction("Copy all Text");
		Edit->addAction("Copy");
		Edit->addAction("Coller");
		Edit->addSeparator();
		Edit->addAction("New Tab");
		//****
		View->addMenu(new QMenu("Change Text Background"));
		View->addMenu(new QMenu("Change Text Foreground"));
		View->addMenu(new QMenu("Choose Font"));

		//MenuBar add Menu**********************
		MenuBar->addMenu(File);
		MenuBar->addMenu(Edit);
		MenuBar->addMenu(View);
		MenuBar->addMenu(Setting);
		MenuBar->show();

		//TabWidget****************************
		Tab *Tabs = new Tab();

		//*****************connection
		  //QFile connection
		QObject::connect(File->findChildren<QAction *>()[1],SIGNAL(triggered(bool)),qobject_cast<TextArea *>(Tabs->currentWidget()),SLOT(OpenFile()));
		QObject::connect(File->findChildren<QAction *>()[2],SIGNAL(triggered(bool)),qobject_cast<TextArea *>(Tabs->currentWidget()),SLOT(SaveFile()));
		QObject::connect(File->findChildren<QAction *>()[3],SIGNAL(triggered(bool)),&app,SLOT(quit()));

		//QEdit connection
		QObject::connect(Edit->findChildren<QAction *>()[5],SIGNAL(triggered(bool)),Tabs,SLOT(createNewTab()));

		//Root Layout***************************
		Vlayout->setMenuBar(MenuBar);
		Vlayout->addWidget(Tabs);

		//adding root layout*****************
		frame->setLayout(Vlayout);

	window.setTextEditor(frame);
	window.show();
	return app.exec();

	ProjectManager pm;
	pm.SelectProject("test_proj");
	pm.CompileProject();

	return 0;



}
