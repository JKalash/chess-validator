//
//  Knight.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/27/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Knight_h
#define Knight_h

#include <algorithm>    // Used for find method (std namespace)
#include <math.h>

#include "Piece.h"

class Knight : public Piece
{
    
public:
    //Force print overload
    virtual ostream& operator<< (ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "Knight";
    }
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() override
    {
        //All locations that share the piece's file (col) or rank (row)
        int row = _location.first, col = _location.second;
        
        //Allowed moves are the ones at a square distance of 5
        // i.e diff in file squared + diff in rank squared equals 5
        vector<pair<int, int>> legalMoves;
        for(int i =0; i < 7; i++)
            for(int j = 0; j < 7; j++) {
                int rank_diff = abs(i - row);
                int file_diff = abs(j - col);
                
                if (5 == rank_diff * rank_diff + file_diff * file_diff)
                    legalMoves.push_back(make_pair(i, j));
            }
        
        return legalMoves;
    }
    
    //Returns true if piece can do legal move to newLocation
    virtual bool isLegalMove(pair<int, int> newLocation) override
    {
        //Cannot move to position where we're at
        if ( _location == newLocation) return false;
        
        //Legal move if contained in list of legalMoves
        vector<pair<int, int>> legalMoves = this->legalMoves();
        return find(legalMoves.begin(), legalMoves.end(), newLocation) != legalMoves.end();
    }

};

#endif /* Knight_h */
