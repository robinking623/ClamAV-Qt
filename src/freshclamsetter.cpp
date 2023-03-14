#include "freshclamsetter.h"
#include "ui_freshclamsetter.h"

freshclamsetter::freshclamsetter(QWidget *parent) : QWidget(parent), ui(new Ui::freshclamsetter)
{
QDir tempDir;
QStringList parameters;

    ui->setupUi(this);
    updateLogHighLighter = new highlighter(ui->logPlainText->document());
    freshclamLogHighLighter = new highlighter(ui->deamonLogText->document());
    setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");

    readSettings();
    updater = new QProcess(this);
    connect(updater,SIGNAL(readyReadStandardError()),this,SLOT(slot_updaterHasOutput()));
    connect(updater,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_updaterHasOutput()));
    connect(updater,SIGNAL(finished(int)),this,SLOT(slot_updaterFinished(int)));

    deamonCheckerProcess = new QProcess(this);
    connect(deamonCheckerProcess,SIGNAL(finished(int)),this,SLOT(slot_deamonCheckerProcessFinished()));
    connect(deamonCheckerProcess,SIGNAL(readyReadStandardError()),this,SLOT(slot_deamonCheckerProcessHasOutput()));
    connect(deamonCheckerProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_deamonCheckerProcessHasOutput()));
    parameters << "ax";

    pidFileWatcher = new QFileSystemWatcher(this);
    connect(pidFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(slot_fileSystemWatcherTriggered()));

    logFileWatcher = new QFileSystemWatcher(this);
    connect(logFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(slot_logFileWatcherTriggered()));

    updateLogFileWatcher = new QFileSystemWatcher(this);
    connect(updateLogFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(slot_updateFileWatcherTriggered()));

    if (tempDir.exists(QDir::homePath() + "/.clamav-gui/update.log") == true) {
        slot_updateFileWatcherTriggered();
        updateLogFileWatcher->addPath(QDir::homePath() + "/.clamav-gui/update.log");
    } else {
        QFile file(QDir::homePath() + "/.clamav-gui/update.log");
        if (file.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text)){
            QTextStream stream(&file);
            stream << "";
            file.close();
            slot_updateFileWatcherTriggered();
            updateLogFileWatcher->addPath(QDir::homePath() + "/.clamav-gui/update.log");
        }
    }

    deamonCheckerProcess->start("ps",parameters);
    setUpdaterInfo();
}

freshclamsetter::~freshclamsetter()
{
    delete ui;
}

void freshclamsetter::readSettings(){
QString value = setupFile->getSectionValue("Directories","LoadSupportedDBFiles");
    value = value.mid(value.indexOf("|") + 1);
    ui->dataBaseDirectoryLineEdit->setText(value);
    ui->updatesPerDaySpinBox->setValue(setupFile->getSectionIntValue("FreshClam","UpdatesPerDay"));
    ui->enableStatsCheckBox->setChecked(setupFile->getSectionBoolValue("FreshClam","EnableStats"));
    ui->statsIDLineEdit->setText(setupFile->getSectionValue("FreshClam","StatsID"));
    ui->dataBaseComboBox->setCurrentIndex(setupFile->getSectionIntValue("FreshClam","DataBaseToUpdate"));
}

void freshclamsetter::slot_writeSettings(){
    setupFile->setSectionValue("FreshClam","UpdatesPerDay",ui->updatesPerDaySpinBox->value());
    setupFile->setSectionValue("FreshClam","EnableStats",ui->enableStatsCheckBox->isChecked());
    setupFile->setSectionValue("FreshClam","StatsID",ui->statsIDLineEdit->text());
    setupFile->setSectionValue("FreshClam","DataBaseToUpdate",ui->dataBaseComboBox->currentIndex());
}

