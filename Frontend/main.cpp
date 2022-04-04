#include "projectManager.h"
#include "editor.h"

#include <QApplication>
#include <QColumnView>
#include <QFont>

int main(int argc, char** argv){
		//Q_INIT_RESOURCE(application);
	QApplication app(argc, argv);

	QFont font;
	font.setPointSize(48);

	QApplication::setFont(font);

	Editor window;

	window.show();

	return app.exec();

	ProjectManager pm;
	pm.SelectProject("test_proj");
	pm.CompileProject();
	return 0;


}
