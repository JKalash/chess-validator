//
//  Pawn.h
//  chess_validator
//
//  Created by Joseph Kalash on 11/26/17.
//  Copyright © 2017 Joseph Kalash. All rights reserved.
//

#ifndef Pawn_h
#define Pawn_h

#include "Piece.h"

class Pawn : public Piece
{
    public:
    //Force print overload
    virtual ostream& operator<< (ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "Pawn";
    }
    
    //List of allowed moves, disregarding board positioning
    virtual vector<pair<int, int>> legalMoves() override //Note the = 0; This makes the method pure virtual, and makes the class implicitly abstract
    {
        
        //Can move one step forward. Forward for white is upward. Downward if black
        if(_white) {
            vector<pair<int, int>> movesList{ make_pair(min(7, _location.first+1) , _location.second)} ;
            
            //If initial, can also move 2 steps
            //Initial is rank 2 if white
            if( _location.first == 1 )
                movesList.push_back(make_pair(3, _location.second));
            
            return movesList;
        }
        else {
            vector<pair<int, int>> movesList{ make_pair(max(0, _location.first-1) , _location.second)} ;
            
            //If initial, can also move 2 steps
            //Initial is rank 7 if black
            if( _location.first == 7 )
                movesList.push_back(make_pair(5, _location.second));
            
            return movesList;
        }
        
    }
    
};


#endif /* Pawn_h */
