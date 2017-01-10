
#pragma once

#include <Urho3D/Urho3DAll.h>

#include <QWidget>
#include <QResizeEvent>

class QUrhoWidget : public QWidget, public Urho3D::Object
{
	Q_OBJECT
	URHO3D_OBJECT(QUrhoWidget, Object)

private:
	SharedPtr<Engine> engine_;
	SharedPtr<Scene> scene_;
	SharedPtr<Node> cameraNode_;
	int timerId;

public:
	QUrhoWidget(Context* context);

	void timerEvent(QTimerEvent* e) override;
	void resizeEvent(QResizeEvent* e) override;
	void keyPressEvent(QKeyEvent* e) override;

	void Setup();
	void Start();
	void Stop();

	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleUpdate(StringHash eventType, VariantMap& eventData);
};
