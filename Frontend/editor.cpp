#include "editor.h"
#include <QPalette>
#include <QColor>
#include <QLabel>

Editor::Editor(QWidget* parent):
	QWidget(parent)
{
	QPalette windowPalette;
	QPalette contentPalette;

	QColor backgroundColor(28, 27, 25);
	QColor contentColor(252, 232, 195);

	windowPalette.setColor(QPalette::Window, backgroundColor);
	contentPalette.setColor(QPalette::WindowText, contentColor);

	setAutoFillBackground(true);
	setPalette(windowPalette);

	auto* title = new QFrame(this);
	title->setFrameStyle(QFrame::NoFrame);
	title->setPalette(contentPalette);

	auto* titleText = new QLabel(title);
	titleText->setScaledContents(true);
	titleText->setText("Locality Project Manager");

	auto* projects = new QFrame(this);
	projects->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	projects->setPalette(contentPalette);

	auto* actions = new QFrame(this);
	actions->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	actions->setPalette(contentPalette);

	auto* grid = new QGridLayout(this);
	grid->addWidget(title, 0, 0, 1, 2);
	grid->addWidget(actions, 1, 0, 4, 1);
	grid->addWidget(projects, 1, 1, 4, 1);

	setLayout(grid);

	resize(960, 720);
	setWindowTitle("Editor");
}

