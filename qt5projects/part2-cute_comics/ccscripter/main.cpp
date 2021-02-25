#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QTextEdit>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QPrinter>

#include "ui_mainwindow.h"

#include "../cutecomics/entities/characters.h"
#include "scripthighlighter.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	auto charactersListView = w.ui->listView_characters;
	auto addCharacterButton = w.ui->pushButton_addCharacter;
	auto addCharacterInput = w.ui->lineEdit_addCharacter;
	auto scriptEditor = w.ui->textEdit_scriptEditor;
	auto actionSaveAs = w.ui->actionSaveAs;
	auto actionExportPdf = w.ui->actionExportPdf;

	if (scriptEditor)
		new ScriptHighlighter(scriptEditor->document());

	auto characters = new entities::Characters(&a);
	if ( charactersListView)
	{
		charactersListView->setModel(characters);
	}

	if ( addCharacterButton && addCharacterInput )
	{
		QObject::connect(addCharacterButton, &QPushButton::clicked, [characters, addCharacterInput]() {
			if (! addCharacterInput->text().isEmpty()) {
				characters->add(addCharacterInput->text());
				addCharacterInput->clear();
			}
		});
	}

	if ( charactersListView && scriptEditor )
	{
		QObject::connect(charactersListView, &QListView::doubleClicked, [characters, scriptEditor] (QModelIndex index) {
			scriptEditor->append(QString("> %1:").arg(characters->data(index).toString()));
		});
	}

	if ( actionSaveAs && scriptEditor )
	{
		QObject::connect(actionSaveAs, &QAction::triggered, [scriptEditor]() {
			QString fileName = QFileDialog::getSaveFileName();
			if ( !fileName.isEmpty() )
			{
				QFile file(fileName);

				if ( !file.open(QIODevice::WriteOnly) )
				{
					QMessageBox::information(0, "Unable to open file", file.errorString());
					return;
				}
				QDataStream out(&file);
				out << scriptEditor->toPlainText().toUtf8();
			}
		});
	}

	if ( actionExportPdf && scriptEditor )
	{
		QObject::connect(actionExportPdf, &QAction::triggered, [scriptEditor]() {
			QString fileName = QFileDialog::getSaveFileName();
			if ( !fileName.isEmpty() )
			{
				if ( QFileInfo(fileName).suffix().isEmpty() )
				{
					fileName.append(".pdf");
				}

				QPrinter printer(QPrinter::PrinterResolution);
				printer.setOutputFormat(QPrinter::PdfFormat);
				printer.setPaperSize(QPrinter::A4);
				printer.setOutputFileName(fileName);
				printer.setFontEmbeddingEnabled(true);

				auto doc = scriptEditor->document();
				doc->print(&printer);

			}
		});
	}

	w.show();
	return a.exec();
}
