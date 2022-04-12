#ifndef FUNCTIONAL_BUTTON
#define FUNCTIONAL_BUTTON

#include <QPushButton>
#include <QString>
#include <QWidget>

void defaultFunctionalButtonBehavior(void*);

class FButton : public QPushButton{
	Q_OBJECT
		void (*function)(void*);
		void* arg;
	public:
		explicit FButton(const QString& text, QWidget* parent = Q_NULLPTR);
		void setFunction(void f(void*));
		void setArg(void* a);
	public slots:
		void callFunction();
	signals:
		void clicked();
	protected:
		void mousePressEvent(QMouseEvent* e);
};

#endif
