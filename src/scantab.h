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

#ifndef SCANTAB_H
#define SCANTAB_H

#include "dragablepushbutton.h"
#include "cfilesystemmodel.h"
#include "setupfilehandler.h"
#include "highlighter.h"
#include <QFileSystemWatcher>
#include <QFileSystemModel>
#include <QScrollBar>
#include <QButtonGroup>
#include <QObjectList>
#include <QMessageBox>
#include <QProcess>
#include <QWidget>
#include <QLabel>
#include <QMovie>

namespace Ui {
class scanTab;
}

class scanTab : public QWidget
{
    Q_OBJECT

public:
    explicit scanTab(QWidget *parent = 0);
    bool recursivChecked();
    void setStatusMessage(QString);
    void clearLogMessage();
    void setStatusBarMessage(QString,QString bgColor="#ffffff");
    ~scanTab();

private:
    Ui::scanTab         *ui;
    CFileSystemModel    * model;
    QButtonGroup        * deviceGroup;
    QProcess            * whoamiProcess;
    QFileSystemWatcher  * fileSystemWatcher;
    setupFileHandler    * setupFile;
    setupFileHandler    * profiles;
    highlighter         * logHighLighter;
    QStringList           devices;
    QString               username;
    QMovie              * movie;
    QLabel              * busyLabel;
    int                   buttonID;

private slots:
    void slot_scanButtonClicked();
    void slot_homePathButtonClicked();
    void slot_basePathButtonClicked();
    void slot_updateDeviceList();
    void slot_whoamiProcessFinished();
    void slot_deviceButtonClicked(int buttonIndex);
    void slot_recursivScanCheckBoxClicked();
    void slot_virusFoundComboBoxChanged();
    void slot_abortScan();
    void slot_enableForm(bool);
    void slot_requestDropZoneVisible();
    void slot_dirtreeSelectionChanged();

signals:
    void triggerScanRequest(QStringList);
    void abortScan();
    void requestDropZoneVisible();
};

#endif // SCANTAB_H
