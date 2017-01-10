
#include "pch.h"
#include "Urho3DQtMainWindow.h"
#include "Urho3DQtCentralWidget.h"

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

Urho3DQtMainWindow::Urho3DQtMainWindow(QWidget *parent, Qt::WindowFlags flags) :
	QMainWindow(parent, flags)
{
	// Set window icon
	setWindowIcon(QIcon(":/Images/qt-logo.png"));

	// Set central widget
	setCentralWidget(new Urho3DQtCentralWidget(this));

	// Create menu
	QMenu* menu = menuBar()->addMenu(tr("&File"));

	// Create menu actions
	QAction* newAction = menu->addAction(QIcon(":/Images/filenew.png"), tr("New..."));
	QAction* openAction = menu->addAction(QIcon(":/Images/fileopen.png"), tr("Open ..."));
	QAction* saveAction = menu->addAction(QIcon(":/Images/filesave.png"), tr("Save ..."));

	// Create tool bar
	QToolBar* toolBar = addToolBar(tr("File"));
	toolBar->setIconSize(QSize(32, 32));

	toolBar->addAction(newAction);
	toolBar->addAction(openAction);
	toolBar->addAction(saveAction);

	// Show window
	show();
}

Urho3DQtMainWindow::~Urho3DQtMainWindow()
{
}
