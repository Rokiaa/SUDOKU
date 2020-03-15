#include "SudokuSolver.h"

bool SudokuSolver::Possible() const
{
    return board.CellsWithNCandidates[0].empty();
}

bool SudokuSolver::Solved() const
{
    return board.available.empty();
}

void SudokuSolver::StartDuration()
{
    StartTime	= std::chrono::_V2::steady_clock::now();
}

void SudokuSolver::EndDuration()
{
    EndTime		= std::chrono::_V2::steady_clock::now();

    SolvingDuration =
        std::chrono::duration_cast<std::chrono::duration<double>>(EndTime - StartTime);
}

Index SudokuSolver::GetNextCell()
{
    if (board.available.empty()) return { -1, };

    int MinimumCandidates = *(board.available.begin());
    return board.CellsWithNCandidates[MinimumCandidates].GetRandom();
}

void SudokuSolver::Clear()
{
    board.Clear();
}

void SudokuSolver::LoadBoard(const Board& board)
{
    this->board.SetBoard(board, true);
}

bool SudokuSolver::Solve()
{
    NumberOfCalls = 0;
    NumberOfValidCalls = 0;

    StartDuration();
    bool res = Backtrack();
    EndDuration();

    return res;
}

bool SudokuSolver::Backtrack()
{
    ++NumberOfCalls;

    if (!Possible())
        return false;

#if APPLY_STRATEGIES

    State ChangesMade;

    // applies strategies until no changes had been made.
    while (ApplyStrategies(ChangesMade));

#endif

    // index is obtained after applying the strategies.
    Index idx = GetNextCell();

    if (idx.r == -1)
        return true;

    ++NumberOfValidCalls;

    // have to copy the candidates for two reasons:
    //  - the set may change inside the loop.
    //  - to allow random access.
    LiteContainer<int> candidates(board.Candidates);
    while (!candidates.empty())
    {

#if PRINT_DEBUG_ERRORS
        if (!board.SetCell(idx, candidates.PopRandom()))
            std::cout << "Can't Set Cell" << std::endl;
#else
        board.SetCell(idx, candidates.PopRandom());
#endif

        if (Backtrack())
            return true;

#if PRINT_DEBUG_ERRORS
        if (!board.UnsetCell(idx))
            std::cout << "Can't Unset Cell" << std::endl;
#else
        board.UnsetCell(idx);
#endif

    }

#if APPLY_STRATEGIES
    UndoStrategies(ChangesMade);
#endif

    return false;
}

bool SudokuSolver::Validate() const
{
    // checking that every number from 1..N exists in each row, column, box.
    for (int i = 0; i < board.N; i++)
        for (int num = 1; num <= board.N; num++)
            if (!board.inRow(i, num) || !board.inColumn(i, num) || !board.inBox(i, num))
                return false;

    return true;
}

bool SudokuSolver::SetRandomCells(int cells)
{
    if (!Solve())
        return false;

    // TODO
    cells = board.N * board.N - cells;
    while (cells--)
    {
        Index idx;

        while (board.board[idx.r = RNG::GetRandomNumber(board.N)][idx.c = RNG::GetRandomNumber(board.N)] == 0);

        board.UnsetCell(idx);
    }

    return true;
}

void SudokuSolver::GenerateBoards(int num, std::vector<std::vector<std::vector<int>>>& list, int shuffle)
{
    Solve();

    std::vector<int> permutation;
    for (int i = 0; i <= board.N; i++)
        permutation.push_back(i);

    auto begin = permutation.begin();
    begin++;

    do
    {
        if (!num) break;
        num--;

        Board b(board.N, std::vector<int>(board.N));
        ChangeNumbers(b, permutation);
        list.push_back(b);

    } while (std::next_permutation(begin, permutation.end()));

    while (shuffle--)
        std::swap(list[RNG::GetRandomNumber((int)list.size())], list[RNG::GetRandomNumber((int)list.size())]);
}

void SudokuSolver::ChangeNumbers(std::vector<std::vector<int>>& b, std::vector<int>& permutation)
{
    for (int i = 0; i < board.N; i++)
        for (int j = 0; j < board.N; j++)
            b[i][j] = permutation[board.board[i][j]];
}

#if APPLY_STRATEGIES

bool SudokuSolver::ApplyStrategies(State& state)
{
    bool Changed = true;
    while (Changed)
    {
        Changed = false;

#if APPLY_NakedSingles
        // can disabled if no other technique is applyied
        // since the technique is applied anyways.
        Changed |= SetNakedSingles(state);
#endif

#if APPLY_PointingClaming
        // makes it slower.
        Changed |= PointingClaming(state);
#endif

    }
    return Changed;
}

void SudokuSolver::UndoStrategies(const State& state)
{
    for (int i = 0; i < (int)state.CellIndex.size(); i++)
        board.UnsetCell(state.CellIndex[i]);

    for (int i = 0; i < (int)state.CandidateIndex.size(); i++)
        board.AddCandidate(state.CandidateIndex[i], state.CandidateValue[i], true);
}

#endif

