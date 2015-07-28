#include "databackup.h"

const QString DataBackup::PROGRAMS_DIR_NAME = "programs";
const QString DataBackup::PROGRAMS_DIR_PATH = QDir::current().path()
        + QDir::separator() + DataBackup::PROGRAMS_DIR_NAME;

DataBackup::DataBackup(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(dirNotFound(QString)),
            this, SLOT(createDir(QString)));
}

//! ************** PUBLIC FUNCTIONS *************** !//

void DataBackup::checkDir(QString name){
    QDir appDir = QDir::current();
    if(!appDir.cd(name)){
        emit dirNotFound(name);
    }
}

bool DataBackup::saveProgram(Program *p){
    bool programSaved;
    QDir appDir = QDir::current();
    checkDir(PROGRAMS_DIR_NAME);
    appDir.cd(PROGRAMS_DIR_NAME);
    QString programFilePath = appDir.path() + QDir::separator()
            + p->getProgramName() + ".pgm";
    QFile programFile(programFilePath);
    programSaved = programFile.open(QIODevice::ReadWrite);
    programFile.close();

    return programSaved;
}

QFileInfoList DataBackup::listPrograms(){

}


//! ************** PUBLIC SLOTS ******************* !//

bool DataBackup::createDir(QString dirName){
    bool dirCreated;
    QDir appDir = QDir::current();
    if(!appDir.cd(dirName)){
        dirCreated = appDir.mkdir(dirName);
    }
    return dirCreated;
}
