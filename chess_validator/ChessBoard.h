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
#include <assert.h>

#include "Rook.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

class ChessBoard
{
    vector<vector<Piece *>> _board;   //2D array of pointers to pieces. When there is no piece, the pointer is NULL
    
    bool isWhiteTurn = true;    //Keeps track of whose turn is next
    bool isCheck = false;       //Keeps track of wether a check has been made
    
    void deleteBoard()
    {
        for(int i =0; i < _board.size(); i++)
            for(int j = 0; j < _board[i].size(); j++)
                if(_board[i][j])
                    delete _board[i][j];
    }
    
    //Converts a string like 'A8' to a pair like (0, 0)
    pair<int, int> stringToPairPosition(string position)
    {
        
        //Invalid size
        if(position.length() != 2)
            return make_pair(-1, -1);
        
        char first = position[0];
        char second = position[1];
        
        //Enforce characters
        if(first < 'A' || first > 'H')
            return make_pair(-1, -1);
        if(second < '1' || second > '8')
            return make_pair(-1, -1);
        
        int col = first - 'A';
        int row = 7 - (second - '1');
        
        return make_pair(row, col);
    }
    
    bool clearPath(pair<int, int> from, pair<int, int> to)
    {
        //All pieces we are dealing with here check paths between two locations that are either
        //1. On top of each other (same col)
        //2. On the same row
        //3. On the same diagonal
        
        //A clear path means all nodes between them are NULL
        if (from.second == to.second)       //1.
        {
            //Check nodes in all rows between from and to, at column from.second = to.second
            int fromRow = min(from.first, to.first) + 1;
            int toRow = max(from.first, to.first);
            
            for(int row = fromRow; row < toRow; ++row)
                if(_board[row][from.second])
                    return false;       //If any location is NOT null, no clear path
            
            return true;
        }
        else if(from.first == to.first)     //2.
        {
            //Check nodes in all rows between from and to, at column from.second = to.second
            int fromCol = min(from.second, to.second) + 1;
            int toCol = max(from.second, to.second);
            
            for(int col = fromCol; col < toCol; ++col)
                if(_board[from.first][col])
                    return false;       //If any location is NOT null, no clear path
            
            return true;
        }
        else if( abs( from.first - to.first) ==  abs(from.second - to.second))      //3.
        {
            //Check all nodes along the diagonal.
            //Go from 'from' to 'to' one step at a time where one axis increases by 1 & the other decreases by 1
            
            
            int steps = abs( from.first - to.first) - 1;
            int rowIncrementor = from.first < to.first ? 1 : -1;
            int colIncrementor = from.second < to.second ? 1 : -1;
            
            for(int i = 1; i <= steps; i++) {
                if(_board[from.first+ i * rowIncrementor][from.second+ i * colIncrementor])
                    return false;
            }
            
            return true;
            
        }
        
        //Should ideally never be called as we are not interested in other random positionings. assume no clear path
        return false;
    }
    
    //returns true if king (white/black) is in check
    bool moveCausedCheck(bool whiteKing)
    {
        //We need to check if the king is in direct threat by a white piece that can reach it
        pair<int, int> kingInCheckPosition;
        vector<Piece *> opponentPieces;
        
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                //If piece exists
                if(_board[i][j]) {
                    //If piece is opponent color
                    if(_board[i][j]->isWhite() != whiteKing)
                        opponentPieces.push_back(_board[i][j]);
                    else {
                        //Try to see if it's the king
                        King* kingTest = dynamic_cast<King *>(_board[i][j]);
                        if(kingTest)
                            //We found the king
                            kingInCheckPosition = make_pair(i, j);
                    }
                }
        
        //We now have a vector of all opponent pieces and the location of our king.
        //If there is a clear path from a piece to us and we are in its legalMove list, king is in check
        for(Piece * piece : opponentPieces)
        {
            // Get the piece's legal move list. Try to cast to pawn, fallback to regular piece
            Pawn* pawnTest = dynamic_cast<Pawn *>(piece);
            vector<pair<int, int>> legalMoves;
            if(pawnTest) legalMoves = pawnTest->pawnLegalMoves(_board);
            else legalMoves = piece->legalMoves();
            
            //If king position is in legal moves
            if( find(legalMoves.begin(), legalMoves.end(), kingInCheckPosition) != legalMoves.end())
                //If there is a clear path between the piece and the king
                if(this->clearPath(piece->matrixLocation(), kingInCheckPosition))
                    return true;    //King in check
        }

        return false;
    }
    
    bool moveCausedStaleMate()
    {
        // A Stalement move is one that is NOT a check where ANY move is a check state (one wehre the king is in check)
        if(moveCausedCheck(!isWhiteTurn))
            return false;
        
        //A 'checkmate' situation where the king is not in check, is a stale situation
        return moveCausedCheckmate();
    }
    
