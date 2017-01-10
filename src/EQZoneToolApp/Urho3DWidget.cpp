
#include "pch.h"
#include "Urho3DWidget.h"

#include <QApplication>
#include <SDL/SDL.h>

//------------------------------------------------------------------------------------------------------
// key utilities to convert Qt key to SDL key
//------------------------------------------------------------------------------------------------------
static QMap<Qt::Key, SDL_Keycode> g_keymap;
static void         InitKeyMap();
static uint16_t     ConvertQtKeyModifierToSDL(Qt::KeyboardModifiers qtKeyModifiers);
static SDL_Keycode  ConvertQtKeyToSDL(Qt::Key qtKey);

//------------------------------------------------------------------------------------------------------
// map keys Qt/SDL
//------------------------------------------------------------------------------------------------------
void InitKeyMap()
{
	g_keymap[Qt::Key_unknown] = SDLK_UNKNOWN;
	g_keymap[Qt::Key_Escape] = SDLK_ESCAPE;
	g_keymap[Qt::Key_Tab] = SDLK_TAB;
	g_keymap[Qt::Key_Backspace] = SDLK_BACKSPACE;
	g_keymap[Qt::Key_Return] = SDLK_RETURN;
	g_keymap[Qt::Key_Enter] = SDLK_KP_ENTER;
	g_keymap[Qt::Key_Insert] = SDLK_INSERT;
	g_keymap[Qt::Key_Delete] = SDLK_DELETE;
	g_keymap[Qt::Key_Pause] = SDLK_PAUSE;
	g_keymap[Qt::Key_Print] = SDLK_PRINTSCREEN;
	g_keymap[Qt::Key_SysReq] = SDLK_SYSREQ;
	g_keymap[Qt::Key_Home] = SDLK_HOME;
	g_keymap[Qt::Key_End] = SDLK_END;
	g_keymap[Qt::Key_Left] = SDLK_LEFT;
	g_keymap[Qt::Key_Right] = SDLK_RIGHT;
	g_keymap[Qt::Key_Up] = SDLK_UP;
	g_keymap[Qt::Key_Down] = SDLK_DOWN;
	g_keymap[Qt::Key_PageUp] = SDLK_PAGEUP;
	g_keymap[Qt::Key_PageDown] = SDLK_PAGEDOWN;
	g_keymap[Qt::Key_Shift] = SDLK_LSHIFT;
	g_keymap[Qt::Key_Control] = SDLK_LCTRL;
	g_keymap[Qt::Key_Alt] = SDLK_LALT;
	g_keymap[Qt::Key_CapsLock] = SDLK_CAPSLOCK;
	g_keymap[Qt::Key_NumLock] = SDLK_NUMLOCKCLEAR;
	g_keymap[Qt::Key_ScrollLock] = SDLK_SCROLLLOCK;
	g_keymap[Qt::Key_F1] = SDLK_F1;
	g_keymap[Qt::Key_F2] = SDLK_F2;
	g_keymap[Qt::Key_F3] = SDLK_F3;
	g_keymap[Qt::Key_F4] = SDLK_F4;
	g_keymap[Qt::Key_F5] = SDLK_F5;
	g_keymap[Qt::Key_F6] = SDLK_F6;
	g_keymap[Qt::Key_F7] = SDLK_F7;
	g_keymap[Qt::Key_F8] = SDLK_F8;
	g_keymap[Qt::Key_F9] = SDLK_F9;
	g_keymap[Qt::Key_F10] = SDLK_F10;
	g_keymap[Qt::Key_F11] = SDLK_F11;
	g_keymap[Qt::Key_F12] = SDLK_F12;
	g_keymap[Qt::Key_F13] = SDLK_F13;
	g_keymap[Qt::Key_F14] = SDLK_F14;
	g_keymap[Qt::Key_F15] = SDLK_F15;
	g_keymap[Qt::Key_Menu] = SDLK_MENU;
	g_keymap[Qt::Key_Help] = SDLK_HELP;

	// A-Z
	for (int key = 'A'; key <= 'Z'; key++)
		g_keymap[Qt::Key(key)] = key + 32;

	// 0-9
	for (int key = '0'; key <= '9'; key++)
		g_keymap[Qt::Key(key)] = key;
}

//------------------------------------------------------------------------------------------------------
// get SDL key from Qt key
//------------------------------------------------------------------------------------------------------
SDL_Keycode ConvertQtKeyToSDL(Qt::Key qtKey)
{
	SDL_Keycode sldKey = g_keymap.value(Qt::Key(qtKey));

	if (sldKey == 0)
		printf("Warning: Key %d not mapped", qtKey);

	return sldKey;
}

