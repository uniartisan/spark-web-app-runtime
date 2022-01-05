#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QLayout>
#include <QStackedLayout>

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
    DSpinner *m_spinner;
    QStackedLayout *mainLayout;

    QString m_szUrl;

private slots:
    void on_loadStarted();
    void on_loadFinished();
};

#endif // WIDGET_H
