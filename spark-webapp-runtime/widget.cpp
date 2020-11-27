#include "widget.h"

Widget::Widget(QString szUrl, QWidget *parent)
    : QWidget(parent)
    , m_webEngineView(new QWebEngineView)
    , m_page(new WebEnginePage)
    , m_szUrl(szUrl)
    , m_spinner(new DSpinner)
    , main(new QHBoxLayout)
{
    m_spinner->setFixedSize(96, 96);

    setLayout(main);

    m_webEngineView->setObjectName(QStringLiteral("webEngineView"));
    m_webEngineView->setEnabled(true);
    m_webEngineView->setAutoFillBackground(false);
    m_webEngineView->setZoomFactor(1);
    m_webEngineView->setPage(m_page);

    m_webEngineView->setUrl(QUrl(nullptr));
    if (!m_szUrl.isEmpty())
    {
        m_webEngineView->setUrl(QUrl(m_szUrl));
    }

    connect(m_webEngineView, &QWebEngineView::loadStarted, this, &Widget::on_loadStarted);
    connect(m_webEngineView, &QWebEngineView::loadFinished, this, &Widget::on_loadFinished);
}

Widget::~Widget()
{
}

void Widget::goBack()
{
    m_webEngineView->back();
}

void Widget::goForward()
{
    m_webEngineView->forward();
}

void Widget::refresh()
{
    m_webEngineView->reload();
}

void Widget::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item;
    }
}

void Widget::on_loadStarted()
{
    clearLayout(main);

    main->addStretch();
    main->addWidget(m_spinner);
    main->addStretch();

    m_spinner->start();
}

void Widget::on_loadFinished()
{
    m_spinner->stop();
    clearLayout(main);

    main->addWidget(m_webEngineView);
}
