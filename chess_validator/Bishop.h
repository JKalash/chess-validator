//
//  Bishop.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/27/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Bishop_h
#define Bishop_h

#include <algorithm>
#include <math.h>

#include "Piece.h"

class Bishop : public Piece
{
    
public:
    
    Bishop(bool isWhite, const pair<int, int>& initialLocation) : Piece(isWhite, initialLocation) { }
    
    //Force print overload
    virtual ostream& print(ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "Bishop";
    }
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() override
    {
        //All locations that share the piece's file (col) or rank (row)
        int row = _location.first, col = _location.second;
        
        //A bishop may only move diagonally. The set of diagonal entries are all the non-zero distances
        //whose file diff equals rank diff
        vector<pair<int, int>> legalMoves;
        for(int i =0; i < 8; i++)
            for(int j = 0; j < 8; j++) {
                int rank_diff = abs(i - row);
                int file_diff = abs(j - col);
                
                if (0 != file_diff && file_diff == rank_diff)
                    legalMoves.push_back(make_pair(i, j));
            }
        
        return legalMoves;
    }
    
    //Returns true if piece can do legal move to newLocation
    virtual bool isLegalMove(const pair<int, int>& newLocation) override
    {
        //Cannot move to position where we're at
        if ( _location == newLocation) return false;
        
        //Legal move if contained in list of legalMoves
        vector<pair<int, int>> legalMoves = this->legalMoves();
        return find(legalMoves.begin(), legalMoves.end(), newLocation) != legalMoves.end();
    }
    
};


#endif /* Bishop_h */
