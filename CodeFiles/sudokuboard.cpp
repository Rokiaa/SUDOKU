#include "SudokuBoard.h"

SudokuBoard::SudokuBoard(const Board& board, int BoxN)
{
    ResizeBoard(BoxN);
    SetBoard(board);
}

int SudokuBoard::BoxNum(const Index& idx) const
{
    return (idx.r / BoxN * BoxN) + (idx.c / BoxN);
    //  -----------
    // | 0 | 1 | 2 |
    // |---+---+---|
    // | 3 | 4 | 5 | for normal 9 * 9 boards.
    // |---+---+---|
    // | 6 | 7 | 8 |
    //  -----------
}

void SudokuBoard::UpdateAvailable(int CandidatesCount)
{

#if PRINT_DEBUG_FUNCTIONS
    if (print)
    std::cout << "In UpdateAvailable with CandidatesCount = " << CandidatesCount
        << ", CellsWithNCandidates is "
        << (CandidatesCount != -1 && CellsWithNCandidates[CandidatesCount].empty() ? "" : "not ")
        << "empty" << std::endl;
#endif

    if (CandidatesCount != -1)
    {
        if (CellsWithNCandidates[CandidatesCount].empty())
            available.erase(CandidatesCount);
        else
            available.insert(CandidatesCount);

        return;
    }

    for (int i = 1; i <= N; i++)
        UpdateAvailable(i);
}


void SudokuBoard::InsertIdx(int CandidatesCount, const Index& idx)
{

#if PRINT_DEBUG_FUNCTIONS

    std::cout << "In InsertIdx with CandidatesCount = " << CandidatesCount
        << ", idx = { " << idx.r << ", " << idx.c << " }" << std::endl;

#endif

    CellsWithNCandidates[CandidatesCount].insert(idx);
    UpdateAvailable(CandidatesCount);
}

void SudokuBoard::EraseIdx(int CandidatesCount, const Index& idx)
{

#if PRINT_DEBUG_FUNCTIONS

    std::cout << "In EraseIdx with CandidatesCount = " << CandidatesCount
        << ", idx = { " << idx.r << ", " << idx.c << " }" << std::endl;

#endif

    CellsWithNCandidates[CandidatesCount].erase(idx);
    UpdateAvailable(CandidatesCount);
}

bool SudokuBoard::isCandidate(const Index& idx, int num)
{
    return Candidates.find(num) != Candidates.end();
}

bool SudokuBoard::inRow(int r, int num) const
{
    return row[r][num];
}

bool SudokuBoard::inColumn(int c, int num) const
{
    return column[c][num];
}

bool SudokuBoard::inBox(int b, int num) const
{
    return box[b][num];
}

void SudokuBoard::SetBoard(const Board& board, bool clear)
{
    if (clear)
        Clear();

    // TODO: needs validation for input.
    for (int i = 0; i < std::min((int)board.size(), N); i++)
        for (int j = 0; j < std::min((int)board[i].size(), N); j++)
            if (board[i][j])
                SetCell({ i, j }, board[i][j]);
    // only set the cell if it's not empty for two reasons:
    //	- the number 0 is not in the candidates.
    //	- all cells are already empty at the begining. there is no need to empty it again.
}

void SudokuBoard::ResizeBoard(int BoxN, bool keep)
{
    this->BoxN = BoxN;
    N = BoxN * BoxN;

    Board board = keep ? this->board : EmptyBoard;

    Clear();

    // empty cells won't override.
    // only valid cells will be set.
    SetBoard(board);
}

void SudokuBoard::Clear()
{
    // board = N * N vector filled with zeroes (empty).
    board = std::vector < std::vector<int>>(N, std::vector<int>(N, 0));

    candidates = std::vector<std::vector<std::set<int>>>(N, std::vector<std::set<int>>(N));
    // each cell will have all candidates from 1 to N.
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int candidate = 1; candidate <= N; candidate++)
                candidates[i][j].insert(candidate);

    // N + 1 because CellsWithCandidates[N] should be accessible.
    CellsWithNCandidates = std::vector<Container<Index>>(N + 1);

    // all cells will have N candidates at the begining.
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            CellsWithNCandidates[N].insert({ i, j });

    // in an empty board, all cells will have N candidates.
    available.clear();
    available.insert(N);

    // N * (N + 1) vector filled with false.
    // N + 1 because row[N - 1][N] should be accessible.
    row		= std::vector < std::vector<bool> >(N, std::vector<bool>(N + 1, false));
    column	= std::vector < std::vector<bool> >(N, std::vector<bool>(N + 1, false));
    box		= std::vector < std::vector<bool> >(N, std::vector<bool>(N + 1, false));

#if APPLY_PointingClaming

    // at the begining, every cell will have every number as a candidate.
    RowIndices		= std::vector< std::vector< std::set< Index > > >(N, std::vector<std::set<Index>>(N + 1));
    ColumnIndices	= std::vector< std::vector< std::set< Index > > >(N, std::vector<std::set<Index>>(N + 1));
    BoxIndices		= std::vector< std::vector< std::set< Index > > >(N, std::vector<std::set<Index>>(N + 1));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 1; k <= N; k++)
                RowIndices[i][k].insert({ i, j }),
                ColumnIndices[i][k].insert({ j, i }),
                BoxIndices[BoxNum({ i, j })][k].insert({ i, j });

#endif

}

