#ifndef THERAD_CONVO_H
#define THERAD_CONVO_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QQueue>
#include <QStringList>
class therad_convo : public QThread
{
    Q_OBJECT
public:
    therad_convo(const QString &filePath, QObject *parent = nullptr);


signals:
    //void chunkRead(QString chunk);
    void chunkLoaded(const QString& chunk);

protected:
    void run() override;

private:
    QString filePath;
         QQueue<double> queue;
};

#endif // THERAD_CONVO_H
