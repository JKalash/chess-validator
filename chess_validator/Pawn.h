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
    
    Pawn(bool isWhite, const pair<int, int>& initialLocation) : Piece(isWhite, initialLocation) { }
    
    //Force print overload
    virtual ostream& print(ostream& out) const override
    {
        return out << (this->_white ? "White's " : "Black's ") << "Pawn";
    }
    
    //List of allowed moves, disregarding board positioning
    //WE WILL NOT IMPLEMENT
    virtual vector<pair<int, int>> legalMoves() override
    {
        assert(true && "VIRTUAL LEGAL MOVES SHOULD NEVER BE CALLED ON PAWN");
        return {};
    }
    
    //Returns true if piece can do legal move to newLocation
    //WE WILL NOT IMPLEMENT
    virtual bool isLegalMove(const pair<int, int>& newLocation) override
    {
        
        return false;
    }
    
    //Pawn has custom methods for legal moves becauuse it needs to know
    //The board state to check two diagonal top neighbors
    vector<pair<int, int>> pawnLegalMoves(const vector<vector<Piece *>>& board)
    {
        
        
        vector<pair<int, int>> standardLegalMoves;
        
        //Going 1 step forward for white decreases row by 1, and it increases it by 1 for black
        int rowIncrement = isWhite()? -1 : 1;
        
        //A pawn can always go up 1 provided there is an empty spot on top of it
        if(  _location.first+rowIncrement >= 0 && _location.first+rowIncrement < 8 && !board[_location.first+rowIncrement][_location.second])
            standardLegalMoves.push_back(make_pair(_location.first+rowIncrement, _location.second));
            
        //If pawn in initialPosition, can jump 2 if space is empty
        //initialposition of the piece is rowIncrement modulo 7 (1 if black, 6 if white)
        
        if( ( (_location.first == 1 && !isWhite()) || (_location.first == 6 && isWhite()) ) &&  //If on initial ROW AND
           !board[_location.first + 2 * rowIncrement][_location.second] )                        // There is an empty spot 2 jumps ahead
            standardLegalMoves.push_back(make_pair(_location.first+ 2 * rowIncrement, _location.second));
        
        //Diagonals can eaten only
        //Requirement if a diagonal is available + has an oppenent piece -> can go there
        
        
        //Makw sure we are not at the top / bottom border
        if(_location.first + rowIncrement >= 0 && _location.first + rowIncrement < 8)
        {
            
            //Make sure we are not at the left border
            if(_location.second >=0)
            {
                pair<int, int> topLeft = make_pair(_location.first + rowIncrement, _location.second-1);
                
                //If there is a piece and it is of different color
                if(board[topLeft.first][topLeft.second] && board[topLeft.first][topLeft.second]->isWhite() != this->isWhite())
                    standardLegalMoves.push_back(topLeft);
            }
            
            //Make sure we are not at the right border
            if(_location.second < 7)
            {
                pair<int, int> topRight = make_pair(_location.first + rowIncrement, _location.second+1);
                
                //If there is a piece and it is of different color
                if(board[topRight.first][topRight.second] && board[topRight.first][topRight.second]->isWhite() != this->isWhite())
                    standardLegalMoves.push_back(topRight);
            }
        }
        
        return standardLegalMoves;
        
    }
    
    bool isLegalPawnMove(const pair<int, int>& newLocation, vector<vector<Piece *>> board)
    {
        //Cannot move to position where we're at
        if ( _location == newLocation) return false;
        
        //Legal move if contained in list of legalMoves
        vector<pair<int, int>> legalMoves = this->pawnLegalMoves(board);
        return find(legalMoves.begin(), legalMoves.end(), newLocation) != legalMoves.end();
    }
    
};


#endif /* Pawn_h */
