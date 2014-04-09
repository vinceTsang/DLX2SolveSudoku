#include "DLX.h"
ExactCoverMatrix::ExactCoverMatrix( int r, int c, int **matrix)
{
	ROWS = r;
	COLS = c;
	result = new int[r+1];
	ColIndex = new ColHeader*[c+1];
	RowIndex = new Node*[r];
	for( int i = 0; i <= c; ++i ) {
        ColIndex[i] = new ColHeader;
	}
	for ( int i = 0; i < r; ++i ) {
        RowIndex[i] = new Node;
	}
	root = ColIndex[0];
	ColIndex[0]->L = ColIndex[COLS];
	ColIndex[0]->R = ColIndex[1];
	ColIndex[COLS]->R = ColIndex[0];
	ColIndex[COLS]->L = ColIndex[COLS-1];
	for ( int i = 1; i < COLS; ++i ) {
		ColIndex[i]->L = ColIndex[i-1];
		ColIndex[i]->R = ColIndex[i+1];
	}
	for ( int i = 0; i <=COLS; ++i ) {
		ColIndex[i]->U=ColIndex[i]->D=ColIndex[i];
		ColIndex[i]->col = i;
	}
	ColIndex[0]->D = RowIndex[0];
	for ( int i = 0; i < ROWS; ++i ) {
		for ( int j = 0; j < COLS && matrix[i][j] > 0; ++j ) {
                /**
                 * matrix is not the real one
                 * it's just the one whose elements represent the column to be insert
                 * notice that the real matrix's column start from 1
                 * and its row start from 0.
                 */
                insert ( i, matrix[i][j] );
		}
	}
}

ExactCoverMatrix::~ExactCoverMatrix()
{
	delete[] result;
	for ( int i = 1; i <= COLS; ++i ) {
		Node *cur = ColIndex[i]->D;
		Node *next = cur->D;
		while( cur != ColIndex[i] ){
			delete cur;
			cur = next;
			next = next->D;
		}
		delete ColIndex[i];
	}
	delete[] RowIndex;
	delete[] ColIndex;
}

void ExactCoverMatrix::insert(const int r, const int c)
{
	Node *cur = ColIndex[c];
	ColIndex[c]->size++;
	Node *newNode = new Node(r,c);
	while ( (cur->D != ColIndex[c]) && (cur->D->row < r) )
		cur = cur->D;
	newNode->D = cur->D;
	newNode->U = cur;
	cur->D->U = newNode;
	cur->D = newNode;
	if ( RowIndex[r]->R == NULL ){
		RowIndex[r]->R = newNode;
		newNode->L = newNode;
		newNode->R = newNode;
	} else {
		Node* rowHead = RowIndex[r]->R;
		cur = rowHead;
		while ( cur->R != rowHead && cur->R->col < c)
			cur = cur->R;
		newNode->R = cur->R;
		newNode->L = cur;
		cur->R->L = newNode;
		cur->R = newNode;
	}
}

void ExactCoverMatrix::cover(int c)
{
	ColHeader *col = ColIndex[c];
	col->R->L = col->L;
	col->L->R = col->R;
	Node *curR, *curC;
	curC = col->D;
	while ( curC != col ) {
		curR = curC->R;
		while ( curR != curC) {
			curR->D->U = curR->U;
			curR->U->D = curR->D;
			ColIndex[curR->col]->size--;
			curR = curR->R;
		}
		curC = curC->D;
	}
}

void ExactCoverMatrix::uncover( int c )
{
	Node * curR, *curC;
	ColHeader *col = ColIndex[c];
	curC = col->U;
	while( curC != col ) {
		curR = curC->L;
		while ( curR != curC ) {
			ColIndex[curR->col]->size++;
			curR->D->U = curR;
			curR->U->D = curR;
			curR = curR->L;
		}
		curC = curC->U;
	}
	col->R->L = col;
	col->L->R = col;
}

int ExactCoverMatrix::search( int k )
{
    int cur_k = k;
    cout << "searching " << cur_k <<" level..." << endl;
	if( root->R == root ){
        return cur_k;
	}
	ColHeader *choose = (ColHeader*)root->R;
	ColHeader *cur = choose;
	while ( cur !=root ) {
		if( choose->size > cur->size )
			choose = cur;
		cur = (ColHeader*)cur->R;
	}
	if ( choose->size <=0 ){
		return -1;
	}

	cover( choose->col );
	Node *curR = choose->D;
	while ( curR != choose ) {
		result[cur_k] = curR->row;
		Node * node = curR->R;
		while ( node != curR ) {
			cover( node->col );
			node = node->R;
		}
		int res = -1;
		if ( (res = search( cur_k + 1 ) ) > 0 )
			return res;
		node = curR->L;
		while ( node != curR ) {
			uncover( node->col );
			node = node->L;
		}
		curR = curR->D;
	}
	uncover( choose->col );
	return -1;
}
