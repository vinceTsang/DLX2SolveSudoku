#include "DLX.h"
#include <iostream>
using namespace std;

const int N_Cell = 3;
const int N = 9;
const int OFFSET = N * N;
const int ROWS = N * N * N;
const int COLS = N * N * 4;

/**
 * Transform a sudoku problem to an exact cover problem
 * @para grid is pointer to an array, whose elements are integer
 * it's the origin matrix
 */
void solveSudoku ( int (*grid)[N] )
{
    int ** sudoku_matrix = new int*[ROWS];
    //int sudoku_result[N][N];
    for ( int i = 0; i < ROWS; ++i ) {
        sudoku_matrix[i] = new int[5];
    }
    for ( int i = 0; i < N; ++i ){
        for ( int j = 0; j < N; ++j ) {
            int b = (i/N_Cell) * N_Cell + (j/N_Cell); //block
            int r = i*N*N + j*N;
            for ( int k = 1; k <= N; ++k ) {
                sudoku_matrix[r][0] = i * N + j + 1; //notice that the transformed matrix's col start from 1
                sudoku_matrix[r][1] = OFFSET + (i*N+k);
                sudoku_matrix[r][2] = OFFSET*2 + (j*N+k);
                sudoku_matrix[r][3] = OFFSET*3 + (b*N+k);
                sudoku_matrix[r][4] = -1; // tell  initializer of ExactCoverMatrix stop to insert
                ++r;
            }
        }
    }
    ExactCoverMatrix sudoku( ROWS, COLS, sudoku_matrix);
    //cover those have been filled
    for( int i = 0; i < N; ++i ) {
        for( int j = 0; j < N; ++j) {
            int k;
            if( k = grid[i][j] ){
                int b = (i/N_Cell)*N_Cell + j/N_Cell;
                sudoku.cover( i*N+j+1 );
                sudoku.cover( OFFSET + (i*N+k) );
                sudoku.cover( OFFSET*2 + (j*N+k) );
                sudoku.cover( OFFSET*3 + (b*N+k) );
                //sudoku_result[i][j] = k;
            }
        }
    }
    int solved = 0;
    if( (solved = sudoku.search(0)) > 0 ){
        int c, r, k;
        for ( int i = 0; i < solved; ++i ){
            r = sudoku.result[i] / (N*N);
            c = ( sudoku.result[i] / N ) % N;
            k = sudoku.result[i] % N;
            grid[r][c] = k + 1; //just change base on the old matrix
        }
        cout << "final result is:" << endl;
        for ( int i = 0; i < N; ++i ) {
            for ( int j = 0; j < N; ++j ) {
                cout << grid[i][j] <<' ';
            }
            cout << endl;
        }
    } else {
        cout << "no solution" << endl;
    }
}
bool verifyPuzzle( int (*grid)[N] )
{
    int line[N+1]={0};
    for( int i=0; i<N; i++ )
    {
        for( int j=0; j<=N; j++ )
            line[j] = 0;
        for( int j=0; j<N; j++ )
        {
            if( line[grid[i][j]] )
            {
                cout<<"row wrong: "<<i<<endl;
                return false;
            }
            else
                line[grid[i][j]] = 1;
        }
    }
    for( int i=0; i<N; i++ )
    {
        for( int j=0; j<N+1; j++ )
            line[j] = 0;
        for( int j=0; j<N; j++ )
        {
            if( line[grid[j][i]] )
            {
                cout<<"column wrong"<<endl;
                return false;
            }
            else
                line[grid[j][i]] = 1;
        }
    }
    for( int i=0; i<N_Cell; i++ )
        for( int j=0; j<N_Cell; j++ )
        {
            for( int x=0; x<=N; x++ )
                line[x] = 0;
            for( int k=0; k<N_Cell; k++ )
                for( int t=0; t<N_Cell; t++ )
                {
                    int x = k+i*N_Cell;
                    int y = t+j*N_Cell;
                    if( line[grid[x][y]] )
                    {
                        cout<<"block wrong"<<endl;
                        return false;
                    }
                    else
                        line[grid[x][y]] = 1;
                }
        }
    return true;
}

int main()
{
    int puzzle[N][N]={
        {7, 0, 0, 0, 6, 0, 0, 0, 9},
        {0, 2, 0, 0, 0, 8, 0, 0, 0},
        {0, 3, 0, 0, 0, 0, 0, 4, 0},
        {0, 9, 0, 0, 0, 0, 3, 0, 0},
        {0, 0, 8, 0, 0, 2, 6, 0, 0},
        {0, 0, 6, 0, 5, 0, 0, 8, 0},
        {0, 7, 0, 0, 0, 0, 0, 2, 0},
        {0, 0, 0, 2, 0, 0, 0, 3, 0},
        {9, 0, 0, 0, 7, 0, 0, 0, 4}};
    solveSudoku( puzzle );
    if( !verifyPuzzle( puzzle ) )
        cout<<"wrong answer!"<<endl;

}
