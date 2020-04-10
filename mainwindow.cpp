//Written by Adham Nour

#include "mainwindow.h"
#include "ui_mainwindow.h"

const double DT = 0.01;      //used to make plots look better


using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ProcessTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->ProcessNameBox->setEnabled(false);
    ui->ArrivalTimeBox->setEnabled(false);
    ui->RequiredTimeBox->setEnabled(false);
    ui->PriorityBox->setEnabled(false);
    ui->TimeQuantumBox->setEnabled(false);
    ui->AddProcessBtn->setEnabled(false);
    ui->RemoveProcessBtn->setEnabled(false);
    ui->DisplayBtn->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AddProcessBtn_clicked()
{
    if(ui->ProcessNameBox->text() != "")
    {
         ui->ProcessTable->insertRow(ui->ProcessTable->rowCount());
         ui->ProcessTable->setItem(ui->ProcessTable->rowCount()-1, 0,new QTableWidgetItem(ui->ProcessNameBox->text()));
         ui->ProcessTable->setItem(ui->ProcessTable->rowCount()-1, 1,new QTableWidgetItem(QString().setNum(ui->ArrivalTimeBox->value())));
         ui->ProcessTable->setItem(ui->ProcessTable->rowCount()-1, 2,new QTableWidgetItem(QString().setNum(ui->RequiredTimeBox->value())));
         ui->ProcessTable->setItem(ui->ProcessTable->rowCount()-1, 3,new QTableWidgetItem(QString().setNum(ui->PriorityBox->value())));

         for(int i=1; i<4; i++)
            ui->ProcessTable->item(ui->ProcessTable->rowCount()-1, i)->setTextAlignment(Qt::AlignHCenter);

        PList.push_back(Process(noOfProceses++,ui->ProcessNameBox->text(),ui->ArrivalTimeBox->value(),ui->RequiredTimeBox->value(),ui->PriorityBox->value()));

        ui->ProcessNameBox->setText("");
        ui->ArrivalTimeBox->setValue(0);
        ui->RequiredTimeBox->setValue(1);
        ui->PriorityBox->setValue(0);

        ui->DisplayBtn->setEnabled(true);
        ui->RemoveProcessBtn->setEnabled(true);
    }

}

void MainWindow::on_RemoveProcessBtn_clicked()
{
    ui->ProcessTable->removeRow(ui->ProcessTable->rowCount()-1);
    PList.pop_back();
    noOfProceses--;

    if(noOfProceses == 0)
        ui->RemoveProcessBtn->setEnabled(false);

    if(ui->ProcessTable->rowCount() == 0)
        ui->DisplayBtn->setEnabled(false);
}

void MainWindow::on_SchedualSelector_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Select Scheduling Type")
    {
        ui->ProcessNameBox->setEnabled(false);
        ui->ArrivalTimeBox->setEnabled(false);
        ui->RequiredTimeBox->setEnabled(false);
        ui->PriorityBox->setEnabled(false);
        ui->TimeQuantumBox->setEnabled(false);
        ui->AddProcessBtn->setEnabled(false);
        ui->RemoveProcessBtn->setEnabled(false);
        ui->DisplayBtn->setEnabled(false);

        ui->PriorityBox->setValue(0);
    }
    else
    {
        ui->ProcessNameBox->setEnabled(true);
        ui->ArrivalTimeBox->setEnabled(true);
        ui->RequiredTimeBox->setEnabled(true);
        ui->PriorityBox->setEnabled(false);
        ui->TimeQuantumBox->setEnabled(false);
        ui->AddProcessBtn->setEnabled(true);
        ui->RemoveProcessBtn->setEnabled(true);
        ui->DisplayBtn->setEnabled(true);

        ui->PriorityBox->setValue(0);
    }
    if(arg1 == "Round Robin")
        ui->TimeQuantumBox->setEnabled(true);
    if(arg1 == "Priority (Preemptive)" || arg1 == "Priority (Nonpreemptive)")
        ui->PriorityBox->setEnabled(true);
    if(ui->ProcessTable->rowCount() == 0)
        ui->DisplayBtn->setEnabled(false);
}

