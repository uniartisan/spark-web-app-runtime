#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include <QWebEngineView>

class WebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit WebEngineView(QWidget *parent = nullptr);

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

private slots:
    void on_urlChanged(QUrl url);

};

#endif // WEBENGINEVIEW_H
