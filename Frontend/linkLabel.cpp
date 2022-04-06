#include "linkLabel.h"
#include "editor.h"

LinkLabel::LinkLabel(Editor* e, QWidget* parent, Qt::WindowFlags flag):
	QLabel(parent),
	parentSelector(e)
{}

LinkLabel::~LinkLabel(){}

void LinkLabel::printData(){
	parentSelector->setSelected(this);
}

void LinkLabel::mousePressEvent(QMouseEvent* e){
	emit clicked();
}
