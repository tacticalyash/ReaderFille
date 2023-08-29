#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <iomanip>
#include <sstream>
#include <QFileDialog>
#include <QSerialPortInfo>
#include <QElapsedTimer>
#include <QTextStream>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    comboBox = new QComboBox(this);
    QStringList comPorts = getAvailableComPorts();
    comboBox->addItems(comPorts);

    connect(comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onComPortSelected(int)));


    ui->verticalLayout->addWidget(comboBox);




    serialPort.setBaudRate(921600);


    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::handleReadyRead);



    //sendTimer = new QTimer(this);
   // connect(sendTimer, &QTimer::timeout, this, &MainWindow::sendContinuously);
       //connect(thread, &therad_convo::chunkRead, this, &MainWindow::appendToTextEdit);





}

MainWindow::~MainWindow()
{
    if (serialPort.isOpen())
        serialPort.close();
    delete ui;
}




void MainWindow::onComPortSelected(int index)
{

    QString portName = comboBox->itemText(index);

    serialPort.setPortName(portName);
    if (!serialPort.open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this,"INFO","Failed to open serial port!");
    }
}

void MainWindow::on_Send_data_clicked()
{


    read_text();

//    QString filePath = "D:/CHETAN/xds.txt";
//       QFile inputFile(filePath);

//       if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
//       {
//           QString inputText = QString(inputFile.readAll());
//           inputFile.close();

//           inputText = inputText.toUpper();
//           inputText.remove(QChar(' '), Qt::CaseInsensitive);
//           qDebug()<<"read succeed";

         /*  QString formattedOutput;

           QString patternToSearch = "FAFF361B102002";

           const int patternLength = patternToSearch.length() + 50; // Total length is 64

           for (int i = 0; i < inputText.length() - patternLength + 1; i++)
           {
               QString possiblePattern = inputText.mid(i, patternLength);

               if (possiblePattern.startsWith(patternToSearch))
               {
                   formattedOutput += possiblePattern + "\n";
               }
           }

           ui->plainTextEdit->setPlainText(formattedOutput);
       }
       else
       {
           // Handle the case when the file cannot be opened
           qDebug() << "Failed to open the file: " << inputFile.errorString();
       }
*/

}

void MainWindow::sendContinuously()
{



    QByteArray msg = "FAFF361B102002EAC410600400049C4040400CBB7F46003AEBA0003BA98B00FC";
    serialPort.write(msg);
    qDebug()<<"msegade" <<msg;





}


void MainWindow::handleReadyRead()
{


    const int bufferSize = 60000;

    QByteArray dataReceived;

    while (serialPort.bytesAvailable() > 0)
    {
        QByteArray chunk = serialPort.read(bufferSize);
        dataReceived.append(chunk);
    }

    if (!dataReceived.isEmpty())
    {
        QString hexString = dataReceived.toHex().toUpper();
        ui->plainTextEdit_2->appendPlainText(hexString);
    }










}

