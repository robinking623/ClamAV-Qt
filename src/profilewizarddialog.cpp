#include "profilewizarddialog.h"
#include "ui_profilewizarddialog.h"

ProfileWizardDialog::ProfileWizardDialog(QWidget *parent, QString name) : QDialog(parent), profileName(name), ui(new Ui::ProfileWizardDialog)
{
    ui->setupUi(this);

    if (profileName == ""){
        setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");
    } else {
        setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/profiles/" + profileName + ".ini");
        ui->profileNameLineEdit->setText(profileName);
        ui->profileNameLineEdit->setReadOnly(true);
        ui->createButton->setText(tr("Save"));
    }

    model = new CFileSystemModel;
    model->setFilter(QDir::Dirs|QDir::NoDotAndDotDot );
    model->unCheckAll();

    ui->treeView->setModel(model);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    readSettings();

    profileName==""?ui->stackedWidget->setCurrentIndex(0):ui->stackedWidget->setCurrentIndex(3);
    slot_previousButtonClicked();
}

ProfileWizardDialog::~ProfileWizardDialog()
{
    delete ui;
}

void ProfileWizardDialog::readSettings(){
QString section = "REGEXP_and_IncludeExclude";
QString keyword;
QString value;
QString checked;
QString optionText;
QString tooltipText;
QListWidgetItem * item;
QStringList directories = setupFile->getSectionValue(ui->profileNameLineEdit->text(),"Directories").split("\n");
QStringList availableOptions = setupFile->getKeywords("AvailableOptions");
QStringList selectedOptions = setupFile->getKeywords("SelectedOptions");

// DIRTREE
    ui->treeView->collapseAll();
    ui->recursivCheckBox->setChecked(setupFile->getSectionBoolValue(ui->profileNameLineEdit->text(),"Recursion"));
    model->unCheckAll();
    foreach(QString dir,directories){
        if (dir != ""){
            model->setChecked(dir,true);
            QModelIndex index = model->index(dir);
            ui->treeView->scrollTo(index);
            ui->treeView->setCurrentIndex(index);
        }
    }

// DIRECTORIES
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

// INCLUDE / EXCLUDE
    keyword = "DontScanFileNamesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->pwdontScanFileNameCheckBox->setChecked(true):ui->pwdontScanFileNameCheckBox->setChecked(false);
    ui->pwdontScanFileNameLineEdit->setText(value);

    keyword = "DontScanDiretoriesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->pwdontScanDirCheckBox->setChecked(true):ui->pwdontScanDirCheckBox->setChecked(false);
    ui->pwdontScanDirLineEdit->setText(value);

    keyword = "OnlyScanFileNamesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->pwonlyScanFileNameCheckBox->setChecked(true):ui->pwonlyScanFileNameCheckBox->setChecked(false);
    ui->pwonlyScanFileNameLineEdit->setText(value);

    keyword = "OnlyScanDiretoriesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->pwonlyScanDirCheckBox->setChecked(true):ui->pwonlyScanDirCheckBox->setChecked(false);
    ui->pwonlyScanDirLineEdit->setText(value);

// PUA
    keyword = "EnablePUAOptions";
    if (setupFile->getSectionBoolValue(section,keyword) == true){
        ui->pwenablePUACheckBox->setChecked(true);
        ui->pwPUAFrame->setEnabled(true);
    } else {
        ui->pwenablePUACheckBox->setChecked(false);
        ui->pwPUAFrame->setEnabled(false);
    }

    keyword = "SkipPUAPacked";
    ui->pwskipPUAPackedRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUAPackedRadioButon->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAPWTool";
    ui->pwskipPUAPWToolRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUAPWToolRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUANetTool";
    ui->pwskipPUANetToolRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUANetToolRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAP2P";
    ui->pwskipPUAP2PRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUAP2PRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAIRC";
    ui->pwskipPUAIRCRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUAIRCRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUARAT";
    ui->pwskipPUARATRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUARATRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUANetToolSpy";
    ui->pwskipPUANetToolSpyRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUANetToolSpyRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAServer";
    ui->pwskipPUAServerRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUAServerRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAScript";
    ui->pwskipPUAScriptRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->pwloadPUAScriptRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));