    //returns true if last move done caused a checkmate
    bool moveCausedCheckmate()
    {
        
        //If isWhiteTurn, check if black king is checkmate otherwhise check if white king is checkmate
        bool whiteInCheckmate = !isWhiteTurn;
        
        vector<Piece*> victimPieces;
        for(int i = 0; i < 7; i++)
            for(int j = 0; j < 7; j++)
                if(_board[i][j] && _board[i][j]->isWhite() == whiteInCheckmate)
                    victimPieces.push_back(_board[i][j]);
        
        //A check mate is a state where all potential moves by any of the victim's pieces
        //do not resolve the check
        bool foundPossibleMove = false;
        for(Piece* potentialPiece : victimPieces)
        {
            
            //No need to keep checking if we found a possible move
            if(foundPossibleMove) break;
            
            vector<pair<int, int>> potentialPieceMoves;
            Pawn* pawnTest = dynamic_cast<Pawn *>(potentialPiece);
            if(pawnTest) potentialPieceMoves = pawnTest->pawnLegalMoves(_board);
            else potentialPieceMoves = potentialPiece->legalMoves();
                    
            for(int i = 0; i < potentialPieceMoves.size(); i++)
            {
                
                //A possible move is one where
                //1. moving there is possible (no blocking piece + does not contain a piece of same type)
                pair<int, int> potentialMove = potentialPieceMoves[i];
                if(!clearPath(potentialPiece->matrixLocation(), potentialMove))    //no clear path. cannot do move. continue
                     continue;
                
                Piece *destination = _board[potentialMove.first][potentialMove.second];
                if(destination && destination->isWhite() == whiteInCheckmate)  //Destination is same color as our king. cannot move. continue.
                    continue;
                
                //2. moving there removes check
                // To verify this, simulate a move and make sure it removes the check
                pair<int, int> fromPair = potentialPiece->matrixLocation();
                _board[potentialMove.first][potentialMove.second] = _board[fromPair.first][fromPair.second];
                _board[fromPair.first][fromPair.second] = NULL;
                
                //If check still there, move is illegal. Reverse and continue
                if(moveCausedCheck(whiteInCheckmate))
                {
                    _board[fromPair.first][fromPair.second] = _board[potentialMove.first][potentialMove.second];
                    _board[potentialMove.first][potentialMove.second] = destination;
                    continue;
                }
                else
                {
                    //Valid move that removes checkmate. Revert and mark that we found potentialMove so we can break
                    _board[fromPair.first][fromPair.second] = _board[potentialMove.first][potentialMove.second];
                    _board[potentialMove.first][potentialMove.second] = destination;
                    foundPossibleMove = true;
                    break;
                }
            }
        }
        
        return !foundPossibleMove; //Checkmate if we did not find a possible move
    }
    
public:
    ChessBoard()
    {
        resetBoard();
    }
    
    ~ChessBoard()
    {
        deleteBoard();
    }
    
    void resetBoard()
    {
        
        //Delete any existing board
        deleteBoard();
        
        
        _board = vector<vector<Piece *>> {
            {new Rook(false, make_pair(0, 0)), new Knight(false, make_pair(0, 1)), new Bishop(false, make_pair(0, 2)), new Queen(false, make_pair(0, 3)), new King(false, make_pair(0, 4)), new Bishop(false, make_pair(0, 5)), new Knight(false, make_pair(0, 6)), new Rook(false, make_pair(0, 7))},
            {new Pawn(false, make_pair(1, 0)), new Pawn(false, make_pair(1, 1)), new Pawn(false, make_pair(1, 2)), new Pawn(false, make_pair(1, 3)), new Pawn(false, make_pair(1, 4)), new Pawn(false, make_pair(1, 5)), new Pawn(false, make_pair(1, 6)), new Pawn(false, make_pair(1, 7))},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
            {new Pawn(true, make_pair(6, 0)), new Pawn(true, make_pair(6, 1)), new Pawn(true, make_pair(6, 2)), new Pawn(true, make_pair(6, 3)), new Pawn(true, make_pair(6, 4)), new Pawn(true, make_pair(6, 5)), new Pawn(true, make_pair(6, 6)), new Pawn(true, make_pair(6, 7))},
            {new Rook(true, make_pair(7, 0)), new Knight(true, make_pair(7, 1)), new Bishop(true, make_pair(7, 2)), new Queen(true, make_pair(7, 3)), new King(true, make_pair(7, 4)), new Bishop(true, make_pair(7, 5)), new Knight(true, make_pair(7, 6)), new Rook(true, make_pair(7, 7))},
        };
        
        isWhiteTurn = true;
        isCheck = false;
        
        cout << "A new chess game is started!" << endl;
    }
    
