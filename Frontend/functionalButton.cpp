#include "functionalButton.h"

void defaultFunctionalButtonBehavior(void*){}

FButton::FButton(const QString& text, QWidget* parent):
	QPushButton(text, parent),
	function(defaultFunctionalButtonBehavior),
	arg(nullptr)
{
	QObject::connect(
		this, &FButton::clicked,
		this, &FButton::callFunction	
	);
}

void FButton::callFunction(){
	function(arg);
}

void FButton::mousePressEvent(QMouseEvent* e){
	emit clicked();
}

void FButton::setFunction(void f(void*)){
	function = f;
}

void FButton::setArg(void* a){
	arg = a;
}
