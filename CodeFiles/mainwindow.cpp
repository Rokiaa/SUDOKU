#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTimer>

static int Time;
static bool ShowUnlock;
static std::vector<std::vector<QPushButton*>> buttons(9);
static SudokuSolver solver(3);
static Index idx;
static bool enabled[9][9];
static QTimer* timer;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitButtons();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::Clock));
    timer->start(1000);

    on_NewBoard_clicked();
}

void MainWindow::Clock()
{
    Time++;

    int t = Time;
    QString sec, min, hour;

    sec = QString::number(t % 60);
    if (sec.size() == 1)
        sec = "0" + sec;
    t /= 60;

    min = QString::number(t % 60);
    if (min.size() == 1)
        min = "0" + min;
    t /= 60;

    hour = QString::number(t % 60);
    if (hour.size() == 1)
        hour = "0" + hour;

    ui->counter->setText(hour + ":" + min + ":" + sec);
}

void MainWindow::Refresh()
{
    if (solver.board.board[idx.r][idx.c])
        buttons[idx.r][idx.c]->setText(QString('0' + solver.board.board[idx.r][idx.c]));
    else
        buttons[idx.r][idx.c]->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RefreshAll()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (solver.board.board[i][j])
                buttons[i][j]->setText(QString('0' + solver.board.board[i][j]));
            else
                buttons[i][j]->setText("");
}

void MainWindow::Click()
{
    static SelectNum s;

    s.Init(solver.board.board[idx.r][idx.c], idx, solver.board);
    s.exec();

    solver.board.UnsetCell(idx);
    if (s.val)
        solver.board.SetCell(idx, s.val);

    Refresh();

    timer->start(1000);

    if (solver.Solved())
    {
        ui->message->setStyleSheet("QLabel{color: green;}");
        ui->message->setText("Congratulations!");
        timer->stop();
    }
    else if (!solver.Possible())
    {
        ui->message->setStyleSheet("QLabel{color: red;}");
        ui->message->setText("Invalid State...");
    }
    else
        ui->message->clear();

    ui->solveTime->clear();
}

void MainWindow::ResetTime()
{
    Time = 0;
    ui->counter->setText("00:00:00");
}

void MainWindow::on_NewBoard_clicked()
{
    solver.Clear();
    solver.SetRandomCells(ui->KeepCount->value());

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            enabled[i][j] = !solver.board.board[i][j];
    on_cont_clicked();

    ShowUnlock = true;
    ui->unlock->setVisible(true);

    RefreshAll();
    ui->message->clear();
    ui->solveTime->clear();
    ResetTime();
    timer->start(1000);
}

void MainWindow::on_Solve_clicked()
{
    timer->stop();

    if (solver.Solved())
    {
        ui->message->setStyleSheet("QLabel{color: red;}");
        ui->message->setText("Already Solved...");
    }
    else if (solver.Solve())
    {
        RefreshAll();
        ui->message->clear();
        ui->solveTime->setText("Solved in " + QString::number(solver.GetDuration().count(), 'f', 5) + " seconds.");
    }
    else
    {
        ui->message->setStyleSheet("QLabel{color: red;}");
        ui->message->setText("Can't Be Solved...");
    }
}

void MainWindow::on_Reset_clicked()
{
    ResetTime();
    solver.Clear();
    EnableAll();
    on_cont_clicked();
    RefreshAll();
    ui->message->clear();
    ui->solveTime->clear();
    ui->unlock->setVisible(false);
}

void MainWindow::on_pause_clicked()
{
    timer->stop();

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            buttons[i][j]->setEnabled(false);

    if (ShowUnlock)
        ui->unlock->setVisible(false);
}

void MainWindow::on_cont_clicked()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            buttons[i][j]->setEnabled(enabled[i][j]);

    if (ShowUnlock)
        ui->unlock->setVisible(true);

    timer->start(1000);
}

void MainWindow::EnableAll()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            enabled[i][j] = true;
}