//------------------------------------------------------------------------------------------------------
// get SDL key modifier from Qt key modifier
//------------------------------------------------------------------------------------------------------
uint16_t ConvertQtKeyModifierToSDL(Qt::KeyboardModifiers qtKeyModifiers)
{
	uint16_t sdlModifiers = KMOD_NONE;

	if (qtKeyModifiers.testFlag(Qt::ShiftModifier))
		sdlModifiers |= KMOD_LSHIFT | KMOD_RSHIFT;
	if (qtKeyModifiers.testFlag(Qt::ControlModifier))
		sdlModifiers |= KMOD_LCTRL | KMOD_RCTRL;
	if (qtKeyModifiers.testFlag(Qt::AltModifier))
		sdlModifiers |= KMOD_LALT | KMOD_RALT;

	return sdlModifiers;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

Urho3DWidget::Urho3DWidget(Context* context) 
	: Urho3D::Object(context)
{
	// init key map Qt => SDL
	InitKeyMap();

	// start timer to refresh engine at each frame
	timerId = startTimer(0);

#if 1
	// Set mininimum size
	setMinimumSize(1024, 768);

	// Disable update
	setUpdatesEnabled(false);

	// Force set focus
	setFocus(Qt::OtherFocusReason);
#endif
}

void Urho3DWidget::timerEvent(QTimerEvent *e)
{
	QWidget::timerEvent(e);

	//QApplication::processEvents();
	//m_engine->RunFrame();
}

void Urho3DWidget::resizeEvent(QResizeEvent* e)
{
	QWidget::resizeEvent(e);

#if 0
	if (m_engine->IsInitialized())
	{
		int width = e->size().width();
		int height = e->size().height();

		Graphics* graphics = GetSubsystem<Graphics>();

		SDL_Window * win = (SDL_Window*)graphics->GetWindow();
		SDL_SetWindowSize(win, width, height);
	}
#endif
}

void Urho3DWidget::keyPressEvent(QKeyEvent* e)
{
	QWidget::keyPressEvent(e);

	// Transmit key press event to SDL
	SDL_Event sdlEvent;
	sdlEvent.type = SDL_KEYDOWN;
	sdlEvent.key.keysym.sym = ConvertQtKeyToSDL(Qt::Key(e->key()));
	sdlEvent.key.keysym.mod = ConvertQtKeyModifierToSDL(e->modifiers());
	SDL_PushEvent(&sdlEvent);
}

void Urho3DWidget::keyReleaseEvent(QKeyEvent* e)
{
	QWidget::keyReleaseEvent(e);

	// Transmit key release event to SDL
	SDL_Event sdlEvent;
	sdlEvent.type = SDL_KEYUP;
	sdlEvent.key.keysym.sym = ConvertQtKeyToSDL(Qt::Key(e->key()));
	sdlEvent.key.keysym.mod = ConvertQtKeyModifierToSDL(e->modifiers());
	SDL_PushEvent(&sdlEvent);
}

void Urho3DWidget::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);

	// Set focus when mouse press
	setFocus(Qt::MouseFocusReason);

	Input* input = GetSubsystem<Input>();
	input->SetMouseGrabbed(true);
	input->SetMouseGrabbed(MM_RELATIVE);
}

void Urho3DWidget::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);

	Input* input = GetSubsystem<Input>();
	input->SetMouseGrabbed(false);
	input->SetMouseMode(MM_ABSOLUTE);
}

void Urho3DWidget::Setup()
{
	VariantMap engineParameters_;
	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = 1280;
	engineParameters_["WindowHeight"] = 720;
	engineParameters_["WindowResizable"] = true;
	engineParameters_["ExternalWindow"] = (void*)winId();

	m_engine = new Engine(context_);
	m_engine->Initialize(engineParameters_);
}

void Urho3DWidget::Start()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	m_scene = new Scene(context_);
	m_scene->CreateComponent<Octree>();

	Node* planeNode = m_scene->CreateChild("Plane");
	planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
	StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

	Node* lightNode = m_scene->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);

	const unsigned NUM_OBJECTS = 25;
	for (unsigned i = 0; i < NUM_OBJECTS; ++i)
	{
		Node* mushroomNode = m_scene->CreateChild("Mushroom");
		mushroomNode->SetPosition(Vector3(Random(90.0f) - 45.0f, 0.0f, Random(90.0f) - 45.0f));
		mushroomNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
		mushroomNode->SetScale(0.5f + Random(2.0f));
		StaticModel* mushroomObject = mushroomNode->CreateComponent<StaticModel>();
		mushroomObject->SetModel(cache->GetResource<Model>("Models/Mushroom.mdl"));
		mushroomObject->SetMaterial(cache->GetResource<Material>("Materials/Mushroom.xml"));
	}

	m_cameraNode = m_scene->CreateChild("Camera");
	Camera* camera = m_cameraNode->CreateComponent<Camera>();
	m_cameraNode->SetPosition(Vector3(0.0f, 5.0f, 0.0f));


	Renderer* renderer = GetSubsystem<Renderer>();
	Viewport* viewport = new Viewport(context_, m_scene, camera);
	renderer->SetViewport(0, viewport);

	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Urho3DWidget, HandleKeyDown));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Urho3DWidget, HandleUpdate));
}

void Urho3DWidget::Stop()
{
	// exit urho
	m_engine->Exit();

	// close widget
	close();
}


void Urho3DWidget::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;
	float timeStep = eventData["TimeStep"].GetFloat();

	m_cameraNode->Rotate(Quaternion(30.0f * timeStep, Vector3(0, 1, 0)));

}

void Urho3DWidget::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	int key = eventData[P_KEY].GetInt();

	if (key == KEY_ESCAPE)
	{
		Stop();
	}
}
