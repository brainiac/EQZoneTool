
#include "pch.h"
#include "Urho3DWidget.h"

#include <QApplication>
#include <SDL/SDL.h>

//------------------------------------------------------------------------------------------------------
// key utilities to convert Qt key to SDL key
//------------------------------------------------------------------------------------------------------
static QMap<Qt::Key, SDL_Keycode> __keymap;
static void         __initKeyMap();
static Uint16       __convertQtKeyModifierToSDL(Qt::KeyboardModifiers qtKeyModifiers);
static SDL_Keycode  __convertQtKeyToSDL(Qt::Key qtKey);

//------------------------------------------------------------------------------------------------------
// map keys Qt/SDL
//------------------------------------------------------------------------------------------------------
void __initKeyMap()
{
	__keymap[Qt::Key_unknown] = SDLK_UNKNOWN;
	__keymap[Qt::Key_Escape] = SDLK_ESCAPE;
	__keymap[Qt::Key_Tab] = SDLK_TAB;
	__keymap[Qt::Key_Backspace] = SDLK_BACKSPACE;
	__keymap[Qt::Key_Return] = SDLK_RETURN;
	__keymap[Qt::Key_Enter] = SDLK_KP_ENTER;
	__keymap[Qt::Key_Insert] = SDLK_INSERT;
	__keymap[Qt::Key_Delete] = SDLK_DELETE;
	__keymap[Qt::Key_Pause] = SDLK_PAUSE;
	__keymap[Qt::Key_Print] = SDLK_PRINTSCREEN;
	__keymap[Qt::Key_SysReq] = SDLK_SYSREQ;
	__keymap[Qt::Key_Home] = SDLK_HOME;
	__keymap[Qt::Key_End] = SDLK_END;
	__keymap[Qt::Key_Left] = SDLK_LEFT;
	__keymap[Qt::Key_Right] = SDLK_RIGHT;
	__keymap[Qt::Key_Up] = SDLK_UP;
	__keymap[Qt::Key_Down] = SDLK_DOWN;
	__keymap[Qt::Key_PageUp] = SDLK_PAGEUP;
	__keymap[Qt::Key_PageDown] = SDLK_PAGEDOWN;
	__keymap[Qt::Key_Shift] = SDLK_LSHIFT;
	__keymap[Qt::Key_Control] = SDLK_LCTRL;
	__keymap[Qt::Key_Alt] = SDLK_LALT;
	__keymap[Qt::Key_CapsLock] = SDLK_CAPSLOCK;
	__keymap[Qt::Key_NumLock] = SDLK_NUMLOCKCLEAR;
	__keymap[Qt::Key_ScrollLock] = SDLK_SCROLLLOCK;
	__keymap[Qt::Key_F1] = SDLK_F1;
	__keymap[Qt::Key_F2] = SDLK_F2;
	__keymap[Qt::Key_F3] = SDLK_F3;
	__keymap[Qt::Key_F4] = SDLK_F4;
	__keymap[Qt::Key_F5] = SDLK_F5;
	__keymap[Qt::Key_F6] = SDLK_F6;
	__keymap[Qt::Key_F7] = SDLK_F7;
	__keymap[Qt::Key_F8] = SDLK_F8;
	__keymap[Qt::Key_F9] = SDLK_F9;
	__keymap[Qt::Key_F10] = SDLK_F10;
	__keymap[Qt::Key_F11] = SDLK_F11;
	__keymap[Qt::Key_F12] = SDLK_F12;
	__keymap[Qt::Key_F13] = SDLK_F13;
	__keymap[Qt::Key_F14] = SDLK_F14;
	__keymap[Qt::Key_F15] = SDLK_F15;
	__keymap[Qt::Key_Menu] = SDLK_MENU;
	__keymap[Qt::Key_Help] = SDLK_HELP;

	// A-Z
	for (int key = 'A'; key <= 'Z'; key++)
		__keymap[Qt::Key(key)] = key + 32;

	// 0-9
	for (int key = '0'; key <= '9'; key++)
		__keymap[Qt::Key(key)] = key;
}

