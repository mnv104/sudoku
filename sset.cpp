#include <iostream>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <algorithm>
#include "sset.h"

using namespace std;

int matrix::row_adjust[3][2] = {{+1, +2}, {-1, +1}, {-1, -2}};

matrix::matrix(int rows, int columns)
{
	int count = 0;
	_num_filled = 0;
	_rows = rows;
	_columns = columns;
	_m = new int* [rows];
	assert(_m); 
	_s = new sset* [rows];
	assert(_s);
	for (int i = 0; i < rows; i++) 
	{ 
		_m[i] = new int[columns];
		assert(_m[i]); 
		_s[i] = new sset [columns];
		assert(_s[i]);
		for (int j = 0; j < columns; j++)
		{
			_s[i][j].setid(count++);
			_s[i][j].setrowcol(i, j);
		}
	}
}

matrix::~matrix()
{
	for (int i = 0; i < _rows; i++) 
	{
		delete [] _m[i];
		delete [] _s[i];
	}
	delete [] _m;
	delete [] _s;
}

void matrix::input(int rows, int columns, int **m)
{
	int count = 0;
	/* delete what we have */
	if (_m)
	{
		for (int i = 0; i < _rows; i++) 
		{
			delete [] _m[i]; 
			delete [] _s[i];
		}
		delete [] _m;
		delete [] _s;
	}
	_num_filled = 0;
	_rows = rows;
	_columns = columns;
	/* Re-allocate everything */
	_m = new int* [_rows]; 
	assert(_m); 
	_s = new sset* [_rows];
	assert(_s);
	for (int i = 0; i < _rows; i++) 
	{ 
		_m[i] = new int[_columns]; 
		assert(_m[i]); 
		_s[i] = new sset[_columns];
		assert(_s[i]);
		for (int j = 0; j < columns; j++)
		{
			_s[i][j].setid(count++);
			_s[i][j].setrowcol(i, j);
		}
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			_m[i][j] = m[i][j];
			if (_m[i][j] != -1) {
				_s[i][j].singleton(_m[i][j]);
				_num_filled++;
			}
		}
	}
#ifdef DEBUG
	cout << "Initial number of filled elements = " << _num_filled << endl;
#endif
	return;
}

void matrix::input(char *filename)
{
	ifstream fin;
	if (_rows <= 0 || _columns <= 0)
	{
		cerr << "Cannot input without changing # of rows or columns" << endl;
		return;
	}
	fin.open(filename);
	if (!fin)
	{
		cerr << "Cannot open input file" << endl;
		return;
	}
	cout << "Enter elements (row-wise) " << endl;
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _columns; j++) 
		{
			fin >> _m[i][j];
			if (_m[i][j] != -1) {
				_s[i][j].singleton(_m[i][j]);
				_num_filled++;
			}
		}
	}
#ifdef DEBUG
	cout << "Initial number of filled elements = " << _num_filled << endl;
#endif
}

void matrix::input(matrix &a)
{
	int count = 0;
	/* delete what we have */
	if (_m)
	{
		for (int i = 0; i < _rows; i++) 
		{
			delete [] _m[i]; 
			delete [] _s[i];
		}
		delete [] _m;
		delete [] _s;
	}
	_num_filled = 0;
	_rows = a.rows();
	_columns = a.columns();
	/* Re-allocate everything */
	_m = new int* [_rows]; 
	assert(_m); 
	_s = new sset* [_rows];
	assert(_s);
	for (int i = 0; i < _rows; i++) 
	{ 
		_m[i] = new int[_columns]; 
		assert(_m[i]); 
		_s[i] = new sset[_columns];
		assert(_s[i]);
		for (int j = 0; j < _columns; j++)
		{
			_s[i][j].setid(count++);
			_s[i][j].setrowcol(i, j);
		}
	}
	for (int i = 0; i < a.rows(); i++)
	{
		for (int j = 0; j < a.columns(); j++)
		{
			_m[i][j] = a.element(i, j);
			if (_m[i][j] != -1) {
				_s[i][j].singleton(_m[i][j]);
				_num_filled++;
			}
		}
	}
#ifdef DEBUG
	cout << "Initial number of filled elements = " << _num_filled << endl;
#endif
	return;
}

void matrix::show(void)
{
	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			cout << setw(3) << _m[i][j] << " "; 
		}
		cout << endl;
	}
	/*
	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			_s[i][j].show();
		}
	}
	*/
}

