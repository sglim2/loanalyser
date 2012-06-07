#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <iostream>
#include <vector>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up initial values
    ui->termYears_horizontalSlider->setValue(25); // 25 years
    ui->termYears_lineEdit->setText("25");
    ui->termMonths_horizontalSlider->setValue(0); // 0 months
    ui->termMonths_lineEdit->setText("0");
    termMonths = ui->termYears_horizontalSlider->value()*12 + ui->termMonths_horizontalSlider->value();
    ui->interestRate_horizontalSlider->setValue(439); // 4.39%
    ui->interestRate_lineEdit->setText("4.39");
    interestRateAPR = ui->interestRate_horizontalSlider->value()/100./100.;
    interestRateMonth = interestRateAPR/12.;
    ui->loanAmount_horizontalSlider->setValue(167); // 150,000
    ui->loanAmouont_lineEdit->setText("167000");
    LoanAmount = ui->loanAmount_horizontalSlider->value()*1000;

    extraMonthlyPayment = 0.;
    ui->extraMonthly_lineEdit->setText("0");
    extraQuarterlyPayment = 0.;
    ui->extraQuarterly_lineEdit->setText("0");
    extraHalfYearlyPayment = 0.;
    ui->extraHalfYearly_lineEdit->setText("0");
    extraAnnualPayment = 0.;
    ui->extraAnnually_lineEdit->setText("0");
    extraBiYearlyPayment = 0.;
    ui->extraBiYearly_lineEdit->setText("0");
    extraFiveYearlyPayment = 0.;
    ui->extraFiveYearly_lineEdit->setText("0");

    amortization();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::amortization()
{
//    monthlyInterest.resize(termMonths);
//    monthlyPayoff.resize(termMonths);
//    monthlyLoanAmount.resize(termMonths);
//    months.resize(termMonths);

    dmonthlyInterest = new double[termMonths];
    dmonthlyPayoff = new double[termMonths];
    dextraMonthlyPayoff = new double[termMonths];
    dmonthlyLoanAmount = new double[termMonths];
    dmonths = new double[termMonths];

    totalInterest = 0;

    double P = LoanAmount;
    double n = termMonths;
    termMonthsReal = n;
    double r = interestRateMonth;
    double mypow=pow((1+r),n);
    A = P*( r*mypow/(mypow-1) );

    int i=0;
    while (i<n){
        dmonthlyInterest[i]=P*r;
        dmonthlyPayoff[i]=A-dmonthlyInterest[i];
        dextraMonthlyPayoff[i]=0.;

        // extra payments
        dextraMonthlyPayoff[i]+=extraMonthlyPayment;
        if (i%3==0 && i!=0 ){
            dextraMonthlyPayoff[i]+=extraQuarterlyPayment;
        }
        if (i%6==0 && i!=0 ){
            dextraMonthlyPayoff[i]+=extraHalfYearlyPayment;
        }
        if (i%12==0 && i!=0 ){
            dextraMonthlyPayoff[i]+=extraAnnualPayment;
        }
        if (i%24==0 && i!=0 ){
            dextraMonthlyPayoff[i]+=extraBiYearlyPayment;
        }
        if (i%60==0 && i!=0 ){
            dextraMonthlyPayoff[i]+=extraFiveYearlyPayment;
        }

        dmonthlyLoanAmount[i] = P;
        dmonths[i] = i;

        P = P - dmonthlyPayoff[i] - dextraMonthlyPayoff[i];
        if (P<0){
            termMonthsReal = i;
            break;
        }
        totalInterest += dmonthlyInterest[i];
        i++;

    }

    char * textTotalInterest = new char[16];
    sprintf(textTotalInterest,"%8.2f",totalInterest);
    ui->totalInterest_lineEdit->setText(textTotalInterest);

    char * textA = new char[16];
    sprintf(textA,"%4.2f",A);
    ui->monthlyPaymeny_lineEdit->setText(textA);

    // out data has been updated, so replot the graphs
    drawLoanGraph();
    drawInterestPayoffGraph();
    drawExtraPaymentGraph();

}

void MainWindow::drawLoanGraph()
{
    ui->loanAmount_qwtPlot->setAxisTitle(QwtPlot::xBottom, "Repayment Month" );
    ui->loanAmount_qwtPlot->setAxisTitle(QwtPlot::yLeft, "Money" );
    ui->loanAmount_qwtPlot->setTitle("Remaining Loan Amount by Month");

    static QwtPlotCurve *plot = new QwtPlotCurve("loanAmount");

    QColor colour(Qt::magenta   );
    colour.setAlpha(200);

    QBrush bgBrush;
    bgBrush.setStyle(Qt::SolidPattern);
    bgBrush.setColor(QColor(120,120,140));

    // copy the data into the curves
    plot->setRawSamples( dmonths, dmonthlyLoanAmount, termMonthsReal );
    plot->attach(ui->loanAmount_qwtPlot);
    plot->setPen(colour);
    plot->setBrush(colour);

    ui->loanAmount_qwtPlot->setCanvasBackground(bgBrush);
    ui->loanAmount_qwtPlot->setAxisAutoScale(1);
    ui->loanAmount_qwtPlot->replot();
}

