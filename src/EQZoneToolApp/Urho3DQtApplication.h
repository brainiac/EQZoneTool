
#pragma once

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Core/Object.h>

#include <QObject>
#include <QApplication>

#include <memory>

namespace Urho3D
{
	class Context;
	class Engine;

	class Sprite;
	class Node;
	class Scene;
}

class Urho3DQtMainWindow;

using namespace Urho3D;

// Urho3D Qt application class.
class Urho3DQtApplication : public QApplication, public Object
{
	Q_OBJECT
	URHO3D_OBJECT(Urho3DQtApplication, Object)

public:
	// Construct.
	Urho3DQtApplication(int argc, char** argv);
	// Destruct.
	virtual ~Urho3DQtApplication();

	// Run.
	int Run();

	// Return context.
	Context* GetContext() const { return context_; }

private slots:
	// Timeout handler.
	void OnTimeout();

private:
	// Setup sample.
	void SetupSample();
	// Create logo.
	void CreateLogo();
	// Set custom window Title & Icon
	void SetWindowTitleAndIcon();
	// Create console and debug HUD.
	void CreateConsoleAndDebugHud();
	// Handle key down event to process key controls common to all samples.
	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	// Construct the scene content.
	void CreateScene();
	// Construct an instruction text to the UI.
	void CreateInstructions();
	// Set up a viewport for displaying the scene.
	void SetupViewport();
	// Read input and moves the camera.
	void MoveCamera(float timeStep);
	// Subscribe to application-wide logic update events.
	void SubscribeToEvents();
	// Handle the logic update event.
	void HandleUpdate(StringHash eventType, VariantMap& eventData);

private:
	// Engine.
	SharedPtr<Engine> m_engine;
	// Main window.
	std::unique_ptr<Urho3DQtMainWindow> m_mainWindow;

	// Logo sprite.
	SharedPtr<Sprite> m_logoSprite;
	// Scene.
	SharedPtr<Scene> m_scene;
	// Camera scene node.
	SharedPtr<Node> m_cameraNode;
	// Camera yaw angle.
	float m_yaw;
	// Camera pitch angle.
	float m_pitch;
};
