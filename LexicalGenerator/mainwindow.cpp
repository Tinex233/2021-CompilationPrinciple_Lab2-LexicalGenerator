#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nfa.h"
#include "dfa.h"
#include "regex.h"
#include "gendfapro.h"

enum MODEL{_NFA,_DFA,_PROGRAM,_MINDFA};//用于解释函数show显示的内容是哪项

int NumOfRegex=0,CurIndex;//表达式总数，当前所显示信息的索引
MODEL CurStatus=_NFA;//当前状态
vector<QString> content;//输入内容
vector<string> regex;//后缀正则表达式组
vector<NFAGraph> NFAgroup;//NFA组
vector<DFAGraph> DFAgroup;//DFA组
vector<DFAGraph> MDFAgroup;//最小化DFA组
vector<string> LEXprogram;//分析程序

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("词法自动生成器");
    setWindowIcon(QIcon(":/new/prefix1/LGlogo.ico"));//设置程序图标
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    ui->LEFTbtn->setDisabled(1);
    ui->RIGHTbtn->setDisabled(1);
    ui->rexCont->setText("");

    REGEX::initPriNum();//使用前先初始化优先数
}

MainWindow::~MainWindow()
{
    delete ui;
}

void reset()//重置
{
    content.clear();
    regex.clear();
    NFAgroup.clear();
    DFAgroup.clear();
    MDFAgroup.clear();
    LEXprogram.clear();
    NumOfRegex=0;
    CurIndex=0;
}

void MainWindow::showRes()//显示结果
{
    if(NumOfRegex==0)
        return;

    //控制正则表达式的显示内容，防止过长
    if(content[CurIndex].length()<=20)
        ui->rexCont->setText(content[CurIndex]);
    else
        ui->rexCont->setText(content[CurIndex].mid(0,19)+"...");

    //鼠标悬停时展示所有内容
    ui->rexCont->setToolTip(content[CurIndex]);


    if(CurStatus==_NFA)
    {
        ui->NFAwidget->clear();
        int nsize=NFAgroup[CurIndex].getNumOfchar();//获取字符集大小
        int stanum=NFAgroup[CurIndex].getNumOfStatus();//获取状态数
        ui->NFAwidget->setRowCount(stanum);
        ui->NFAwidget->setColumnCount(nsize);


        //设置列表头，为初始状态标记“（-）”，接受状态标记“（+）”
        QStringList vheader;
        int inits=NFAgroup[CurIndex].getInitState(),accs=NFAgroup[CurIndex].getAcceptState();
        for(int i=0;i<NFAgroup[CurIndex].getNumOfStatus();i++)
        {
            QString qstr=QString::number(i+1);
            if(i==NFAgroup[CurIndex].getInitState()-1)
                qstr+="(-)";
            else if(i==accs-1)
                qstr+="(+)";
            vheader.append(qstr);
        }
        ui->NFAwidget->setVerticalHeaderLabels(vheader);

        //添加行表头
        QStringList header;
        bool f=false;
        for(auto c:NFAgroup[CurIndex].getCharSet())
        {
            if(c!='#')
                header.append(QString(c));
            else
                f=true;
        }
        if(f)
            header.append("#");

        QList<QString>::Iterator it = header.begin(),itend = header.end();
         for (int i = 0;it != itend; it++,i++)
         {
             for(int j=0;j<stanum;j++)
             {
                 string str=NFAgroup[CurIndex].getStateTrans(j+1,(*it).at(0).toLatin1());
                 if(str!="")
                     ui->NFAwidget->setItem(j, i, new QTableWidgetItem(QString::fromLocal8Bit(str.c_str())));
             }
         }

         if(f)
         {
             header.pop_back();
             header.push_back("ε");
         }
         ui->NFAwidget->setHorizontalHeaderLabels(header);

         //设置不可编辑
         ui->NFAwidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

         //完整显示表格内容
         ui->NFAwidget->resizeColumnsToContents();
    }
    else if(CurStatus==_DFA)
    {
        ui->DFAwidget->clear();
        vector<set<edge>> ml=DFAgroup[CurIndex].getGraph();
        set<char> chs=DFAgroup[CurIndex].getCharSet();
        int snum=DFAgroup[CurIndex].getNumOfStates(),chnum=chs.size();//状态数，字符数
        ui->DFAwidget->setRowCount(snum);
        ui->DFAwidget->setColumnCount(chnum);

        QStringList vheader,hheader;
        for(auto c:chs)
        {
            string st="";
            st+=c;
            hheader.append(QString::fromStdString(st));
        }

        for(int i=1;i<=snum;i++)
        {
            QString qstr=QString::fromLocal8Bit(DFAgroup[CurIndex].getState(i).c_str());
            qstr="{"+qstr+"}";
            if(i==DFAgroup[CurIndex].getInitState())
                qstr+="(-)";
            if(DFAgroup[CurIndex].acceptState(i))
                qstr+="(+)";
            vheader.append(qstr);

            for(auto l:ml[i])
            {
                int col=0;
                char ch=l.CvType;
                QString qs=QString::fromLocal8Bit(DFAgroup[CurIndex].getState(l.vj).c_str());
                qs="{"+qs+"}";

                for(auto cc:chs)
                {
                    if(cc==ch)
                        break;
                    col++;
                }
                ui->DFAwidget->setItem(i-1,col,new QTableWidgetItem(qs));
            }

        }

        ui->DFAwidget->setVerticalHeaderLabels(vheader);
        ui->DFAwidget->setHorizontalHeaderLabels(hheader);

        //设置不可编辑
        ui->DFAwidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //完整显示表格内容
        ui->DFAwidget->resizeColumnsToContents();
    }
    else if(CurStatus==_MINDFA)//和上一个方法基本相同
    {
        ui->MDFAwidget->clear();
        vector<set<edge>> ml=MDFAgroup[CurIndex].getGraph();
        set<char> chs=MDFAgroup[CurIndex].getCharSet();
        int snum=MDFAgroup[CurIndex].getNumOfStates(),chnum=chs.size();//状态数，字符数
        ui->MDFAwidget->setRowCount(snum);
        ui->MDFAwidget->setColumnCount(chnum);

        QStringList vheader,hheader;
        for(auto c:chs)
        {
            string st="";
            st+=c;
            hheader.append(QString::fromStdString(st));
        }

        for(int i=1;i<=snum;i++)
        {
            QString qstr=QString::fromLocal8Bit(MDFAgroup[CurIndex].getState(i).c_str());
            if(i==MDFAgroup[CurIndex].getInitState())
                qstr+="(-)";
            if(MDFAgroup[CurIndex].acceptState(i))
                qstr+="(+)";
            vheader.append(qstr);

            for(auto l:ml[i])
            {
                int col=0;
                char ch=l.CvType;
                QString qs=QString::fromLocal8Bit(MDFAgroup[CurIndex].getState(l.vj).c_str());

                for(auto cc:chs)
                {
                    if(cc==ch)
                        break;
                    col++;
                }
                ui->MDFAwidget->setItem(i-1,col,new QTableWidgetItem(qs));
            }

        }

        ui->MDFAwidget->setVerticalHeaderLabels(vheader);
        ui->MDFAwidget->setHorizontalHeaderLabels(hheader);

        //设置不可编辑
        ui->MDFAwidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //完整显示表格内容
        ui->MDFAwidget->resizeColumnsToContents();
    }
    else
    {
        ui->pgEdit->clear();
        ui->pgEdit->setText(QString::fromLocal8Bit(LEXprogram[CurIndex].c_str()));
    }
}

