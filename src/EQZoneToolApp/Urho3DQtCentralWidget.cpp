
#include "pch.h"
#include "Urho3DQtCentralWidget.h"

Urho3DQtCentralWidget::Urho3DQtCentralWidget(QWidget* parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
	// Set mininimum size
	setMinimumSize(1024, 768);

	// Disable update
	setUpdatesEnabled(false);

	// Force set focus
	setFocus(Qt::OtherFocusReason);
}

void Urho3DQtCentralWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);

	// Set focus when mouse press
	setFocus(Qt::MouseFocusReason);
}
