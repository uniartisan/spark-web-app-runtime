#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QLayout>

#include <DSpinner>

#include "webengineview.h"

DWIDGET_USE_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString szUrl = nullptr, QWidget *parent = nullptr);
    ~Widget();

    QWebEnginePage *getPage();
    void goBack();
    void goForward();
    void refresh();

private:
    WebEngineView *m_webEngineView;
    QString m_szUrl;
    DSpinner *m_spinner;

    QHBoxLayout *main;

    void clearLayout(QLayout *layout);

private slots:
    void on_loadStarted();
    void on_loadFinished();
};

#endif // WIDGET_H
