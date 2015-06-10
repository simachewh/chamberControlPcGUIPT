#ifndef CHAMBER_H
#define CHAMBER_H

#include <QWidget>
/*********************************************************
 ** This class holds properties to represent the blocks in
 ** the message sent from the controllBox
 * */
class Chamber : public QWidget
{
    Q_OBJECT
private:
    char *stx;
    char *ext;
    char *chkdsk;
    char *end;
    char *id1;
    char *id2;
    char *chamberMsg;
public:
    explicit Chamber(QWidget *parent = 0);
    ~Chamber();

signals:

public slots:
};

#endif // CHAMBER_H
