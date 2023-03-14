#include "profilemanager.h"
#include "ui_profilemanager.h"

ProfileManager::ProfileManager(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager)
{
    setupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/settings.ini");
    ui->setupUi(this);
    getProfileList();
    slot_readProfileSettings();

}

ProfileManager::~ProfileManager()
{
    delete ui;
}

void ProfileManager::getProfileList(){
QStringList profiles = setupFile->getKeywords("Profiles");
QStringList selectableProfiles;

    foreach(QString profile,profiles){
        setupFileHandler * sf = new setupFileHandler(QDir::homePath() + "/.clamav-gui/profiles/" + profile + ".ini");
        if (sf->getSectionValue(profile,"Directories") != "") selectableProfiles << profile;
    }

    ui->profileComboBox->clear();
    ui->profileComboBox->addItems(profiles);
}

void ProfileManager::slot_readProfileSettings(){
QString profileName = ui->profileComboBox->currentText();
setupFileHandler * tempSetupFile = new setupFileHandler(QDir::homePath() + "/.clamav-gui/profiles/" + profileName + ".ini");
QStringList targets;
QString targetLabel;
QStringList options;
QString optionLabel;
QString logFile = tempSetupFile->getSectionValue("Directories","ScanReportToFile");

    targets = tempSetupFile->getSectionValue(profileName,"Directories").split('\n');
    options = tempSetupFile->getKeywords("SelectedOptions");

    if ((targets[0] != "") &(targets.count() > 0)) targetLabel = targetLabel + targets[0];
    for (int i = 1; i < targets.count(); i++){
        if ((targets[i] != "") & (targetLabel != "")){
            targetLabel = targetLabel + "\n" + targets[i];
        } else {
            if (targets[i] != "") targetLabel = targets[i];
        }
    }

    ui->targetsListLabel->setText(targetLabel);

    if (options.count() > 0) optionLabel = optionLabel + options[0];
    for (int i = 1; i < options.count(); i++){
        optionLabel = optionLabel + "\n" + options[i];
    }
    if (tempSetupFile->getSectionBoolValue(profileName,"Recursion") == true){
        if (optionLabel != ""){
            optionLabel = optionLabel + "\n" + "-r";
        } else {
            optionLabel = "-r";
        }
    }

    optionLabel = optionLabel.replace("<equal>","=");
    ui->optionsListLabel->setText(optionLabel);

    if (logFile.left(logFile.indexOf("|")) == "checked"){
        logFile = logFile.mid(logFile.indexOf("|") + 1);
    } else {
        logFile = "";
    }

    ui->filenameTextLabel->setText(logFile);
}

void ProfileManager::slot_addProfileButtonClicked(){

    profileWizard = new ProfileWizardDialog(this);
    connect(profileWizard,SIGNAL(signal_profileSaved()),this,SLOT(slot_profileSaved()));
    profileWizard->setModal(true);
    profileWizard->show();
}

void ProfileManager::slot_editProfileButtonClicked(){
QString profileName = ui->profileComboBox->currentText();

    if (profileName != ""){
        profileWizard = new ProfileWizardDialog(this,profileName);
        connect(profileWizard,SIGNAL(signal_profileSaved()),this,SLOT(slot_profileSaved()));
        profileWizard->setModal(true);
        profileWizard->show();
        slot_readProfileSettings();
    }
}

void ProfileManager::slot_eraseProfileButtonClicked(){
QString profileName = ui->profileComboBox->currentText();
QStringList scanJobs = setupFile->getKeywords("ScanJobs");
QString line;
QString logfileName;
bool found = false;

    if (profileName != ""){
        foreach(QString scanJob,scanJobs){
            line = setupFile->getSectionValue("ScanJobs",scanJob);
            if (line.indexOf("|"+profileName+"|") != -1) found = true;
        }
        if (found == false){
            int rc = QMessageBox::question(this,tr("WARNING"),tr("Do you realy want to remove this (") + profileName + tr(") profile"),QMessageBox::Yes,QMessageBox::No);
            QFile tempFile(QDir::homePath() + "/.clamav-gui/profiles/" + ui->profileComboBox->currentText() + ".ini");
                if (rc == QMessageBox::Yes){
                    setupFileHandler * sf = new setupFileHandler(QDir::homePath() + "/.clamav-gui/profiles/" + ui->profileComboBox->currentText() + ".ini");
                    logfileName = sf->getSectionValue("Directories","ScanReportToFile").mid(sf->getSectionValue("Directories","ScanReportToFile").indexOf("|") + 1);
                    if (logfileName != "" ){
                        if (QMessageBox::question(this,tr("Info"),tr("There is a log-file associated with this profile. Shall I remove the log-file as well?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
                            QFile fileRemover(logfileName);
                            fileRemover.remove();
                        }
                    }
                    setupFile->removeKeyword("Profiles",ui->profileComboBox->currentText());
                    tempFile.remove();
                    QMessageBox::information(this,tr("Info"),tr("Profile \"") + ui->profileComboBox->currentText() + tr("\" removed"));
                    getProfileList();
                    slot_readProfileSettings();
                    emit triggerProfilesChanged();
                }
        } else {
            QMessageBox::information(this,tr("ERROR"),tr("Profile can not be removed because it is in use by the scheduler!"));
        }
    }
}

void ProfileManager::slot_profileSaved(){
    emit triggerProfilesChanged();
    getProfileList();
}
