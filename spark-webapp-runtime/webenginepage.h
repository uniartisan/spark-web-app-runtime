#ifndef WEBENGINEPAGE_H
#define WEBENGINEPAGE_H

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QObject>
#include <QProcess>

class WebEnginePage : public QWebEnginePage
{
public:
    WebEnginePage(QObject *parent = nullptr);

protected:
    QWebEnginePage *createWindow(WebWindowType type) override;

private:
    QWebEngineProfile *m_profile;

    QProcess *process;

private slots:
    void on_urlChanged(const QUrl url);
    void on_download(QWebEngineDownloadItem *item);
    void on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void on_downloadFinished();

};

#endif // WEBENGINEPAGE_H
