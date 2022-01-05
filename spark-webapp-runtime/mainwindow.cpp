#include "mainwindow.h"

#include <DWidgetUtil>
#include <DTitlebar>
#include <DMessageManager>
#include <DDesktopServices>

#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QCloseEvent>

#include "webengineview.h"

MainWindow::MainWindow(QString szTitle,
                       QString szUrl,
                       int nWidth,
                       int nHeight,
                       bool nTray,
                       bool nFullScreen,
                       bool nFixSize,
                       bool nHideButtons,
                       QDialog *dialog,
                       QWidget *parent)
    : DMainWindow(parent)
    , m_title(szTitle)
    , m_url(szUrl)
    , m_width(nWidth)
    , m_height(nHeight)
    , m_isTrayEnabled(nTray)
    , m_isFullScreen(nFullScreen)
    , m_isFixedSize(nFixSize)
    , m_isHideButton(nHideButtons)
    , m_widget(new Widget(m_url, this))
    , m_dialog(dynamic_cast<DAboutDialog *>(dialog))
    , m_tray(new QSystemTrayIcon(this))
    , btnBack(new DToolButton(titlebar()))
    , btnForward(new DToolButton(titlebar()))
    , btnRefresh(new DToolButton(titlebar()))
    , m_menu(new QMenu(titlebar()))
    , m_fullScreen(new QAction(tr("Full Screen"), this))
    , m_fixSize(new QAction(tr("Fix Size"), this))
    , m_hideButtons(new QAction(tr("Hide Buttons"), this))
    , m_clearCache(new QAction(tr("Clear Cache"), this))
    , t_menu(new QMenu(this))
    , t_show(new QAction(tr("Show MainWindow"), this))
    , t_about(new QAction(tr("About"), this))
    , t_exit(new QAction(tr("Exit"), this))
    , downloadMessage(new DFloatingMessage(DFloatingMessage::ResidentType, this))
    , downloadProgressWidget(new QWidget(downloadMessage))
    , progressBarLayout(new QHBoxLayout(downloadProgressWidget))
    , downloadProgressBar(new DProgressBar(downloadProgressWidget))
    , btnPause(new DPushButton(tr("Pause"), downloadProgressWidget))
    , btnResume(new DPushButton(tr("Resume"), downloadProgressWidget))
    , btnCancel(new DPushButton(tr("Cancel"), downloadProgressWidget))
    , isCanceled(false)
{
    initUI();
    initTrayIcon();
    initConnections();
}

MainWindow::~MainWindow()
{
    emit sigQuit();
    delete m_dialog;
}

void MainWindow::setIcon(QString szIconPath)
{
    if (QFileInfo(szIconPath).exists()) {
        titlebar()->setIcon(QIcon(szIconPath));
        setWindowIcon(QIcon(szIconPath));
        m_tray->setIcon(QIcon(szIconPath));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!m_fixSize->isChecked()) // 固定窗口大小时禁止全屏
    {
        if (event->key() == Qt::Key_F11) // 绑定键盘快捷键 F11
        {
            m_fullScreen->trigger();
            m_menu->update();
        }
    }
    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (this->isFullScreen()) {
        m_fullScreen->setChecked(true);
    } else {
        m_fullScreen->setChecked(false);
        if (!m_isFixedSize) {
            m_fixSize->setEnabled(true); // 命令行参数没有固定窗口大小时，窗口模式下允许手动选择固定窗口大小
        }
    }
    DMainWindow::resizeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!m_isTrayEnabled) {
        m_dialog->close(); // 不启用托盘时，关闭主窗口则关闭关于窗口
    }
    event->accept();
}

void MainWindow::initUI()
{
    // 初始化 MainWindow
    setCentralWidget(m_widget);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    resize(m_width, m_height);

    moveToCenter(this);

    setWindowIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    initTitleBar();
    initDownloadProgressBar();

    fixSize();
    fullScreen();
}

void MainWindow::initTitleBar()
{
    titlebar()->setTitle(m_title);
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
    m_fullScreen->setChecked(m_isFullScreen);
    m_fullScreen->setDisabled(m_isFixedSize); // NOTE: 固定窗口大小时禁用全屏模式，避免标题栏按钮显示问题
    m_fixSize->setCheckable(true);
    m_fixSize->setChecked(m_isFixedSize);
    m_fixSize->setDisabled(m_isFixedSize);
    m_hideButtons->setCheckable(true);
    m_hideButtons->setChecked(m_isHideButton);
    m_hideButtons->setDisabled(m_isHideButton);

    // 命令行设置参数后 GUI 中隐藏对应选项
    if (!m_isFixedSize) {
        m_menu->addAction(m_fullScreen);
        m_menu->addAction(m_fixSize);
    }

    if (!m_isHideButton) {
        m_menu->addAction(m_hideButtons);
    }

    if (m_menu->actions().size() > 0) {
        m_menu->addSeparator();
        m_menu->addAction(m_clearCache);
    }

    titlebar()->setMenu(m_menu);
    titlebar()->setAutoHideOnFullscreen(true);
}