//------------------------------------------------------------------------------------------------------
// get SDL key from Qt key
//------------------------------------------------------------------------------------------------------
SDL_Keycode __convertQtKeyToSDL(Qt::Key qtKey)
{
	SDL_Keycode sldKey = __keymap.value(Qt::Key(qtKey));

	if (sldKey == 0)
		printf("Warning: Key %d not mapped", qtKey);

	return sldKey;
}

//------------------------------------------------------------------------------------------------------
// get SDL key modifier from Qt key modifier
//------------------------------------------------------------------------------------------------------
Uint16 __convertQtKeyModifierToSDL(Qt::KeyboardModifiers qtKeyModifiers)
{
	Uint16 sdlModifiers = KMOD_NONE;

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


QUrhoWidget::QUrhoWidget(Context* context) :
	Urho3D::Object(context),
	engine_(nullptr),
	scene_(nullptr)
{
	// init key map Qt => SDL
	__initKeyMap();

	// start timer to refresh engine at each frame
	timerId = startTimer(0);
}

void QUrhoWidget::timerEvent(QTimerEvent *e)
{
	QWidget::timerEvent(e);
	QApplication::processEvents();

	engine_->RunFrame();
}

void QUrhoWidget::resizeEvent(QResizeEvent* e)
{
	QWidget::resizeEvent(e);

	if (engine_->IsInitialized())
	{
		int width = e->size().width();
		int height = e->size().height();

		Graphics* graphics = GetSubsystem<Graphics>();

		//SDL_Window * win = (SDL_Window*)graphics->GetWindow();
		//SDL_SetWindowSize(win, width, height);
	}
}

void QUrhoWidget::keyPressEvent(QKeyEvent* e)
{
	QWidget::keyPressEvent(e);

	// Transmit key press event to SDL
	SDL_Event sdlEvent;
	sdlEvent.type = SDL_KEYDOWN;
	sdlEvent.key.keysym.sym = __convertQtKeyToSDL(Qt::Key(e->key()));
	sdlEvent.key.keysym.mod = __convertQtKeyModifierToSDL(e->modifiers());
	SDL_PushEvent(&sdlEvent);
}

void QUrhoWidget::Setup()
{
	VariantMap engineParameters_;
	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = 1280;
	engineParameters_["WindowHeight"] = 720;
	engineParameters_["WindowResizable"] = true;
	engineParameters_["ExternalWindow"] = (void*)winId();

	engine_ = new Engine(context_);
	engine_->Initialize(engineParameters_);
}

void QUrhoWidget::Start()
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	scene_ = new Scene(context_);
	scene_->CreateComponent<Octree>();

	Node* planeNode = scene_->CreateChild("Plane");
	planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
	StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
	planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

	Node* lightNode = scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);

	const unsigned NUM_OBJECTS = 25;
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

	cameraNode_ = scene_->CreateChild("Camera");
	Camera* camera = cameraNode_->CreateComponent<Camera>();
	cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));


	Renderer* renderer = GetSubsystem<Renderer>();
	Viewport* viewport = new Viewport(context_, scene_, camera);
	renderer->SetViewport(0, viewport);

	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(QUrhoWidget, HandleKeyDown));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(QUrhoWidget, HandleUpdate));
}

void QUrhoWidget::Stop()
{
	// exit urho
	engine_->Exit();

	// close widget
	close();
}


void QUrhoWidget::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	using namespace Update;
	float timeStep = eventData["TimeStep"].GetFloat();

	cameraNode_->Rotate(Quaternion(30.0f * timeStep, Vector3(0, 1, 0)));

}

void QUrhoWidget::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	int key = eventData[P_KEY].GetInt();

	if (key == KEY_ESCAPE)
	{
		Stop();
	}
}
