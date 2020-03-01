#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <limits.h>
#include <math.h>
using namespace std;

// utility to convert anything to a string
#include <sstream>
template<typename T> std::string tostring(const T& x) {
    std::ostringstream os;
    os << x;
    return os.str();
}

template <typename T>
class Cell {
    public:
        T data;
        Cell<T> *up;
        Cell<T> *down;
        Cell<T> *left;
        Cell<T> *right;
        bool active;
        int row;
        int col;
        string color;

        Cell() {
            up = nullptr;
            down = nullptr;
            left = nullptr;
            right = nullptr;
            active = false;
            data = "0";
            color = "\33[37m";
        }

        Cell(T data) {
            this->data = data;
            up = nullptr;
            down = nullptr;
            left = nullptr;
            right = nullptr;
            active = true;
        }
};

template <typename T>
class Excel {
    public:
        int rows;
        int cols;
        Cell<T> *head;
        Cell<T> *selected;

        Excel(){
            rows = 5;
            cols = 5;
            head = new Cell<T>();
            selected = head;

            this->makeGrid();
        }

        void makeGrid(){

            // after head cell
            Cell<T> *temp = head;
            
            // making first column
            for (int i = 1; i < rows; i++){
                Cell<T> *newCell = new Cell<T>();
                temp->down = newCell;
                newCell->up = temp;
                temp = temp->down;
            }

            temp = head;

            // making first row
            for (int i = 1; i < cols; i++){
                Cell<T> *newCell = new Cell<T>();
                temp->right = newCell;
                newCell->left = temp;
                temp = temp->right;
            }

            temp = head;
            
            // now making the rest of the grid
            Cell<T> *prevPointer = head;
            Cell<T> *currPointer = head->right;
            
            for (int i = 1; i < cols; i++){
                Cell<T> *prev = prevPointer->down;
                Cell<T> *curr = currPointer;
                for (int j = 1; j < rows; j++){
                    Cell<T> *newCell = new Cell<T>();
                    prev->right = newCell;
                    newCell->left = prev;
                    prev = prev->down;
                    curr->down = newCell;
                    newCell->up = curr;
                    curr = newCell;
                }
                prevPointer = currPointer;
                currPointer = currPointer->right;
            }
        }

        void insertAbove(){

            // head onto the first cell of the selected row
            Cell<T> *rowHead = selected;
            while(rowHead->left != nullptr){
                rowHead = rowHead->left;
            }

            if (rowHead->up){
                // if there is a row above the selected row

                Cell<T> *prevRowHead = rowHead->up;
                Cell<T> *prevPointer = prevRowHead->right;
                Cell<T> *currPointer = rowHead->right;

                Cell <T> *newRowHead = new Cell<T>();
                prevRowHead->down = newRowHead;
                newRowHead->up = prevRowHead;
                newRowHead->down = rowHead;
                rowHead->up = newRowHead;

                Cell<T> *newRowPointer = newRowHead;

                for (int i = 1; i < cols; i++){

                    Cell<T> *newCell = new Cell<T>();
                    newRowPointer->right = newCell;
                    newCell->left = newRowPointer;

                    prevPointer->down = newCell;
                    newCell->up = prevPointer;

                    currPointer->up = newCell;
                    newCell->down = currPointer;

                    newRowPointer = newCell;
                    prevPointer = prevPointer->right;
                    currPointer = currPointer->right;

                }
            }
            else{
                // this is the first row
                makeNewRowAtTop();
            }
            rows++;
        }

        void makeNewRowAtTop(){
            Cell<T> *newRowHead = new Cell<T>();
            Cell<T> *temp = head->right;
            head->up = newRowHead;
            newRowHead->down = head;

            Cell<T> *curr = newRowHead;
            for (int i = 1; i < cols; i++){
                Cell<T> *newCell = new Cell<T>();
                newCell->left = curr;
                curr->right = newCell;
                temp->up = newCell;
                newCell->down = temp;

                temp = temp->right;
                curr = newCell;
            }

            head = newRowHead;
        }

        void insertBelow(){

            // head onto the first cell of the selected row
            Cell<T> *rowHead = selected;
            while(rowHead->left != nullptr){
