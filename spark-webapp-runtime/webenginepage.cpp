#include "webenginepage.h"

#include <QDir>
#include <QDebug>

WebEnginePage::WebEnginePage(QObject *parent)
    : QWebEnginePage(parent)
    , m_profile(this->profile())
    , process(new QProcess)
{
    connect(m_profile, &QWebEngineProfile::downloadRequested, this, &WebEnginePage::on_download);
}

QWebEnginePage *WebEnginePage::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type)
    WebEnginePage *page = new WebEnginePage();
    connect(page, &QWebEnginePage::urlChanged, this, &WebEnginePage::on_urlChanged);
    return page;
}

void WebEnginePage::on_urlChanged(const QUrl url)
{
    setUrl(url);
    sender()->deleteLater();
}

void WebEnginePage::on_download(QWebEngineDownloadItem *item)

{
    connect(item, &QWebEngineDownloadItem::downloadProgress, this, &WebEnginePage::on_downloadProgress);
    connect(item, &QWebEngineDownloadItem::finished, this, &WebEnginePage::on_downloadFinished);

    //  无法指定下载位置，原因未知；默认位置为 ~/Downloads
    //  item->setPath(QDir::homePath() + "/Downloads/");
    item->accept();

    process->start(QString("notify-send -a spark-webapp-runtime -t 5000 \"%1\"").arg(tr("Start downloading...")));
}

void WebEnginePage::on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "Download Progress:\tbytesReceived: " << bytesReceived << "\tbytesTotal: " << bytesTotal;
}

void WebEnginePage::on_downloadFinished()
{
    process->start(QString("notify-send -a spark-webapp-runtime -t 5000 \"%1\"").arg(tr("Download finished!")));
}
