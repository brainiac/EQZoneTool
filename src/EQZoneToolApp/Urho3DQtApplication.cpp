
#include "pch.h"
#include "Urho3DQtApplication.h"
#include "Urho3DQtMainWindow.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include <SDL/SDL_keycode.h>

#include <QTimer>

Urho3DQtApplication::Urho3DQtApplication(int argc, char** argv) : QApplication(argc, argv), Object(new Context())
{
	yaw_ = 0.0f;
	pitch_ = 0.0f;
	mainWindow_ = new Urho3DQtMainWindow();
}

Urho3DQtApplication::~Urho3DQtApplication()
{
	delete mainWindow_;
}

int Urho3DQtApplication::Run()
{
	if (!mainWindow_)
		return -1;

	engine_ = new Engine(context_);

	VariantMap engineParameters;
	engineParameters["FrameLimiter"] = false;
	engineParameters["LogName"] = "Urho3DQtSample.log";
	QWidget* centralWidget = mainWindow_->centralWidget();
	engineParameters["ExternalWindow"] = (void*)centralWidget->winId();

	if (!engine_->Initialize(engineParameters))
		return -1;

	SetupSample();

	QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
	timer.start(1000 / 60);

	return QApplication::exec();
}

void Urho3DQtApplication::OnTimeout()
{
	if (engine_ && !engine_->IsExiting())
		engine_->RunFrame();
}

void Urho3DQtApplication::SetupSample()
{
	// Create logo
	CreateLogo();

	// Set custom window Title & Icon
	SetWindowTitleAndIcon();

	// Create console and debug HUD
	CreateConsoleAndDebugHud();

	// Subscribe key down event
	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Urho3DQtApplication, HandleKeyDown));

	// Create the scene content
	CreateScene();

	// Create the UI content
	CreateInstructions();

	// Setup the viewport for displaying the scene
	SetupViewport();

	// Hook up to the frame update events
	SubscribeToEvents();
}

void Urho3DQtApplication::CreateLogo()
{
	// Get logo texture
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Texture2D* logoTexture = cache->GetResource<Texture2D>("Textures/LogoLarge.png");
	if (!logoTexture)
		return;

	// Create logo sprite and add to the UI layout
	UI* ui = GetSubsystem<UI>();
	logoSprite_ = ui->GetRoot()->CreateChild<Sprite>();

	// Set logo sprite texture
	logoSprite_->SetTexture(logoTexture);

	int textureWidth = logoTexture->GetWidth();
	int textureHeight = logoTexture->GetHeight();

	// Set logo sprite scale
	logoSprite_->SetScale(256.0f / textureWidth);

	// Set logo sprite size
	logoSprite_->SetSize(textureWidth, textureHeight);

	// Set logo sprite hot spot
	logoSprite_->SetHotSpot(0, textureHeight);

	// Set logo sprite alignment
	logoSprite_->SetAlignment(HA_LEFT, VA_BOTTOM);

	// Make logo not fully opaque to show the scene underneath
	logoSprite_->SetOpacity(0.75f);

	// Set a low priority for the logo so that other UI elements can be drawn on top
	logoSprite_->SetPriority(-100);
}

void Urho3DQtApplication::SetWindowTitleAndIcon()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Graphics* graphics = GetSubsystem<Graphics>();
	Image* icon = cache->GetResource<Image>("Textures/LogoLarge.png");
	graphics->SetWindowIcon(icon);
	graphics->SetWindowTitle("Urho3D Sample");
}

void Urho3DQtApplication::CreateConsoleAndDebugHud()
{
	// Get default style
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

	// Create console
	Console* console = engine_->CreateConsole();
	console->SetDefaultStyle(xmlFile);

	// Create debug HUD.
	DebugHud* debugHud = engine_->CreateDebugHud();
	debugHud->SetDefaultStyle(xmlFile);
}

