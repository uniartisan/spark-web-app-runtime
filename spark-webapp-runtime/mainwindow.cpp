#include "mainwindow.h"

#include <DMainWindow>
#include <DWidgetUtil>
#include <DTitlebar>
#include <DMessageManager>
#include <DPushButton>

#include <QLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QCloseEvent>
#include <QDebug>

#include "webenginepage.h"

MainWindow::MainWindow(QString szTitle,
                       QString szUrl,
                       int nWidth,
                       int nHeight,
                       bool nFullScreen,
                       bool nFixSize,
                       bool nHideButtons,
                       DAboutDialog *dialog,
                       QWidget *parent)
    : DMainWindow(parent)
    , m_widget(new Widget(szUrl))
    , m_dialog(dialog)
    , btnBack(new DToolButton(titlebar()))
    , btnForward(new DToolButton(titlebar()))
    , btnRefresh(new DToolButton(titlebar()))
    , m_menu(new QMenu)
    , m_fullScreen(new QAction(tr("Full Screen")))
    , m_fixSize(new QAction(tr("Fix Size")))
    , m_hideButtons(new QAction(tr("Hide Buttons")))
    , bar(new DProgressBar)
    , message(new DFloatingMessage(DFloatingMessage::ResidentType))
    , process(new QProcess)
    , m_width(nWidth)
    , m_height(nHeight)
{
    setCentralWidget(m_widget);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    resize(m_width, m_height);

    moveToCenter(this);

    setWindowIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    titlebar()->setTitle(szTitle);
    titlebar()->setIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    btnBack->setIcon(QIcon(":/images/go-previous-24.svg"));
    btnBack->setIconSize(QSize(36, 36));
    btnForward->setIcon(QIcon(":/images/go-next-24.svg"));
    btnForward->setIconSize(QSize(36, 36));
    btnRefresh->setIcon(QIcon(":/images/view-refresh.svg"));
    btnRefresh->setIconSize(QSize(36, 36));

    titlebar()->addWidget(btnBack, Qt::AlignLeft);
    titlebar()->addWidget(btnForward, Qt::AlignLeft);
    titlebar()->addWidget(btnRefresh, Qt::AlignLeft);

    m_fullScreen->setCheckable(true);
    m_fullScreen->setChecked(nFullScreen);
    m_fullScreen->setDisabled(nFixSize);    //  固定窗口大小时禁用全屏模式，避免标题栏按钮 BUG
    m_fixSize->setCheckable(true);
    m_fixSize->setChecked(nFixSize);
    m_fixSize->setDisabled(nFixSize);
    m_hideButtons->setCheckable(true);
    m_hideButtons->setChecked(nHideButtons);
    m_hideButtons->setDisabled(nHideButtons);
    m_menu->addAction(m_fullScreen);
    m_menu->addAction(m_fixSize);
    m_menu->addAction(m_hideButtons);
    titlebar()->setMenu(m_menu);

    titlebar()->setAutoHideOnFullscreen(true);

    fullScreen();
    fixSize();
    hideButtons();

    connect(btnBack, &DToolButton::clicked, this, [&]()
    {
        m_widget->goBack();
    });
    connect(btnForward, &DToolButton::clicked, this, [&]()
    {
        m_widget->goForward();
    });
    connect(btnRefresh, &DToolButton::clicked, this, [&]()
    {
        m_widget->refresh();
    });

    connect(m_fullScreen, &QAction::triggered, this, [=]()
    {
        fullScreen();
    });
    connect(m_fixSize, &QAction::triggered, this, [=]()
    {
        fixSize();
    });
    connect(m_hideButtons, &QAction::triggered, this, [=]()
    {
        hideButtons();
    });

    connect(m_widget->getPage()->profile(), &QWebEngineProfile::downloadRequested, this, &MainWindow::on_downloadStart);
}

MainWindow::~MainWindow()
{
    emit sigQuit();
    delete m_widget;
    delete m_dialog;
}