void MainWindow::initDownloadProgressBar()
{
    // 初始化 DownloadProgressBar
    downloadProgressBar->setFixedSize(250, 8);
    btnPause->setFixedSize(80, 32);
    btnResume->setFixedSize(80, 32);
    btnCancel->setFixedSize(80, 32);

    progressBarLayout->setMargin(0);
    progressBarLayout->setSpacing(0);
    progressBarLayout->setAlignment(Qt::AlignCenter);
    progressBarLayout->addWidget(downloadProgressBar);
    progressBarLayout->addSpacing(10);
    progressBarLayout->addWidget(btnPause);
    progressBarLayout->addWidget(btnResume);
    progressBarLayout->addWidget(btnCancel);

    downloadMessage->setIcon(QIcon::fromTheme("deepin-download").pixmap(64, 64));
    downloadMessage->setWidget(downloadProgressWidget);
    downloadMessage->hide();
}

void MainWindow::initTrayIcon()
{
    // 初始化 TrayIcon
    t_menu->addAction(t_show);
    t_menu->addAction(t_about);
    t_menu->addAction(t_exit);
    m_tray->setContextMenu(t_menu);
    m_tray->setToolTip(m_title);
    m_tray->setIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    if (m_isTrayEnabled) {
        m_tray->show(); // 启用托盘时显示
    }
}

void MainWindow::initConnections()
{
    connect(btnBack, &DToolButton::clicked, this, [&]() {
        m_widget->goBack();
    });
    connect(btnForward, &DToolButton::clicked, this, [&]() {
        m_widget->goForward();
    });
    connect(btnRefresh, &DToolButton::clicked, this, [&]() {
        m_widget->refresh();
    });

    connect(m_fullScreen, &QAction::triggered, this, [=]() {
        fullScreen();
    });
    connect(m_fixSize, &QAction::triggered, this, [=]() {
        fixSize();
    });
    connect(m_hideButtons, &QAction::triggered, this, [=]() {
        hideButtons();
    });
    connect(m_clearCache, &QAction::triggered, this, [=]() {
        clearCache();
    });

    connect(t_show, &QAction::triggered, this, [=]() {
        this->activateWindow();
        fixSize();
    });
    connect(t_about, &QAction::triggered, this, [=]() {
        m_dialog->activateWindow();
        m_dialog->show();
    });
    connect(t_exit, &QAction::triggered, this, [=]() {
        exit(0);
    });
    connect(m_tray, &QSystemTrayIcon::activated, this, &MainWindow::on_trayIconActivated);

    connect(m_widget->getPage()->profile(), &QWebEngineProfile::downloadRequested, this, &MainWindow::on_downloadStart);

    connect(m_widget->getPage(), &QWebEnginePage::windowCloseRequested, this, [=]() {
        this->close();
    });
}

void MainWindow::fullScreen()
{
    if (m_fullScreen->isChecked()) {
        m_fixSize->setChecked(false);
        m_fixSize->setDisabled(true);
        m_menu->update();
        showFullScreen();
        // DMessageManager::instance()->sendMessage(this, QIcon::fromTheme("dialog-information").pixmap(64, 64), QString(tr("%1Fullscreen Mode")).arg("    "));
    } else {
        if (!m_isFixedSize) {
            m_fixSize->setDisabled(false); // 命令行参数没有固定窗口大小时，窗口模式下允许手动选择固定窗口大小
        }
        m_menu->update();
        showNormal();
        // DMessageManager::instance()->sendMessage(this, QIcon::fromTheme("dialog-information").pixmap(64, 64), QString(tr("%1Windowed Mode")).arg("    "));
    }
}

void MainWindow::fixSize()
{
    if (m_fixSize->isChecked()) {
        m_fullScreen->setChecked(false);
        m_fullScreen->setDisabled(true);
        m_menu->update();
        setFixedSize(this->size());
        // BUG: 启用托盘图标后，若手动选择固定窗口大小，并且关闭窗口，再次打开时会丢失最大化按钮，且无法恢复。
    } else {
        m_fullScreen->setDisabled(false);
        m_menu->update();
        setMinimumSize(m_width, m_height);
        setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    }

    fullScreen();
}

void MainWindow::hideButtons()
{
    if (m_hideButtons->isChecked()) {
        btnBack->hide();
        btnForward->hide();
        btnRefresh->hide();
    } else {
        btnBack->show();
        btnForward->show();
        btnRefresh->show();
    }
}

