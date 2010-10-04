#ifndef __PLUGIN_INTERFACE_H__
#define __PLUGIN_INTERFACE_H__

#include <QObject>

class ParserInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~ParserInterface() { };

	virtual void parse(const QString &localPath, const QString &filename) = 0;
};

class PluginInterface
{
public:
	struct MenuItem {
		QString label;
		int key;
		bool checked;
		MenuItem(QString const& l = "", int k = 0, bool c = false): label(l), key(k), checked(c) {}
	};
	struct MenuGroup {
		QString label;
		QList<MenuItem> items;
	};

	virtual ~PluginInterface() {};

	virtual QStringList mimeTypes() = 0;

	virtual QWidget *reader(QWidget *parent = 0) = 0;

	virtual ParserInterface *parser() = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.shortcovers.nickel.PluginInterface")

#endif
