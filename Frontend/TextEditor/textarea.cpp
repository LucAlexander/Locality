#include "textarea.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

TextArea::TextArea(){}

void TextArea::OpenFile(){
	QString fileurl = QFileDialog::getOpenFileName(this,"Choose one  files", "", "*");
	if(fileurl.length()!=0){
		QFile *activeFile = new QFile(fileurl);
		if(checkExtension(activeFile)){
			//valid extension;
			currentFile = activeFile;
			currentFile->open(QFile::ReadOnly|QFile::Text);
			QTextStream line(currentFile);
			this->setPlainText("");
			while(!line.atEnd()){
			    this->setPlainText(this->toPlainText()+line.readLine()+"\n");
			}
			currentFile->close();

		}
		else{
			QMessageBox::critical(this,"Cannot read File","Extention not valid");
		}
	}
}

void TextArea::SaveFile(){
	if(!currentFile){
		// Null file
		currentFile = new QFile(QFileDialog::getSaveFileName(this,"Save File"));
		SaveFile();
	}
	else{
		// Not Null file
		currentFile->open(QFile::ReadWrite|QFile::Text);
		currentFile->write(this->toPlainText().toUtf8());
		currentFile->close();
	}

}
bool TextArea::checkExtension(QFile *f){
	for(QString ext:fileextension){
		if(f->fileName().endsWith(ext)){
		    return true;
		}
	}
	return false;
}
