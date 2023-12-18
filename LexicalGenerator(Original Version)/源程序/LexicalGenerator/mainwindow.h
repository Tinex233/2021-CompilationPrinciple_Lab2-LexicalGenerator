#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTextEdit>
#include <QDebug>
#include <QTextBlock>
#include <QStringList>
#include <QList>
#include <QTextCodec>
#include <QTableWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_NFAbtn_clicked();

    void on_DFAbtn_clicked();

    void on_MinDFAbtn_clicked();

    void on_PGbtn_clicked();


    void on_CREATEbtn_clicked();

    void on_SAVEbtn_clicked();

    void on_savePGbtn_clicked();

    void on_OPENbtn_clicked();

    void on_LEFTbtn_clicked();

    void on_RIGHTbtn_clicked();

private:
    Ui::MainWindow *ui;

    void SaveFile(QTextEdit *txted);

    void showRes();
};
#endif // MAINWINDOW_H