    void submitMove(string from, string to)
    {
        pair<int, int> fromPair = stringToPairPosition(from);
        pair<int, int> toPair = stringToPairPosition(to);
        
        assert(fromPair != make_pair(-1, -1) && "Invalid from position");
        assert(toPair != make_pair(-1, -1) && "Invalid to position");
        
        Piece *fromPiece = _board[fromPair.first][fromPair.second];
        
        //Check if from position contains piece
        if( fromPiece == NULL)
        {
            cout << "There is no piece at position " << from << "!" << endl;
            return;
        }
        
        //Check if from position's color matches that of next player turn
        bool fromPieceIsWhite = fromPiece->isWhite();
        if(fromPieceIsWhite && !isWhiteTurn)
        {
            cout << "It is not White’s turn to move!" << endl;
            return;
        }
        
        if(!fromPieceIsWhite && isWhiteTurn)
        {
            cout << "It is not Black's turn to move!" << endl;
            return;
        }
        
        //If we reach this point, a proper piece wants to be moved by the right player.
        //Check if piece can be moved from 'from' to 'to'
        
        //1. Check that 'to' is a legal move from from
        //Try to cast piece to pawn otherwise fallback to regular piece
        Pawn * pawnTest = dynamic_cast<Pawn*>(fromPiece);
        if(  (pawnTest && !pawnTest->isLegalPawnMove(toPair, _board)) ||
             (!pawnTest && !fromPiece->isLegalMove(toPair))
           ) {
           cout << *fromPiece << " cannot move to " << to << "!" << endl;
            return;
        }
        
        
        //If legal move, check that no obstructions from 'from' to 'to', unless it's a knight
        
        //knightTest will be NULL if it's not a knight when trying to cast it *dynamically*
        Knight *knightTest = dynamic_cast<Knight *>(fromPiece);
        if(knightTest || clearPath(fromPair, toPair))   //If piece is a knight, or there's a clear path
        {
            
            //We can reach destination.
            
            //If state is check, this move needs to *uncheck* otherwise cannot move
            if(isCheck)
            {
                
                //Simulate the move and test for no more check
                Piece *tempDestinationPieceCopy = _board[toPair.first][toPair.second];
                _board[toPair.first][toPair.second] = _board[fromPair.first][fromPair.second];
                _board[fromPair.first][fromPair.second] = NULL;
                
                //If check still there, move is illegal
                if(moveCausedCheck(isWhiteTurn))
                {
                    cout << *_board[toPair.first][toPair.second] << " cannot move to " << to << "!" << endl;
                    
                    //reverse and return
                    _board[fromPair.first][fromPair.second] = _board[toPair.first][toPair.second];
                    _board[toPair.first][toPair.second] = tempDestinationPieceCopy;
                    return;
                }
                
                //If reach this point here, it means that the move causes the check to be gone. Still need to verify
                //Move validity (the destination doesn't have a piece of the same color as us) -> can be done as regular check (outside this if statement)
                
                //Reverse first
                _board[fromPair.first][fromPair.second] = _board[toPair.first][toPair.second];
                _board[toPair.first][toPair.second] = tempDestinationPieceCopy;
                
            }
            
            
            //Destination needs to either be clear or contain an *opponent* piece
            Piece* destinationPiece = _board[toPair.first][toPair.second];
            
            if(destinationPiece == NULL)
            {
                //Just move the piece to destination
                _board[toPair.first][toPair.second] = _board[fromPair.first][fromPair.second];
                _board[toPair.first][toPair.second]->updateLocation(toPair);    //Update location property of the piece
                
                _board[fromPair.first][fromPair.second] = NULL;
                
                cout << *_board[toPair.first][toPair.second] << " moves from " << from << " to " << to << endl;
                
                //No check
                isCheck = false;
            }
            else if(destinationPiece->isWhite() != fromPiece->isWhite())
            {
                //Move and eat
                _board[toPair.first][toPair.second] = _board[fromPair.first][fromPair.second];
                _board[toPair.first][toPair.second]->updateLocation(toPair);    //Update location property of the piece
                
                _board[fromPair.first][fromPair.second] = NULL;
                
                cout << *_board[toPair.first][toPair.second] << " moves from " << from << " to " << to << " taking " << *destinationPiece << endl;
                
                //Delete unnecessary piece
                delete destinationPiece;
                
                //No check
                isCheck = false;
            }
            else
            {
                //Trying to move to a position containing on of our pieces
                cout << *fromPiece << " cannot move to " << to << "!" << endl;
                return;
            }
            
            //Move successful, verify if move caused check on other player, or a checkmate state or stalemate
            
            //Move successful, verify if move caused check on other player, or a checkmate state
            isCheck = moveCausedCheck(!isWhiteTurn);
            
            if(isCheck)
            {
                if (moveCausedCheckmate())
                {
                    cout << (isWhiteTurn ? "Black" : "White") << " is in checkmate" << endl;
                    return;
                }
                
                cout << (isWhiteTurn ? "Black" : "White") << " is in check" << endl;
            }
            
            else if(moveCausedStaleMate())
            {
                cout << "Game is in stalemate!" << endl;
                
            }
            
            isWhiteTurn = !isWhiteTurn;    //Flip turn
            
        }
        else    //No clear path, cannot move
        {
            //Trying to move to a position containing on of our pieces
            cout << *fromPiece << " cannot move to " << to << "!" << endl;
        }
            
    }
    
    
};

#endif /* Board_h */
