#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DAboutDialog>
#include <DToolButton>

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
               bool nFixSize = false,
               bool nHideButtons = false,
               DAboutDialog *dialog = nullptr,
               QWidget *parent = nullptr);
    ~MainWindow();

    void setIcon(QString szIconPath);

private:
    Widget *m_widget;
    DAboutDialog *m_dialog;

    DToolButton *btnBack;
    DToolButton *btnForward;
    DToolButton *btnRefresh;

    QMenu *m_menu;
    QAction *m_fixSize;
    QAction *m_hideButtons;

    int m_width, m_height;

    void fixSize();
    void hideButtons();

    void closeEvent(QCloseEvent *event);

signals:
    void sigQuit();

};

#endif // MAINWINDOW_H
