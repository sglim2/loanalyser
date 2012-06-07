#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void amortization();
    double amortization(double P, double n, double r);
    void drawLoanGraph();
    void drawInterestPayoffGraph();
    void drawExtraPaymentGraph();

private slots:
    void on_termMonths_horizontalSlider_sliderMoved(int position);
    void on_interestRate_horizontalSlider_sliderMoved(int position);
    void on_loanAmount_horizontalSlider_sliderMoved(int position);
    void on_termYears_horizontalSlider_sliderMoved(int position);
    void on_termYears_lineEdit_textEdited(const QString &arg1);
    void on_termMonths_lineEdit_textEdited(const QString &arg1);
    void on_interestRate_lineEdit_textEdited(const QString &arg1);
    void on_loanAmouont_lineEdit_textEdited(const QString &arg1);
    void on_extraMonthly_horizontalSlider_sliderMoved(int position);
    void on_extraQuarterly_horizontalSlider_sliderMoved(int position);
    void on_extraHalfYearly_horizontalSlider_sliderMoved(int position);
    void on_extraAnnually_horizontalSlider_sliderMoved(int position);
    void on_extraBiYearly_horizontalSlider_sliderMoved(int position);
    void on_extraFiveYearly_horizontalSlider_sliderMoved(int position);

    void on_extraMonthly_lineEdit_textEdited(const QString &arg1);

    void on_extraQuarterly_lineEdit_textEdited(const QString &arg1);

    void on_extraHalfYearly_lineEdit_textEdited(const QString &arg1);

    void on_extraAnnually_lineEdit_textEdited(const QString &arg1);

    void on_extraBiYearly_lineEdit_textEdited(const QString &arg1);

    void on_extraFiveYearly_lineEdit_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;

    double LoanAmount;
    double interestRateAPR;
    double interestRateMonth;
    int    termMonths;
    int    termMonthsReal; // the point at which the loan payment reaches zero

    double totalInterest;
    double A; // amortization

    double extraMonthlyPayment;
    double extraQuarterlyPayment;
    double extraHalfYearlyPayment;
    double extraAnnualPayment;
    double extraBiYearlyPayment;
    double extraFiveYearlyPayment;

    //std::vector<double> monthlyInterest;
    //std::vector<double> monthlyPayoff;
    //std::vector<double> monthlyLoanAmount;
    //std::vector<double> months;

    double * dmonthlyInterest;
    double * dmonthlyPayoff;
    double * dextraMonthlyPayoff;
    double * dmonthlyLoanAmount;
    double * dmonths;
};

#endif // MAINWINDOW_H
