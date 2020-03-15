#pragma once

#include "FLAGS.h"
#include "SudokuBoard.h"
#include <functional>
#include <chrono>
#include <algorithm>
#include "RNG.h"

#define TimePoint std::chrono::time_point<std::chrono::steady_clock>
#define SudokuSolverDuration std::chrono::duration<double>

struct State
{
	std::vector<Index> CellIndex, CandidateIndex;
	std::vector<int> CandidateValue;
};

class SudokuSolver
{
    friend class MainWindow;

	// starts with an empty 9 * 9 board.
	SudokuBoard board;

	bool Possible() const;
	void StartDuration();
	void EndDuration();


public:

	// number of recursive calls happened.
	int NumberOfCalls;
	// number of calls that didn't stop on a base-case.
	int NumberOfValidCalls;

	// unless solved, StartTime, EndTime, duration will not be useful.
	// Starting and Ending time for solving.
	TimePoint StartTime, EndTime;

	// the time it took to solve (in seconds).
    SudokuSolverDuration SolvingDuration;

	SudokuSolver() {};
	SudokuSolver(SudokuBoard board) : board(board) {};

	const SudokuBoard& GetBoard() const { return board; }
    const SudokuSolverDuration& GetDuration() const { return SolvingDuration; }

	void Clear();
	void LoadBoard(const Board& board);

	// if there exist more than best cell, the next one is chosen randomly.
	Index GetNextCell(); 

	bool Solve();
    bool SetRandomCells(int cells);
	bool Backtrack();
	bool Validate() const;
	void GenerateBoards(int n, std::vector<std::vector<std::vector<int>>>& list, int shuffle = 0);
	void ChangeNumbers(std::vector<std::vector<int>>& b, std::vector<int>& permutation);

#if APPLY_STRATEGIES	

	bool ApplyStrategies(State& state);
	void UndoStrategies(const State& state);
	
#if APPLY_NakedSingles
	bool SetNakedSingles(State& state);
#endif

#if APPLY_PointingClaming
	bool PointingClaming(State& state);
#endif

#if APPLY_Naked
	bool Naked(State& state);
#endif

#endif
    bool Solved() const;
};