void MainWindow::format()
{
    union ulf {
        unsigned long ul;
        float rec, rec1, rec2;
    };

    ulf uus, uu1s, uu2s, uu, uu1, uu2;
    QString formattedG, formattedG1, formattedG2, formattedF, formattedF1, formattedF2;



    QString inputData = ui->plainTextEdit->toPlainText();
    QString patternHeader = "FAFF361B";
    static int patternLength = 64;
    QStringList patterns;

    int index = inputData.indexOf(patternHeader);
    while (index != -1 && index + patternLength <= inputData.size()) {
        QString pattern = inputData.mid(index, patternLength);
        patterns.append(pattern);

        index = inputData.indexOf(patternHeader, index + 1);


    }


    QStandardItemModel *model = new QStandardItemModel(patterns.size(), 2, this); // Change column count to 2
    model->setHorizontalHeaderItem(0, new QStandardItem("Packet"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Packet Counter")); // Add new header
    model->setHorizontalHeaderItem(2, new QStandardItem("Sample Time"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Sensor Type"));
    model->setHorizontalHeaderItem(4, new QStandardItem("Size of Sensor"));
    model->setHorizontalHeaderItem(5,new QStandardItem("Ax"));
    model->setHorizontalHeaderItem(6,new QStandardItem("Ay"));
    model->setHorizontalHeaderItem(7,new QStandardItem("Az"));
    model->setHorizontalHeaderItem(8,new QStandardItem("Gx"));
    model->setHorizontalHeaderItem(9,new QStandardItem("Gy"));
    model->setHorizontalHeaderItem(10,new QStandardItem("Gz"));
    model->setHorizontalHeaderItem(11,new QStandardItem("Checksum"));


    for (int row = 0; row < patterns.size(); ++row) {
        QStandardItem *patternItem = new QStandardItem(patterns.at(row));
        model->setItem(row, 0, patternItem);
        QString packetCounter = patternItem->text().mid(14, 4);
        int pacDec = packetCounter.toInt(nullptr, 16);
        QString sampleTime = patternItem->text().mid(25,8);

        int sns = sampleTime.toInt(nullptr, 16);
        QString sensorType = patternItem->text().mid(32, 4);

        if (sensorType == "8040") {
            QString Gx= patternItem->text().mid(38, 8);

            QString Gy = patternItem->text().mid(46, 8);

            QString Gz = patternItem->text().mid(54, 8);


            std::string str = Gx.toStdString();
            std::string str2 = Gy.toStdString();
            std::string str3 = Gz.toStdString();

            std::stringstream ss(str);
            std::stringstream ss1(str2);
            std::stringstream ss2(str3);

            ss >> std::hex >> uus.ul;
            ss1 >> std::hex >> uu1s.ul;
            ss2 >> std::hex >> uu2s.ul;

            float f = uus.rec;

            float f1 = uu1s.rec1;

            float f2 = uu2s.rec2;


            formattedF = QString::number(f, 'f', 7);
            formattedF1 = QString::number(f1, 'f', 7);
            formattedF2 = QString::number(f2, 'f', 7);
            QStandardItem *Gx_col = new QStandardItem(formattedF);
            QStandardItem *Gy_col = new QStandardItem(formattedF1);
            QStandardItem *Gz_col = new QStandardItem(formattedF2);

            model->setItem(row, 8, Gx_col);
            model->setItem(row, 9, Gy_col);
            model->setItem(row, 10, Gz_col);
        } else {
            QString Ax = patternItem->text().mid(38, 8);

            QString Ay = patternItem->text().mid(46, 8);

            QString Az = patternItem->text().mid(54, 8);

            std::string str = Ax.toStdString();
            std::string str2 = Ay.toStdString();
            std::string str3 = Az.toStdString();

            std::stringstream ss(str);
            std::stringstream ss1(str2);
            std::stringstream ss2(str3);

            ss >> std::hex >> uu.ul;
            ss1 >> std::hex >> uu1.ul;
            ss2 >> std::hex >> uu2.ul;

            float g = uu.rec;
            float g1 = uu1.rec1;
            float g2 = uu2.rec2;

            formattedG = QString::number(g, 'f', 7);
            formattedG1 = QString::number(g1, 'f', 7);
            formattedG2 = QString::number(g2, 'f', 7);
            QStandardItem *Ax_col = new QStandardItem(formattedG);
            QStandardItem *Ay_col = new QStandardItem(formattedG1);
            QStandardItem *Az_col = new QStandardItem(formattedG2);

            model->setItem(row, 5, Ax_col);
            model->setItem(row, 6, Ay_col);
            model->setItem(row, 7, Az_col);
        }
        QString checksumNumber = patternItem->text().mid(62, 2);
        QString sizee_sens = patternItem->text().mid(36,2);
        QStandardItem *counterItem = new QStandardItem(QString::number(pacDec));
        model->setItem(row, 1, counterItem); // Add packet counter item
        QStandardItem *sensort = new QStandardItem(QString::number(sns));
        model->setItem(row,2,sensort);
        QStandardItem *sens = new QStandardItem(sensorType);
        model->setItem(row,3,sens);
        QStandardItem *size_sensor = new QStandardItem(sizee_sens);
        model->setItem(row,4,size_sensor);

        QStandardItem *checksum = new QStandardItem(checksumNumber);
        model->setItem(row,11,checksum);
    }


    ui->tableView_unique->setModel(model);



   /* QStandardItemModel *uniqueModel = new QStandardItemModel(0, 11, this);
    uniqueModel->setHorizontalHeaderItem(0, new QStandardItem("Packet Data"));
    uniqueModel->setHorizontalHeaderItem(1, new QStandardItem("Packet Counter"));
    uniqueModel-> setHorizontalHeaderItem(2, new QStandardItem("Sample Time"));
    uniqueModel->setHorizontalHeaderItem(3, new QStandardItem("Sensor Type"));
    uniqueModel->setHorizontalHeaderItem(4, new QStandardItem("Size of Sensor"));
    uniqueModel->setHorizontalHeaderItem(5,new QStandardItem("Ax"));
    uniqueModel->setHorizontalHeaderItem(6,new QStandardItem("Ay"));
    uniqueModel->setHorizontalHeaderItem(7,new QStandardItem("Az"));
    uniqueModel->setHorizontalHeaderItem(8,new QStandardItem("Gx"));
    uniqueModel->setHorizontalHeaderItem(9,new QStandardItem("Gy"));
    uniqueModel->setHorizontalHeaderItem(10,new QStandardItem("Gz"));
    uniqueModel->setHorizontalHeaderItem(11,new QStandardItem("Checksum"));

    QSet<QString> uniqueRows;

    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < model->columnCount(); ++col) {
            rowData << model->index(row, col).data().toString();
        }

        QString rowStr = rowData.join(",");
        if (!uniqueRows.contains(rowStr)) {
            uniqueRows.insert(rowStr);

            QList<QStandardItem*> items;
            for (const QString& itemText : rowData) {
                items.append(new QStandardItem(itemText));
            }
            uniqueModel->appendRow(items);
        }
    }

    ui->tableView_unique->setModel(uniqueModel);*/

}










void MainWindow::on_pushButton_clicked()
{

    if (sendTimer->isActive())
    {
        sendTimer->stop();
    }

    close();

}



void MainWindow::on_TESTT_clicked()
{
    format();

    QMessageBox::information(this,"INFO","Conversion Completed Successfully!");


}

void MainWindow::onPlainTextChanged()
{
    //printFaffPatterns();
}






void MainWindow::printFaffPatterns() {
    QString data = ui->plainTextEdit_2->toPlainText().toUpper();
    QRegularExpression pattern("FFAB[0-9A-F]{60}");
    QRegularExpressionMatchIterator it = pattern.globalMatch(data);
    QString Ax, Ay, Az, Gx, Gy, Gz;
    float f, f1, f2, g, g1, g2;
    QSet<int> uniquePacketCounters;
    QMap<int, QMultiMap<QString, QString>> uniqueRows;

    union ulf {
        unsigned long ul;
        float rec, rec1, rec2;
    };

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString patternString = match.captured();
        QString packetNumber = patternString.mid(15, 4);
        int ZC = packetNumber.toInt(nullptr, 16);


        if (!uniquePacketCounters.contains(ZC)) {
            uniquePacketCounters.insert(ZC);

            QString sampleTime = patternString.mid(25, 8);
            QString sensorType = patternString.mid(33, 4);
            QString sizeOfSensor = patternString.mid(38, 2);
            QString dataCenter = patternString.mid(40, 24);
            QString checksumNumber = patternString.mid(62, 2);

            int VB = sampleTime.toInt(nullptr, 16);

            ulf uus, uu1s, uu2s, uu, uu1, uu2;
            QString formattedG, formattedG1, formattedG2, formattedF, formattedF1, formattedF2;




            if (sensorType == "8040") {
                Gx = dataCenter.mid(0, 8);
                Gy = dataCenter.mid(8, 8);
                Gz = dataCenter.mid(16, 8);

                std::string str = Gx.toStdString();
                std::string str2 = Gy.toStdString();
                std::string str3 = Gz.toStdString();

                std::stringstream ss(str);
                std::stringstream ss1(str2);
                std::stringstream ss2(str3);

                ss >> std::hex >> uus.ul;
                ss1 >> std::hex >> uu1s.ul;
                ss2 >> std::hex >> uu2s.ul;

                f = uus.rec;
                f1 = uu1s.rec1;
                f2 = uu2s.rec2;

                formattedF = QString::number(f, 'f', 7);
                formattedF1 = QString::number(f1, 'f', 7);
                formattedF2 = QString::number(f2, 'f', 7);

            } else {
                Ax = dataCenter.mid(0, 8);
                Ay = dataCenter.mid(8, 8);
                Az = dataCenter.mid(16, 8);

                std::string str = Ax.toStdString();
                std::string str2 = Ay.toStdString();
                std::string str3 = Az.toStdString();

                std::stringstream ss(str);
                std::stringstream ss1(str2);
                std::stringstream ss2(str3);

                ss >> std::hex >> uu.ul;
                ss1 >> std::hex >> uu1.ul;
                ss2 >> std::hex >> uu2.ul;

                g = uu.rec;
                g1 = uu1.rec1;
                g2 = uu2.rec2;

                formattedG = QString::number(g, 'f', 7);
                formattedG1 = QString::number(g1, 'f', 7);
                formattedG2 = QString::number(g2, 'f', 7);

            }



            QMultiMap<QString, QString> rowData;
            rowData.insert("ZC", packetNumber);
            rowData.insert("VB", QString::number(VB));
            rowData.insert("SensorType", sensorType);
            rowData.insert("SizeOfSensor", sizeOfSensor);
            rowData.insert("Gx", sensorType == "8040" ? formattedG : formattedG1);
            rowData.insert("Gy", sensorType == "8040" ? formattedG1 : formattedG);
            rowData.insert("Gz", sensorType == "8040" ? formattedG2 : formattedG2);
            rowData.insert("F", sensorType == "8040" ? formattedF : formattedF1);
            rowData.insert("F1", sensorType == "8040" ? formattedF1 : formattedF);
            rowData.insert("F2", sensorType == "8040" ? formattedF2 : formattedF2);
            rowData.insert("Checksum", checksumNumber);
            rowData.insert("PatternString", patternString);


            uniqueRows.insert(ZC, rowData);
        }
    }


    ui->tableView_unique->clearSelection();


    QMap<int, QMultiMap<QString, QString>>::const_iterator itRows = uniqueRows.constBegin();
    while (itRows != uniqueRows.constEnd()) {
        QMultiMap<QString, QString> rowData = itRows.value();
        QList<QStandardItem *> rowItems;

        rowItems << new QStandardItem(rowData.value("ZC"))
                 << new QStandardItem(rowData.value("VB"))
                 << new QStandardItem(rowData.value("SensorType"))
                 << new QStandardItem(rowData.value("SizeOfSensor"))
                 << new QStandardItem(rowData.value("Gx"))
                 << new QStandardItem(rowData.value("Gy"))
                 << new QStandardItem(rowData.value("Gz"))
                 << new QStandardItem(rowData.value("F"))
                 << new QStandardItem(rowData.value("F1"))
                 << new QStandardItem(rowData.value("F2"))
                 << new QStandardItem(rowData.value("Checksum"))
                 << new QStandardItem(rowData.value("PatternString"));


        model.appendRow(rowItems);

        ++itRows;
    }

    ui->tableView_unique->setModel(&model);



}
QStringList MainWindow::getAvailableComPorts()
{
    QStringList ports;
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        ports.append(portInfo.portName());
    }
    return ports;
}

void MainWindow::on_EXCELL_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save CSV File", "", "CSV Files (*.csv);;All Files (*)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        return;
    }

    QTextStream out(&file);

    QSet<QString> uniquePacketCounters;
    int packetCounterColumn = 0;


    QStringList header;
    for (int col = 0; col < ui->tableView_unique->model()->columnCount(); ++col) {
        header << ui->tableView_unique->model()->headerData(col, Qt::Horizontal).toString();
    }
    out << header.join(",") <<'\n';


    for (int row = 0; row < ui->tableView_unique->model()->rowCount(); ++row) {
        QString packetCounter = ui->tableView_unique->model()->index(row, packetCounterColumn).data().toString();
        if (!uniquePacketCounters.contains(packetCounter)) {
            QStringList rowData;
            for (int col = 0; col < ui->tableView_3->model()->columnCount(); ++col) {
                rowData << ui->tableView_3->model()->index(row, col).data().toString();
            }
            out << rowData.join(",") << '\n';
            uniquePacketCounters.insert(packetCounter);
        }
    }

    file.close();

}
void MainWindow::on_pushButton_2_clicked()
{
//    QString filePath = "D:/CHETAN/25.08.2023_17.txt";
//        therad_convo* fileReaderThread = new therad_convo(filePath, ui->plainTextEdit_2);

//        connect(fileReaderThread, &therad_convo::chunkLoaded, this, &MainWindow::appendText);
//        fileReaderThread->start();



}