void matrix::eliminate_element(int row, int column, int *num_ptr)
{
	if (_m[row][column] != -1)
	{
		/* Remove from all elements of the column */
		for (int k = 0; k < _rows; k++)
		{
			if (k != row)
			{
				/* see if any elements can be uncovered very simply because of the constraints! */
				if (_s[k][column].remove(_m[row][column]) == 1)
				{
					if (_m[k][column] == -1)
					{
						/* Row k, Col column has became a singleton */
						_m[k][column] = _s[k][column].head();
						(*num_ptr)++;
						_num_filled++;
#ifdef DEBUG
						cout << _num_filled << ". Filling in [ " << k << " , " << column << " ]  with " << _m[k][column] << endl;
#endif
						if (notdone() == true)
						{
							/* Recurse */
							eliminate_element(k, column, num_ptr);
						}
						else {
							return;
						}
					}
				}
			}
		}
		/* Remove from all elements of the row */
		for (int k = 0; k < _columns; k++)
		{
			if (k != column)
			{
				/* see if any elements can be uncovered very simply because of the constraints */
				if (_s[row][k].remove(_m[row][column]) == 1)
				{
					if (_m[row][k] == -1)
					{
						/* Row row, Col k has became a singleton */
						_m[row][k] = _s[row][k].head();
						(*num_ptr)++;
						_num_filled++;
#ifdef DEBUG
						cout << _num_filled << ". Filling in [ " << row << " , " << k << " ]  with " << _m[row][k] << endl;
#endif
						if (notdone() == true)
						{
							/* Recurse */
							eliminate_element(row, k, num_ptr);
						}
						else {
							return;
						}
					}
				}
			}
		}
		int row_mod = row % 3;
		int col_mod = column % 3;
		int row_adjusts[2], col_adjusts[2];
		/* Remove from all elements of the grid */
		row_adjusts[0] = row_adjust[row_mod][0];
		row_adjusts[1] = row_adjust[row_mod][1];
		col_adjusts[0] = row_adjust[col_mod][0];
		col_adjusts[1] = row_adjust[col_mod][1];
		//cout << "Original Row = " << row << " Original Column = " << column << endl;
		for (int i = 0; i < 2; i++) 
		{
			for (int j = 0; j < 2; j++)
			{
				int new_row = row + row_adjusts[i];
				int new_col = column + col_adjusts[j];
				//cout << "Neighbour row = " << new_row << " Neighbour column = " << new_col << endl;
				if (_s[new_row][new_col].remove(_m[row][column]) == 1)
				{
					if (_m[new_row][new_col] == -1)
					{
						/* Row new_row, Col new_col has became a singleton */
						_m[new_row][new_col] = _s[new_row][new_col].head();
						(*num_ptr)++;
						_num_filled++;
#ifdef DEBUG
						cout << _num_filled << ". Filling in [ " << new_row << " , " << new_col << " ]  with " << _m[new_row][new_col] << endl;
#endif
						if (notdone() == true)
						{
							/* Recurse */
							eliminate_element(new_row, new_col, num_ptr);
						}
						else {
							return;
						}
					}
				}
			}
		}
	}
}

