#include "widget.h"

Widget::Widget(QString szUrl, QWidget *parent)
    : QWidget(parent)
    , m_webEngineView(new WebEngineView(this))
    , m_spinner(new DSpinner(this))
    , mainLayout(new QStackedLayout(this))
    , m_szUrl(szUrl)
{
    m_spinner->setFixedSize(96, 96);

    m_webEngineView->setObjectName(QStringLiteral("webEngineView"));
    m_webEngineView->setEnabled(true);
    m_webEngineView->setAutoFillBackground(false);
    m_webEngineView->setZoomFactor(1.0);

    QWebEnginePage *page = new QWebEnginePage(m_webEngineView);
    m_webEngineView->setPage(page);

    m_webEngineView->setUrl(QUrl(nullptr));
    if (!m_szUrl.isEmpty()) {
        m_webEngineView->setUrl(QUrl(m_szUrl));
    }

    QWidget *spinnerWidget = new QWidget(this);
    QHBoxLayout *spinnerLayout = new QHBoxLayout(spinnerWidget);
    spinnerLayout->setMargin(0);
    spinnerLayout->setSpacing(0);
    spinnerLayout->setAlignment(Qt::AlignCenter);
    spinnerLayout->addStretch();
    spinnerLayout->addWidget(m_spinner);
    spinnerLayout->addStretch();

    mainLayout->addWidget(spinnerWidget);
    mainLayout->addWidget(m_webEngineView);

    connect(m_webEngineView, &QWebEngineView::loadStarted, this, &Widget::on_loadStarted);
    connect(m_webEngineView, &QWebEngineView::loadFinished, this, &Widget::on_loadFinished);
}

Widget::~Widget()
{
    delete m_webEngineView;
    delete m_spinner;
}

QWebEnginePage *Widget::getPage()
{
    return this->m_webEngineView->page();
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

void Widget::on_loadStarted()
{
    mainLayout->setCurrentIndex(0);
    m_spinner->start();
}

void Widget::on_loadFinished()
{
    m_spinner->stop();
    mainLayout->setCurrentIndex(1);
}
