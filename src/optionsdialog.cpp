#include "optionsdialog.h"
#include "ui_optionsdialog.h"

optionsDialog::optionsDialog(QWidget *parent) : QWidget(parent), ui(new Ui::optionsDialog)
{
    ui->setupUi(this);
    setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");
    if (setupFile->sectionExists("AvailableOptions") == true) updateOptionLists(); else writeOptionLists();
    updateDirectories();
    ui->availableOptionsList->setSortingEnabled(true);
    ui->selectedOptionsList->setSortingEnabled(true);
    ui->tabWidget->setTabText(0,"Directories");
    scanLimits = new scanLimitsTab(this);
    ui->tabWidget->addTab(scanLimits,QIcon(":/icons/icons/scanlimits.png"),"Scan Limitations");
    incExcOptTab = new includeExcludeOptions(this);

    ui->tabWidget->addTab(incExcOptTab,QIcon(":/icons/icons/includeexclude.png"),"Include/Exclude");
    ui->tabWidget->setIconSize(QSize(24,24));
}

optionsDialog::~optionsDialog()
{
    delete ui;
}

void optionsDialog::slot_addButtonClicked(){
QListWidgetItem * item;

    if (ui->availableOptionsList->currentItem() != 0) {
        item = ui->availableOptionsList->takeItem(ui->availableOptionsList->currentRow());
        ui->selectedOptionsList->addItem(item);
        writeOptionLists();
    }
}

void optionsDialog::slot_removeButtonClicked(){
QListWidgetItem * item;

    if (ui->selectedOptionsList->currentItem() != 0) {
        item = ui->selectedOptionsList->takeItem(ui->selectedOptionsList->currentRow());
        ui->availableOptionsList->addItem(item);
        writeOptionLists();
    }
}

void optionsDialog::updateOptionLists(){
QStringList availableOptions = setupFile->getKeywords("AvailableOptions");
QStringList selectedOptions = setupFile->getKeywords("SelectedOptions");
QString optionText;
QString tooltipText;
QListWidgetItem * item;
QString temp;

    ui->availableOptionsList->clear();

    for (int i = 0; i < availableOptions.count(); i++){
        optionText = availableOptions.at(i);
        tooltipText = setupFile->getSectionValue("AvailableOptions",optionText);
        optionText = optionText.replace("<equal>","=");
        item = new QListWidgetItem(optionText);
        item->setToolTip(tr(tooltipText.toLatin1().data()));
        ui->availableOptionsList->addItem(item);
    }

    ui->selectedOptionsList->clear();

    for (int i = 0; i < selectedOptions.count(); i++){
        optionText = selectedOptions.at(i);
        tooltipText = setupFile->getSectionValue("SelectedOptions",optionText);
        optionText = optionText.replace("<equal>","=");
        item = new QListWidgetItem(optionText);
        item->setToolTip(tr(tooltipText.toLatin1().data()));
        ui->selectedOptionsList->addItem(item);
    }
}

void optionsDialog::slot_selectLVDButtonClicked(){
QString rc;
    rc = QFileDialog::getExistingDirectory(this,tr("Select Directory"),QDir::homePath() + "/.clamav-gui/signatures",QFileDialog::ShowDirsOnly);
    if (rc != "") {
        ui->loadVirusDatabaseLineEdit->setText(rc);
        writeDirectories();
        emit dataBaseDirectoryChanged();
    }
}

void optionsDialog::slot_selectCDButtonClicked(){
QString rc;
    rc = QFileDialog::getExistingDirectory(this,tr("Select Directory"),QDir::homePath() + "/.clamav-gui/quarantine");
    if (rc != "") ui->copyDirectoryLineEdit->setText(rc);
    writeDirectories();
}

void optionsDialog::slot_selectMDButtonClicked(){
QString rc;
    rc = QFileDialog::getExistingDirectory(this,tr("Select Directory"),QDir::homePath() + "/.clamav-gui/quarantine");
    if (rc != "") ui->moveDirectoryLineEdit->setText(rc);

    writeDirectories();
}

void optionsDialog::slot_selectSRTFButtonClicked(){
    QString rc;
        rc = QFileDialog::getSaveFileName(this,tr("Select File"),QDir::homePath() + "/.clamav-gui/logs");
        if (rc != "") ui->scanReportToFileLineEdit->setText(rc);
        writeDirectories();
    }

void optionsDialog::slot_selectSFFButtonClicked(){
QString rc;
    rc = QFileDialog::getOpenFileName(this,tr("Select File"),QDir::homePath());
    if (rc != "") ui->scanFilesFromFileLineEdit->setText(rc);
    writeDirectories();
}

void optionsDialog::slot_selectTDButtonClicked(){
QString rc;
    rc = QFileDialog::getExistingDirectory(this,tr("Select Directory"),QDir::homePath());
    if (rc != "") ui->tempFilesLineEdit->setText(rc);
    writeDirectories();
}