void MainWindow::setIcon(QString szIconPath)
{
    QFileInfo fi(szIconPath);
    if (fi.exists())
    {
        titlebar()->setIcon(QIcon(szIconPath));
        setWindowIcon(QIcon(szIconPath));
        qDebug() << szIconPath << "is Set!";
    }
    else
    {
        qDebug() << szIconPath << "is Not Exists!";
    }
}

void MainWindow::fullScreen()
{
    if(m_fullScreen->isChecked())
    {
        showFullScreen();
        DMessageManager::instance()->sendMessage(this, QIcon::fromTheme("dialog-information").pixmap(64, 64), QString(tr("%1Fullscreen Mode")).arg("    "));
    }
    else
    {
        showNormal();
        DMessageManager::instance()->sendMessage(this, QIcon::fromTheme("dialog-information").pixmap(64, 64), QString(tr("%1Windowed Mode")).arg("    "));
    }
}

void MainWindow::fixSize()
{
    if(m_fixSize->isChecked())
    {
        setFixedSize(this->width(), this->height());
    }
    else
    {
        setMinimumSize(m_width, m_height);
        setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    }
}

void MainWindow::hideButtons()
{
    if(m_hideButtons->isChecked())
    {
        btnBack->hide();
        btnForward->hide();
        btnRefresh->hide();
    }
    else
    {
        btnBack->show();
        btnForward->show();
        btnRefresh->show();
    }
}

QString MainWindow::saveAs(QString fileName)
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath() + "/Downloads/" + fileName);
    if(!saveFile.isEmpty())
    {
        //  判断上层目录是否可写入
        if(QFileInfo(QFileInfo(saveFile).absolutePath()).permissions().testFlag(QFile::WriteUser))
        {
            return saveFile;
        }
        else
        {
            saveAs(fileName);
        }
    }
    return nullptr;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_fixSize->isEnabled())
    {
        if(event->key() == Qt::Key_F11)
        {
            m_fullScreen->trigger();
            m_menu->update();
        }
        event->accept();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_dialog->close();
    event->accept();
}

void MainWindow::on_downloadStart(QWebEngineDownloadItem *item)

{
    QString fileName = QFileInfo(item->path()).fileName();
    if(saveAs(fileName).isEmpty())
    {
        return;
    }
    item->setPath(saveAs(fileName));
    QString filePath = QFileInfo(item->path()).absoluteFilePath();

    connect(item, &QWebEngineDownloadItem::downloadProgress, this, &MainWindow::on_downloadProgress);
    connect(item, &QWebEngineDownloadItem::finished, this, [=]
    {
        on_downloadFinish(filePath);
    });

    DFloatingMessage *message = new DFloatingMessage(DFloatingMessage::TransientType);
    message->setIcon(QIcon::fromTheme("dialog-information").pixmap(64, 64));
    message->setMessage(QString(tr("%1Start downloading %2")).arg("    ").arg(fileName));
    DMessageManager::instance()->sendMessage(this, message);

    item->accept();
}

void MainWindow::on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int value = int(double(bytesReceived) / double(bytesTotal) * 100.0);

    bar->setFixedSize(250, 8);
    bar->setValue(value);
    message->setIcon(QIcon::fromTheme("deepin-download").pixmap(64, 64));
    message->setMessage("    " + QString::number(value) + "%");
    message->setWidget(bar);
    DMessageManager::instance()->sendMessage(this, message);
}

void MainWindow::on_downloadFinish(QString filePath)
{
    message->hide();

    DPushButton *button = new DPushButton(tr("Open"));

    DFloatingMessage *message = new DFloatingMessage(DFloatingMessage::ResidentType);
    message->setIcon(QIcon::fromTheme("dialog-ok").pixmap(64, 64));
    message->setMessage(QString("    %1 %2 %3").arg(QFileInfo(filePath).fileName()).arg(tr("download finished.")).arg(tr("Show in file manager?")));
    message->setWidget(button);
    DMessageManager::instance()->sendMessage(this, message);

    connect(button, &DPushButton::clicked, this, [=]
    {
        process->start("dde-file-manager --show-item " + filePath);
        message->hide();
    });
}