void freshclamsetter::slot_selectDataBaseDirectoryButtonClicked(){
QString rc = QFileDialog::getExistingDirectory(this,"Select Directory",QDir::homePath(),QFileDialog::ShowDirsOnly);

    if (rc != ""){
        ui->dataBaseDirectoryLineEdit->setText(rc);
        QString value = setupFile->getSectionValue("Directories","LoadSupportedDBFiles");
        QString checked = value.left(value.indexOf("|"));
        setupFile->setSectionValue("Directories","LoadSupportedDBFiles",checked + "|" + rc);
        emit dataBaseDirectoryChanged();
    }
}

void freshclamsetter::slot_updatesPerDaySpinBoxActivated(){
    slot_writeSettings();
}

void freshclamsetter::slot_updateNowButtonClicked(){
QStringList parameters;

    busyLabel = new progressDialog(this);
    busyLabel->setStyleSheet("background-color: rgba(192,192,192, 200);");
    busyLabel->setGeometry((this->width() - 300) / 2,(this->height() - 160) / 2,300,160);
    busyLabel->show();
    busyLabel->setText(tr("Update process startet ....."));
    setForm(false);
    parameters << "--datadir=" + ui->dataBaseDirectoryLineEdit->text();
    if (ui->dataBaseComboBox->currentIndex() > 0) parameters << "--update-db=" + ui->dataBaseComboBox->currentText();
    parameters << "-l" << QDir::homePath() + "/.clamav-gui/update.log";
    updater->start("freshclam",parameters);
    updateLogFileWatcher->removePath(QDir::homePath() + "/.clamav-gui/update.log");
    updateLogFileWatcher->addPath(QDir::homePath() + "/.clamav-gui/update.log");
}

void freshclamsetter::slot_statsIdLineEditActivated(){
    slot_writeSettings();
}

void freshclamsetter::slot_startStopDeamonButtonClicked(){
QStringList parameters;
QDir tempDir;

    if (pidFile == ""){
        if (logFile != "") logFileWatcher->removePath(logFile);
        pidFile = "/tmp/freshclam.pid";
        logFile = QDir::homePath() + "/.clamav-gui/freshclam.log";
        parameters << "-p" << "/tmp/freshclam.pid" << "-d" << "--datadir=" + ui->dataBaseDirectoryLineEdit->text();
        parameters << "--checks=" + QString::number(ui->updatesPerDaySpinBox->value());
        parameters << "-l" << logFile;
        QProcess::execute("freshclam",parameters);
        pidFileWatcher->addPath(pidFile);
        logFileWatcher->addPath(logFile);
        setupFile->setSectionValue("Freshclam","Started",true);
    } else {
        QFile tempFile(pidFile);
        tempFile.open(QIODevice::ReadOnly);
        QTextStream stream(&tempFile);
        QString pidString = stream.readLine();
        parameters << "-sigterm" << pidString;
        QProcess::execute("kill",parameters);
        setupFile->setSectionValue("Freshclam","Started",false);
    }
    if ((pidFile != "") && (tempDir.exists(pidFile) == true)){
        ui->startStopDeamonButton->setText(tr("Deamon running - stop deamon"));
        ui->startStopDeamonButton->setStyleSheet("background:green");
        ui->startStopDeamonButton->setIcon(QIcon(":/icons/icons/Clam.png"));
        pidFileWatcher->addPath(pidFile);
    } else {
        pidFile = "";
        ui->startStopDeamonButton->setText(tr("Deamon not running - start deamon"));
        ui->startStopDeamonButton->setStyleSheet("background:red");
        ui->startStopDeamonButton->setIcon(QIcon(":/icons/icons/Clam.png"));

    }
}

void freshclamsetter::slot_enableStatsCheckBoxActivated(){
    slot_writeSettings();
}

void freshclamsetter::slot_dataBaseComboBoxActivated(){
    slot_writeSettings();
}

void freshclamsetter::slot_dataBaseDirectoryChanged(){
QString value = setupFile->getSectionValue("Directories","LoadSupportedDBFiles");
    value = value.mid(value.indexOf("|") + 1);
    ui->dataBaseDirectoryLineEdit->setText(value);
}

