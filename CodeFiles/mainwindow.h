#pragma once

#include <QMainWindow>
#include "selectnum.h"
#include "SudokuBoard.h"
#include "SudokuSolver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Refresh();
    void Click();
    void Refresh(int i, int j);

    void InitButtons();
    void RefreshAll();
    void EnableAll();
    void DisableAll();
    void UpdateTime();

    void ResetTime();
private slots:

    void Clock();
    void on_b00_clicked();
    void on_b01_clicked();
    void on_b02_clicked();
    void on_b03_clicked();
    void on_b04_clicked();
    void on_b05_clicked();
    void on_b06_clicked();
    void on_b07_clicked();
    void on_b08_clicked();
    void on_b10_clicked();
    void on_b11_clicked();
    void on_b12_clicked();
    void on_b13_clicked();
    void on_b14_clicked();
    void on_b15_clicked();
    void on_b16_clicked();
    void on_b17_clicked();
    void on_b18_clicked();
    void on_b20_clicked();
    void on_b21_clicked();
    void on_b22_clicked();
    void on_b23_clicked();
    void on_b24_clicked();
    void on_b25_clicked();
    void on_b26_clicked();
    void on_b27_clicked();
    void on_b28_clicked();
    void on_b30_clicked();
    void on_b31_clicked();
    void on_b32_clicked();
    void on_b33_clicked();
    void on_b34_clicked();
    void on_b35_clicked();
    void on_b36_clicked();
    void on_b37_clicked();
    void on_b38_clicked();
    void on_b40_clicked();
    void on_b41_clicked();
    void on_b42_clicked();
    void on_b43_clicked();
    void on_b44_clicked();
    void on_b45_clicked();
    void on_b46_clicked();
    void on_b47_clicked();
    void on_b48_clicked();
    void on_b50_clicked();
    void on_b51_clicked();
    void on_b52_clicked();
    void on_b53_clicked();
    void on_b54_clicked();
    void on_b55_clicked();
    void on_b56_clicked();
    void on_b57_clicked();
    void on_b58_clicked();
    void on_b60_clicked();
    void on_b61_clicked();
    void on_b62_clicked();
    void on_b63_clicked();
    void on_b64_clicked();
    void on_b65_clicked();
    void on_b66_clicked();
    void on_b67_clicked();
    void on_b68_clicked();
    void on_b70_clicked();
    void on_b71_clicked();
    void on_b72_clicked();
    void on_b73_clicked();
    void on_b74_clicked();
    void on_b75_clicked();
    void on_b76_clicked();
    void on_b77_clicked();
    void on_b78_clicked();
    void on_b80_clicked();
    void on_b81_clicked();
    void on_b82_clicked();
    void on_b83_clicked();
    void on_b84_clicked();
    void on_b85_clicked();
    void on_b86_clicked();
    void on_b87_clicked();
    void on_b88_clicked();

    void on_NewBoard_clicked();

    void on_Solve_clicked();

    void on_Reset_clicked();

    void on_pause_clicked();

    void on_unlock_clicked();

    void on_cont_clicked();

    void on_showMessages_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

