#include "includeexcludeoptions.h"
#include "ui_includeexcludeoptions.h"

includeExcludeOptions::includeExcludeOptions(QWidget *parent) : QWidget(parent), ui(new Ui::includeExcludeOptions)
{
    ui->setupUi(this);
    setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");
    if (setupFile->sectionExists("REGEXP_and_IncludeExclude") == true){
        readSettings();
    }
}

includeExcludeOptions::~includeExcludeOptions()
{
    delete ui;
}

void includeExcludeOptions::slot_writeSettings(){
QString section = "REGEXP_and_IncludeExclude";
QString checked;
QString keyword;
QString value;

    keyword = "DontScanFileNamesMatchingRegExp";
    value = ui->dontScanFileNameLineEdit->text();
    ui->dontScanFileNameCheckBox->isChecked()?checked="checked":checked="not checked";
    setupFile->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "DontScanDiretoriesMatchingRegExp";
    value = ui->dontScanDirLineEdit->text();
    ui->dontScanDirCheckBox->isChecked()?checked="checked":checked="not checked";
    setupFile->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "OnlyScanFileNamesMatchingRegExp";
    value = ui->onlyScanFileNameLineEdit->text();
    ui->onlyScanFileNameCheckBox->isChecked()?checked="checked":checked="not checked";
    setupFile->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "OnlyScanDiretoriesMatchingRegExp";
    value = ui->onlyScanDirLineEdit->text();
    ui->onlyScanDirCheckBox->isChecked()?checked="checked":checked="not checked";
    setupFile->setSectionValue(section,keyword,checked + "|" + value);

    keyword = "EnablePUAOptions";
    setupFile->setSectionValue(section,keyword,ui->enablePUACheckBox->isChecked());

    keyword = "SkipPUAPacked";
    setupFile->setSectionValue(section,keyword,ui->skipPUAPackedRadioButton->isChecked());

    keyword = "SkipPUAPWTool";
    setupFile->setSectionValue(section,keyword,ui->skipPUAPWToolRadioButton->isChecked());

    keyword = "SkipPUANetTool";
    setupFile->setSectionValue(section,keyword,ui->skipPUANetToolRadioButton->isChecked());

    keyword = "SkipPUAP2P";
    setupFile->setSectionValue(section,keyword,ui->skipPUAP2PRadioButton->isChecked());

    keyword = "SkipPUAIRC";
    setupFile->setSectionValue(section,keyword,ui->skipPUAIRCRadioButton->isChecked());

    keyword = "SkipPUARAT";
    setupFile->setSectionValue(section,keyword,ui->skipPUARATRadioButton->isChecked());

    keyword = "SkipPUANetToolSpy";
    setupFile->setSectionValue(section,keyword,ui->skipPUANetToolSpyRadioButton->isChecked());

    keyword = "SkipPUAServer";
    setupFile->setSectionValue(section,keyword,ui->skipPUAServerRadioButton->isChecked());

    keyword = "SkipPUAScript";
    setupFile->setSectionValue(section,keyword,ui->skipPUAScriptRadioButton->isChecked());

}

void includeExcludeOptions::readSettings(){
QString section = "REGEXP_and_IncludeExclude";
QString checked;
QString keyword;
QString value;

    keyword = "DontScanFileNamesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->dontScanFileNameCheckBox->setChecked(true):ui->dontScanFileNameCheckBox->setChecked(false);
    ui->dontScanFileNameLineEdit->setText(value);

    keyword = "DontScanDiretoriesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->dontScanDirCheckBox->setChecked(true):ui->dontScanDirCheckBox->setChecked(false);
    ui->dontScanDirLineEdit->setText(value);

    keyword = "OnlyScanFileNamesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->onlyScanFileNameCheckBox->setChecked(true):ui->onlyScanFileNameCheckBox->setChecked(false);
    ui->onlyScanFileNameLineEdit->setText(value);

    keyword = "OnlyScanDiretoriesMatchingRegExp";
    value = setupFile->getSectionValue(section,keyword);
    checked = value.left(value.indexOf("|"));
    value = value.mid(value.indexOf("|") + 1);
    checked=="checked"?ui->onlyScanDirCheckBox->setChecked(true):ui->onlyScanDirCheckBox->setChecked(false);
    ui->onlyScanDirLineEdit->setText(value);

    keyword = "EnablePUAOptions";
    if (setupFile->getSectionBoolValue(section,keyword) == true){
        ui->enablePUACheckBox->setChecked(true);
        ui->PUAFrame->setEnabled(true);
    } else {
        ui->enablePUACheckBox->setChecked(false);
        ui->PUAFrame->setEnabled(false);
    }

    keyword = "SkipPUAPacked";
    ui->skipPUAPackedRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUAPackedRadioButon->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAPWTool";
    ui->skipPUAPWToolRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUAPWToolRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUANetTool";
    ui->skipPUANetToolRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUANetToolRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAP2P";
    ui->skipPUAP2PRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUAP2PRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAIRC";
    ui->skipPUAIRCRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUAIRCRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUARAT";
    ui->skipPUARATRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUARATRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUANetToolSpy";
    ui->skipPUANetToolSpyRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUANetToolSpyRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAServer";
    ui->skipPUAServerRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUAServerRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

    keyword = "SkipPUAScript";
    ui->skipPUAScriptRadioButton->setChecked(setupFile->getSectionBoolValue(section,keyword));
    ui->loadPUAScriptRadioButton->setChecked(!setupFile->getSectionBoolValue(section,keyword));

}

void includeExcludeOptions::slot_enablePUACheckBoxClicked(){
    ui->PUAFrame->setEnabled(ui->enablePUACheckBox->isChecked());
    slot_writeSettings();
}
