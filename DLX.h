#ifndef DLX_H
#define DLX_H
#include <iostream>
using namespace std;
struct Node
{
	Node *L, *R, *U, *D;
	int col, row;
	Node(){
		L = R = U = D = NULL;
		col = row = 0;
	}
	Node( int r, int c ){
		L = R = U = D = NULL;
		col = c; row = r;
	}

};

struct ColHeader : public Node
{
	int size;
	ColHeader(){
		size = 0;
	}
};

class ExactCoverMatrix
{
	public:
		int ROWS, COLS;
		int *result;
		ExactCoverMatrix( int r, int c,int **matrix);
		~ExactCoverMatrix();
		void insert(const int r, const int c);
		void cover(const int c);
		void uncover(const int c);
		int search( int k );
	protected:
		ColHeader *root;
		ColHeader **ColIndex;
		Node **RowIndex;
};
#endif
