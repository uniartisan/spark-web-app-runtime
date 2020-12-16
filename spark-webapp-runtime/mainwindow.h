#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DAboutDialog>
#include <DToolButton>
#include <DProgressBar>
#include <DPushButton>
#include <DFloatingMessage>

#include <QSystemTrayIcon>
#include <QProcess>
#include <QMutex>

#include "widget.h"
#include "globaldefine.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString szTitle = DEFAULT_TITLE,
               QString szUrl = DEFAULT_URL,
               int nWidth = DEFAULT_WIDTH,
               int nHeight = DEFAULT_HEIGHT,
               bool tray = false,
               bool nFullScreen = false,
               bool nFixSize = false,
               bool nHideButtons = false,
               DAboutDialog *dialog = nullptr,
               QWidget *parent = nullptr);
    ~MainWindow();

    void setIcon(QString szIconPath);

private:
    Widget *m_widget;
    DAboutDialog *m_dialog;
    QSystemTrayIcon *m_tray;

    DToolButton *btnBack;
    DToolButton *btnForward;
    DToolButton *btnRefresh;

    QMenu *m_menu;
    QAction *m_fullScreen;
    QAction *m_fixSize;
    QAction *m_hideButtons;

    QMenu *t_menu;
    QAction *t_show;
    QAction *t_about;
    QAction *t_exit;

    QWidget *downloadProgressBar;
    DProgressBar *bar;
    DPushButton *pause;
    DPushButton *resume;
    DPushButton *cancel;
    QHBoxLayout *progress;
    DFloatingMessage *message;

    QMutex mutex;       // 通过 Mutex 互斥量禁止同时下载多个文件（使用简单的 bool 变量应该也可以实现该功能？）
    bool isCanceled;    // 判断是否为取消下载

    bool mtray, mFixSize;
    int m_width, m_height;

    void fullScreen();
    void fixSize();
    void hideButtons();

    QString saveAs(QString fileName);

    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void on_downloadStart(QWebEngineDownloadItem *item);
    void on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void on_downloadFinish(QString filePath);
    void on_downloadPause(QWebEngineDownloadItem *item);
    void on_downloadResume(QWebEngineDownloadItem *item);
    void on_downloadCancel(QWebEngineDownloadItem *item);

signals:
    void sigQuit();

};

#endif // MAINWINDOW_H