// SCANLIMITS
    value = setupFile->getSectionValue("ScanLimitations","Files larger than this will be skipped and assumed clean");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwfilesLargerThanThisComboBox->setCurrentIndex(ui->pwfilesLargerThanThisComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwfilesLargerThanThisComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwfilesLargerThanThisCheckBox->setChecked(true):ui->pwfilesLargerThanThisCheckBox->setChecked(false);
    ui->pwfilesLargerThanThisSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","The maximum amount of data to scan for each container file");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxAmountForContainerComboBox->setCurrentIndex(ui->pwmaxAmountForContainerComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
       ui->pwmaxAmountForContainerComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxAmountForContainerCheckBox->setChecked(true):ui->pwmaxAmountForContainerCheckBox->setChecked(false);
    ui->pwmaxAmountForContainerSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","The maximum number of files to scan for each container file");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxNumberForContainerComboBox->setCurrentIndex(ui->pwmaxNumberForContainerComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxNumberForContainerComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxNumberForContainerCheckBox->setChecked(true):ui->pwmaxNumberForContainerCheckBox->setChecked(false);
    ui->pwmaxNumberForContainerSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum archive recursion level for container file");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxArchiveRecursionForContainerComboBox->setCurrentIndex(ui->pwmaxArchiveRecursionForContainerComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxArchiveRecursionForContainerComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxArchiveRecursionForContainerCheckBox->setChecked(true):ui->pwmaxArchiveRecursionForContainerCheckBox->setChecked(false);
    ui->pwmaxArchiveRecursionForContainerSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum directory recursion level");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxDirRecursionLevelComboBox->setCurrentIndex(ui->pwmaxDirRecursionLevelComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxDirRecursionLevelComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxDirRecursionLevelCheckBox->setChecked(true):ui->pwmaxDirRecursionLevelCheckBox->setChecked(false);
    ui->pwmaxDirRecursionLevelSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size file to check for embedded PE");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxSizeFileForPEComboBox->setCurrentIndex(ui->pwmaxSizeFileForPEComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxSizeFileForPEComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxSizeFileForPECheckBox->setChecked(true):ui->pwmaxSizeFileForPECheckBox->setChecked(false);
    ui->pwmaxSizeFileForPESpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size of HTML file to normalize");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxSizeHTMLFileToNormalizeComboBox->setCurrentIndex(ui->pwmaxSizeHTMLFileToNormalizeComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxSizeHTMLFileToNormalizeComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxSizeHTMLFileToNormalizeCheckBox->setChecked(true):ui->pwmaxSizeHTMLFileToNormalizeCheckBox->setChecked(false);
    ui->pwmaxSizeHTMLFileToNormalizeSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size of normalized HTML file to scan");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxSizeOfNormalizedHTMLFileComboBox->setCurrentIndex(ui->pwmaxSizeOfNormalizedHTMLFileComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxSizeOfNormalizedHTMLFileComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxSizeOfNormalizedHTMLFileCheckBox->setChecked(true):ui->pwmaxSizeOfNormalizedHTMLFileCheckBox->setChecked(false);
    ui->pwmaxSizeOfNormalizedHTMLFileSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size of script file to normalize");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxSizeOfScriptFileToNormalizeComboBox->setCurrentIndex(ui->pwmaxSizeOfScriptFileToNormalizeComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxSizeOfScriptFileToNormalizeComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxSizeOfScriptFileToNormalizeCheckBox->setChecked(true):ui->pwmaxSizeOfScriptFileToNormalizeCheckBox->setChecked(false);
    ui->pwmaxSizeOfScriptFileToNormalizeSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size zip to type reanalyze");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxSizeZipToTypeReanalzeComboBox->setCurrentIndex(ui->pwmaxSizeZipToTypeReanalzeComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxSizeZipToTypeReanalzeComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxSizeZipToTypeReanalzeCheckBox->setChecked(true):ui->pwmaxSizeZipToTypeReanalzeCheckBox->setChecked(false);
    ui->pwmaxSizeZipToTypeReanalzeSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum number of partitions in disk image to be scanned");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxNumberOfPartitionsInDiskImageComboBox->setCurrentIndex(ui->pwmaxNumberOfPartitionsInDiskImageComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxNumberOfPartitionsInDiskImageComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxNumberOfPartitionsInDiskImageCheckBox->setChecked(true):ui->pwmaxNumberOfPartitionsInDiskImageCheckBox->setChecked(false);
    ui->pwmaxNumberOfPartitionsInDiskImageSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum number of icons in PE file to be scanned");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->pwmaxNumberOfIconsInPEFileComboBox->setCurrentIndex(ui->pwmaxNumberOfIconsInPEFileComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    } else {
        ui->pwmaxNumberOfIconsInPEFileComboBox->setCurrentIndex(0);
    }
    checked=="checked"?ui->pwmaxNumberOfIconsInPEFileCheckBox->setChecked(true):ui->pwmaxNumberOfIconsInPEFileCheckBox->setChecked(false);
    ui->pwmaxNumberOfIconsInPEFileSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Number of seconds to wait for waiting a response back from the stats server");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->pwnumberOfSecondsToWaitForResponseCheckBox->setChecked(true):ui->pwnumberOfSecondsToWaitForResponseCheckBox->setChecked(false);
    ui->pwnumberOfSecondsToWaitForResponseSpinBox->setValue(value.toInt());

