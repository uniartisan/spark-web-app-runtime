#ifndef WEBENGINEPAGE_H
#define WEBENGINEPAGE_H

#include <QWebEnginePage>
#include <QObject>

class WebEnginePage : public QWebEnginePage
{
public:
    WebEnginePage(QObject *parent = nullptr);

protected:
    QWebEnginePage *createWindow(WebWindowType type) override;

private slots:
    void on_urlChanged(const QUrl url);

};

#endif // WEBENGINEPAGE_H