void Urho3DQtApplication::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;

	int key = eventData[P_KEY].GetInt();

	// Close console (if open) or exit when ESC is pressed
	if (key == SDLK_ESCAPE)
	{
		Console* console = GetSubsystem<Console>();
		if (console->IsVisible())
			console->SetVisible(false);
	}

	// Toggle console with F1
	else if (key == KEY_F1)
		GetSubsystem<Console>()->Toggle();

	// Toggle debug HUD with F2
	else if (key == KEY_F2)
		GetSubsystem<DebugHud>()->ToggleAll();

	// Common rendering quality controls, only when UI has no focused element
	else if (!GetSubsystem<UI>()->GetFocusElement())
	{
		Renderer* renderer = GetSubsystem<Renderer>();

		// Texture quality
		if (key == '1')
		{
			int quality = renderer->GetTextureQuality();
			++quality;
			if (quality > QUALITY_HIGH)
				quality = QUALITY_LOW;
			renderer->SetTextureQuality(quality);
		}

		// Material quality
		else if (key == '2')
		{
			int quality = renderer->GetMaterialQuality();
			++quality;
			if (quality > QUALITY_HIGH)
				quality = QUALITY_LOW;
			renderer->SetMaterialQuality(quality);
		}

		// Specular lighting
		else if (key == '3')
			renderer->SetSpecularLighting(!renderer->GetSpecularLighting());

		// Shadow rendering
		else if (key == '4')
			renderer->SetDrawShadows(!renderer->GetDrawShadows());

		// Shadow map resolution
		else if (key == '5')
		{
			int shadowMapSize = renderer->GetShadowMapSize();
			shadowMapSize *= 2;
			if (shadowMapSize > 2048)
				shadowMapSize = 512;
			renderer->SetShadowMapSize(shadowMapSize);
		}

		// Shadow depth and filtering quality
		else if (key == '6')
		{
			int quality = renderer->GetShadowQuality();
			++quality;
			if (quality > SHADOWQUALITY_PCF_24BIT)
				quality = SHADOWQUALITY_SIMPLE_16BIT;
			renderer->SetShadowQuality(static_cast<Urho3D::ShadowQuality>(quality));
		}

		// Occlusion culling
		else if (key == '7')
		{
			bool occlusion = renderer->GetMaxOccluderTriangles() > 0;
			occlusion = !occlusion;
			renderer->SetMaxOccluderTriangles(occlusion ? 5000 : 0);
		}

		// Instancing
		else if (key == '8')
			renderer->SetDynamicInstancing(!renderer->GetDynamicInstancing());

		// Take screenshot
		else if (key == '9')
		{
			Graphics* graphics = GetSubsystem<Graphics>();
			Image screenshot(context_);
			graphics->TakeScreenShot(screenshot);
			// Here we save in the Data folder with date and time appended
			screenshot.SavePNG(GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Screenshot_" +
				Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_') + ".png");
		}
	}
}