// OPTIONS
    ui->availableOptionsList->clear();
    for (int i = 0; i < availableOptions.count(); i++){
        optionText = availableOptions.at(i);
        tooltipText = setupFile->getSectionValue("AvailableOptions",optionText);
        optionText = optionText.replace("<equal>","=");
        item = new QListWidgetItem(optionText);
        item->setToolTip(tooltipText);
        ui->availableOptionsList->addItem(item);
    }

    ui->selectedOptionsList->clear();
    for (int i = 0; i < selectedOptions.count(); i++){
        optionText = selectedOptions.at(i);
        tooltipText = setupFile->getSectionValue("SelectedOptions",optionText);
        optionText = optionText.replace("<equal>","=");
        item = new QListWidgetItem(optionText);
        item->setToolTip(tooltipText);
        ui->selectedOptionsList->addItem(item);
    }
}

void ProfileWizardDialog::slot_nextButtonClicked(){
int index = ui->stackedWidget->currentIndex();
int count = ui->stackedWidget->count() - 1;

    if (index < count) {
        index++;
        ui->stackedWidget->setCurrentIndex(index);
    }
    slot_checkNavigationButtons();
}

void ProfileWizardDialog::slot_previousButtonClicked(){
int index = ui->stackedWidget->currentIndex();

    if (index > 0){
        index--;
        ui->stackedWidget->setCurrentIndex(index);
    }
    slot_checkNavigationButtons();
}

void ProfileWizardDialog::slot_checkNavigationButtons(){
int index = ui->stackedWidget->currentIndex();
QFile checkFile(QDir::homePath() + "/.clamav-gui/profiles/" + ui->profileNameLineEdit->text() + ".ini");

    switch (index){
    case 0  :   ui->previousButton->setEnabled(false);
                    break;
    case 1  :   ui->previousButton->setEnabled(true);
                if ((ui->profileNameLineEdit->text() == "") | (checkFile.exists())) ui->nextButton->setEnabled(false); else ui->nextButton->setEnabled(true);
                if (profileName != "") ui->nextButton->setEnabled(true);
                    break;
    case 2  :   slot_listChanged();
                if (profileName != "") ui->previousButton->setEnabled(false);
                    break;
    default :   ui->previousButton->setEnabled(true);
                if (index == ui->stackedWidget->count() - 1) ui->nextButton->setEnabled(false); else ui->nextButton->setEnabled(true);
                if (index == ui->stackedWidget->count() - 1) ui->createButton->setEnabled(true); else ui->createButton->setEnabled(false);
                    break;

    }
}

