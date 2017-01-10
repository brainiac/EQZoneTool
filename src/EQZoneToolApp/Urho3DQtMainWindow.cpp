
#include "pch.h"
#include "Urho3DQtMainWindow.h"
#include "Urho3DQtCentralWidget.h"
#include "Urho3DWidget.h"

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

Urho3DQtMainWindow::Urho3DQtMainWindow(QWidget *parent, Qt::WindowFlags flags) :
	QMainWindow(parent, flags)
{
	// Set window icon
	setWindowIcon(QIcon(":/Icons/AppIcon32.png"));

	Urho3D::Context* context = new Urho3D::Context();

	QUrhoWidget* urhoWidget = new QUrhoWidget(context);
	urhoWidget->Setup();
	urhoWidget->Start();
	urhoWidget->resize(800, 600);
	urhoWidget->show();

	// Set central widget
	setCentralWidget(urhoWidget);

	// Create menu
	QMenu* menu = menuBar()->addMenu(tr("&File"));

	// Create menu actions
	QAction* newAction = menu->addAction(QIcon(":/Images/filenew.png"), tr("New..."));
	QAction* openAction = menu->addAction(QIcon(":/Images/fileopen.png"), tr("Open ..."));
	QAction* saveAction = menu->addAction(QIcon(":/Images/filesave.png"), tr("Save ..."));

	// Show window
	show();
}

Urho3DQtMainWindow::~Urho3DQtMainWindow()
{
}
