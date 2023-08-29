#include "therad_convo.h"
#include <QDebug>



therad_convo::therad_convo(const QString &filePath, QObject *parent)
    : QThread(parent), filePath(filePath) {}

void therad_convo::run() {
    QFile file(filePath);
     if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QTextStream in(&file);
         while (!in.atEnd()) {
             QString chunk = in.read(1024); // Read 1 KB chunk
             emit chunkLoaded(chunk);
         }
         file.close();
     }




}
