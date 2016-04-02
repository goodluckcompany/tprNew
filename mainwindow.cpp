#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(calculate()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),ui->doubleSpinBox_7,SLOT(setEnabled(bool)));
    connect(ui->checkBox_2,SIGNAL(toggled(bool)),ui->doubleSpinBox_8,SLOT(setEnabled(bool)));
    connect(ui->checkBox_3,SIGNAL(toggled(bool)),ui->doubleSpinBox_9,SLOT(setEnabled(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

Answer * MainWindow::calculateBayesianDecision(Alternative **_alternatives)
{
    Alternative ** alt = _alternatives;
    double u1,u2; //Функция полезности при принятии решения d1 и d2 соответствено;
    u1 = alt[0]->getProbability()*alt[0]->getValuation()+alt[1]->getProbability()*alt[1]->getValuation();
    u2 = alt[2]->getProbability()*alt[2]->getValuation()+alt[3]->getProbability()*alt[3]->getValuation();

    if(u1 >= u2)
    {
        return new Answer(u1,alt[0]->getDecision());
    }
    else
    {
        return new Answer(u2,alt[2]->getDecision());
    }
}

Answer * MainWindow::calculateMinMaxDecision(Alternative **_alternatives)
{
    Alternative ** alt = _alternatives;
    double u1,u2; //Функция полезности при принятии решения d1 и d2 соответствено;
    u1 = alt[1]->getValuation();
    u2 = alt[3]->getValuation();

    if(u1 >= u2)
    {
        return new Answer(u1,alt[1]->getDecision());
    }
    else
    {
        return new Answer(u2,alt[2]->getDecision());
    }
}

Answer * MainWindow::calculateUsefulnessAccurateInformation(Alternative **_alternatives)
{
    Alternative ** alt = _alternatives;
    double v1 = alt[0]->getValuation();
    double v2 = alt[1]->getValuation();
    double v3 = alt[2]->getValuation();

    double indP = (v2 - v3)/(v1 - v3);
    double u1 = alt[0]->getProbability()*(v1 - v2);
    double u2 = (1 - alt[0]->getProbability())*(v2 - v3);

    if(alt[0]->getProbability() <= indP)
    {
        return new Answer(u1,"");
    }
    else
    {
        return new Answer(u2,"");
    }

}

void MainWindow::calculate()
{

    /*Массива вероятностей события*/

    double probability[4];
    probability[0]  =  ui->doubleSpinBox->value(); // вероятность успеха простого товара
    probability[1] = 1 - probability[0];
    probability[2] = ui->doubleSpinBox_2->value(); // вероятность успеха иновационного товара
    probability[3] = 1 - probability[2];

    /*Переменная для учота дополнительный рисков*/
     double addprob = 1;

    if(ui->doubleSpinBox_7->isEnabled())
        addprob = addprob * ui->doubleSpinBox_7->value();
    if (ui->doubleSpinBox_8->isEnabled())
        addprob = addprob * ui->doubleSpinBox_8->value();
    if (ui->doubleSpinBox_9->isEnabled())
        addprob = addprob * ui->doubleSpinBox_9->value();

    if (addprob != 1)
    {

     probability[2] = probability[2] - addprob;
     probability[3] = 1 - probability[2];
     if (probability[2]<0)
     {
         probability[2] = 0;
         probability[3] = 1;
     }
    }


    /*Массив возможных решений*/
    QString decisions[2];
    decisions[0] = QString(trUtf8("вкладывать средства в разработку нового простого товара"));
    decisions[1] = QString(trUtf8("вкладывать средства в разработку инновационного товара"));

    /*Массива ценности каждой альтернативы*/
    double valuation[4];

    valuation[0] =  ui->doubleSpinBox_3->value();//ценность первой альтернативы
    valuation[1] =  ui->doubleSpinBox_4->value();//ценность второй альтернативы
    valuation[2] =  ui->doubleSpinBox_5->value();//ценность третей альтернативы
    valuation[3] =  ui->doubleSpinBox_6->value();//ценность четвертой альтернативы

    /*Массив альтернатив*/
    Alternative * alernatives[4];
    //успешное вложение в простой товар
    alernatives[0] = new Alternative(decisions[0],probability[0],valuation[0]);

    //неуспешное вложили в простой товар
    alernatives[1] = new Alternative(decisions[0],probability[1],valuation[1]);

    //успешное вложение в иновационный товар
    alernatives[2] = new Alternative(decisions[1],probability[2],valuation[2]);

    //неуспешное вложение в иновационный товар
    alernatives[3] = new Alternative(decisions[1],probability[3],valuation[3]);


    //Произодим расчет Байесовского решения
    Answer * BayesianDecision = calculateBayesianDecision(alernatives);
    //Производим расчет Минимксного решения
    Answer * MinMaxDecision = calculateMinMaxDecision(alernatives);
    Answer * UsefulnessAccurateInformation = calculateUsefulnessAccurateInformation(alernatives);


    //Заполняем поле вывода
    ui->plainTextEdit->setPlainText(trUtf8("Было принято решение: ")+BayesianDecision->getDecision());
    ui->plainTextEdit->appendPlainText(trUtf8("Полезность решения: ")+QString::number(BayesianDecision->getUsefulness()));
    ui->plainTextEdit->appendPlainText(trUtf8("Было принято решение: ")+MinMaxDecision->getDecision());
    ui->plainTextEdit->appendPlainText(trUtf8("Полезность решения: ")+QString::number(MinMaxDecision->getUsefulness()));
    ui->plainTextEdit->appendPlainText(trUtf8("Полезность точной информации: ")+QString::number(UsefulnessAccurateInformation->getUsefulness()));

}