void Urho3DQtApplication::CreateScene()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	scene_ = new Scene(context_);

	// Create the Octree component to the scene. This is required before adding any drawable components, or else nothing will
	// show up. The default octree volume will be from (-1000, -1000, -1000) to (1000, 1000, 1000) in world coordinates; it
	// is also legal to place objects outside the volume but their visibility can then not be checked in a hierarchically
	// optimizing manner
	scene_->CreateComponent<Octree>();

	// Create a child scene node (at world origin) and a StaticModel component into it. Set the StaticModel to show a simple
	// plane mesh with a "stone" material. Note that naming the scene nodes is optional. Scale the scene node larger
	// (100 x 100 world units)
	Node* planeNode = scene_->CreateChild("Plane");
	planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
	StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

	// Create a directional light to the world so that we can see something. The light scene node's orientation controls the
	// light direction; we will use the SetDirection() function which calculates the orientation from a forward direction vector.
	// The light will use default settings (white light, no shadows)
	Node* lightNode = scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f)); // The direction vector does not need to be normalized
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);

	// Create more StaticModel objects to the scene, randomly positioned, rotated and scaled. For rotation, we construct a
	// quaternion from Euler angles where the Y angle (rotation about the Y axis) is randomized. The mushroom model contains
	// LOD levels, so the StaticModel component will automatically select the LOD level according to the view distance (you'll
	// see the model get simpler as it moves further away). Finally, rendering a large number of the same object with the
	// same material allows instancing to be used, if the GPU supports it. This reduces the amount of CPU work in rendering the
	// scene.
	const unsigned NUM_OBJECTS = 200;
	for (unsigned i = 0; i < NUM_OBJECTS; ++i)
	{
		Node* mushroomNode = scene_->CreateChild("Mushroom");
		mushroomNode->SetPosition(Vector3(Random(90.0f) - 45.0f, 0.0f, Random(90.0f) - 45.0f));
		mushroomNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
		mushroomNode->SetScale(0.5f + Random(2.0f));
		StaticModel* mushroomObject = mushroomNode->CreateComponent<StaticModel>();
		mushroomObject->SetModel(cache->GetResource<Model>("Models/Mushroom.mdl"));
		mushroomObject->SetMaterial(cache->GetResource<Material>("Materials/Mushroom.xml"));
	}

	// Create a scene node for the camera, which we will move around
	// The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
	cameraNode_ = scene_->CreateChild("Camera");
	cameraNode_->CreateComponent<Camera>();

	// Set an initial position for the camera scene node above the plane
	cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
}

void Urho3DQtApplication::CreateInstructions()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	UI* ui = GetSubsystem<UI>();

	// Construct new Text object, set string to display and font to use
	Text* instructionText = ui->GetRoot()->CreateChild<Text>();
	instructionText->SetText("Use WASD keys and mouse to move");
	instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

	// Position the text relative to the screen center
	instructionText->SetHorizontalAlignment(HA_CENTER);
	instructionText->SetVerticalAlignment(VA_CENTER);
	instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
}

void Urho3DQtApplication::SetupViewport()
{
	Renderer* renderer = GetSubsystem<Renderer>();

	// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen. We need to define the scene and the camera
	// at minimum. Additionally we could configure the viewport screen size and the rendering path (eg. forward / deferred) to
	// use, but now we just use full screen and default render path configured in the engine command line options
	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
	renderer->SetViewport(0, viewport);
}

void Urho3DQtApplication::MoveCamera(float timeStep)
{
	// Do not move if the UI has a focused element (the console)
	if (GetSubsystem<UI>()->GetFocusElement())
		return;

	Input* input = GetSubsystem<Input>();
	// Movement speed as world units per second
	const float MOVE_SPEED = 20.0f;
	// Mouse sensitivity as degrees per pixel
	const float MOUSE_SENSITIVITY = 0.1f;

	if (input->GetMouseButtonDown(1))
	{
		// Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
		IntVector2 mouseMove = input->GetMouseMove();
		yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
		pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
		pitch_ = Clamp(pitch_, -90.0f, 90.0f);
		// Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
		cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
	}

	// Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
	// Use the TranslateRelative() function to move relative to the node's orientation. Alternatively we could
	// multiply the desired direction with the node's orientation quaternion, and use just Translate()
	if (input->GetKeyDown('W'))
		cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
	if (input->GetKeyDown('S'))
		cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
	if (input->GetKeyDown('A'))
		cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
	if (input->GetKeyDown('D'))
		cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
}

void Urho3DQtApplication::SubscribeToEvents()
{
	// Subscribe HandleUpdate() function for processing update events
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Urho3DQtApplication, HandleUpdate));
}

void Urho3DQtApplication::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;

	// Take the frame time step, which is stored as a float
	float timeStep = eventData[P_TIMESTEP].GetFloat();

	// Move the camera, scale movement with time step
	MoveCamera(timeStep);
}
