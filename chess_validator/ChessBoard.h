//
//  ChessBoard.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/27/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Board_h
#define Board_h

#include <vector>
#include <utility>

#include "Rook.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

class ChessBoard
{
    vector<vector<Piece *>> _board;   //2D array of pointers to pieces. When there is no piece, the pointer is NULL
    
    void deleteBoard()
    {
        for(int i =0; i < _board.size(); i++)
            for(int j = 0; j < _board[i].size(); j++)
                if(_board[i][j])
                    delete _board[i][j];
    }
public:
    ChessBoard()
    {
        resetBoard();
    }
    
    ~ChessBoard()
    {
        this->deleteBoard();
    }
    
    
    void resetBoard()
    {
        
        //Delete any existing board
        deleteBoard();
        
        
        _board = vector<vector<Piece *>> {
            {new Rook(false, make_pair(0, 0)), new Knight(false, make_pair(0, 1)), new Bishop(false, make_pair(0, 2)), new Queen(false, make_pair(0, 3)), new King(false, make_pair(0, 4)), new Bishop(false, make_pair(0, 5)), new Knight(false, make_pair(0, 6)), new Rook(false, make_pair(0, 7))},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {new Rook(true, make_pair(7, 0)), new Knight(true, make_pair(7, 1)), new Bishop(true, make_pair(7, 2)), new Queen(true, make_pair(7, 3)), new King(true, make_pair(7, 4)), new Bishop(true, make_pair(7, 5)), new Knight(true, make_pair(7, 6)), new Rook(true, make_pair(7, 7))},
        };
    }
    
    void submitMove(string from, string to)
    {
        
    }
    
    
};

#endif /* Board_h */
