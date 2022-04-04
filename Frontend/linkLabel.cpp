#include "linkLabel.h"
#include "editor.h"
#include <QTextStream>

LinkLabel::LinkLabel(Editor* e, QWidget* parent, Qt::WindowFlags flag):
	QLabel(parent),
	parentSelector(e)
{}

LinkLabel::~LinkLabel(){}

void LinkLabel::printData(){
	QTextStream out(stdout);
	out << "hello world" << Qt::endl;
	parentSelector->setSelected(this);
}

void LinkLabel::mousePressEvent(QMouseEvent* e){
	emit clicked();
}
