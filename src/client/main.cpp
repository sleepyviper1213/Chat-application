#include "utilities/connectDatabase.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);
	connectToDatabase();

	QQmlApplicationEngine engine;

	engine.loadFromModule("ChatClient", "Main");
	if (engine.rootObjects().isEmpty()) return -1;

	return QGuiApplication::exec();
}
