/***************************************************************************
 *   Copyright (C) 2015 by Joerg Zopes                                     *
 *   joerg.zopes@gmx.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FRESHCLAMSETTER_H
#define FRESHCLAMSETTER_H

#include <QWidget>
#include <QMovie>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "setupfilehandler.h"
#include "highlighter.h"
#include "progressdialog.h"

namespace Ui {
class freshclamsetter;
}

class freshclamsetter : public QWidget
{
    Q_OBJECT

public:
    explicit freshclamsetter(QWidget *parent = 0);
    ~freshclamsetter();

private:
    Ui::freshclamsetter *ui;
    setupFileHandler    *setupFile;
    QProcess            *updater;
    QProcess            *deamonCheckerProcess;
    QString             pidFile;
    QString             logFile;
    progressDialog      * busyLabel;
    QFileSystemWatcher  *pidFileWatcher;
    QFileSystemWatcher  *logFileWatcher;
    QFileSystemWatcher  *updateLogFileWatcher;
    highlighter         *updateLogHighLighter;
    highlighter         *freshclamLogHighLighter;
    void readSettings();
    void setForm(bool);
    void setUpdaterInfo();

private slots:
    void slot_writeSettings();
    void slot_selectDataBaseDirectoryButtonClicked();
    void slot_updatesPerDaySpinBoxActivated();
    void slot_updateNowButtonClicked();
    void slot_statsIdLineEditActivated();
    void slot_startStopDeamonButtonClicked();
    void slot_enableStatsCheckBoxActivated();
    void slot_dataBaseComboBoxActivated();
    void slot_dataBaseDirectoryChanged();
    void slot_updaterFinished(int);
    void slot_deamonCheckerProcessFinished();
    void slot_deamonCheckerProcessHasOutput();
    void slot_fileSystemWatcherTriggered();
    void slot_logFileWatcherTriggered();
    void slot_updateFileWatcherTriggered();
    void slot_clearLogButtonClicked();
    void slot_clearDeamonLogButtonClicked();
    void slot_updaterHasOutput();

signals:
    void dataBaseDirectoryChanged();
    void setBallonMessage(int, QString,QString);
};

#endif // FRESHCLAMSETTER_H
