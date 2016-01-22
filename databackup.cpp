#include "databackup.h"

const QString DataBackup::PROGRAMS_DIR_NAME = "programs";
const QString DataBackup::PRGM_FILE_EXT = ".PGM";
const QString DataBackup::PID_DIR_NAME = "pids";
const QString DataBackup::TXT_FILE_EXT = ".txt";


const QString DataBackup::PROGRAMS_DIR_PATH = QDir::current().path()
        + QDir::separator() + DataBackup::PROGRAMS_DIR_NAME;
const QString DataBackup::PID_DIR_PATH = QDir::current().path()
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
    QFile pFile(PROGRAMS_DIR_PATH + QDir::separator()
                + name + DataBackup::PRGM_FILE_EXT );

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
        ts << QString("%1").arg(step->getTemperature(), 6, 'f', 2, '0') << ":"
           << QString("%1").arg(step->getHumidity(), 4, 'f', 1, '0') << ":"
           << QString("%1").arg(step->getHours()) << ":"
           << QString("%1").arg(step->getMinutes()) << ":"
           << QString("%1").arg(step->getWaiting()) << ":"
           << QString("%1").arg(step->getHR()) << ":"
           << QString("%1").arg(step->getOne()) << ":"
           << QString("%1").arg(step->getTwo()) << ":"
           << QString("%1").arg(step->getThree()) << endl;
        isAdded = true;
    }else{
        isAdded = false;
    }

    return isAdded;
}

void DataBackup::loadTestProgram(QString pgmFileName, Program *prgm)
{
    qDebug() << "DataBackup::loadTestProgram: Entered";
    QString path = fileLives(PRGM, pgmFileName);
    qDebug() << path;

    if(path.isEmpty())
    {
        qDebug() << "DataBackup::loadTestProgram: " << "path is empty";
        return;
    }

    QFile pgmFile(path);
    QTextStream ts(&pgmFile);

    if(!pgmFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "DataBackup::loadTestProgram: Program cant open";
        return;
    }
    QMap<int, Step*> *loadedSteps = new QMap<int, Step*>;
    prgm->getSteps().clear();
    prgm->setProgramName(pgmFileName);
    int stepNo = 0;
    while(!ts.atEnd())
    {
        QString line = ts.readLine();
        qDebug() << line;
        if(line.startsWith('c', Qt::CaseInsensitive)){
            QStringList pParams = line.split(':', QString::SkipEmptyParts);
            prgm->setCycle(pParams.at(1).toInt());
        }else{
            QStringList stepParams = line.split(':', QString::SkipEmptyParts);
            qDebug() << "loadTestProgram: steps split size" << stepParams.size();
            foreach (QString s, stepParams) {
                qDebug() << s ;
            }
            Step *s = new Step();
            s->setStepNumber(stepNo);
            s->setTemperature(stepParams.at(0).toDouble());
            s->setHumidity(stepParams.at(1).toDouble());
            s->setHours(stepParams.at(2).toInt());
            s->setMinutes(stepParams.at(3).toInt());
            s->setWaiting(stepParams.at(4).toInt());
            s->setHR(stepParams.at(5).toInt());
            s->setOne(stepParams.at(6).toInt());
            s->setTwo(stepParams.at(7).toInt());
            s->setThree(stepParams.at(8).toInt());

            loadedSteps->insert(s->getStepNumber(), s);
            stepNo++;
        }        
    }
    qDebug() << "DataBackup::loadProgram: loadedSteps size" << loadedSteps->size();
    prgm->setSteps(*loadedSteps);
//    prgm->setCurrentCycle(1);//TODO: check why this is being set to one
//    prgm->setCurrentStepNum(0);
}

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
    case PID_DATA:
        path = PID_DIR_PATH + QDir::separator()
                + name + TXT_FILE_EXT;
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

bool DataBackup::renameProgram(QString newName, QString oldName)
{
    QString path = fileLives(PRGM, oldName);
    if(path.isEmpty()){
        return false;
    }
    QFile pgmFile(path);

    if(!pgmFile.exists()){
        return false;
    }
    bool renamed = false;
    if(pgmFile.open(QIODevice::ReadWrite)){
        renamed = pgmFile.rename(PROGRAMS_DIR_PATH + QDir::separator()
                                 + newName + PRGM_FILE_EXT);
        pgmFile.close();
    }
    return renamed;
}

bool DataBackup::removeProgram(QString name)
{
    QString path = fileLives(PRGM, name);
    if(path.isEmpty()){
        return false;
    }
    QFile pgmFile(path);
    if(!pgmFile.open(QIODevice::ReadWrite)){
        return false;
    }
    bool removed = pgmFile.remove();

    return removed;
}

void DataBackup::insertPID(QDataStream &stream, PID *pid)
{
    QMap<quint16, PID*> pidList;
    //stream >> pidList;
    pidList.insert(pidList.size(), pid);
    stream << pidList;
}

QList<PID> DataBackup::loadPIDList(int choice)
{
    QString name = "temperature";
    if(choice == 1){
        name = "humidity";
    }
    QList<PID> pidList;
    QDir appDir = QDir::current();
    checkDir(PID_DIR_NAME);
    appDir.cd(PID_DIR_NAME);
    QString path = appDir.path() + QDir::separator()
            + name + TXT_FILE_EXT;
    QFile pidFile(path);
    pidFile.open(QIODevice::ReadWrite);
    QDataStream stream(&pidFile);
    if(pidFile.size() > 0){
        stream >> pidList;
    }
    foreach (PID pid, pidList) {
        qDebug() << "DataBackup::loadPIDList: Default" << pid.getChoosen() << " : " << pid.toString();
    }
    pidFile.close();
    return pidList;
}

void DataBackup::replacePIDList(QList<PID> pidList, int choice)
{
    QString name = "temperature";
    if(choice == 1){
        name = "humidity";
    }
    QDir appDir = QDir::current();
    checkDir(PID_DIR_NAME);
    appDir.cd(PID_DIR_NAME);
    QString path = appDir.path() + QDir::separator()
            + name + TXT_FILE_EXT;
    QFile pidFile(path);
    pidFile.open(QIODevice::ReadWrite);
    QDataStream stream(&pidFile);
    stream.device()->reset();
    stream << pidList;
    pidFile.close();
}


bool DataBackup::createDir(QString dirName){
    bool dirCreated;
    QDir appDir = QDir::current();
    if(!appDir.cd(dirName)){
        dirCreated = appDir.mkdir(dirName);
    }
    return dirCreated;
}

void DataBackup::on_pidFormSubmited(double p, double i, double d, int choice)
{
    QString name = "temperature";
    if(choice == 1){
        name = "humidity";
    }

    PID pid;
    pid.setKp(p);
    pid.setKi(i);
    pid.setKd(d);

    QDir appDir = QDir::current();
    checkDir(PID_DIR_NAME);
    appDir.cd(PID_DIR_NAME);
    QString path = appDir.path() + QDir::separator()
            + name + TXT_FILE_EXT;
    QFile pidFile(path);
    pidFile.open(QIODevice::ReadWrite);
    QDataStream stream(&pidFile);

    QList<PID> pidList;
    if(pidFile.size() > 0){
        stream >> pidList;
    }
    pidList.insert(pidList.size(), pid);
    stream.device()->reset();
    stream << pidList;
    foreach (PID var, pidList) {
        qDebug() << "AFTER: "<< var.getKp();
    }

    qDebug() << "Pid map size: " << pidList.size() << " File Size: "<< pidFile.size();

    pidList.clear();
    pidFile.close();
}


