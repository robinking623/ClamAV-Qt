#include "clamav_gui.h"
#include <QTranslator>
#include <QApplication>
#include <QtNetwork/QTcpServer>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include "schedulescanobject.h"
#include "setupfilehandler.h"

#define PORT_NUM 55000

int main(int argc, char *argv[])
{
QTranslator translator(0);
QTcpServer server;
QStringList parameters;
QStringList databaseDirSet;
QString databaseDir;
QString lang;
QString rc;

    QApplication a(argc, argv);
    lang = QLocale::system().name();

    if (argc > 1) rc=(QString)argv[1];

    if (rc == "--scan"){
        setupFileHandler * setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");
        databaseDirSet = setupFile->getSectionValue("Directories","LoadSupportedDBFiles").split("|");
        if (databaseDirSet.at(0) == "checked") databaseDir = databaseDirSet.at(1);

        parameters << "-r";
        if (databaseDir != "") parameters << "--database=" + databaseDir;
        for (int i = 2; i < argc; i++){
            parameters << (QString)argv[i];
        }

        scheduleScanObject * scanObject = new scheduleScanObject(0,"Direct Scan",parameters);

        scanObject->setWindowTitle("Direct Scan-Job");
        scanObject->setWindowIcon(QIcon(":/icons/icons/media.png"));
        scanObject->setModal(true);
        scanObject->exec();
        delete scanObject;
    } else {
        if (rc != "--force"){
            if( !server.listen( QHostAddress::LocalHost, PORT_NUM ) ) {
                qDebug() << "Application already running!";
                exit(0);
            }
        }

        QString filename = "clamav-gui-" + lang + ".qm";

        if (QFile::exists("/usr/share/clamav-gui/" + filename)){
            translator.load(filename,"/usr/share/clamav-gui");
        }

        a.installTranslator(&translator);

        clamav_gui w;
        w.show();

        return a.exec();
    }
}