void ProfileWizardDialog::slot_addButtonClicked(){
QListWidgetItem * item;

    if (ui->availableOptionsList->currentItem() != 0) {
        item = ui->availableOptionsList->takeItem(ui->availableOptionsList->currentRow());
        ui->selectedOptionsList->addItem(item);
    }
}

void ProfileWizardDialog::slot_removeButtonClicked(){
QListWidgetItem * item;

    if (ui->selectedOptionsList->currentItem() != 0) {
        item = ui->selectedOptionsList->takeItem(ui->selectedOptionsList->currentRow());
        ui->availableOptionsList->addItem(item);
    }
}

void ProfileWizardDialog::slot_enablePUACheckBoxActivated(){
    ui->pwenablePUACheckBox->isChecked()?ui->pwPUAFrame->setEnabled(true):ui->pwPUAFrame->setEnabled(false);
}

void ProfileWizardDialog::slot_homeButtonClicked(){
QModelIndex index = model->index(QDir::homePath());

    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
}

void ProfileWizardDialog::slot_rootButtonClicked(){
QModelIndex index = model->index("/");

    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
}

void ProfileWizardDialog::slot_listChanged(){
QList <QPersistentModelIndex> list = model->checkedIndexes.toList();

    list.count()==0?ui->nextButton->setEnabled(false):ui->nextButton->setEnabled(true);
}

void ProfileWizardDialog::slot_loadAllSupportedDBButtonClicked(){
QString folder = QFileDialog::getExistingDirectory(this,tr("Signature Folder"),ui->loadVirusDatabaseLineEdit->text());
    if (folder != "") ui->loadVirusDatabaseLineEdit->setText(folder);
}

void ProfileWizardDialog::slot_saveScanReportToFileButtonClicked(){
QString filename = QFileDialog::getOpenFileName(this,tr("Log-File"),ui->scanReportToFileLineEdit->text());
    if (filename != "") ui->scanReportToFileLineEdit->setText(filename);
}

void ProfileWizardDialog::slot_scanFromFileButtonClicked(){
QString filename = QFileDialog::getOpenFileName(this,tr("Scan from File"),ui->scanFilesFromFileLineEdit->text());
    if (filename != "") ui->scanFilesFromFileLineEdit->setText(filename);
}

void ProfileWizardDialog::slot_createTempFilesButtonClicked(){
QString folder = QFileDialog::getExistingDirectory(this,tr("Folder for temporary files"),ui->tempFilesLineEdit->text());
    if (folder != "") ui->tempFilesLineEdit->setText(folder);
}

void ProfileWizardDialog::slot_moveInfectedFilesButtonClicked(){
QString folder = QFileDialog::getExistingDirectory(this,tr("Move Folder for infected Files"),ui->moveDirectoryLineEdit->text());
    if (folder != "") ui->moveDirectoryLineEdit->setText(folder);
}

void ProfileWizardDialog::slot_copyInfectedFilesButtonClicked(){
QString folder = QFileDialog::getExistingDirectory(this,tr("Copy Folder for infected Files"),ui->copyDirectoryLineEdit->text());
    if (folder != "") ui->copyDirectoryLineEdit->setText(folder);
}

