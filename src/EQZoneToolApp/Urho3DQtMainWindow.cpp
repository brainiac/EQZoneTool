
#include "pch.h"
#include "Urho3DQtMainWindow.h"
#include "Urho3DWidget.h"

#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

Urho3DQtMainWindow::Urho3DQtMainWindow(Urho3D::Context* context, QWidget* parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	// Set window icon
	setWindowIcon(QIcon(":/Icons/AppIcon32.png"));

	Urho3DWidget* urhoWidget = new Urho3DWidget(context);
	//urhoWidget->Setup();
	//urhoWidget->Start();
	//urhoWidget->resize(1024, 768);
	//urhoWidget->show();

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