void matrix::symmetric_difference(const sset &_set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set1 = *(_set1._s), set2 = *(_set2._s);
	set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::symmetric_difference(const set<int, less<int> >&set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set2 = *(_set2._s);
	set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::intersection(const sset &_set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set1 = *(_set1._s), set2 = *(_set2._s);
	set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::intersection(const set<int, less<int> >&set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set2 = *(_set2._s);
	set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::setunion(const sset &_set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set1 = *(_set1._s), set2 = *(_set2._s);
	set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::setunion(const set<int, less<int> >&set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set2 = *(_set2._s);
	set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::setunion(const set<int, less<int> >&set1, const set<int, less<int> >&set2, set<int, less<int> >&result)
{
	set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::difference(const sset &_set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set1 = *(_set1._s), set2 = *(_set2._s);
	set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::difference(const set<int, less<int> >&set1, const sset &_set2, set<int, less<int> >&result)
{
	set<int, less<int> > set2 = *(_set2._s);
	set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

void matrix::difference(const set<int, less<int> >&set1, const set<int, less<int> > &set2, set<int, less<int> >&result)
{
	set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.begin()));
	return;
}

static void print_set(char *name, const set<int, less<int> >&set1)
{
	cout << name << endl;
	copy(set1.begin(), set1.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
	return;
}

int matrix::find_independant_elements(vector<sset *> &vec)
{
	int count = 0;
	int num_independant = 0;

	count = vec.size();
	/* All elements of this row has been solved. nothing to do */
	if (count == 0)
	{
		return 0;
	}
	int row, col;
	/* Only 1 unsolved element */
	if (count == 1)
	{
		int head = vec[0]->head();
		vec[0]->getrowcol(row, col);
		if (head < 0)
		{
			cerr << "Only 1 unsolved element. Yet we don't have a singleton set!" << endl;
			assert(0);
		}
		if (_m[row][col] == -1)
		{
			_m[row][col] = head;
			_num_filled++;
#ifdef DEBUG
			cout << _num_filled << ". Filling in [ " << row << " , " << col << " ]  with " << _m[row][col] << endl;
#endif
			/* Eliminate this from row and col */
			eliminate_element(row, col, &num_independant);
			return 1;
		}
		return 0;
	}
	/* Here we compute the following
	 * (A U B U C U ....) - ((A ^ B) U (A ^ C) U (A ^ D) ... U (B ^ C) U (B ^ D) ....))
	 */
	set<int, less<int> > all_union;
	for (int i = 0; i < count; i++)
	{
#ifdef DEBUG
		cout << "Set " << i << ": ";
		vec[i]->show(0);
#endif
		setunion(all_union, *(vec[i]), all_union);
	}
#ifdef DEBUG
	print_set("Union of all sets", all_union);
#endif

	set<int, less<int> > all_union_intersection;
	for (int i = 0; i < count; i++) {
		for (int j = i + 1; j < count; j++) {
			set<int, less<int> > pairintersect;
			intersection(*(vec[i]), *(vec[j]), pairintersect);
			setunion(all_union_intersection, pairintersect, all_union_intersection);
		}
	}

#ifdef DEBUG
	print_set("AllUnionIntersect", all_union_intersection);
#endif

	set<int, less<int> > diff;
	difference(all_union, all_union_intersection, diff);
#ifdef DEBUG
	print_set("Difference", diff);
#endif

	/* No commonality detected */
	if (diff.size() == 0)
	{
		return 0;
	}
	set<int, less<int> >::iterator diff_iter = diff.begin();
	while (diff_iter != diff.end()) {
		int i;
		/* Find out which set this element belongs to */
		for (i = 0; i < count; i++) {
			if (vec[i]->find(*diff_iter) == true)
			{
				break;
			}
		}
		vec[i]->getrowcol(row, col);
		if (_m[row][col] == -1)
		{
			_m[row][col] = *diff_iter;
			_num_filled++;
#ifdef DEBUG
			cout << _num_filled << ". Filling in [ " << row << " , " << col << " ]  with " << _m[row][col] << endl;
#endif
			/* Zap this set to a singleton */
			_s[row][col].singleton(*diff_iter);
			/* Eliminate this from row and col */
			eliminate_element(row, col, &num_independant);
		}
		diff_iter++;
	}
	return 1;
}

struct details {
	int id;
	int count;
};

bool comp_fn (const details &a, const details &b) 
{
	return (a.count < b.count);
}

void matrix::row_column_eliminate(void)
{
	int num_eliminated = 0;
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _columns; j++)
		{
			eliminate_element(i, j, &num_eliminated);
			if (notdone() == false)
				return;
		}
	}
	vector<details> row_totals(_rows), column_totals(_columns);
	vector<int> row_numbers(_rows), column_numbers(_columns);
	vector<vector<sset *> > row_sets(_rows), column_sets(_columns);

	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _columns; j++)
		{
			if (_m[i][j] == -1)
			{
				row_totals[i].id = i;
				row_totals[i].count++;
				column_totals[j].id = j;
				column_totals[j].count++;
				row_sets[i].push_back(&_s[i][j]);
				column_sets[j].push_back(&_s[i][j]);
			}
		}
	}
	/* sort by increasing number of unknowns both row-wise and column-wise */
	sort(row_totals.begin(), row_totals.end(), comp_fn);
	sort(column_totals.begin(), column_totals.end(), comp_fn);
	/* 
	 * Visit each such row and find out if any element 
	 * occurs exactly in one set and not in any other. If so such an
	 * element has to occur in that position alone.
	 */
	 for (int i = 0; i < _rows; i++)
	 {
		 int rownumber = row_totals[i].id;
#ifdef DEBUG
		 cout << "Eliminate row number = " << rownumber << endl;
#endif
		 find_independant_elements(row_sets[rownumber]);
		 if (notdone() == false)
			 return;
	 }
	
	 /*
	  * and every such column as well
	  */
	 for (int i = 0; i < _columns; i++)
	 {
		 int columnnumber = column_totals[i].id;
#ifdef DEBUG
		 cout << "Eliminate column number = " << columnnumber << endl;
#endif
		 find_independant_elements(column_sets[columnnumber]);
		 if (notdone() == false)
			 return;
	 }
	 return;
}

void matrix::solve(void)
{
	for (;;) {
		row_column_eliminate();
#ifdef DEBUG
		cout << "Number filled = " << _num_filled << " Rows = " << _rows << " Columns = " << _columns << endl;
#endif
		if (notdone() == false)
			break;
	}
}
