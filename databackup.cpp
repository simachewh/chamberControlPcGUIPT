#include "databackup.h"

const QString DataBackup::PROGRAMS_DIR_NAME = "programs";
const QString DataBackup::PROGRAMS_DIR_PATH = QDir::current().path()
        + QDir::separator() + DataBackup::PROGRAMS_DIR_NAME;
const QString DataBackup::FILE_EXT = ".PGM";

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
    //! check if programs directory doesn't exist and create it
    checkDir(PROGRAMS_DIR_NAME);
    appDir.cd(PROGRAMS_DIR_NAME);
    QString programFilePath = appDir.path() + QDir::separator()
            + p->getProgramName() + FILE_EXT;
    QFile programFile(programFilePath);
    programSaved = programFile.open(QIODevice::WriteOnly);

    //QDataStream outing(&programFile);
    QTextStream outing(&programFile);

    outing << "Cycle: " + QString::number(p->getCycle()) << endl;
    outing << "Cycle: " + QString::number(p->getCycle()) << endl;

    //QByteArray cBa = "cycle: " + p->getCycle();
    //programFile.write(cBa);
    programFile.close();

    return programSaved;
}

QFileSystemModel * DataBackup::listPrograms(){
    QFileSystemModel *programsListModel = new QFileSystemModel();
    programsListModel->setFilter(QDir::Files);
    programsListModel->setRootPath(DataBackup::PROGRAMS_DIR_PATH);
    return programsListModel;
}

bool DataBackup::programExists(QString name)
{
    QFile pFile(PROGRAMS_DIR_PATH + QDir::separator() + name + DataBackup::FILE_EXT );

    return pFile.exists();
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
