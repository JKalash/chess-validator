//
//  Rook.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/26/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Rook_h
#define Rook_h

#include "Piece.h"

using namespace std;

class Rook : public Piece
{
    
public:
    //Force print overload
    virtual ostream& operator<< (ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "Rook";
    }
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() override //Note the = 0; This makes the method pure virtual, and makes the class implicitly abstract
    {
        //All locations that share the piece's file (col) or rank (row)
        int row = _location.first, col = _location.second;
        vector<pair<int, int>> locationsWithSameFile{ make_pair(1, col), make_pair(2, col), make_pair(3, col), make_pair(4, col), make_pair(5, col), make_pair(6, col), make_pair(7, col), make_pair(8, col) };
        vector<pair<int, int>> locationsWithSameRank{ make_pair(row, 1), make_pair(row, 2), make_pair(row, 3), make_pair(row, 4), make_pair(row, 5), make_pair(row, 6), make_pair(row, 7), make_pair(row, 8) };
        
        
        //Remove our location from both lists
        locationsWithSameFile.erase(remove(locationsWithSameFile.begin(), locationsWithSameFile.end(), _location), locationsWithSameFile.end());
        locationsWithSameRank.erase(remove(locationsWithSameRank.begin(), locationsWithSameRank.end(), _location), locationsWithSameRank.end());
        
        //Concatenate both arrays into list of all allowed moves
        locationsWithSameFile.insert(locationsWithSameFile.end(), locationsWithSameRank.begin(), locationsWithSameRank.end());
        
        return locationsWithSameFile;
    }
    
    //Returns true if piece can do legal move to newLocation
    virtual bool isLegalMove(pair<int, int> newLocation) override
    {
        //Cannot move to position where we're at
        if ( _location == newLocation) return false;
        
        //Same file or rank
        return _location.first == newLocation.first || _location.second == newLocation.second;
    }
    
};

#endif /* Rook_h */