void optionsDialog::writeOptionLists(){
QString optionString;
QString tooltipString;
QListWidgetItem * item;

    setupFile->removeSection("AvailableOptions");
    setupFile->removeSection("SelectedOptions");

    for (int i = 0; i < ui->availableOptionsList->count(); i++){
        item = ui->availableOptionsList->item(i);
        optionString = item->text();
        optionString = optionString.replace("=","<equal>");
        tooltipString = item->toolTip();
        setupFile->setSectionValue("AvailableOptions",optionString,tooltipString);
    }

    for (int i = 0; i < ui->selectedOptionsList->count(); i++){
        item = ui->selectedOptionsList->item(i);
        optionString = item->text();
        optionString = optionString.replace("=","<equal>");
        tooltipString = item->toolTip();
        setupFile->setSectionValue("SelectedOptions",optionString,tooltipString);
    }
}

void optionsDialog::updateDirectories(){
QString value;
QString checked;

    value=setupFile->getSectionValue("Directories","LoadSupportedDBFiles");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->loadVirusDatabaseCheckBox->setChecked(true):ui->loadVirusDatabaseCheckBox->setChecked(false);
    ui->loadVirusDatabaseLineEdit->setText(value);

    value=setupFile->getSectionValue("Directories","ScanReportToFile");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->scanReportToFileCheckBox->setChecked(true):ui->scanReportToFileCheckBox->setChecked(false);
    ui->scanReportToFileLineEdit->setText(value);

    value=setupFile->getSectionValue("Directories","ScanFilesFromFile");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->scanFilesFromFileCheckBox->setChecked(true):ui->scanFilesFromFileCheckBox->setChecked(false);
    ui->scanFilesFromFileLineEdit->setText(value);

    value=setupFile->getSectionValue("Directories","TmpFile");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->tempFileCheckBox->setChecked(true):ui->tempFileCheckBox->setChecked(false);
    ui->tempFilesLineEdit->setText(value);

    value=setupFile->getSectionValue("Directories","MoveInfectedFiles");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->moveDirectoryCheckBox->setChecked(true):ui->moveDirectoryCheckBox->setChecked(false);
    ui->moveDirectoryLineEdit->setText(value);

    value=setupFile->getSectionValue("Directories","CopyInfectedFiles");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->copyDirectoryCheckBox->setChecked(true):ui->copyDirectoryCheckBox->setChecked(false);
    ui->copyDirectoryLineEdit->setText(value);

    value=setupFile->getSectionValue("Directories","FollowDirectorySymLinks");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->followDirectorySymlinksCheckBox->setChecked(true):ui->followDirectorySymlinksCheckBox->setChecked(false);
    ui->followDirectorySymlinksComboBox->setCurrentIndex(value.toInt());

    value=setupFile->getSectionValue("Directories","FollowFileSymLinks");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->followFileSymlinksCheckBox->setChecked(true):ui->followFileSymlinksCheckBox->setChecked(false);
    ui->followFileSymlinksComboBox->setCurrentIndex(value.toInt());
}

void optionsDialog::writeDirectories(){
QString keyword;
QString value;
QString checked;

    keyword = "LoadSupportedDBFiles";
    value = ui->loadVirusDatabaseLineEdit->text();
    ui->loadVirusDatabaseCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "ScanReportToFile";
    value = ui->scanReportToFileLineEdit->text();
    ui->scanReportToFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "ScanFilesFromFile";
    value = ui->scanFilesFromFileLineEdit->text();
    ui->scanFilesFromFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "TmpFile";
    value = ui->tempFilesLineEdit->text();
    ui->tempFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "MoveInfectedFiles";
    value = ui->moveDirectoryLineEdit->text();
    ui->moveDirectoryCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "CopyInfectedFiles";
    value = ui->copyDirectoryLineEdit->text();
    ui->copyDirectoryCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "FollowDirectorySymLinks";
    value = QString::number(ui->followDirectorySymlinksComboBox->currentIndex());
    ui->followDirectorySymlinksCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "FollowFileSymLinks";
    value = QString::number(ui->followFileSymlinksComboBox->currentIndex());
    ui->followFileSymlinksCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("Directories",keyword,checked + "|" + value);
}

void optionsDialog::slot_updateDirectories(){
    updateDirectories();
}

void optionsDialog::slot_logViewerButtonClicked(){
QFile file;

    if ((ui->scanReportToFileCheckBox->isChecked() == true) & (ui->scanReportToFileLineEdit->text() != "")){
        file.setFileName(ui->scanReportToFileLineEdit->text());
        if (file.exists()){
            logViewObject * logView = new logViewObject(this,ui->scanReportToFileLineEdit->text());
            logView->setModal(true);
            logView->showMaximized();
        }
    }
}
