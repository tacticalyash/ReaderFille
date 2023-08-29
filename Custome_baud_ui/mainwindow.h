#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QStandardItemModel>
#include <QRegularExpression>
#include <QDebug>
#include <QTableView>
#include <QComboBox>
#include <QProgressBar>
#include "therad_convo.h"





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);


    void onPlainTextChanged();
    ~MainWindow();

public slots:
//    void appendToTextEdit(QString chunk);
void appendText(const QString& chunk);


private slots:
    void on_Send_data_clicked();
    void sendContinuously();

    //void appendToTextEdit(QString chunk);

    void on_pushButton_clicked();

    void on_TESTT_clicked();
    void printFaffPatterns();
    void onComPortSelected(int index);
    QStringList getAvailableComPorts();

    void on_EXCELL_clicked();
    void handleReadyRead();
    void format();
    void read_text();
    // float hexStringToFloat(const QString &hexString);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    therad_convo *thread;


    QSerialPort serialPort;
    QTimer *sendTimer;
    QStandardItemModel model; // Create the model
    QTableView *tableView;
    QStandardItemModel *tableModel;
    QComboBox *comboBox;
    QByteArray buffer;
    QProgressBar *progressBar;



};

#endif // MAINWINDOW_H
