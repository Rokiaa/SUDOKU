#pragma once

#include "FLAGS.h"
#include "Container.h"
#include <vector>
#include <set>

#define Board	   std::vector<std::vector<int>>
#define EmptyBoard std::vector<std::vector<int>>()
#define Candidates candidates[idx.r][idx.c]

struct Index
{
	int r, c;
	bool operator< (const Index& idx) const
	{
		return r < idx.r || (r == idx.r && c < idx.c);
	}
};

class SudokuBoard
{
    friend class MainWindow;
	friend class SudokuSolver;
	friend class SudokuViewer;

	int N, BoxN;											// board = N * N, BoxN = sqrt(N).
	Board board;											// 2d vector of int.
	std::vector<std::vector<bool>> row, column, box;		// 2d vector of bool.
	std::vector<std::vector<std::set<int>>> candidates;		// 2d vector of Container of int.
	std::set<int> available;								// stores the indcies of non empty Containers in CellsWithNCandidates.

	// CellsWithNCandidates[i] stores the indcies of cells with i candidates.
	// if CellsWithNCandidates[0] is not empty then the board is invalid.
	std::vector<Container<Index>> CellsWithNCandidates;

#if APPLY_PointingClaming

	// xxxIndices[i][j] will have the indices of cells in xxx number i that contain the candidate j.
	std::vector<std::vector<std::set<Index>>> RowIndices, ColumnIndices, BoxIndices;

#endif

public:
	SudokuBoard(const Board& board = EmptyBoard, int BoxN = 3);
	SudokuBoard(int BoxN, const Board& board = EmptyBoard)
		: SudokuBoard(board, BoxN) {}

	int BoxNum(const Index& idx) const;

	// if CandidatesCount == -1, updates for every number.
	void UpdateAvailable(int CandidatesCount = -1);

	// inserts idx to CellsWithNCandidates[CandidatesCound] 
	// if not present, then calls UpdateAvailable(CandidatesCount).
	void InsertIdx(int CandidatesCount, const Index& idx);

	// erases idx from CellsWithNCandidates[CandidatesCound] 
	// if present, then calls UpdateAvailable(CandidatesCount).
	void EraseIdx (int CandidatesCount, const Index& idx); 

	bool AddCandidate(const Index& idx, int num, bool propagate = false);
	bool DeleteCandidate(const Index& idx, int num, bool propagete = false);
	bool SetCell(const Index& idx, int num);
	bool UnsetCell(const Index& idx);

	bool isCandidate(const Index& idx, int num); // not const...
	bool inRow(int r, int num) const;
	bool inColumn(int c, int num) const;
	bool inBox(int b, int num) const;

	// if the given board is smaller, the rest is cosidered empty.
	// if the given board is bigger, the rest is ignored.
	void SetBoard(const Board& board, bool clear = false);
	void ResizeBoard(int BoxN, bool keep = false);
	void Clear();

	template <typename T, typename... Args>
	void ForEachCellInRow(int c, T func, Args... args);

	template <typename T, typename... Args>
	void ForEachCellInColumn(int r, T func, Args... args);

	template <typename T, typename... Args>
	void ForEachCellInBox(int b, T func, Args... args);
};


template <typename T, typename... Args>
void SudokuBoard::ForEachCellInRow(int r, T func, Args... args)
{
	for (int c = 0; c < N; c++)
		(this->*func)({ r, c }, args...);
}

template <typename T, typename... Args>
void SudokuBoard::ForEachCellInColumn(int c, T func, Args... args)
{
	for (int r = 0; r < N; r++)
		(this->*func)({ r, c }, args...);
}

template <typename T, typename... Args>
void SudokuBoard::ForEachCellInBox(int b, T func, Args... args)
{
	int r = (b / BoxN) * BoxN;
	int c = (b % BoxN) * BoxN;

	for (int i = 0; i < BoxN; i++)
		for (int j = 0; j < BoxN; j++)
			(this->*func)({ r + i, c + j }, args...);
}
