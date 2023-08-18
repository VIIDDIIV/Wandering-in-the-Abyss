#include "Board.h"
#include <cstdlib> // для rand() и srand()
#include <vector>
#include <math.h>
#include <Windows.h>
#include <string>
Board::Board(int columns, int rows) : m_Columns(columns), m_Rows(rows), m_monster(6), m_spear(true)
{
	
		SetupBoard();
}
void Board::SetupBoard()
{
	if(m_arrBoard == nullptr)
		CreateBoard();
	for (int row = 0; row < m_Rows; row++)
		for (int col = 0; col < m_Columns; col++)
			m_arrBoard[row][col] = (rand() % 7 > 0) ? 0 : 1;
			
	m_arrBoard[m_Rows - 1][m_Columns / 2] = 2;
	m_arrBoard[0][m_Columns / 2] = 3;
	
	for (int i = 0; i < m_monster;)
	{
		int row = rand() % (m_Rows-3);
		int col = rand() % m_Columns;
		if (m_arrBoard[row][col] == 0)
		{
			m_arrBoard[row][col] = 4;
			i++;
		}
	}
	align();
}
void Board::CreateBoard()
{
	// Если у нас осталась доска с предыдущего раза, то удаляем её
	if (m_arrBoard != nullptr)
		DeleteBoard();

	// Создаем массив для хранения строк
	m_arrBoard = new int* [m_Rows];

	// Создаем отдельно каждую строку
	for (int row = 0; row < m_Rows; row++)
	{
		m_arrBoard[row] = new int[m_Columns];

		// Устанавливаем для каждого блока значение цвета, равное цвету фона
		for (int col = 0; col < m_Columns; col++)
				m_arrBoard[row][col] = 0;
		
	}
	m_vBoard = new int* [m_Rows];

	// Создаем отдельно каждую строку
	for (int row = 0; row < m_Rows; row++)
	{
		m_vBoard[row] = new int[m_Columns];

		// Устанавливаем для каждого блока значение цвета, равное цвету фона
		for (int col = 0; col < m_Columns; col++)
			m_vBoard[row][col] = 0;

	}
}
void Board::DeleteBoard(void)
{
	if (m_arrBoard != nullptr)
	{
		for (int row = 0; row < m_Rows; row++)
		{
			if (m_arrBoard[row] != nullptr)
			{
				// Сначала удаляем каждую отдельную строку
				delete[] m_arrBoard[row];
				m_arrBoard[row] = nullptr;
			}
		}
		// В конце удаляем массив, содержащий строки
		delete[] m_arrBoard;
		m_arrBoard = nullptr;
	}
}
int Board::getBoard(int row, int col)
{
	return m_arrBoard[row][col];
}
bool Board::motion(int row, int col, Board::Direction dir, bool player)
{
	bool boo = true;
	if (!player)
	{
		boo = attackM(row, col);
	}
	if (boo)
	{
		if (dir == Board::Direction::Up)
		{
			if (row - 1 < 0 || m_arrBoard[row - 1][col] == 1 || (m_arrBoard[row - 1][col] == 3 && !player))
				return false;
			if (m_arrBoard[row - 1][col] == 5 && player)
				m_spear = true;
			if (m_arrBoard[row - 1][col] == 3)
			{
				m_monster++;
				SetupBoard();
			}
			
			else
			{
				m_arrBoard[row - 1][col] = m_arrBoard[row][col];
				m_arrBoard[row][col] = 0;
				if (player)
					attackP(row, col, dir);

				return true;
			}
		}

		if (dir == Board::Direction::Down)
		{
			if (row + 1 >= m_Rows || m_arrBoard[row + 1][col] == 1 || (m_arrBoard[row + 1][col] == 3 && !player))
				return false;
			if (m_arrBoard[row + 1][col] == 5 && player)
				m_spear = true;
			if (m_arrBoard[row + 1][col] == 3)
			{
				m_monster++;
				SetupBoard();
			}
			
			else
			{
				m_arrBoard[row + 1][col] = m_arrBoard[row][col];
				m_arrBoard[row][col] = 0;
				if (player)
					attackP(row, col, dir);

				return true;
			}
		}

		if (dir == Board::Direction::Left)
		{
			if (col - 1 < 0 || m_arrBoard[row][col - 1] == 1 || (m_arrBoard[row][col - 1] == 3 && !player))
				return false;
			if (m_arrBoard[row][col - 1] == 5 && player)
				m_spear = true;
			if (m_arrBoard[row][col-1] == 3)
			{
				m_monster++;
				SetupBoard();
			}
			else
			{
				m_arrBoard[row][col - 1] = m_arrBoard[row][col];
				m_arrBoard[row][col] = 0;
				if (player)
					attackP(row, col, dir);
				return true;
			}
		}

		if (dir == Board::Direction::Right)
		{
			if (col + 1 >= m_Columns || m_arrBoard[row][col + 1] == 1 || (m_arrBoard[row][col + 1] == 3 && !player))
				return false;
			if (m_arrBoard[row][col + 1] == 5 && player)
				m_spear = true;
			if (m_arrBoard[row][col + 1] == 3)
			{
				m_monster++;
				SetupBoard();
			}
			
			else
			{
				m_arrBoard[row][col + 1] = m_arrBoard[row][col];
				m_arrBoard[row][col] = 0;
				if (player)
					attackP(row, col, dir);
				return true;
			}
		}
	}
	return false;
}
Board::RowCol Board::getPlayer()
{
	RowCol r;
	for (int row = 0; row < m_Rows; row++)
		for (int col = 0; col < m_Columns; col++)
			if (m_arrBoard[row][col] == 2)
			{
				r.row = row;
				r.col = col;
				return r;
			}
	
	r.row = m_Rows;//если игрока не нашли 
	r.col = m_Columns;
	return  r;
}
void Board::moveMonster()
{
	align();
	for (int row = 0; row < m_Rows; row++)
		for (int col = 0; col < m_Columns; col++)
			if (m_vBoard[row][col] == 4 )
				motion(row, col, logic(row, col));
		
	align();
}
void Board::align()
{
	for (int row = 0; row < m_Rows; row++)
		for (int col = 0; col < m_Columns; col++)
			m_vBoard[row][col] = m_arrBoard[row][col];
}
bool Board::ProverkaLogic(int row, int col, Direction dir)
{
	if (dir == Board::Direction::Up)
	{
		if (row - 1 < 0 || m_arrBoard[row - 1][col] == 1 || m_arrBoard[row - 1][col] == 3 || m_arrBoard[row - 1][col] == 4)
			return false;
	
		return true;
	}

	if (dir == Board::Direction::Down)
	{
		if (row + 1 >= m_Rows || m_arrBoard[row + 1][col] == 1 || m_arrBoard[row + 1][col] == 3 || m_arrBoard[row + 1][col] == 4)
			return false;
		
		return true;
		
	}

	if (dir == Board::Direction::Left)
	{
		if (col - 1 < 0 || m_arrBoard[row][col - 1] == 1 || m_arrBoard[row][col - 1] == 3 || m_arrBoard[row][col - 1] == 4)
			return false;
		
		return true;
		
	}

	if (dir == Board::Direction::Right)
	{
		if (col + 1 >= m_Columns || m_arrBoard[row][col + 1] == 1 || m_arrBoard[row][col + 1] == 3 || m_arrBoard[row][col + 1] == 4)
			return false;
		
		return true;
		
	}
}
Board::Direction Board::logic(int Mrow, int Mcol)
{
	int Prow = getPlayer().row;
	int Pcol = getPlayer().col;
	int row = Mrow - Prow;
	int col = Mcol - Pcol;

	int Prow1 =Prow - 2;
	int row1 = Mrow - Prow1;

	int Prow2 = Prow + 2;
	int row2 = Mrow - Prow2;

	int Pcol1 = Pcol - 2;
	int col1 = Mcol - Pcol1;

	int Pcol2 = Pcol + 2;
	int col2 = Mcol - Pcol2;
	if (abs(row2) + abs(col) < abs(row1) + abs(col) && abs(row2) + abs(col) < abs(row) + abs(col1) && abs(row2) + abs(col) < abs(row) + abs(col2))
	{
		if (abs(row2) > abs(col))
		{
			if (row2 > 0 && ProverkaLogic(Mrow, Mcol, Up))
				return Up;
			else if (ProverkaLogic(Mrow, Mcol, Down))
				return Down;
		}
		else if (abs(row2) < abs(col))
		{
			if (col > 0 && ProverkaLogic(Mrow, Mcol, Left))
				return Left;
			else if (ProverkaLogic(Mrow, Mcol, Right))
				return Right;
		}
		else
		{
			if (rand() % 2)
				if (row2 > 0 && ProverkaLogic(Mrow, Mcol, Up))
					return Up;
				else if (ProverkaLogic(Mrow, Mcol, Down))
					return Down;
				else
					if (col > 0 && ProverkaLogic(Mrow, Mcol, Left))
						return Left;
					else if (ProverkaLogic(Mrow, Mcol, Right))
						return Right;
		}
	}
	else if (abs(row1) + abs(col) < abs(row) + abs(col1) && abs(row1) + abs(col) < abs(row) + abs(col2))
	{
		if (abs(row1) > abs(col))
		{
			if (row1 > 0 && ProverkaLogic(Mrow, Mcol, Up))
				return Up;
			else if (ProverkaLogic(Mrow, Mcol, Down))
				return Down;
		}
		else if (abs(row1) < abs(col))
		{
			if (col > 0 && ProverkaLogic(Mrow, Mcol, Left))
				return Left;
			else if (ProverkaLogic(Mrow, Mcol, Right))
				return Right;
		}
		else
		{
			if (rand() % 2)
				if (row1 > 0 && ProverkaLogic(Mrow, Mcol, Up))
					return Up;
				else if (ProverkaLogic(Mrow, Mcol, Down))
					return Down;
				else
					if (col > 0 && ProverkaLogic(Mrow, Mcol, Left))
						return Left;
					else if (ProverkaLogic(Mrow, Mcol, Right))
						return Right;
		}
	}
	else if (abs(row) + abs(col1) < abs(row) + abs(col2))
	{
		if (abs(row) > abs(col1))
		{
			if (row > 0 && ProverkaLogic(Mrow, Mcol, Up))
				return Up;
			else if (ProverkaLogic(Mrow, Mcol, Down))
				return Down;
		}
		else if (abs(row) < abs(col1))
		{
			if (col1 > 0 && ProverkaLogic(Mrow, Mcol, Left))
				return Left;
			else if (ProverkaLogic(Mrow, Mcol, Right))
				return Right;
		}
		else
		{
			if (rand() % 2)
				if (row > 0 && ProverkaLogic(Mrow, Mcol, Up))
					return Up;
				else if (ProverkaLogic(Mrow, Mcol, Down))
					return Down;
				else
					if (col1 > 0 && ProverkaLogic(Mrow, Mcol, Left))
						return Left;
					else if (ProverkaLogic(Mrow, Mcol, Right))
						return Right;
		}
	}
	else if (abs(row) + abs(col1) > abs(row) + abs(col2))
	{
		if (abs(row) > abs(col2))
		{
			if (row > 0 && ProverkaLogic(Mrow, Mcol, Up))
				return Up;
			else if (ProverkaLogic(Mrow, Mcol, Down))
				return Down;
		}
		else if (abs(row) < abs(col2))
		{
			if (col2 > 0 && ProverkaLogic(Mrow, Mcol, Left))
				return Left;
			else if (ProverkaLogic(Mrow, Mcol, Right))
				return Right;
		}
		else
		{
			if (rand() % 2)
				if (row > 0 && ProverkaLogic(Mrow, Mcol, Up))
					return Up;
				else if (ProverkaLogic(Mrow, Mcol, Down))
					return Down;
				else
					if (col2 > 0 && ProverkaLogic(Mrow, Mcol, Left))
						return Left;
					else if (ProverkaLogic(Mrow, Mcol, Right))
						return Right;
		}
	}
	return static_cast<Board::Direction> (rand() % 4);
}
bool Board::attackM(int row, int col)
{
	if (row - 1 >= 0 && m_arrBoard[row - 1][col] == 2)
	{
		m_arrBoard[row - 1][col] = 0;
		return false;
	}
	if (row + 1 < m_Rows && m_arrBoard[row + 1][col] == 2)
	{
		m_arrBoard[row + 1][col] = 0;
		return false;
	}
	if (col + 1 < m_Columns && m_arrBoard[row][col+1] == 2)
	{
		m_arrBoard[row][col+1] = 0;
		return false;
	}
	if (col - 1 >= 0 && m_arrBoard[row][col-1] == 2)
	{
		m_arrBoard[row][col-1] = 0;
		return false;
	}
	return true;
}
void Board::attackP(int row, int col, Direction dir)
{
	if (dir == Down)
	{
		//if (row + 2 < m_Rows && m_arrBoard[row + 2][col] == 4)
		//	m_arrBoard[row + 2][col] = 0;
		if(col+1<m_Columns && m_arrBoard[row][col+1] == 4)
			m_arrBoard[row][col + 1] = 0;

		if (col-1>=0 && m_arrBoard[row][col - 1] == 4)
			m_arrBoard[row][col - 1] = 0;

		if (row+1 <m_Rows && col + 1 < m_Columns && m_arrBoard[row+1][col + 1] == 4)
			m_arrBoard[row+1][col + 1] = 0;

		if (row + 1 < m_Rows && col - 1 >= 0 && m_arrBoard[row+1][col - 1] == 4)
			m_arrBoard[row+1][col - 1] = 0;
	}
	if (dir == Up)
	{
		//if (row - 2 > 0 && m_arrBoard[row - 2][col] == 4)
			//m_arrBoard[row - 2][col] = 0;
		if (col + 1 < m_Columns && m_arrBoard[row][col + 1] == 4)
			m_arrBoard[row][col + 1] = 0;

		if (col - 1 >= 0 && m_arrBoard[row][col - 1] == 4)
			m_arrBoard[row][col - 1] = 0;

		if (row - 1>=0 && col + 1 < m_Columns && m_arrBoard[row - 1][col + 1] == 4)
			m_arrBoard[row - 1][col + 1] = 0;

		if (row - 1 >= 0 && col - 1 >= 0 && m_arrBoard[row - 1][col - 1] == 4)
			m_arrBoard[row - 1][col - 1] = 0;
	}
	if (dir == Left)
	{
		//if (col - 2 >= 0 && m_arrBoard[row][col - 2] == 4)
		//	m_arrBoard[row][col-2] = 0;
		if (row + 1 < m_Rows && m_arrBoard[row + 1][col] == 4)
			m_arrBoard[row + 1][col] = 0;

		if (row - 1 >= 0 && m_arrBoard[row - 1][col] == 4)
			m_arrBoard[row - 1][col] = 0;

		if (row + 1 < m_Rows && col - 1 >= 0 && m_arrBoard[row + 1][col-1] == 4)
			m_arrBoard[row + 1][col-1] = 0;

		if (row - 1 >= 0 && col - 1 >= 0 && m_arrBoard[row - 1][col-1] == 4)
			m_arrBoard[row - 1][col-1] = 0;
	}
	if (dir == Right)
	{
		//if (col + 2 < m_Columns && m_arrBoard[row][col + 2] == 4)
		//	m_arrBoard[row][col + 2] = 0;
		if (row + 1 < m_Rows && m_arrBoard[row + 1][col] == 4)
			m_arrBoard[row + 1][col] = 0;

		if (row - 1 >= 0 && m_arrBoard[row - 1][col] == 4)
			m_arrBoard[row - 1][col] = 0;

		if (row + 1 < m_Rows && col + 1 < m_Columns && m_arrBoard[row + 1][col + 1] == 4)
			m_arrBoard[row + 1][col + 1] = 0;

		if (row - 1 >= 0 && col + 1 < m_Columns && m_arrBoard[row - 1][col + 1] == 4)
			m_arrBoard[row - 1][col + 1] = 0;
	}
}
void Board::attackSpear(int row, int col)
{
	m_arrBoard[row][col] = 5;
	if (getPlayer().row > row)
		if (getPlayer().col == col)
			m_slant = Slant::SUp;
		else if (getPlayer().col > col)
			m_slant = Slant::LeftUp;
		else
			m_slant = Slant::RightUp;
	else if (getPlayer().row < row)
		if (getPlayer().col == col)
			m_slant = Slant::SDown;
		else if (getPlayer().col > col)
			m_slant = Slant::LeftDown;
		else
			m_slant = Slant::RightDown;
	else if (getPlayer().col > col)
		m_slant = Slant::SLeft;
	else if (getPlayer().col < col)
		m_slant = Slant::SRight;

	m_spear = false;
}