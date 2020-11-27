#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QLayout>

#include <DSpinner>

#include "webenginepage.h"

DWIDGET_USE_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString szUrl = nullptr, QWidget *parent = nullptr);
    ~Widget();

    void goBack();
    void goForward();
    void refresh();

private:
    QWebEngineView *m_webEngineView;
    WebEnginePage *m_page;
    QString m_szUrl;
    DSpinner *m_spinner;

    QHBoxLayout *main;

    void clearLayout(QLayout *layout);

private slots:
    void on_loadStarted();
    void on_loadFinished();
};

#endif // WIDGET_H