void ProfileWizardDialog::slot_createButtonClicked(){
QList <QPersistentModelIndex> list = model->checkedIndexes.toList();
QString directories;
QString profileName = ui->profileNameLineEdit->text();
setupFileHandler * profiles = new setupFileHandler(QDir::homePath() + "/.clamav-gui/profiles/" + profileName + ".ini");
QString line;
QString section = "REGEXP_and_IncludeExclude";
QString checked;
QString keyword;
QString value;
QString optionString;
QString tooltipString;
QListWidgetItem * item;

    for (int i = 0; i < list.count(); i++){
        if (i < list.count() - 1){
            directories = directories + list[i].data(QFileSystemModel::FilePathRole).toString() + "\n";
        } else {
            directories = directories + list[i].data(QFileSystemModel::FilePathRole).toString();
        }
    }

    profiles->setSectionValue(profileName,"Directories",directories);
    profiles->setSectionValue(profileName,"Recursion",ui->recursivCheckBox->isChecked());
    setupFile->setSectionValue("Profiles",profileName,profileName);

    keyword = "LoadSupportedDBFiles";
    value = ui->loadVirusDatabaseLineEdit->text();
    ui->loadVirusDatabaseCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "ScanReportToFile";
    value = ui->scanReportToFileLineEdit->text();
    ui->scanReportToFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "ScanFilesFromFile";
    value = ui->scanFilesFromFileLineEdit->text();
    ui->scanFilesFromFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "TmpFile";
    value = ui->tempFilesLineEdit->text();
    ui->tempFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "MoveInfectedFiles";
    value = ui->moveDirectoryLineEdit->text();
    ui->moveDirectoryCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "CopyInfectedFiles";
    value = ui->copyDirectoryLineEdit->text();
    ui->copyDirectoryCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "FollowDirectorySymLinks";
    value = QString::number(ui->followDirectorySymlinksComboBox->currentIndex());
    ui->followDirectorySymlinksCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    keyword = "FollowFileSymLinks";
    value = QString::number(ui->followFileSymlinksComboBox->currentIndex());
    ui->followFileSymlinksCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("Directories",keyword,checked + "|" + value);

    profiles->removeSection("AvailableOptions");
    profiles->removeSection("SelectedOptions");

    for (int i = 0; i < ui->availableOptionsList->count(); i++){
        item = ui->availableOptionsList->item(i);
        optionString = item->text();
        optionString = optionString.replace("=","<equal>");
        tooltipString = item->toolTip();
        profiles->setSectionValue("AvailableOptions",optionString,tooltipString);
    }

    for (int i = 0; i < ui->selectedOptionsList->count(); i++){
        item = ui->selectedOptionsList->item(i);
        optionString = item->text();
        optionString = optionString.replace("=","<equal>");
        tooltipString = item->toolTip();
        profiles->setSectionValue("SelectedOptions",optionString,tooltipString);
    }

    //triggerSaveProfile(ui->profileComboBox->currentText());

    keyword = "DontScanFileNamesMatchingRegExp";
    value = ui->pwdontScanFileNameLineEdit->text();
    ui->pwdontScanFileNameCheckBox->isChecked()?checked="checked":checked="not checked";
    profiles->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "DontScanDiretoriesMatchingRegExp";
    value = ui->pwdontScanDirLineEdit->text();
    ui->pwdontScanDirCheckBox->isChecked()?checked="checked":checked="not checked";
    profiles->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "OnlyScanFileNamesMatchingRegExp";
    value = ui->pwonlyScanFileNameLineEdit->text();
    ui->pwonlyScanFileNameCheckBox->isChecked()?checked="checked":checked="not checked";
    profiles->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "OnlyScanDiretoriesMatchingRegExp";
    value = ui->pwonlyScanDirLineEdit->text();
    ui->pwonlyScanDirCheckBox->isChecked()?checked="checked":checked="not checked";
    profiles->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "EnablePUAOptions";
    profiles->setSectionValue(section,keyword,ui->pwenablePUACheckBox->isChecked());

    keyword = "SkipPUAPacked";
    profiles->setSectionValue(section,keyword,ui->pwskipPUAPackedRadioButton->isChecked());

    keyword = "SkipPUAPWTool";
    profiles->setSectionValue(section,keyword,ui->pwskipPUAPWToolRadioButton->isChecked());

    keyword = "SkipPUANetTool";
    profiles->setSectionValue(section,keyword,ui->pwskipPUANetToolRadioButton->isChecked());

    keyword = "SkipPUAP2P";
    profiles->setSectionValue(section,keyword,ui->pwskipPUAP2PRadioButton->isChecked());

    keyword = "SkipPUAIRC";
    profiles->setSectionValue(section,keyword,ui->pwskipPUAIRCRadioButton->isChecked());

    keyword = "SkipPUARAT";
    profiles->setSectionValue(section,keyword,ui->pwskipPUARATRadioButton->isChecked());

    keyword = "SkipPUANetToolSpy";
    profiles->setSectionValue(section,keyword,ui->pwskipPUANetToolSpyRadioButton->isChecked());

    keyword = "SkipPUAServer";
    profiles->setSectionValue(section,keyword,ui->pwskipPUAServerRadioButton->isChecked());

    keyword = "SkipPUAScript";
    profiles->setSectionValue(section,keyword,ui->pwskipPUAScriptRadioButton->isChecked());


    keyword = "Files larger than this will be skipped and assumed clean";
    value = QString::number(ui->pwfilesLargerThanThisSpinBox->value()) + ui->pwfilesLargerThanThisComboBox->currentText();
    ui->pwfilesLargerThanThisCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "The maximum amount of data to scan for each container file";
    value = QString::number(ui->pwmaxAmountForContainerSpinBox->value()) + ui->pwmaxAmountForContainerComboBox->currentText();
    ui->pwmaxAmountForContainerCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "The maximum number of files to scan for each container file";
    value = QString::number(ui->pwmaxNumberForContainerSpinBox->value()) + ui->pwmaxNumberForContainerComboBox->currentText();
    ui->pwmaxNumberForContainerCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum archive recursion level for container file";
    value = QString::number(ui->pwmaxArchiveRecursionForContainerSpinBox->value()) + ui->pwmaxArchiveRecursionForContainerComboBox->currentText();
    ui->pwmaxArchiveRecursionForContainerCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum directory recursion level";
    value = QString::number(ui->pwmaxDirRecursionLevelSpinBox->value()) + ui->pwmaxDirRecursionLevelComboBox->currentText();
    ui->pwmaxDirRecursionLevelCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size file to check for embedded PE";
    value = QString::number(ui->pwmaxSizeFileForPESpinBox->value()) + ui->pwmaxSizeFileForPEComboBox->currentText();
    ui->pwmaxSizeFileForPECheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size of HTML file to normalize";
    value = QString::number(ui->pwmaxSizeHTMLFileToNormalizeSpinBox->value()) + ui->pwmaxSizeHTMLFileToNormalizeComboBox->currentText();
    ui->pwmaxSizeHTMLFileToNormalizeCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size of normalized HTML file to scan";
    value = QString::number(ui->pwmaxSizeOfNormalizedHTMLFileSpinBox->value()) + ui->pwmaxSizeOfNormalizedHTMLFileComboBox->currentText();
    ui->pwmaxSizeOfNormalizedHTMLFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size of script file to normalize";
    value = QString::number(ui->pwmaxSizeOfScriptFileToNormalizeSpinBox->value()) + ui->pwmaxSizeOfScriptFileToNormalizeComboBox->currentText();
    ui->pwmaxSizeOfScriptFileToNormalizeCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size zip to type reanalyze";
    value = QString::number(ui->pwmaxSizeZipToTypeReanalzeSpinBox->value()) + ui->pwmaxSizeZipToTypeReanalzeComboBox->currentText();
    ui->pwmaxSizeZipToTypeReanalzeCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum number of partitions in disk image to be scanned";
    value = QString::number(ui->pwmaxNumberOfPartitionsInDiskImageSpinBox->value()) + ui->pwmaxNumberOfPartitionsInDiskImageComboBox->currentText();
    ui->pwmaxNumberOfPartitionsInDiskImageCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum number of icons in PE file to be scanned";
    value = QString::number(ui->pwmaxNumberOfIconsInPEFileSpinBox->value()) + ui->pwmaxNumberOfIconsInPEFileComboBox->currentText();
    ui->pwmaxNumberOfIconsInPEFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Number of seconds to wait for waiting a response back from the stats server";
    value = QString::number(ui->pwnumberOfSecondsToWaitForResponseSpinBox->value());
    ui->pwnumberOfSecondsToWaitForResponseCheckBox->isChecked() == true?checked="checked":checked="not checked";
    profiles->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    QMessageBox::information(this,"Info",tr("Profile \"") + profileName + tr("\" saved."));
    emit signal_profileSaved();
    this->accept();
}
