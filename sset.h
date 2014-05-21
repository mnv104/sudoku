#ifndef _SSET_H
#define _SSET_H

#include <stdio.h>
#include <set>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

class matrix;
class sset;

class matrix {
	private:
		int  _rows, _columns;
		int **_m;
		sset **_s;
		int find_independant_elements(vector<sset *> &);
		void eliminate_element(int, int, int *);
		void row_column_eliminate(void);
		static int row_adjust[3][2];
		int  _num_filled;
	public:
		matrix() : _rows(0), _columns(0), _m(NULL), _num_filled(0) { }
		matrix(int rows, int columns);  
		~matrix();
		bool notdone(void) { return _num_filled < (_rows * _columns); }
		void show(void);
		void  setrows(int rows) { _rows = rows; }
		void  setcolumns(int columns) { _columns = columns;  }
		int  rows(void) { return _rows; }
		int  columns(void) { return _columns; }
		int  element(int i, int j) { if (_m) return _m[i][j]; else assert(0); }
		void input(int rows, int columns, int **m);
		void input(char *);
		void solve(void);
		void input(matrix &);
		void symmetric_difference(const sset &set1, const sset &set2, set<int, less<int> >&result);
		void symmetric_difference(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		void difference(const sset &set1, const sset &set2, set<int, less<int> >&result);
		void difference(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		void difference(const set<int, less<int> >&set1, const set<int, less<int> > &set2, set<int, less<int> >&result);
		void intersection(const sset &set1, const sset &set2, set<int, less<int> >&result);
		void intersection(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		void setunion(const sset &set1, const sset &set2, set<int, less<int> >&result);
		void setunion(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		void setunion(const set<int, less<int> >&set1, const set<int, less<int> > &set2, set<int, less<int> >&result);
};

class sset {

	private:
		set<int, less<int> > *_s;
		int       _id;
		int       _count;
		int       _row, _col;

	public:
		sset() : _id(-1), _count(9) { 
			_s = new set<int>; 
			assert(_s); 
			for (int i = 1; i < 10; i++) 
				_s->insert(i);
		}
		sset(int id) : _id(id), _count(9) {
			_s = new set<int>; 
			assert(_s); 
			for (int i = 1; i < 10; i++) 
				_s->insert(i);
		}
		sset(int id, int max) : _id(id), _count(max) {
			_s = new set<int>; assert(_s); 
			for (int i = 1; i < max; i++) 
				_s->insert(i); 
		}
		void setid(int id) { _id = id; }
		int  getid(void)   { return _id; }
		void setrowcol(int col, int row) { _col = col; _row = row;}
		void getrowcol(int &col, int &row) { col = _col; row = _row; }
		~sset() { 
			delete _s; 
		}
		/* must be called when # of elements in set is equal to 1 */
		int  head(void) {
			if (_count != 1)
			{
				return -1;
			}
			return *(_s->begin());
		}
		bool find(int val) {
			return _s->find(val) != _s->end();
		}
		void singleton(int val) { 
			for (set<int>::iterator it = _s->begin(); it != _s->end(); it++) 
			{ 
				if (*it == val) 
					continue; 
				else 
					_s->erase(it);
			}
			_count = 1;
			return;
		}
		int remove(int val) {
			set<int>::iterator it = _s->find(val); 
			if (it != _s->end()) {
				_s->erase(it); 
				_count--;
				assert(_count > 0);
				return _count;
			}
			return -1; 
		}
		void show(void) { 
			cout << "Set # " << setw(3) << _id << " [Row " << _row << " Col " <<  _col << " ] has " << _count << " elements " << endl;
			for (set<int>::iterator it = _s->begin(); it != _s->end(); it++) 
				cout << " " << *it; 
			cout << endl;
		}
		void show(int less) {
			for (set<int>::iterator it = _s->begin(); it != _s->end(); it++) 
				cout << " " << *it; 
			cout << endl;
		}
		friend void matrix::symmetric_difference(const sset &set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::symmetric_difference(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::difference(const sset &set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::difference(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::intersection(const sset &set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::intersection(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::setunion(const sset &set1, const sset &set2, set<int, less<int> >&result);
		friend void matrix::setunion(const set<int, less<int> >&set1, const sset &set2, set<int, less<int> >&result);
};

#endif
