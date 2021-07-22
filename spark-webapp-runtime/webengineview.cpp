#include "webengineview.h"

WebEngineView::WebEngineView(QWidget *parent)
    : QWebEngineView(parent)
{

}

QWebEngineView *WebEngineView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type)

    QWebEngineView *view = new QWebEngineView;
    connect(view, &QWebEngineView::urlChanged, this, &WebEngineView::on_urlChanged);

    return view;
}

void WebEngineView::on_urlChanged(QUrl url)
{
    setUrl(url);
    sender()->deleteLater();
}
