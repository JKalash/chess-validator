//
//  Queen.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/27/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Queen_h
#define Queen_h

#include <algorithm>
#include <math.h>

#include "Piece.h"

class Queen : public Piece
{
    
public:
    
    Queen(bool isWhite, const pair<int, int>& initialLocation) : Piece(isWhite, initialLocation) { }
    
    //Force print overload
    virtual ostream& print(ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "Queen";
    }
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() override
    {
        //A queen's legal moves is the union of the legal moves of a rook and a bishop.
        //That means any position whose file_diff equals rank diff, or shares the rank or file of the position to our queen
        //Is a legal move
        
        int row = _location.first, col = _location.second;
        
        vector<pair<int, int>> legalMoves;
        for(int i =0; i < 8; i++)
            for(int j = 0; j < 8; j++) {
                int rank_diff = abs(i - row);
                int file_diff = abs(j - col);
                
                if (0 != file_diff && file_diff == rank_diff)   //Bishop move set
                    legalMoves.push_back(make_pair(i, j));
                else if( (i == row || j == col) && max(rank_diff, file_diff) > 0)     //Rook move set
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


#endif /* Queen_h */
