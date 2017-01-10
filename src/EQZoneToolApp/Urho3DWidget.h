
#pragma once

#include <Urho3D/Urho3DAll.h>

#include <QWidget>
#include <QResizeEvent>

class Urho3DWidget : public QWidget, public Urho3D::Object
{
	Q_OBJECT
	URHO3D_OBJECT(Urho3DWidget, Object)

public:
	Urho3DWidget(Context* context);

	virtual void timerEvent(QTimerEvent* e) override;
	virtual void resizeEvent(QResizeEvent* e) override;
	virtual void keyPressEvent(QKeyEvent* e) override;
	virtual void keyReleaseEvent(QKeyEvent* e) override;

	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

	void Setup();
	void Start();
	void Stop();

	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleUpdate(StringHash eventType, VariantMap& eventData);

private:
	SharedPtr<Engine> m_engine;
	SharedPtr<Scene> m_scene;
	SharedPtr<Node> m_cameraNode;
	int timerId;
};