void freshclamsetter::slot_updaterFinished(int rc){
    delete busyLabel;

    if (rc == 0) {
        emit setBallonMessage(0,tr("INFO"),tr("Update-Process finished"));
        setUpdaterInfo();
    } else {
        emit setBallonMessage(1,tr("WARNING"),tr("Update-Process failed!\nRead log-messages for possible reason."));
    }
    setForm(true);
}

void freshclamsetter::slot_deamonCheckerProcessFinished(){
QString line;
QDir tempDir;

    pidFile = pidFile + deamonCheckerProcess->readAllStandardError();
    pidFile = pidFile + deamonCheckerProcess->readAllStandardOutput();
    if (pidFile.indexOf("freshclam") > -1){
        pidFile = pidFile.mid(pidFile.indexOf("freshclam"));
        logFile = pidFile;
        if (pidFile.indexOf("--pid=") == -1){
            pidFile = pidFile.mid(pidFile.indexOf("-p") + 3);
            pidFile = pidFile.left(pidFile.indexOf(" "));
        } else{
            pidFile = pidFile.mid(pidFile.indexOf("--pid=") + 6);
            pidFile = pidFile.left(pidFile.indexOf(" "));
        }
        if (logFile.indexOf("--log=") == -1){
            logFile = logFile.mid(logFile.indexOf("-l") + 3);
            logFile = logFile.left(logFile.indexOf("\n"));
            logFile = logFile.left(logFile.indexOf(" "));
        } else{
            logFile = logFile.mid(logFile.indexOf("--log=") + 6);
            logFile = logFile.left(logFile.indexOf("\n"));
            logFile = logFile.left(pidFile.indexOf(" "));
        }
    } else {
        pidFile = "";
    }

    if ((pidFile != "") && (tempDir.exists(pidFile) == true)){
        ui->startStopDeamonButton->setText(tr("Deamon running - stop deamon"));
        ui->startStopDeamonButton->setStyleSheet("background:green");
        ui->startStopDeamonButton->setIcon(QIcon(":/icons/icons/Clam.png"));
        pidFileWatcher->addPath(pidFile);
    } else {
        ui->startStopDeamonButton->setText(tr("Deamon not running - start deamon"));
        ui->startStopDeamonButton->setStyleSheet("background:red");
        ui->startStopDeamonButton->setIcon(QIcon(":/icons/icons/freshclam.png"));
        if (setupFile->getSectionBoolValue("Freshclam","Started") == true){
            slot_startStopDeamonButtonClicked();
        }
    }

    if ((logFile != "") && (tempDir.exists(logFile) == true)){
        logFileWatcher->addPath(logFile);
        slot_logFileWatcherTriggered();
    } else {
        logFile = QDir::homePath() + "/.clamav-gui/freshclam.log";
        if (tempDir.exists(logFile) == true){
            logFileWatcher->addPath(logFile);
            slot_logFileWatcherTriggered();
        } else {
            QFile file(QDir::homePath() + "/.clamav-gui/freshclam.log");
            if (file.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text)){
                QTextStream stream(&file);
                stream << "";
                file.close();
                logFileWatcher->addPath(logFile);
                slot_logFileWatcherTriggered();
            }
        }
    }
}

void freshclamsetter::slot_deamonCheckerProcessHasOutput(){
    pidFile = pidFile + deamonCheckerProcess->readAllStandardError();
    pidFile = pidFile + deamonCheckerProcess->readAllStandardOutput();
}

void freshclamsetter::slot_fileSystemWatcherTriggered(){
QDir tempDir;

    if ((pidFile != "") && (tempDir.exists(pidFile) == true)){
        ui->startStopDeamonButton->setText(tr("Deamon running - stop deamon"));
        ui->startStopDeamonButton->setStyleSheet("background:green");
        ui->startStopDeamonButton->setIcon(QIcon(":/icons/icons/Clam.png"));
        pidFileWatcher->addPath(pidFile);
    } else {
        pidFile = "";
        ui->startStopDeamonButton->setText(tr("Deamon not running - start deamon"));
        ui->startStopDeamonButton->setStyleSheet("background:red");
        ui->startStopDeamonButton->setIcon(QIcon(":/icons/icons/freshclam.png"));
        if (setupFile->getSectionBoolValue("Freshclam","Started") == true) slot_startStopDeamonButtonClicked();
    }
}

