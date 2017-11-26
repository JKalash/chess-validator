//
//  Piece.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/26/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Piece_h
#define Piece_h

#include <string>
#include <vector>
#include <utility>

using namespace std;

class Piece
{
protected:
    bool _white;
    pair<int, int> _location;
    
public:
    pair<int, int> matrixLocation() const
    {
        return _location;
    }
    string chessLocation() const
    {
        const char column = 'A' + _location.second;
        const int chessRow = 8 - _location.first;
        char row = '0' + chessRow;
        const char location[] = {column, row, '\0'};
        return string(location);
    }
    
    bool isWhite() const
    {
        return _white;
    }
    
    //Force print overload
    virtual ostream& operator<< (ostream& out) const;
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() = 0;  //Note the = 0; This makes the method pure virtual, and makes the class implicitly abstract
    
    //Returns true if piece can do legal move to newLocation
    virtual bool isLegalMove(pair<int, int> newLocation) = 0;
};

#endif /* Piece_h */
