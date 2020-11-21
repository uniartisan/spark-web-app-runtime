#include "mainwindow.h"

#include <DMainWindow>
#include <DTitlebar>
#include <DToolButton>

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
{
    //  setFixedSize(nWidth, nHeight);
    //  应 shenmo 要求改成设置最小尺寸试试效果
    setMinimumSize(nWidth, nHeight);

    setCentralWidget(m_widget);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    setWindowIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    titlebar()->setTitle(szTitle);
    titlebar()->setIcon(QIcon(":/images/spark-webapp-runtime.svg"));

    DToolButton *btnBackward = new DToolButton(titlebar());
    btnBackward->setIcon(QIcon(":/images/go-previous-24.svg"));
    btnBackward->setIconSize(QSize(36, 36));
    DToolButton *btnForward = new DToolButton(titlebar());
    btnForward->setIcon(QIcon(":/images/go-next-24.svg"));
    btnForward->setIconSize(QSize(36, 36));
    DToolButton *btnRefresh = new DToolButton(titlebar());
    btnRefresh->setIcon(QIcon(":/images/view-refresh.svg"));
    btnRefresh->setIconSize(QSize(36, 36));

    titlebar()->addWidget(btnBackward, Qt::AlignLeft);
    titlebar()->addWidget(btnForward, Qt::AlignLeft);
    titlebar()->addWidget(btnRefresh, Qt::AlignLeft);

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_dialog->close();
    event->accept();
}
