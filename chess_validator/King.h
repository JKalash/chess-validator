//
//  King.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/27/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef King_h
#define King_h

#include <algorithm>
#include <math.h>

#include "Piece.h"

class King : public Piece
{
    
public:
    
    King(bool isWhite, const pair<int, int>& initialLocation) : Piece(isWhite, initialLocation) { }
    
    //Force print overload
    virtual ostream& operator<< (ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "King";
    }
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() override
    {
        //A King's legal moves are all the 8 neighbor locations
        
        vector<pair<int, int>> legalMoves{
            make_pair(_location.first-1, _location.second-1),       //top left
            make_pair(_location.first-1, _location.second),         //top
            make_pair(_location.first-1, _location.second+1),       //top right
            make_pair(_location.first, _location.second-1),         //left
            make_pair(_location.first, _location.second+1),         //right
            make_pair(_location.first+1, _location.second-1),       //bottom left
            make_pair(_location.first+1, _location.second),         //bottom
            make_pair(_location.first+1, _location.second+1)        //bottom right
        };
        
        //Remove from legalMoves those who are not in the board (file or rank not between 0-7)
        //To do this, we call the remove_if which takes a predicate function as parameter
        //That tells it when to remove something from an array. It returns the index of element to delete back to the
        //Erase function which removes the pair from the legalMoves list
        legalMoves.erase(remove_if(legalMoves.begin(),
                                   legalMoves.end(),
                                   [&](const pair<int, int>& position)-> bool
                                            { return position.first < 0 || position.first > 7 || position.second < 0 || position.second > 7; }
                                   ),
                         legalMoves.end()
                         );
        
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




#endif /* King_h */