void MainWindow::clearCache()
{
    // 清除缓存文件夹并刷新页面
    QDir dir(QDir::homePath() + "/.local/share/" + ORGANIZATION_NAME + "/" + APPLICATION_NAME);
    if (dir.exists()) {
        dir.removeRecursively();
    }

    emit btnRefresh->clicked();
}

QString MainWindow::saveAs(QString fileName)
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath() + "/Downloads/" + fileName);
    if (!saveFile.isEmpty()) {
        // 判断上层目录是否可写入
        if (QFileInfo(QFileInfo(saveFile).absolutePath()).isWritable()) {
            return saveFile;
        } else {
            return saveAs(fileName);
        }
    }
    return nullptr;
}

void MainWindow::on_trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    /* 响应托盘点击事件 */
    case QSystemTrayIcon::Trigger:
        this->activateWindow();
        fixSize();
        break;
    default:
        break;
    }
}

void MainWindow::on_downloadStart(QWebEngineDownloadItem *item)
{
    // 尝试加锁互斥量，禁止同时下载多个文件
    if (mutex.tryLock()) {
        QString fileName = QFileInfo(item->path()).fileName();
        QString filePath = saveAs(fileName);
        if (filePath.isEmpty()) {
            mutex.unlock();
            return;
        }
        item->setPath(filePath);
        filePath = QFileInfo(item->path()).absoluteFilePath();

        connect(item, &QWebEngineDownloadItem::downloadProgress, this, &MainWindow::on_downloadProgress);
        connect(item, &QWebEngineDownloadItem::finished, this, [=]() {
            on_downloadFinish(filePath);
        });

        connect(btnPause, &DPushButton::clicked, this, [=]() {
            on_downloadPause(item);
        });
        connect(btnResume, &DPushButton::clicked, this, [=]() {
            on_downloadResume(item);
        });
        connect(btnCancel, &DPushButton::clicked, this, [=]() {
            on_downloadCancel(item);
        });

        DFloatingMessage *message = new DFloatingMessage(DFloatingMessage::TransientType);
        message->setIcon(QIcon::fromTheme("dialog-information").pixmap(64, 64));
        message->setMessage(QString(tr("%1Start downloading %2")).arg("    ").arg(fileName));
        DMessageManager::instance()->sendMessage(this, message);

        item->accept();

        // 重置 DownloadProgressBar 状态
        isCanceled = false;
        btnResume->hide();
        btnPause->show();
        this->downloadMessage->show(); // 上一次下载完成后隐藏了进度条，这里要重新显示
    } else {
        DMessageManager::instance()->sendMessage(this, QIcon::fromTheme("dialog-cancel").pixmap(64, 64), QString(tr("%1Wait for previous download to complete!")).arg("    "));
    }
}

void MainWindow::on_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int value = int(double(bytesReceived) / double(bytesTotal) * 100.0);
    downloadProgressBar->setValue(value);

    downloadMessage->setMessage("    " + QString::number(value) + "%");

    DMessageManager::instance()->sendMessage(this, downloadMessage);
}

void MainWindow::on_downloadFinish(QString filePath)
{
    mutex.unlock(); // 解锁互斥量，允许下载新文件

    downloadMessage->hide();

    // 下载完成显示提示信息
    if (!isCanceled) {
        DPushButton *button = new DPushButton(tr("Open"));

        DFloatingMessage *message = new DFloatingMessage(DFloatingMessage::ResidentType);
        message->setIcon(QIcon::fromTheme("dialog-ok").pixmap(64, 64));
        message->setMessage(QString("    %1 %2 %3").arg(QFileInfo(filePath).fileName()).arg(tr("download finished.")).arg(tr("Show in file manager?")));
        message->setWidget(button);
        DMessageManager::instance()->sendMessage(this, message);

        connect(button, &DPushButton::clicked, this, [=]() {
            DDesktopServices::showFileItem(filePath);
            message->hide();
        });
    }
}

void MainWindow::on_downloadPause(QWebEngineDownloadItem *item)
{
    item->pause();

    downloadMessage->setIcon(QIcon::fromTheme("package-download-failed").pixmap(64, 64));
    btnResume->show();
    btnPause->hide();
}

void MainWindow::on_downloadResume(QWebEngineDownloadItem *item)
{
    item->resume();

    downloadMessage->setIcon(QIcon::fromTheme("deepin-download").pixmap(64, 64));
    btnResume->hide();
    btnPause->show();
}

void MainWindow::on_downloadCancel(QWebEngineDownloadItem *item)
{
    isCanceled = true; // 取消下载
    item->cancel();

    mutex.unlock();

    downloadMessage->hide();
    DMessageManager::instance()->sendMessage(this, QIcon::fromTheme("dialog-error").pixmap(64, 64), QString(tr("%1Download canceled!")).arg("    "));
}