void MainWindow::NFA_clicked()
{
    CurIndex=0;
    CurStatus=_NFA;
    ui->LEFTbtn->setDisabled(1);
    if(NumOfRegex>1)
        ui->RIGHTbtn->setEnabled(1);
    showRes();
}

void MainWindow::DFA_clicked()
{
    CurIndex=0;
    CurStatus=_DFA;
    ui->LEFTbtn->setDisabled(1);
    if(NumOfRegex>1)
        ui->RIGHTbtn->setEnabled(1);
    showRes();
}

void MainWindow::MinDFA_clicked()
{
    CurIndex=0;
    CurStatus=_MINDFA;
    ui->LEFTbtn->setDisabled(1);
    if(NumOfRegex>1)
        ui->RIGHTbtn->setEnabled(1);
    showRes();
}

void MainWindow::PG_clicked()
{
    CurIndex=0;
    CurStatus=_PROGRAM;
    ui->LEFTbtn->setDisabled(1);
    if(NumOfRegex>1)
        ui->RIGHTbtn->setEnabled(1);
    showRes();
}



void MainWindow::on_CREATEbtn_clicked()//生成按钮
{
    if(ui->regexEdit->document()->isEmpty())
    {
        QMessageBox::information(this,"提示","内容为空！",QMessageBox::Ok);
        return;
    }

    reset();

    int lc=ui->regexEdit->document()->lineCount();


    for(int i=0,l=0;i<lc&&l<lc;l++)
    {
        QString curstr = ui->regexEdit->document()->findBlockByLineNumber(l).text();

        if(curstr==""||curstr=="\0"||curstr=="\n")//跳过空串
            continue;

        NumOfRegex++;
        content.push_back(curstr);
        regex.push_back(content[i].toStdString());

        REGEX tool;
        //判断是否合法
        if(!tool.postfix(regex[i],regex[i]))
         {
            QMessageBox::information(this,"警告","正则表达式或输入格式存在错误！\n\n"
                                               "提示：\n"
                                               "    1.请检查是否有符号错误；\n"
                                               "    2.请按行输入正则表达式；\n"
                                               "    3.每行正则表达式中不应出现空格、Tab等。",QMessageBox::Ok);
            return;
         }

         //添加连接符号后
         //cout<<REGEX::addOP(regex[i])<<endl;

         //转后缀后
         //cout<<regex[i]<<endl;

         //构造NFA
         NFAgroup.push_back(NFAGraph(regex[i]));
         //qDebug()<<"NFA构造完成";

        //查看ε-闭包
//        vector<set<int>> v=NFAgroup[i].getClosure();
//        qDebug()<<v.size();
//        for(int t=1;t<v.size();t++)
//        {
//            qDebug()<<t<<":";
//            for(auto j:v[t])
//                qDebug()<<j<<" ";
//            qDebug()<<"\n";
//        }

        //构造DFA
        DFAgroup.push_back(DFAGraph(NFAgroup[i]));
        //qDebug()<<"DFA构造完成";

        //构造最小化DFA
        MDFAgroup.push_back(DFAgroup[i].toMinDFA());
        //qDebug()<<"MDFA构造完成";

        //生成词法分析程序
        LEXprogram.push_back(GenDFAProgram(MDFAgroup[i]));

        i++;
    }

    ui->LEFTbtn->setDisabled(1);
    NumOfRegex>1?ui->RIGHTbtn->setEnabled(1):ui->RIGHTbtn->setDisabled(1);

    showRes();

}

