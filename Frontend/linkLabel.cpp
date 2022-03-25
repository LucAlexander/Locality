#include "linkLabel.h"
#include <QTextStream>

LinkLabel::LinkLabel(QWidget* parent, Qt::WindowFlags flags):
	QLabel(parent)
{}

LinkLabel::~LinkLabel(){}

void LinkLabel::printData(){
	QTextStream out(stdout);
	out << "hello world" << Qt::endl;
}

void LinkLabel::mousePressEvent(QMouseEvent* e){
	emit clicked();
}