void MainWindow::DisableAll()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            enabled[i][j] = false;
}

void MainWindow::on_unlock_clicked()
{
    EnableAll();
    ShowUnlock = false;

    on_cont_clicked();
    ui->unlock->setVisible(false);
}

void MainWindow::InitButtons()
{
    buttons[0].push_back(ui->b00);
    buttons[0].push_back(ui->b01);
    buttons[0].push_back(ui->b02);
    buttons[0].push_back(ui->b03);
    buttons[0].push_back(ui->b04);
    buttons[0].push_back(ui->b05);
    buttons[0].push_back(ui->b06);
    buttons[0].push_back(ui->b07);
    buttons[0].push_back(ui->b08);
    buttons[1].push_back(ui->b10);
    buttons[1].push_back(ui->b11);
    buttons[1].push_back(ui->b12);
    buttons[1].push_back(ui->b13);
    buttons[1].push_back(ui->b14);
    buttons[1].push_back(ui->b15);
    buttons[1].push_back(ui->b16);
    buttons[1].push_back(ui->b17);
    buttons[1].push_back(ui->b18);
    buttons[2].push_back(ui->b20);
    buttons[2].push_back(ui->b21);
    buttons[2].push_back(ui->b22);
    buttons[2].push_back(ui->b23);
    buttons[2].push_back(ui->b24);
    buttons[2].push_back(ui->b25);
    buttons[2].push_back(ui->b26);
    buttons[2].push_back(ui->b27);
    buttons[2].push_back(ui->b28);
    buttons[3].push_back(ui->b30);
    buttons[3].push_back(ui->b31);
    buttons[3].push_back(ui->b32);
    buttons[3].push_back(ui->b33);
    buttons[3].push_back(ui->b34);
    buttons[3].push_back(ui->b35);
    buttons[3].push_back(ui->b36);
    buttons[3].push_back(ui->b37);
    buttons[3].push_back(ui->b38);
    buttons[4].push_back(ui->b40);
    buttons[4].push_back(ui->b41);
    buttons[4].push_back(ui->b42);
    buttons[4].push_back(ui->b43);
    buttons[4].push_back(ui->b44);
    buttons[4].push_back(ui->b45);
    buttons[4].push_back(ui->b46);
    buttons[4].push_back(ui->b47);
    buttons[4].push_back(ui->b48);
    buttons[5].push_back(ui->b50);
    buttons[5].push_back(ui->b51);
    buttons[5].push_back(ui->b52);
    buttons[5].push_back(ui->b53);
    buttons[5].push_back(ui->b54);
    buttons[5].push_back(ui->b55);
    buttons[5].push_back(ui->b56);
    buttons[5].push_back(ui->b57);
    buttons[5].push_back(ui->b58);
    buttons[6].push_back(ui->b60);
    buttons[6].push_back(ui->b61);
    buttons[6].push_back(ui->b62);
    buttons[6].push_back(ui->b63);
    buttons[6].push_back(ui->b64);
    buttons[6].push_back(ui->b65);
    buttons[6].push_back(ui->b66);
    buttons[6].push_back(ui->b67);
    buttons[6].push_back(ui->b68);
    buttons[7].push_back(ui->b70);
    buttons[7].push_back(ui->b71);
    buttons[7].push_back(ui->b72);
    buttons[7].push_back(ui->b73);
    buttons[7].push_back(ui->b74);
    buttons[7].push_back(ui->b75);
    buttons[7].push_back(ui->b76);
    buttons[7].push_back(ui->b77);
    buttons[7].push_back(ui->b78);
    buttons[8].push_back(ui->b80);
    buttons[8].push_back(ui->b81);
    buttons[8].push_back(ui->b82);
    buttons[8].push_back(ui->b83);
    buttons[8].push_back(ui->b84);
    buttons[8].push_back(ui->b85);
    buttons[8].push_back(ui->b86);
    buttons[8].push_back(ui->b87);
    buttons[8].push_back(ui->b88);
}

