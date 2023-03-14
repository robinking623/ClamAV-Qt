#include "scanlimitstab.h"
#include "ui_scanlimitstab.h"

scanLimitsTab::scanLimitsTab(QWidget *parent) : QWidget(parent), ui(new Ui::scanLimitsTab)
{
    ui->setupUi(this);
    setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");
    if (setupFile->sectionExists("ScanLimitations") == true) updateLimits(); else writeLimits();
}

scanLimitsTab::~scanLimitsTab()
{
    delete ui;
}

void scanLimitsTab::writeLimits(){
QString keyword;
QString value;
QString checked;

    keyword = "Files larger than this will be skipped and assumed clean";
    value = QString::number(ui->filesLargerThanThisSpinBox->value()) + ui->filesLargerThanThisComboBox->currentText();
    ui->filesLargerThanThisCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "The maximum amount of data to scan for each container file";
    value = QString::number(ui->maxAmountForContainerSpinBox->value()) + ui->maxAmountForContainerComboBox->currentText();
    ui->maxAmountForContainerCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "The maximum number of files to scan for each container file";
    value = QString::number(ui->maxNumberForContainerSpinBox->value()) + ui->maxNumberForContainerComboBox->currentText();
    ui->maxNumberForContainerCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum archive recursion level for container file";
    value = QString::number(ui->maxArchiveRecursionForContainerSpinBox->value()) + ui->maxArchiveRecursionForContainerComboBox->currentText();
    ui->maxArchiveRecursionForContainerCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum directory recursion level";
    value = QString::number(ui->maxDirRecursionLevelSpinBox->value()) + ui->maxDirRecursionLevelComboBox->currentText();
    ui->maxDirRecursionLevelCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size file to check for embedded PE";
    value = QString::number(ui->maxSizeFileForPESpinBox->value()) + ui->maxSizeFileForPEComboBox->currentText();
    ui->maxSizeFileForPECheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size of HTML file to normalize";
    value = QString::number(ui->maxSizeHTMLFileToNormalizeSpinBox->value()) + ui->maxSizeHTMLFileToNormalizeComboBox->currentText();
    ui->maxSizeHTMLFileToNormalizeCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size of normalized HTML file to scan";
    value = QString::number(ui->maxSizeOfNormalizedHTMLFileSpinBox->value()) + ui->maxSizeOfNormalizedHTMLFileComboBox->currentText();
    ui->maxSizeOfNormalizedHTMLFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size of script file to normalize";
    value = QString::number(ui->maxSizeOfScriptFileToNormalizeSpinBox->value()) + ui->maxSizeOfScriptFileToNormalizeComboBox->currentText();
    ui->maxSizeOfScriptFileToNormalizeCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum size zip to type reanalyze";
    value = QString::number(ui->maxSizeZipToTypeReanalzeSpinBox->value()) + ui->maxSizeZipToTypeReanalzeComboBox->currentText();
    ui->maxSizeZipToTypeReanalzeCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum number of partitions in disk image to be scanned";
    value = QString::number(ui->maxNumberOfPartitionsInDiskImageSpinBox->value()) + ui->maxNumberOfPartitionsInDiskImageComboBox->currentText();
    ui->maxNumberOfPartitionsInDiskImageCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Maximum number of icons in PE file to be scanned";
    value = QString::number(ui->maxNumberOfIconsInPEFileSpinBox->value()) + ui->maxNumberOfIconsInPEFileComboBox->currentText();
    ui->maxNumberOfIconsInPEFileCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);

    keyword = "Number of seconds to wait for waiting a response back from the stats server";
    value = QString::number(ui->numberOfSecondsToWaitForResponseSpinBox->value());
    ui->numberOfSecondsToWaitForResponseCheckBox->isChecked() == true?checked="checked":checked="not checked";
    setupFile->setSectionValue("ScanLimitations",keyword,checked + "|" + value);
}

