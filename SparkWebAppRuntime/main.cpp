/*
 *
 * 如果你想学习更多有关知识，可以访问以下网址：
 * DTK相关文档地址：https://linuxdeepin.github.io/dtk
 * Deepin项目地址：https://github.com/linuxdeepin
 * 社区项目投递地址：https://gitee.com/deepin-opensource
*/
#include "mainwindow.h"
#include <DApplication>
#include <DWidgetUtil>
#include <DAboutDialog>
#include <DMainWindow>

#include <QCommandLineParser>
#include <QCommandLineOption>

#include "globaldefine.h"
#include <QTranslator>

#include <QFileInfo>
#include <QSettings>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    DAboutDialog dialog;
    a.loadTranslator();
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setApplicationName(DEFAULT_TITLE);
    a.setProductName(DEFAULT_TITLE);
    a.setAboutDialog(&dialog);

    //License
    dialog.setLicense(QObject::tr("This program is open source under GPLv3"));    // 本程序按GPL第三版开源

    //Title
    dialog.setWindowTitle(DEFAULT_TITLE);
    //descrition
    dialog.setProductName(QString("<span>%1</span>").arg(DEFAULT_TITLE));
    //Icons
    dialog.setProductIcon(QIcon(":/images/deepin-launcher.svg"));
    //Organization logo
    dialog.setCompanyLogo(QPixmap(":/images/Logo-Spark.png"));
    //about
    QString szDefaultDesc = QString("<span style=' font-size:12pt;font-weight:500;'>%1</span><br/>"
                                    "<a href='https://gitee.com/deepin-community-store'>https://gitee.com/deepin-community-store</a><br/>"
                                    "<span style=' font-size:12pt;'>%2</span>")
                            .arg(DEFAULT_TITLE)
                            .arg(QObject::tr("Presented By Spark developers # HadesStudio"));
    dialog.setDescription(szDefaultDesc);
    //Version
    dialog.setVersion(DApplication::buildVersion(QString("%1 %2").arg(QObject::tr("Version")).arg("1.0")));
    //Website name
    dialog.setWebsiteName("spark-app.store");
    //Website link
    dialog.setWebsiteLink("https://www.spark-app.store/");

    QCommandLineParser parser;

    parser.setApplicationDescription(QObject::tr("Description: %1").arg(DEFAULT_TITLE));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption optParser(QStringList() << "p" << "parser",
                                 QObject::tr("Enable CommandLineParser. Default is false."));
    parser.addOption(optParser);

    QCommandLineOption optTitle(QStringList() << "t" << "title",
                                QObject::tr("The Title of Application. Default is %1.").arg(DEFAULT_TITLE),
                                "title",
                                DEFAULT_TITLE);
    parser.addOption(optTitle);

    QCommandLineOption optUrl(QStringList() << "u" << "url",
                              QObject::tr("The target URL. Default is Blank."),
                              "url",
                              DEFAULT_URL);
    parser.addOption(optUrl);

    QCommandLineOption optWidth(QStringList() << "w" << "width",
                                QObject::tr("The Width of Application. Default is %1.").arg(DEFAULT_WIDTH),
                                "width",
                                QString::number(DEFAULT_WIDTH));
    parser.addOption(optWidth);

    QCommandLineOption optHeight(QStringList() << "H" << "height",
                                 QObject::tr("The Height of Application. Default is %1").arg(DEFAULT_HEIGHT),
                                 "height",
                                 QString::number(DEFAULT_HEIGHT));
    parser.addOption(optHeight);

    QCommandLineOption optIcon(QStringList() << "i" << "ico",
                               QObject::tr("The ICON of Application."),
                               "ico",
                               DEFAULT_ICON);
    parser.addOption(optIcon);

    QCommandLineOption optDesc(QStringList() << "d" << "desc",
                               QObject::tr("The Description of Application."),
                               "desc",
                               DEFAULT_DESC);
    parser.addOption(optDesc);

    QCommandLineOption optCfgFile(QStringList() << "c" << "cfg",
                                  QObject::tr("The Configuration file of Application."),
                                  "cfg",
                                  DEFAULT_CFG);
    parser.addOption(optCfgFile);

    parser.process(a);

    QString szTitle = DEFAULT_TITLE;
    QString szUrl = DEFAULT_URL;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    QString szIcon = DEFAULT_ICON;
    QString szDesc = DEFAULT_DESC;

    QString szCfgFile = DEFAULT_CFG;
    if (parser.isSet(optCfgFile))
    {
        szCfgFile = parser.value(optCfgFile);
        if (!szCfgFile.isEmpty())
        {
            QFileInfo fi(szCfgFile);
            if (fi.exists())
            {
                QSettings settings(szCfgFile, QSettings::IniFormat);
                szTitle = settings.value("SpartWebAppRuntime/Title", DEFAULT_TITLE).toString();
                szUrl = settings.value("SpartWebAppRuntime/URL", DEFAULT_TITLE).toString();
                width = settings.value("SpartWebAppRuntime/Width", DEFAULT_WIDTH).toUInt();
                height = settings.value("SpartWebAppRuntime/Height", DEFAULT_HEIGHT).toUInt();
                szIcon = settings.value("SpartWebAppRuntime/Ico", DEFAULT_ICON).toString();
                szDesc = QString("%1<br/><br/>%2")
                         .arg(settings.value("SpartWebAppRuntime/Desc", QString()).toString())
                         .arg(szDefaultDesc);
            }
        }
    }

    // 命令行级别优先, 覆盖配置文件的设置
    if (parser.isSet(optTitle))
    {
        szTitle = parser.value(optTitle);
    }
    if (parser.isSet(optUrl))
    {
        szUrl = parser.value(optUrl);
    }
    if (parser.isSet(optWidth))
    {
        width = parser.value(optWidth).toInt();
    }
    if (parser.isSet(optHeight))
    {
        height = parser.value(optHeight).toInt();
    }

    if (parser.isSet(optDesc))
    {
        szDesc = QString("%1<br/><br/>%2").arg(parser.value(optDesc))
                 .arg(szDefaultDesc);
    }

    if (!parser.isSet(optParser))
    {
        do
        {
            // 按照固定顺序级别最优先
            if (argc != 7)
            {
#if 0
                QMessageBox::information(nullptr, QObject::tr("Usage:"),
                                         QObject::tr("The first usage: \n"
                                                     "%1 %2\n"
                                                     "The second usage:\n"
                                                     "%1 %3")
                                         .arg(argv[0])
                                         .arg(QObject::tr("Title URL Width Height"))
                                         .arg(QObject::tr("-h to view parameter list.")));
                return 0;
#endif
                break;
            }

            szTitle = argv[1];
            szUrl = argv[2];
            width = QString(argv[3]).toInt();
            height = QString(argv[4]).toInt();
            szIcon = QString(argv[5]);
            szDesc = QString("%1<br/><br/>%2").arg(QString(argv[6]))
                     .arg(szDefaultDesc);;
        }
        while (false);
    }

    MainWindow w(szTitle, szUrl, width, height);

    if (parser.isSet(optIcon))
    {
        szIcon = parser.value(optIcon);
    }

    if (!szIcon.isEmpty())
    {
        dialog.setIcon(QIcon(szIcon));
        dialog.setProductIcon(QIcon(szIcon));
        w.setIcon(szIcon);
    }
    if (!szDesc.isEmpty())
    {
        dialog.setDescription(szDesc);
    }

    w.show();

    Dtk::Widget::moveToCenter(&w);
    return a.exec();
}