void MainWindow::on_b00_clicked()
{
    idx = {0, 0};
    Click();
}

void MainWindow::on_b01_clicked()
{
    idx = {0, 1};
    Click();
}

void MainWindow::on_b02_clicked()
{
    idx = {0, 2};
    Click();
}

void MainWindow::on_b03_clicked()
{
    idx = {0, 3};
    Click();
}

void MainWindow::on_b04_clicked()
{
    idx = {0, 4};
    Click();
}

void MainWindow::on_b05_clicked()
{
    idx = {0, 5};
    Click();
}

void MainWindow::on_b06_clicked()
{
    idx = {0, 6};
    Click();
}

void MainWindow::on_b07_clicked()
{
    idx = {0, 7};
    Click();
}

void MainWindow::on_b08_clicked()
{
    idx = {0, 8};
    Click();
}

void MainWindow::on_b10_clicked()
{
    idx = {1, 0};
    Click();
}

void MainWindow::on_b11_clicked()
{
    idx = {1, 1};
    Click();
}

void MainWindow::on_b12_clicked()
{
    idx = {1, 2};
    Click();
}

void MainWindow::on_b13_clicked()
{
    idx = {1, 3};
    Click();
}

void MainWindow::on_b14_clicked()
{
    idx = {1, 4};
    Click();
}

void MainWindow::on_b15_clicked()
{
    idx = {1, 5};
    Click();
}

void MainWindow::on_b16_clicked()
{
    idx = {1, 6};
    Click();
}

void MainWindow::on_b17_clicked()
{
    idx = {1, 7};
    Click();
}

void MainWindow::on_b18_clicked()
{
    idx = {1, 8};
    Click();
}

void MainWindow::on_b20_clicked()
{
    idx = {2, 0};
    Click();
}

void MainWindow::on_b21_clicked()
{
    idx = {2, 1};
    Click();
}

void MainWindow::on_b22_clicked()
{
    idx = {2, 2};
    Click();
}

void MainWindow::on_b23_clicked()
{
    idx = {2, 3};
    Click();
}

void MainWindow::on_b24_clicked()
{
    idx = {2, 4};
    Click();
}

void MainWindow::on_b25_clicked()
{
    idx = {2, 5};
    Click();
}

void MainWindow::on_b26_clicked()
{
    idx = {2, 6};
    Click();
}

void MainWindow::on_b27_clicked()
{
    idx = {2, 7};
    Click();
}

void MainWindow::on_b28_clicked()
{
    idx = {2, 8};
    Click();
}

void MainWindow::on_b30_clicked()
{
    idx = {3, 0};
    Click();
}

void MainWindow::on_b31_clicked()
{
    idx = {3, 1};
    Click();
}

void MainWindow::on_b32_clicked()
{
    idx = {3, 2};
    Click();
}

void MainWindow::on_b33_clicked()
{
    idx = {3, 3};
    Click();
}

void MainWindow::on_b34_clicked()
{
    idx = {3, 4};
    Click();
}

void MainWindow::on_b35_clicked()
{
    idx = {3, 5};
    Click();
}

void MainWindow::on_b36_clicked()
{
    idx = {3, 6};
    Click();
}

void MainWindow::on_b37_clicked()
{
    idx = {3, 7};
    Click();
}

void MainWindow::on_b38_clicked()
{
    idx = {3, 8};
    Click();
}

void MainWindow::on_b40_clicked()
{
    idx = {4, 0};
    Click();
}

void MainWindow::on_b41_clicked()
{
    idx = {4, 1};
    Click();
}

void MainWindow::on_b42_clicked()
{
    idx = {4, 2};
    Click();
}

void MainWindow::on_b43_clicked()
{
    idx = {4, 3};
    Click();
}

void MainWindow::on_b44_clicked()
{
    idx = {4, 4};
    Click();
}

