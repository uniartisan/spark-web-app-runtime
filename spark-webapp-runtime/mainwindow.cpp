#include "mainwindow.h"

#include <DMainWindow>
#include <DTitlebar>

#include <QLayout>
#include <QFileInfo>
#include <QCloseEvent>

MainWindow::MainWindow(QString szTitle,
                       QString szUrl,
                       int nWidth,
                       int nHeight,
                       DAboutDialog *dialog,
                       QWidget *parent)
    : DMainWindow(parent)
    , m_widget(new Widget(szUrl))
    , m_dialog(dialog)
    , btnBackward(new DToolButton(titlebar()))
    , btnForward(new DToolButton(titlebar()))
    , btnRefresh(new DToolButton(titlebar()))
    , m_menu(new QMenu)
    , m_fixSize(new QAction(tr("Fix Size")))
    , m_hideButtons(new QAction(tr("Hide Buttons")))
    , m_width(nWidth)
    , m_height(nHeight)
{
    //  setFixedSize(nWidth, nHeight);
    //  应 shenmo 要求改成设置最小尺寸试试效果
    setMinimumSize(m_width, m_height);

    setCentralWidget(m_widget);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    setWindowIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    titlebar()->setTitle(szTitle);
    titlebar()->setIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    btnBackward->setIcon(QIcon(":/images/go-previous-24.svg"));
    btnBackward->setIconSize(QSize(36, 36));
    btnForward->setIcon(QIcon(":/images/go-next-24.svg"));
    btnForward->setIconSize(QSize(36, 36));
    btnRefresh->setIcon(QIcon(":/images/view-refresh.svg"));
    btnRefresh->setIconSize(QSize(36, 36));

    titlebar()->addWidget(btnBackward, Qt::AlignLeft);
    titlebar()->addWidget(btnForward, Qt::AlignLeft);
    titlebar()->addWidget(btnRefresh, Qt::AlignLeft);

    m_fixSize->setCheckable(true);
    m_fixSize->setChecked(false);
    m_hideButtons->setCheckable(true);
    m_hideButtons->setChecked(false);
    m_menu->addAction(m_fixSize);
    m_menu->addAction(m_hideButtons);
    titlebar()->setMenu(m_menu);

    connect(btnBackward, &DToolButton::clicked, this, [&]()
    {
        if (m_widget)
        {
            m_widget->goBack();
        }
    });
    connect(btnForward, &DToolButton::clicked, this, [&]()
    {
        if (m_widget)
        {
            m_widget->goForward();
        }
    });
    connect(btnRefresh, &DToolButton::clicked, this, [&]()
    {
        if (m_widget)
        {
            m_widget->refresh();
        }
    });

    connect(m_fixSize, &QAction::triggered, this, [=]()
    {
        fixSize();
    });
    connect(m_hideButtons, &QAction::triggered, this, [=]()
    {
        hideButtons();
    });
}

MainWindow::~MainWindow()
{
    emit sigQuit();
    if (m_widget)
    {
        delete m_widget;
        m_widget = nullptr;
    }
    if (m_dialog)
    {
        delete m_dialog;
        m_dialog = nullptr;
    }
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

void MainWindow::fixSize()
{
    if(m_fixSize->isChecked())
    {
        setFixedSize(this->width(), this->height());    // setFixedSize() 等同于同时设置 MaximumSize 和 MinimumSize
        resize(this->width(), this->height());

        /*
         * 尝试固定窗口大小后禁用最大化按钮，但是取消勾选后无法恢复
         * titlebar()->setDisableFlags(Qt::WindowMaximizeButtonHint);
         */
    }
    else
    {
        setMinimumSize(m_width, m_height);
        setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
        resize(this->width(), this->height());
    }
}

void MainWindow::hideButtons()
{
    if(m_hideButtons->isChecked())
    {
        titlebar()->removeWidget(btnBackward);
        titlebar()->removeWidget(btnForward);
        titlebar()->removeWidget(btnRefresh);
    }
    else
    {
        titlebar()->addWidget(btnBackward, Qt::AlignLeft);
        titlebar()->addWidget(btnForward, Qt::AlignLeft);
        titlebar()->addWidget(btnRefresh, Qt::AlignLeft);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_dialog->close();
    event->accept();
}