void MainWindow::drawInterestPayoffGraph()
{
    ui->interestPayoff_qwtPlot->setAxisTitle(QwtPlot::xBottom, "Repayment Month" );
    ui->interestPayoff_qwtPlot->setAxisTitle(QwtPlot::yLeft, "Money" );
    ui->interestPayoff_qwtPlot->setTitle("Amortization Interest and Payoff Amounts by Month");

    static QwtPlotCurve *plot1 = new QwtPlotCurve("interestAmount");
    static QwtPlotCurve *plot2 = new QwtPlotCurve("PayoffAmount");

    QColor colour1(Qt::red);
    colour1.setAlpha(200);
    QColor colour2(Qt::green);
    colour2.setAlpha(200);

    QBrush bgBrush;
    bgBrush.setStyle(Qt::SolidPattern);
    bgBrush.setColor(QColor(120,120,140));

    // copy the data into the curves
    plot1->setRawSamples( dmonths, dmonthlyInterest, termMonthsReal );
    plot1->attach(ui->interestPayoff_qwtPlot);
    plot1->setPen(colour1);
    plot1->setBrush(colour1);

    // copy the data into the curves
    plot2->setRawSamples( dmonths, dmonthlyPayoff, termMonthsReal );
    plot2->attach(ui->interestPayoff_qwtPlot);
    plot2->setPen(colour2);
    plot2->setBrush(colour2);

    ui->interestPayoff_qwtPlot->setCanvasBackground(bgBrush);
    ui->interestPayoff_qwtPlot->setAxisAutoScale(1);
    ui->interestPayoff_qwtPlot->replot();
}

void MainWindow::drawExtraPaymentGraph()
{
    ui->extraPayment_qwtPlot->setAxisTitle(QwtPlot::xBottom, "Repayment Month" );
    ui->extraPayment_qwtPlot->setAxisTitle(QwtPlot::yLeft, "Money" );
    ui->extraPayment_qwtPlot->setTitle("Non-Compulsory Extra Payment by Month");

    static QwtPlotCurve *plot = new QwtPlotCurve("extraPaymentAmount");

    QColor colour(Qt::blue);
    colour.setAlpha(200);

    QBrush bgBrush;
    bgBrush.setStyle(Qt::SolidPattern);
    bgBrush.setColor(QColor(120,120,140));

    // copy the data into the curves
    plot->setRawSamples( dmonths, dextraMonthlyPayoff, termMonthsReal );
    plot->attach(ui->extraPayment_qwtPlot);
    plot->setPen(colour);
    plot->setBrush(colour);

    ui->extraPayment_qwtPlot->setCanvasBackground(bgBrush);
    ui->extraPayment_qwtPlot->setAxisAutoScale(1);
    ui->extraPayment_qwtPlot->replot();
}
double MainWindow::amortization(double P, double n, double r)
{
    double mypow=pow((1+r),n);
    return P*( r*mypow/(mypow-1) );
}

void MainWindow::on_termYears_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:35] (int position)
 * corresponding directly to term in integer years (months taken care of separately).
 */
{
    termMonths = position*12 + ui->termMonths_horizontalSlider->value();
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->termYears_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_termYears_lineEdit_textEdited(const QString &arg1)
{
    int newTermYears = atoi(arg1.toLatin1().data());
    ui->termYears_horizontalSlider->setValue(newTermYears);
    termMonths = ui->termYears_horizontalSlider->value()*12 + ui->termMonths_horizontalSlider->value();

    amortization();
}

void MainWindow::on_termMonths_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:11] (int position)
 * corresponding directly to term in Months, remaining from integer years.
 */
{
    termMonths = position + ui->termYears_horizontalSlider->value()*12;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->termMonths_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_termMonths_lineEdit_textEdited(const QString &arg1)
{
    int newTermMonths = atoi(arg1.toLatin1().data());
    ui->termMonths_horizontalSlider->setValue(newTermMonths);
    termMonths = ui->termYears_horizontalSlider->value()*12 + ui->termMonths_horizontalSlider->value();

    amortization();
}

void MainWindow::on_interestRate_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:2000] (int position)
 * interestAPR is given as position/100/100.
 */
{
    interestRateAPR = position/100./100.;
    interestRateMonth = interestRateAPR/12.;
    char * positionText = new char[16];
    sprintf(positionText,"%4.2f",interestRateAPR*100);
    ui->interestRate_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_interestRate_lineEdit_textEdited(const QString &arg1)
{
    double newInterestRateAPR = atof(arg1.toLatin1().data())/100.;
    ui->interestRate_horizontalSlider->setValue(newInterestRateAPR*100.*100.);
    interestRateAPR = ui->interestRate_horizontalSlider->value()/100./100;
    interestRateMonth = interestRateAPR/12.;

    amortization();
}

void MainWindow::on_loanAmount_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:300] (int position)
 * corresponding to loan value in thousands.
 */
{
   LoanAmount = position*1000;
   char * positionText = new char[16];
   sprintf(positionText,"%d",position*1000);
   ui->loanAmouont_lineEdit->setText(positionText);

   amortization();
}


