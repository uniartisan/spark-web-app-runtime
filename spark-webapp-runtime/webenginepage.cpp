#include "webenginepage.h"

WebEnginePage::WebEnginePage(QObject *parent)
    : QWebEnginePage(parent)
{
}

QWebEnginePage *WebEnginePage::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type)
    WebEnginePage *page = new WebEnginePage;
    connect(page, &QWebEnginePage::urlChanged, this, &WebEnginePage::on_urlChanged);
    return page;
}

void WebEnginePage::on_urlChanged(const QUrl url)
{
    setUrl(url);
    sender()->deleteLater();
}