void freshclamsetter::slot_clearLogButtonClicked(){
QFile file(QDir::homePath() + "/.clamav-gui/update.log");

    file.remove();
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);
    stream << "";
    file.close();
    ui->logPlainText->setPlainText("");
}

void freshclamsetter::slot_logFileWatcherTriggered(){
QFile file(logFile);
QString content;
QString value;
int pos;

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    content = stream.readAll();
    pos = content.lastIndexOf("ClamAV update process started at");
    if (pos != -1){
        value = content.mid(pos + 33,content.indexOf("\n",pos + 33) - (pos + 33));
        setupFile->setSectionValue("Updater","LastUpdate",value);
    }
    pos = content.lastIndexOf("main.cvd updated");
    if (pos != -1){
        value = content.mid(pos + 17,content.indexOf("\n",pos + 17) - (pos + 17));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","MainVersion",value);
    }
    pos = content.lastIndexOf("main.cvd is up to date");
    if (pos != -1){
        value = content.mid(pos + 23,content.indexOf("\n",pos + 23) - (pos + 23));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","MainVersion",value);
    }
    pos = content.lastIndexOf("daily.cvd updated");
    if (pos != -1){
        value = content.mid(pos + 18,content.indexOf("\n",pos + 18) - (pos + 18));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","DailyVersion",value);
    }
    pos = content.lastIndexOf("daily.cld updated");
    if (pos != -1){
        value = content.mid(pos + 18,content.indexOf("\n",pos + 18) - (pos + 18));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","DailyVersion",value);
    }
    pos = content.lastIndexOf("daily.cld is up to date");
    if (pos != -1){
        value = content.mid(pos + 24,content.indexOf("\n",pos + 24) - (pos + 24));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","DailyVersion",value);
    }
    pos = content.lastIndexOf("bytecode.cvd is up to date");
    if (pos != -1){
        value = content.mid(pos + 27,content.indexOf("\n",pos + 27) - (pos + 27));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","BytecodeVersion",value);
    }
    pos = content.lastIndexOf("bytecode.cvd updated");
    if (pos != -1){
        value = content.mid(pos + 21,content.indexOf("\n",pos + 21) - (pos + 21));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","BytecodeVersion",value);
    }
    pos = content.lastIndexOf("Database updated");
    if (pos != -1){
        value = content.mid(pos,content.indexOf("\n",pos) - (pos));
        setupFile->setSectionValue("Updater","DatabaseFrom",value);
    }

    setUpdaterInfo();

    ui->deamonLogText->clear();
    QStringList lines = content.split("\n");
    foreach(QString line,lines){
        ui->deamonLogText->insertPlainText(line + "\n");
        ui->deamonLogText->ensureCursorVisible();
    }
}

