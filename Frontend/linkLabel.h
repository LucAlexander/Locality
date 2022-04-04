#ifndef LINK_LABEL_H
#define LINK_LABEL_H

#include <QWidget>
#include <QLabel>

class Editor;

class LinkLabel : public QLabel{
	Q_OBJECT
		Editor* parentSelector;
	public:
		explicit LinkLabel(Editor* e, QWidget* parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
		~LinkLabel();
	public slots:
		void printData();
	signals:
		void clicked();
	protected:
		void mousePressEvent(QMouseEvent* e);

};

#endif