void MainWindow::on_loanAmouont_lineEdit_textEdited(const QString &arg1)
{
    double newLoanAmount = atof(arg1.toLatin1().data())/1000.;
    ui->loanAmount_horizontalSlider->setValue(newLoanAmount);
    LoanAmount = ui->loanAmount_horizontalSlider->value()*1000;

    amortization();
}

void MainWindow::on_extraMonthly_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:1000] (int position)
 * corresponding directly to extra loan payment.
 */
{
    extraMonthlyPayment = position;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->extraMonthly_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_extraMonthly_lineEdit_textEdited(const QString &arg1)
{
    double newExtraMonthlyPayment = atof(arg1.toLatin1().data());
    ui->extraMonthly_horizontalSlider->setValue(newExtraMonthlyPayment);
    extraMonthlyPayment = ui->extraMonthly_horizontalSlider->value();

    amortization();
}

void MainWindow::on_extraQuarterly_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:1000] (int position)
 * corresponding directly to extra loan payment.
 */
{
    extraQuarterlyPayment = position;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->extraQuarterly_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_extraQuarterly_lineEdit_textEdited(const QString &arg1)
{
    double newExtraQuarterlyPayment = atof(arg1.toLatin1().data());
    ui->extraQuarterly_horizontalSlider->setValue(newExtraQuarterlyPayment);
    extraQuarterlyPayment = ui->extraQuarterly_horizontalSlider->value();

    amortization();
}

void MainWindow::on_extraHalfYearly_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:1000] (int position)
 * corresponding directly to extra loan payment.
 */
{
    extraHalfYearlyPayment = position;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->extraHalfYearly_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_extraHalfYearly_lineEdit_textEdited(const QString &arg1)
{
    double newExtraHalfYearlyPayment = atof(arg1.toLatin1().data());
    ui->extraHalfYearly_horizontalSlider->setValue(newExtraHalfYearlyPayment);
    extraHalfYearlyPayment = ui->extraHalfYearly_horizontalSlider->value();

    amortization();
}

void MainWindow::on_extraAnnually_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:1000] (int position)
 * corresponding directly to extra loan payment.
 */
{
    extraAnnualPayment = position;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->extraAnnually_lineEdit->setText(positionText);

    amortization();
}


void MainWindow::on_extraAnnually_lineEdit_textEdited(const QString &arg1)
{
    double newExtraAnnualPayment = atof(arg1.toLatin1().data());
    ui->extraAnnually_horizontalSlider->setValue(newExtraAnnualPayment);
    extraAnnualPayment = ui->extraAnnually_horizontalSlider->value();

    amortization();
}

void MainWindow::on_extraBiYearly_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:1000] (int position)
 * corresponding directly to extra loan payment.
 */
{
    extraBiYearlyPayment = position;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->extraBiYearly_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_extraBiYearly_lineEdit_textEdited(const QString &arg1)
{
    double newExtraBiYearlyPayment = atof(arg1.toLatin1().data());
    ui->extraBiYearly_horizontalSlider->setValue(newExtraBiYearlyPayment);
    extraBiYearlyPayment = ui->extraBiYearly_horizontalSlider->value();

    amortization();
}

void MainWindow::on_extraFiveYearly_horizontalSlider_sliderMoved(int position)
/*
 * Slider has valid range [0:1000] (int position)
 * corresponding directly to extra loan payment.
 */
{
    extraFiveYearlyPayment = position;
    char * positionText = new char[16];
    sprintf(positionText,"%d",position);
    ui->extraFiveYearly_lineEdit->setText(positionText);

    amortization();
}

void MainWindow::on_extraFiveYearly_lineEdit_textEdited(const QString &arg1)
{
    double newExtraFiveYearlyPayment = atof(arg1.toLatin1().data());
    ui->extraFiveYearly_horizontalSlider->setValue(newExtraFiveYearlyPayment);
    extraFiveYearlyPayment = ui->extraFiveYearly_horizontalSlider->value();

    amortization();
}