#if APPLY_NakedSingles

bool SudokuSolver::SetNakedSingles(State& state)
{
    bool Changed = !board.CellsWithNCandidates[1].empty();
    while (!board.CellsWithNCandidates[1].empty())
    {
        Index idx = board.CellsWithNCandidates[1].GetRandom();
        board.SetCell(idx, *board.Candidates.begin());
        state.CellIndex.push_back(idx);
    }
    return Changed;
}

#endif

#if APPLY_PointingClaming
bool SudokuSolver::PointingClaming(State& state)
{
    // if n (n < BoxN) cells are the only cells with candidate c
    // in box b and they are in the same row/column, delete the
    // candidate c from every other cell outside the box b in
    // the same row/column.

    // TODO: can be done in reverse
    // if some candidate is only in some cells in the same box,
    // delete all the instances of that candidate in that box.

    bool Changed = false;

    for (int candidate = 1; candidate <= board.N; candidate++)
    {
        for (int box = 0; box < board.N; box++)
        {
            int size = board.BoxIndices[box][candidate].size();
            if (!size || size > board.BoxN) continue;

            int r = box / board.BoxN * board.BoxN;
            int c = box % board.BoxN * board.BoxN;

            int row = 0, col = 0;
            for (int i = 0; i < board.BoxN; i++)
            {
                if (board.isCandidate({ r, c + i }, candidate))
                    row++;

                if (board.isCandidate({ r + i, c }, candidate))
                    col++;
            }

            // TODO: row and colmun code are almost the same.
            if (row == size && board.RowIndices[r][candidate].size() > row)
            {
                Changed = true;

                // TODO: alot of duplicate code.
                for (int i = 0; i < c; i++)
                {
                    if (board.DeleteCandidate({ r, i }, candidate))
                    {
                        state.CandidateIndex.push_back({ r, i });
                        state.CandidateValue.push_back(candidate);
                    }
                }

                for (int i = c + board.BoxN; i < board.N; i++)
                {
                    if (board.DeleteCandidate({ r, i }, candidate))
                    {
                        state.CandidateIndex.push_back({ r, i });
                        state.CandidateValue.push_back(candidate);
                    }
                }
            }

            if (col == size && board.ColumnIndices[c][candidate].size() > col)
            {
                Changed = true;

                for (int i = 0; i < r; i++)
                {
                    if (board.DeleteCandidate({ i, c }, candidate))
                    {
                        state.CandidateIndex.push_back({ i, c });
                        state.CandidateValue.push_back(candidate);
                    }
                }

                for (int i = r + board.BoxN; i < board.N; i++)
                {
                    if (board.DeleteCandidate({ i, c }, candidate))
                    {
                        state.CandidateIndex.push_back({ i, c });
                        state.CandidateValue.push_back(candidate);
                    }
                }
            }
        }
    }

    return Changed;
}

#endif

#if APPLY_Naked

// Not Complete.
bool SudokuSolver::Naked(State& state)
{
    std::vector<int> parent(board.N + 1);

    std::function<int(int)> GetParent = [&parent, &GetParent](int i)
    {
        if (parent[i] <= 0) return i;
        return parent[i] = GetParent(parent[i]);
    };

    std::function<int(int)> GetCount = [&parent, &GetParent](int i)
    {
        return -parent[GetParent(i)];
    };

    std::function<void(int, int)> Merge = [&parent, &GetCount, &GetParent](int i, int j)
    {
        if (GetParent(i) == GetParent(j))
            return;

        if (GetCount(i) > GetCount(j))
            std::swap(i, j);

        parent[GetParent(i)] += parent[GetParent(j)];
        parent[GetParent(j)] = GetParent(i);
    };

    for (int row = 0; row < board.N; row++)
    {
        std::fill(parent.begin(), parent.end(), -1);

        for (int col = 0; col < board.N; col++)
        {
            int FirstCandidate = *board.candidates[row][col].begin();
            for (int candidate : board.candidates[row][col])
                Merge(FirstCandidate, candidate);
        }

        std::vector<std::set<int>> indices;
        for (int col = 0; col < board.N; col++)
            for (int candidate : board.candidates[row][col])
                indices[GetParent(candidate)].insert(col);

    }

    return true;
}

#endif

// Strategies:

// 3 - for each row, column, box, if there exist n cells with n(or subset) candidates, remove the candidates from every cell in the same row, column, box.
//naked singles, pairs, quads......
//
// 2 - pointing pairs, triples
//if a candidate appers only in two or three cells in a box, and the cells are in one column, all other apearances of the candidate are removed from the column.
//
// 2 - claming pairs, triples
//pointing pairs but rows instead of columns
//
// 4 - if n cells in the same row, column or box have some n(or more) candidates, other candidates are removed from those cells.the candidates have to appear only in these cells.
//hidden single, pair....
//
//
//to be done :
//swordfish, x - wing
//for each 3 boxes in a row or a column, if a candidate exists in a 3 lines(row / coumn) of one of the boxes, and only two same lines in the other boxes, delete that number from the candidates of the two lines of that block

