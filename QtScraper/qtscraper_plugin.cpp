#include "qtscraper_plugin.h"

#include "qwebscraper.h"

#include <qqml.h>

void QtScraperPlugin::registerTypes(const char *uri)
{
    // @uri com.ifba.edu.scraping
    qmlRegisterType<QWebScraper>(uri, 1, 0, "QWebScraper");
    qmlRegisterUncreatableType<QWebScraperStatus>(uri, 1, 0, "QWebScraperStatus", "Not creatable as it is an enum type");
    qmlRegisterUncreatableType<QWebScraperParser>(uri, 1, 0, "QWebScraperParser", "Not creatable as it is an enum type");
}