void MainWindow::appendText(const QString& chunk) {
//    QString filePath = "D:/CHETAN/xds.txt";
//       QFile inputFile(filePath);

//       if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
//       {
//           QString inputText = QString(inputFile.readAll());
//           inputFile.close();

//           inputText = inputText.toUpper();
//           inputText.remove(QChar(' '), Qt::CaseInsensitive);
//           qDebug()<<"read succeed";
//}
}

//void MainWindow::read_text()
//{
//    QString filePath = "D:/CHETAN/25.08.2023_17.txt";
//    QFile inputFile(filePath);

//    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        const int bufferSize = 5096; // Adjust the buffer size as needed
//        char buffer[bufferSize];

//        while (!inputFile.atEnd())
//        {
//            qint64 bytesRead = inputFile.read(buffer, bufferSize);
//            if (bytesRead > 0)
//            {
//                // Process the buffer or perform your desired operations here
//                // You can print, search for patterns, etc.

//                // For example, printing the buffer content:
//                QString bufferContent = QString::fromUtf8(buffer, bytesRead);
//                qDebug() << bufferContent;
//            }
//        }

//        inputFile.close();

//        qDebug() << "File read successfully.";
//    }
//    else
//    {
//        // Handle the case when the file cannot be opened
//        qDebug() << "Failed to open the file: " << inputFile.errorString();
//    }
//}
void MainWindow::read_text()
{
    QFile inputFile("D:/CHETAN/25.08.2023_17.txt");

        if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open the file: " << inputFile.errorString();
            return;
        }

        const int chunkSize = 100000; // Set the chunk size to 1,000,000 bytes
        char buffer[chunkSize];

        QString totalContent; // To store the entire content of the file

        while (!inputFile.atEnd())
        {
            qint64 bytesRead = inputFile.read(buffer, chunkSize);
            if (bytesRead > 0)
            {
                // Process the buffer or perform your desired operations here
                QString chunkContent;

                for (int i = 0; i < bytesRead; ++i)
                {
                    if (buffer[i] != '\0') // Check if the byte is non-zero
                    {
                        chunkContent.append(buffer[i]);
                    }
                }

                totalContent.append(chunkContent);
            }
            else
            {
                // No data read from the file
                qDebug() << "No data read from the file.";
            }
        }

        // Set the content of the QPlainTextEdit widget with the entire file content
        ui->plainTextEdit_2->setPlainText(totalContent);

        inputFile.close();

}
