
#pragma once

#include <Urho3D/Core/Context.h>
#include <QMainWindow>

// Urho3D Qt main window class.
class Urho3DQtMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	// Construct.
	Urho3DQtMainWindow(Urho3D::Context* context, QWidget* parent = 0, Qt::WindowFlags flags = 0);

	// Destruct.
	virtual ~Urho3DQtMainWindow();
};
