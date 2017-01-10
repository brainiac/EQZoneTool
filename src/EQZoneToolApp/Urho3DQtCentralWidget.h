
#pragma once

#include <QWidget.h>

class Urho3DQtCentralWidget : public QWidget
{
	Q_OBJECT

public:
	Urho3DQtCentralWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

private:
	virtual void mousePressEvent(QMouseEvent* event);
};