void MainWindow::on_DisplayBtn_clicked()
{
    vector<Process> PL = PList;
    sort(PL.begin(),PL.end(),CompArrivalTime);
    QLineSeries *series = new QLineSeries();


    //Calculate the Graph According to the Schedualing Algorithm Chosen

    if(ui->SchedualSelector->currentIndex() == 1)   //First Come First Served - John Bahaa
    {
        queue <Process> Queue;
        series->append(0,0);

        for(float t=0; !Queue.empty() || !PL.empty() ; t+=DT)
        {
            //Check if a Process needs to enter the Queue
            while(!PL.empty())
            {
                if(PL[0].getArrivalTime() > t)
                    break;
                else
                    Queue.push(PopFirst(PL));
            }

            //decrease the remaining time of the first process by 1
            if(Queue.empty())
            {
                series->append(t,0);
                series->append(t+DT,0);
            }
            else
            {
                Queue.front().decReqTime(DT);
                series->append(t, Queue.front().getPID()+1);
                series->append(t+DT,Queue.front().getPID()+1);
            }

            //if remaining time == 0 remove process
            if(!Queue.empty() && Queue.front().getRequiredTime() == 0)
                Queue.pop();
        }
    }
    else if(ui->SchedualSelector->currentIndex() == 2)   //Shortest Job First (Preemptive)
    {
        vector <Process> Queue;

        for(float t=0; !Queue.empty() || !PL.empty() ; t+=DT)
        {
            //Check if a Process needs to enter the Queue
           while(!PL.empty())
            {
                if(PL[0].getArrivalTime() > t)
                    break;
                else
                {
                    Queue.push_back(PopFirst(PL));
                    sort(Queue.begin(),Queue.end(),CompReqTime);
                }
            }

            //decrease the remaining time of the first process by 1
            if(Queue.empty())
            {
                series->append(t, 0);
                series->append(t+DT, 0);
            }
            else
            {
                Queue.front().decReqTime(DT);
                series->append(t, Queue.front().getPID()+1);
                series->append(t+DT,Queue.front().getPID()+1);
            }

            //if remaining time == 0 remove process
            if(!Queue.empty() && Queue.front().getRequiredTime() == 0)
                PopFirst(Queue);
        }
    }
    else if(ui->SchedualSelector->currentIndex() == 3)   //Shortest Job First (nonPreemptive)
    {
        deque <Process> Queue;

        for(double t=0; !Queue.empty() || !PL.empty()  ; t+=DT)
        {
            //Check if a Process needs to enter the Queue
            vector<Process> tempV;
            while(!PL.empty())
            {
                if(PL[0].getArrivalTime() > t)
                    break;
                else
                {
                    tempV.push_back(PopFirst(PL));
                }
            }
            sort(tempV.begin(),tempV.end(),CompRequiredTime());
            int tempSize = tempV.size();
            for(int i=0; i< tempSize; i++)
                Queue.push_back(PopFirst(tempV));

            //decrease the remaining time of the first process by 1 unit
            if(Queue.empty())
            {
                series->append(t, 0);
                series->append(t+DT,0);
            }
            else
            {
                for(double i=0; i < Queue.front().requiredTime; i+=DT)
                {
                    series->append(t, Queue.front().pid+1);
                    series->append(t+DT, Queue.front().pid+1);
                    t+=DT;
                }
                t-=DT;
                Queue.pop_front();
            }
        }
    }
    else if(ui->SchedualSelector->currentIndex() == 4)   //Priority (Preemptive)
    {
        vector <Process> Queue;

        for(float t=0; !Queue.empty() || !PL.empty() ; t+=DT)
        {
            //Check if a Process needs to enter the Queue
           while(!PL.empty())
            {
                if(PL[0].getArrivalTime() > t)
                    break;
                else
                {
                    Queue.push_back(PopFirst(PL));
                    sort(Queue.begin(),Queue.end(),CompPri);
                }
            }

            //decrease the remaining time of the first process by 1
            if(Queue.empty())
            {
                series->append(t,0);
                series->append(t+DT,0);
            }
            else
            {
                Queue.front().decReqTime(DT);
                series->append(t, Queue.front().getPID()+1);
                series->append(t+DT,Queue.front().getPID()+1);
            }

            //if remaining time == 0 remove process
            if(!Queue.empty() && Queue.front().getRequiredTime() == 0)
                PopFirst(Queue);
        }
    }
    else if(ui->SchedualSelector->currentIndex() == 5)   //Priority (nonPreemptive)
    {
        deque <Process> Queue;

        for(double t=0; !Queue.empty() || !PL.empty()  ; t+=DT)
        {
            //Check if a Process needs to enter the Queue
            vector<Process> tempV;
            while(!PL.empty())
            {
                if(PL[0].getArrivalTime() > t)
                    break;
                else
                {
                    tempV.push_back(PopFirst(PL));
                }
            }
            sort(tempV.begin(),tempV.end(),CompPriority());
            int tempSize = tempV.size();
            for(int i=0; i< tempSize; i++)
                Queue.push_back(PopFirst(tempV));

            //decrease the remaining time of the first process by 1 unit
            if(Queue.empty())
            {
                series->append(t, 0);
                series->append(t+DT,0);
            }
            else
            {
                for(double i=0; i < Queue.front().requiredTime; i+=DT)
                {
                    series->append(t, Queue.front().pid+1);
                    series->append(t+DT, Queue.front().pid+1);
                    t+=DT;
                }
                t-=DT;
                Queue.pop_front();
            }
        }
    }
    else if(ui->SchedualSelector->currentIndex() == 6)   //Round Robin - John Bahaa
    {
        queue <Process> Queue;

        for(float t=0; !Queue.empty() || !PL.empty() ; t+=DT)
        {
            //Check if a Process needs to enter the Queue
            while(!PL.empty())
            {
                if(PL[0].getArrivalTime() > t)
                    break;
                else
                    Queue.push(PopFirst(PL));
            }

            //decrease the remaining time of the first process by 1
            if(Queue.empty())
            {
                series->append(t,0);
                series->append(t+DT,0);
            }
            else
            {
                for(float i=0; i<ui->TimeQuantumBox->value() && i<Queue.front().getRequiredTime(); i+=DT)
                {
                    series->append(t, Queue.front().getPID()+1);
                    series->append(t+DT,Queue.front().getPID()+1);
                    t+=DT;
                }
                t-=DT;
                Queue.front().decReqTime(ui->TimeQuantumBox->value());

                //if remaining time == 0 remove process
                if(Queue.front().getRequiredTime() == 0)
                    Queue.pop();
                else
                {
                    Process P = Queue.front();
                    Queue.pop();
                    Queue.push(P);
                }
            }
        }
    }


    //Plot the Chart

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Gantt Chart");

    QStringList categories;
    categories << "No Process";
    for(size_t pid=0; pid<PList.size(); pid++)
        categories << (QString::number(PList[pid].getPID()) + ") " + PList[pid].getName());

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chart->createDefaultAxes();
    chart->setAxisY(axisY,series);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTickType(QValueAxis::TicksDynamic);

    if(TotalTime(PList) <= 1)
        axisX->setTickInterval(0.1);
    else if(TotalTime(PList) <= 5)
        axisX->setTickInterval(0.5);
    else if(TotalTime(PList) <= 10)
        axisX->setTickInterval(1);
    else if(TotalTime(PList) <= 50)
        axisX->setTickInterval(5);
    else if(TotalTime(PList) <= 100)
        axisX->setTickInterval(10);
    else if(TotalTime(PList) <= 500)
        axisX->setTickInterval(50);
    else if(TotalTime(PList) <= 1000)
        axisX->setTickInterval(100);
    else if(TotalTime(PList) <= 5000)
        axisX->setTickInterval(500);
    else
        axisX->setTickInterval(1000);

    chart->setAxisX(axisX,series);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800,600);
    window.show();

    //wait for exit chart
    QEventLoop loop;
    connect(&window, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    delete series;
    delete axisX;
    delete axisY;
    delete chartView;

}
