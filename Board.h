#pragma once
class Board
{
public:
	enum Slant
	{
		SUp,
		SDown,
		SLeft,
		SRight,
		LeftUp,
		LeftDown,
		RightUp,
		RightDown
	};
	enum Direction
	{
		Up,
		Down,
		Left,
		Right,
	};
	struct RowCol
	{
		int row;
		int col;
	};
private:
	int m_Columns;
	int m_Rows;
	int** m_arrBoard;
	int m_monster;
	int** m_vBoard;//������� ��� �������� ��������
	bool m_spear;
	Slant m_slant;

public:
	Board(int columns, int rows);
	void SetupBoard();
	void CreateBoard();
	void DeleteBoard();
	int getBoard(int row, int col);
	Direction logic(int Mrow, int Mcol);
	RowCol getPlayer();

public:
	int getColumns() { return m_Columns; }
	int getRows() { return m_Rows; }
	int getMonster() { return m_monster; }
	bool getSpear() { return m_spear; }
	Slant getSlant() { return m_slant; }

public:
	bool motion(int row, int col, Direction dir, bool player = false); //����� ��������
	void moveMonster();
	void align();//������� ��� �������� ��������
	bool attackM(int row, int col);
	void attackP(int row, int col, Direction dir);
	bool ProverkaLogic(int row, int col, Direction dir);//������� ����� �� �������� � ���� ����������
	void attackSpear(int row, int col);
};