void MainWindow::on_b45_clicked()
{
    idx = {4, 5};
    Click();
}

void MainWindow::on_b46_clicked()
{
    idx = {4, 6};
    Click();
}

void MainWindow::on_b47_clicked()
{
    idx = {4, 7};
    Click();
}

void MainWindow::on_b48_clicked()
{
    idx = {4, 8};
    Click();
}

void MainWindow::on_b50_clicked()
{
    idx = {5, 0};
    Click();
}

void MainWindow::on_b51_clicked()
{
    idx = {5, 1};
    Click();
}

void MainWindow::on_b52_clicked()
{
    idx = {5, 2};
    Click();
}

void MainWindow::on_b53_clicked()
{
    idx = {5, 3};
    Click();
}

void MainWindow::on_b54_clicked()
{
    idx = {5, 4};
    Click();
}

void MainWindow::on_b55_clicked()
{
    idx = {5, 5};
    Click();
}

void MainWindow::on_b56_clicked()
{
    idx = {5, 6};
    Click();
}

void MainWindow::on_b57_clicked()
{
    idx = {5, 7};
    Click();
}

void MainWindow::on_b58_clicked()
{
    idx = {5, 8};
    Click();
}

void MainWindow::on_b60_clicked()
{
    idx = {6, 0};
    Click();
}

void MainWindow::on_b61_clicked()
{
    idx = {6, 1};
    Click();
}

void MainWindow::on_b62_clicked()
{
    idx = {6, 2};
    Click();
}

void MainWindow::on_b63_clicked()
{
    idx = {6, 3};
    Click();
}

void MainWindow::on_b64_clicked()
{
    idx = {6, 4};
    Click();
}

void MainWindow::on_b65_clicked()
{
    idx = {6, 5};
    Click();
}

void MainWindow::on_b66_clicked()
{
    idx = {6, 6};
    Click();
}

void MainWindow::on_b67_clicked()
{
    idx = {6, 7};
    Click();
}

void MainWindow::on_b68_clicked()
{
    idx = {6, 8};
    Click();
}

void MainWindow::on_b70_clicked()
{
    idx = {7, 0};
    Click();
}

void MainWindow::on_b71_clicked()
{
    idx = {7, 1};
    Click();
}

void MainWindow::on_b72_clicked()
{
    idx = {7, 2};
    Click();
}

void MainWindow::on_b73_clicked()
{
    idx = {7, 3};
    Click();
}

void MainWindow::on_b74_clicked()
{
    idx = {7, 4};
    Click();
}

void MainWindow::on_b75_clicked()
{
    idx = {7, 5};
    Click();
}

void MainWindow::on_b76_clicked()
{
    idx = {7, 6};
    Click();
}

void MainWindow::on_b77_clicked()
{
    idx = {7, 7};
    Click();
}

void MainWindow::on_b78_clicked()
{
    idx = {7, 8};
    Click();
}

void MainWindow::on_b80_clicked()
{
    idx = {8, 0};
    Click();
}

void MainWindow::on_b81_clicked()
{
    idx = {8, 1};
    Click();
}

void MainWindow::on_b82_clicked()
{
    idx = {8, 2};
    Click();
}

void MainWindow::on_b83_clicked()
{
    idx = {8, 3};
    Click();
}

void MainWindow::on_b84_clicked()
{
    idx = {8, 4};
    Click();
}

void MainWindow::on_b85_clicked()
{
    idx = {8, 5};
    Click();
}

void MainWindow::on_b86_clicked()
{
    idx = {8, 6};
    Click();
}

void MainWindow::on_b87_clicked()
{
    idx = {8, 7};
    Click();
}

void MainWindow::on_b88_clicked()
{
    idx = {8, 8};
    Click();
}


void MainWindow::on_showMessages_stateChanged(int isSet)
{
    if (isSet)
        ui->message->setVisible(true);
    else
        ui->message->setVisible(false);
}