void freshclamsetter::slot_updateFileWatcherTriggered(){
QFile file(QDir::homePath() + "/.clamav-gui/update.log");
QString content;
QString value;
int pos;

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    content = stream.readAll();
    pos = content.indexOf("ClamAV update process started at");
    if (pos != -1){
        value = content.mid(pos + 33,content.indexOf("\n",pos + 33) - (pos + 33));
        setupFile->setSectionValue("Updater","LastUpdate",value);
    }
    pos = content.indexOf("main.cvd updated");
    if (pos != -1){
        value = content.mid(pos + 17,content.indexOf("\n",pos + 17) - (pos + 17));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","MainVersion",value);
    }
    pos = content.indexOf("daily.cvd updated");
    if (pos != -1){
        value = content.mid(pos + 18,content.indexOf("\n",pos + 18) - (pos + 18));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","DailyVersion",value);
    }
    pos = content.indexOf("bytecode.cvd updated");
    if (pos != -1){
        value = content.mid(pos + 21,content.indexOf("\n",pos + 21) - (pos + 21));
        value.replace("(","");
        value.replace(")","");
        setupFile->setSectionValue("Updater","BytecodeVersion",value);
    }
    pos = content.indexOf("Database updated");
    if (pos != -1){
        value = content.mid(pos,content.indexOf("\n",pos) - (pos));
        setupFile->setSectionValue("Updater","DatabaseFrom",value);
    }

    ui->logPlainText->setPlainText("");
    QStringList lines = content.split("\n");
    foreach(QString line,lines){
        ui->logPlainText->insertPlainText(line + "\n");
        ui->logPlainText->ensureCursorVisible();
    }
}

void freshclamsetter::setForm(bool mode){
    ui->clearLogButton->setEnabled(mode);
    ui->dataBaseComboBox->setEnabled(mode);
    ui->deamonClearLogButton->setEnabled(mode);
    ui->deamonClearLogButton->setEnabled(mode);
    ui->enableStatsCheckBox->setEnabled(mode);
    ui->selectDataBaseDirectoryButton->setEnabled(mode);
    ui->startStopDeamonButton->setEnabled(mode);
    ui->statsIDLineEdit->setEnabled(mode);
    ui->updateNowButton->setEnabled(mode);
    ui->updateSettingsGroupBox->setEnabled(mode);
    ui->updatesPerDaySpinBox->setEnabled(mode);
}

void freshclamsetter::slot_clearDeamonLogButtonClicked(){
QFile file(QDir::homePath() + "/.clamav-gui/freshclam.log");

    ui->deamonLogText->setPlainText("");
    updateLogFileWatcher->removePath(QDir::homePath() + "/.clamav-gui/freshclam.log");
    file.remove();
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);
    stream << "";
    file.close();
    updateLogFileWatcher->addPath(QDir::homePath() + "/.clamav-gui/freshclam.log");

}

void freshclamsetter::setUpdaterInfo(){
    QString htmlCode = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>";
    htmlCode = htmlCode + "<table><tr><td width='200'>";
    if (setupFile->getSectionValue("Updater","DatabaseFrom") != "") {
        htmlCode = htmlCode + tr("Database origin : </td><td>") + setupFile->getSectionValue("Updater","DatabaseFrom") + "</td></tr><tr><td>";
    }
    if (setupFile->getSectionValue("Updater","LastUpdate") != "") {
        htmlCode = htmlCode + tr("Last Update : </td><td>") + setupFile->getSectionValue("Updater","LastUpdate") + "</td></tr><tr><td>";
    }
    if (setupFile->getSectionValue("Updater","MainVersion") != "") {
        htmlCode = htmlCode + tr("Main File : </td><td>") + setupFile->getSectionValue("Updater","MainVersion") + "</td></tr><tr><td>";
    }
    if (setupFile->getSectionValue("Updater","DailyVersion") != "") {
        htmlCode = htmlCode + tr("Daily File : </td><td>") + setupFile->getSectionValue("Updater","DailyVersion") + "</td></tr><tr><td>";
    }
    if (setupFile->getSectionValue("Updater","BytecodeVersion") != "") {
        htmlCode = htmlCode + tr("ByteCode File : </td><td>") + setupFile->getSectionValue("Updater","BytecodeVersion") + "</td></tr></table>";
    }
    htmlCode = htmlCode + "</body></html>";
    ui->updateInfoText->setHtml(htmlCode);
}

void freshclamsetter::slot_updaterHasOutput(){
static QString oldLine;
QString output = updater->readAll();
int start = output.lastIndexOf("Downloading");
int stop = output.lastIndexOf(char(13));
QString line = output.mid(start,stop - start);

    if (line != oldLine){
        busyLabel->setText(line);
    }
    oldLine = line;
}
