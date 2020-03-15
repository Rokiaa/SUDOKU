#include "selectnum.h"
#include "ui_selectnum.h"

static std::vector<QPushButton*> buttons;

SelectNum::SelectNum(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectNum)
{
    ui->setupUi(this);
    buttons.push_back(ui->b0);
    buttons.push_back(ui->b1);
    buttons.push_back(ui->b2);
    buttons.push_back(ui->b3);
    buttons.push_back(ui->b4);
    buttons.push_back(ui->b5);
    buttons.push_back(ui->b6);
    buttons.push_back(ui->b7);
    buttons.push_back(ui->b8);
    buttons.push_back(ui->b9);
}


void SelectNum::Init(int val, Index& idx, SudokuBoard& board)
{
    this->val = val;

    for (int i = 1; i <= 9; i++)
        if (board.isCandidate(idx, i))
            buttons[i]->setEnabled(true);
        else
            buttons[i]->setEnabled(false);
}

SelectNum::~SelectNum()
{
    delete ui;
}

void SelectNum::on_b0_clicked()
{
    val = 0;
    close();
}

void SelectNum::on_b1_clicked()
{
    val = 1;
    close();
}

void SelectNum::on_b2_clicked()
{
    val = 2;
    close();
}

void SelectNum::on_b3_clicked()
{
    val = 3;
    close();
}

void SelectNum::on_b4_clicked()
{
    val = 4;
    close();
}

void SelectNum::on_b5_clicked()
{
    val = 5;
    close();
}

void SelectNum::on_b6_clicked()
{
    val = 6;
    close();
}

void SelectNum::on_b7_clicked()
{
    val = 7;
    close();
}

void SelectNum::on_b8_clicked()
{
    val = 8;
    close();
}

void SelectNum::on_b9_clicked()
{
    val = 9;
    close();
}
