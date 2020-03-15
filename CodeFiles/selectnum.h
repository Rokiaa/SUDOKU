#pragma once

#include <QDialog>
#include <vector>
#include <set>
#include "SudokuBoard.h"

namespace Ui {
class SelectNum;
}

class SelectNum : public QDialog
{
    Q_OBJECT

public:
    int val = 0;

    explicit SelectNum(QWidget *parent = nullptr);
    void Init(int val, Index& idx, SudokuBoard& board);
    ~SelectNum();

private slots:
    void on_b1_clicked();

    void on_b2_clicked();

    void on_b3_clicked();

    void on_b4_clicked();

    void on_b5_clicked();

    void on_b6_clicked();

    void on_b7_clicked();

    void on_b8_clicked();

    void on_b9_clicked();

    void on_b0_clicked();

private:
    Ui::SelectNum *ui;
};

