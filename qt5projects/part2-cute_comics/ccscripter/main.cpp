#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QTextEdit>
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

	w.show();
	return a.exec();
}