// TODO: AddCandidate and DeleteCandidate are almost identical.

bool SudokuBoard::AddCandidate(const Index& idx, int num, bool propagate)
{

#if PRINT_DEBUG_FUNCTIONS

    std::cout << "In AddCandidate with idx = { " << idx.r << ", " << idx.c << " }, "
        << "num = " << num << ", propagate = " << std::boolalpha << propagate << std::endl;

#endif

#if PRINT_DEBUG_ERRORS

    //if (propagate && Candidates.empty() && !board[idx.r][idx.c])
    //	std::cout << "Add" << std::endl << std::endl;

#endif

    if (inRow(idx.r, num) || inColumn(idx.c, num) || inBox(BoxNum(idx), num))
        return false;

    auto it = Candidates.find(num);

    // candidate found.
    // doesn't continue if the candidate is already added.
    // the check should be done before calling EraseIdx.
    if (it != Candidates.end())
        return false;

    if (propagate)
    {
        ForEachCellInRow(idx.r, &SudokuBoard::AddCandidate, num, false);
        ForEachCellInColumn(idx.c, &SudokuBoard::AddCandidate, num, false);
        ForEachCellInBox(BoxNum(idx), &SudokuBoard::AddCandidate, num, false);
        return true;
    }

#if APPLY_PointingClaming

    RowIndices[idx.r][num].insert(idx);
    ColumnIndices[idx.c][num].insert(idx);
    BoxIndices[BoxNum(idx)][num].insert(idx);

#endif

    EraseIdx(Candidates.size(), idx);

    // it's normal to add candidates even if the cell is not empty.
    Candidates.insert(num);

    // if the cell is not empty, don't add it as available.
    if (!board[idx.r][idx.c])
        InsertIdx(Candidates.size(), idx);

    return true;
}

bool SudokuBoard::DeleteCandidate(const Index& idx, int num, bool propagate)
{

#if PRINT_DEBUG_FUNCTIONS

    std::cout << "In DeleteCandidate with idx = { " << idx.r << ", " << idx.c << " }, "
        << "num = " << num << ", propagate = " << std::boolalpha << propagate << std::endl;

#endif

#if PRINT_DEBUG_ERRORS

    if (propagate && Candidates.empty() && !board[idx.r][idx.c])
        std::cout << "Delete." << std::endl << std::endl;

#endif

    auto it = Candidates.find(num);

    // doesn't continue if the candidate isn't found.
    // the check should be done before calling EraseIdx.
    if (it == Candidates.end())
        return false;

    if (propagate)
    {
        ForEachCellInRow(idx.r, &SudokuBoard::DeleteCandidate, num, false);
        ForEachCellInColumn(idx.c, &SudokuBoard::DeleteCandidate, num, false);
        ForEachCellInBox(BoxNum(idx), &SudokuBoard::DeleteCandidate, num, false);
        return true;
    }

#if APPLY_PointingClaming

    RowIndices[idx.r][num].erase(idx);
    ColumnIndices[idx.c][num].erase(idx);
    BoxIndices[BoxNum(idx)][num].erase(idx);

#endif

    EraseIdx(Candidates.size(), idx);

    // it's normal to erase candidates even if the cell is not empty.
    Candidates.erase(it);

    // if the board is not empty, don't add it as available.
    if (!board[idx.r][idx.c])
        InsertIdx(Candidates.size(), idx);

    return true;
}

bool SudokuBoard::SetCell(const Index& idx, int num)
{

#if PRINT_DEBUG_FUNCTIONS

    std::cout << "In SetCell with idx = { " << idx.r << ", " << idx.c << " }, "
        << "num = " << num << std::endl;

#endif

    // not in the candidates.
    if (!isCandidate(idx, num))
        return false;

    // can't set a cell until you unset it manually.
    if (board[idx.r][idx.c])
        //UnsetCell(idx);
        return false;

    board[idx.r][idx.c] = num;

    row[idx.r][num] = true;
    column[idx.c][num] = true;
    box[BoxNum(idx)][num] = true;

#if PRINT_DEBUG_BOARDS

#if PRINT_DEBUG_BOARDS_INFORMATION
    std::cout << "Set Cell { " << idx.r << ", " << idx.c << " } the number " << num << std::endl;
#endif

    std::cout << x << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << board[i][j] << ' ';
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

#endif

    DeleteCandidate(idx, num, true);

    return true;
}

bool SudokuBoard::UnsetCell(const Index& idx)
{

#if PRINT_DEBUG_FUNCTIONS

    std::cout << "In UnsetCell with idx = { " << idx.r << ", " << idx.c << " }" << std::endl;

#endif

    // the cell is empty.
    // assuming that the program is correct, this can be commented.
    if (!board[idx.r][idx.c])
        return false;

    int num = board[idx.r][idx.c];

    // has to be done before adding candidates
    // since it won't be listed as available if it's set.
    board[idx.r][idx.c] = 0; // 0 = empty.

    row[idx.r][num] = false;
    column[idx.c][num] = false;
    box[BoxNum(idx)][num] = false;

#if PRINT_DEBUG_BOARDS

#if PRINT_DEBUG_BOARDS_INFORMATION
    std::cout << "Unset Cell { " << idx.r << ", " << idx.c << " } the number " << num << std::endl;
#endif

    std::cout << x << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << board[i][j] << ' ';
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

#endif

    AddCandidate(idx, num, true);

    return true;
}