void scanLimitsTab::updateLimits(){
QString keyword;
QString value;
QString checked;

    value = setupFile->getSectionValue("ScanLimitations","Files larger than this will be skipped and assumed clean");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->filesLargerThanThisComboBox->setCurrentIndex(ui->filesLargerThanThisComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->filesLargerThanThisCheckBox->setChecked(true):ui->filesLargerThanThisCheckBox->setChecked(false);
    ui->filesLargerThanThisSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","The maximum amount of data to scan for each container file");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxAmountForContainerComboBox->setCurrentIndex(ui->maxAmountForContainerComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxAmountForContainerCheckBox->setChecked(true):ui->maxAmountForContainerCheckBox->setChecked(false);
    ui->maxAmountForContainerSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","The maximum number of files to scan for each container file");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxNumberForContainerComboBox->setCurrentIndex(ui->maxNumberForContainerComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxNumberForContainerCheckBox->setChecked(true):ui->maxNumberForContainerCheckBox->setChecked(false);
    ui->maxNumberForContainerSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum archive recursion level for container file");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxArchiveRecursionForContainerComboBox->setCurrentIndex(ui->maxArchiveRecursionForContainerComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxArchiveRecursionForContainerCheckBox->setChecked(true):ui->maxArchiveRecursionForContainerCheckBox->setChecked(false);
    ui->maxArchiveRecursionForContainerSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum directory recursion level");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxDirRecursionLevelComboBox->setCurrentIndex(ui->maxDirRecursionLevelComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxDirRecursionLevelCheckBox->setChecked(true):ui->maxDirRecursionLevelCheckBox->setChecked(false);
    ui->maxDirRecursionLevelSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size file to check for embedded PE");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxSizeFileForPEComboBox->setCurrentIndex(ui->maxSizeFileForPEComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxSizeFileForPECheckBox->setChecked(true):ui->maxSizeFileForPECheckBox->setChecked(false);
    ui->maxSizeFileForPESpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size of HTML file to normalize");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxSizeHTMLFileToNormalizeComboBox->setCurrentIndex(ui->maxSizeHTMLFileToNormalizeComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxSizeHTMLFileToNormalizeCheckBox->setChecked(true):ui->maxSizeHTMLFileToNormalizeCheckBox->setChecked(false);
    ui->maxSizeHTMLFileToNormalizeSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size of normalized HTML file to scan");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxSizeOfNormalizedHTMLFileComboBox->setCurrentIndex(ui->maxSizeOfNormalizedHTMLFileComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxSizeOfNormalizedHTMLFileCheckBox->setChecked(true):ui->maxSizeOfNormalizedHTMLFileCheckBox->setChecked(false);
    ui->maxSizeOfNormalizedHTMLFileSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size of script file to normalize");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxSizeOfScriptFileToNormalizeComboBox->setCurrentIndex(ui->maxSizeOfScriptFileToNormalizeComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxSizeOfScriptFileToNormalizeCheckBox->setChecked(true):ui->maxSizeOfScriptFileToNormalizeCheckBox->setChecked(false);
    ui->maxSizeOfScriptFileToNormalizeSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum size zip to type reanalyze");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxSizeZipToTypeReanalzeComboBox->setCurrentIndex(ui->maxSizeZipToTypeReanalzeComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxSizeZipToTypeReanalzeCheckBox->setChecked(true):ui->maxSizeZipToTypeReanalzeCheckBox->setChecked(false);
    ui->maxSizeZipToTypeReanalzeSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum number of partitions in disk image to be scanned");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxNumberOfPartitionsInDiskImageComboBox->setCurrentIndex(ui->maxNumberOfPartitionsInDiskImageComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxNumberOfPartitionsInDiskImageCheckBox->setChecked(true):ui->maxNumberOfPartitionsInDiskImageCheckBox->setChecked(false);
    ui->maxNumberOfPartitionsInDiskImageSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Maximum number of icons in PE file to be scanned");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    if ((value.right(1) == "K") || (value.right(1) == "M") || (value.right(1) == "G")){
        ui->maxNumberOfIconsInPEFileComboBox->setCurrentIndex(ui->maxNumberOfIconsInPEFileComboBox->findText(value.right(1)));
        value = value.left(value.length() - 1);
    }
    checked=="checked"?ui->maxNumberOfIconsInPEFileCheckBox->setChecked(true):ui->maxNumberOfIconsInPEFileCheckBox->setChecked(false);
    ui->maxNumberOfIconsInPEFileSpinBox->setValue(value.toInt());

    value = setupFile->getSectionValue("ScanLimitations","Number of seconds to wait for waiting a response back from the stats server");
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->numberOfSecondsToWaitForResponseCheckBox->setChecked(true):ui->numberOfSecondsToWaitForResponseCheckBox->setChecked(false);
    ui->numberOfSecondsToWaitForResponseSpinBox->setValue(value.toInt());
}