void MainWindow::on_SAVEbtn_clicked()//保存表达式按钮
{
    SaveFile(ui->regexEdit);
}

void MainWindow::on_savePGbtn_clicked()//保存分析程序
{
    SaveFile(ui->pgEdit);
}

void MainWindow::SaveFile(QTextEdit *txted)//保存TextEdit内容为文本文件
{
    if(txted->document()->isEmpty())
    {
        QMessageBox::information(this,"提示","内容为空！",QMessageBox::Ok);
        return;
    }

    //以文本格式保存正则表达式内容
    QFileDialog dlg(this);

    //获取内容的保存路径
    QString fileName = dlg.getSaveFileName(this, tr("保存为"), "./", tr("Text File(*.txt)"));

    if( fileName == "" )
        return;

    //内容保存到路径文件
    QFile file(fileName);

    //以文本方式打开
    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file); //IO设备对象的地址对其进行初始化

        out << txted->toPlainText(); //输出

        QMessageBox::warning(this, tr("提示"), tr("保存文件成功！"));

        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("错误"), tr("保存文件失败！"));
    }
}

void MainWindow::on_OPENbtn_clicked()
{
    ui->regexEdit->clear();

    //打开并读取文件
    QFileDialog* f = new QFileDialog(this);
    f->setWindowTitle("选择txt文件*.txt");
    f->setNameFilter("*.txt");
    f->setViewMode(QFileDialog::Detail);

    QString filePath;
    if(f->exec() == QDialog::Accepted)
        filePath = f->selectedFiles()[0];

    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream readStream(&file);
        readStream.setCodec("UTF-8");//设置文件流编码方式

        while(!readStream.atEnd())
        {
            ui->regexEdit->append(readStream.readLine());
        }
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("未打开文件或文件打开失败！"));
        return;
    }

}

void MainWindow::on_LEFTbtn_clicked()
{
    CurIndex--;
    if(CurIndex==0)
        ui->LEFTbtn->setDisabled(1);  
    showRes();
    ui->RIGHTbtn->setEnabled(1);
}

void MainWindow::on_RIGHTbtn_clicked()
{
    CurIndex++;
    if(CurIndex==NumOfRegex-1)
        ui->RIGHTbtn->setDisabled(1);
    showRes();
    ui->LEFTbtn->setEnabled(1);
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    switch (index)
    {
        case 0:
            CurStatus=_NFA;
            break;
        case 1:
            CurStatus=_DFA;
            break;
        case 2:
            CurStatus=_MINDFA;
            break;
        case 3:
            CurStatus=_PROGRAM;
            break;
        default:
            CurStatus=_NFA;
    }
    showRes();
}
