#ifndef QTSCRAPER_PLUGIN_H
#define QTSCRAPER_PLUGIN_H

#include <QQmlExtensionPlugin>

class QtScraperPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // QTSCRAPER_PLUGIN_H
