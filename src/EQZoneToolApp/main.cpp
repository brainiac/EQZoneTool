
#include "pch.h"
#include "Urho3DQtApplication.h"
#include "Urho3DWidget.h"

int main(int argc, char *argv[])
{
	//Urho3DQtApplication app(argc, argv);
	//return app.Run();

	int ret;

	QApplication app(argc, argv);

	Urho3D::Context* context = new Urho3D::Context();

	QUrhoWidget* urhoWidget = new QUrhoWidget(context);
	urhoWidget->Setup();
	urhoWidget->Start();
	urhoWidget->resize(800, 600);
	urhoWidget->show();

	ret = app.exec();
}
