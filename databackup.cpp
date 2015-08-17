#include "databackup.h"

const QString DataBackup::PROGRAMS_DIR_NAME = "programs";
const QString DataBackup::PROGRAMS_DIR_PATH = QDir::current().path()
        + QDir::separator() + DataBackup::PROGRAMS_DIR_NAME;
const QString DataBackup::PRGM_FILE_EXT = ".PGM";

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
            + p->getProgramName() + PRGM_FILE_EXT;
    QFile programFile(programFilePath);
    programSaved = programFile.open(QIODevice::WriteOnly);

    //QDataStream outing(&programFile);
    QTextStream outing(&programFile);

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
    QFile pFile(PROGRAMS_DIR_PATH + QDir::separator() + name + DataBackup::PRGM_FILE_EXT );

    return pFile.exists();
}

bool DataBackup::writeStepToFile(Step *step, Program *prgm)
{
    bool isAdded = false;
    QString prgmName = prgm->getProgramName();
    QString path = fileLives(PRGM, prgmName);

    if(path.isEmpty()){
        isAdded = false;
        return isAdded;
    }
    QFile prgmFile(path);
    if(prgmFile.open(QIODevice::Append)){
        QTextStream ts(&prgmFile);
        ts << step->getStepNumber() << " : "
           << QString("%1").arg(step->getTemperature(), 6, 'f', 2, '0') << " : "
           << QString("%1").arg(step->getHumidity(), 4, 'f', 1, '0') << " : "
           << QString("%1").arg(step->getHours()) << " : "
           << QString("%1").arg(step->getMinutes()) << " : "
           << QString("%1").arg(step->getWaiting()) << " : "
           << QString("%1").arg(step->getHR()) << " : "
           << QString("%1").arg(step->getOne()) << " : "
           << QString("%1").arg(step->getTwo()) << " : "
           << QString("%1").arg(step->getThree()) << endl;
        isAdded = true;
    }else{
        isAdded = false;
    }

    return isAdded;
}

//! ************************ END OF PUBLIC FUNCTIONS *************** !//

QString DataBackup::fileLives(File_Type type, QString name)
{
    QString path = "";
    switch (type) {
    case PRGM:
        path = PROGRAMS_DIR_PATH + QDir::separator()
                + name + PRGM_FILE_EXT;
        break;
    case TST_DATA:
        path = "";
        break;
    case SYS_BOOT:
        path = "";
    case SYS_WARN:
        path = "";
        break;
    default:
        break;
    }
    QFile fileToCheck(path);
    if(fileToCheck.exists()){
        return path;
    }else{
        return QString("");
    }
}

//! ************************ PRIVATE FUNCTIONS ********************** !//


//! ************************ END OF PRIVATE FUNCTIONS *************** !//


//! ************** PUBLIC SLOTS ******************* !//

bool DataBackup::createDir(QString dirName){
    bool dirCreated;
    QDir appDir = QDir::current();
    if(!appDir.cd(dirName)){
        dirCreated = appDir.mkdir(dirName);
    }
    return dirCreated;
}
