#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DAboutDialog>
#include <DToolButton>
#include <DProgressBar>
#include <DFloatingMessage>

#include <QSystemTrayIcon>
#include <QProcess>

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

    DProgressBar *bar;
    DFloatingMessage *message;

    QProcess *process;

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

signals:
    void sigQuit();

};

#endif // MAINWINDOW_H
