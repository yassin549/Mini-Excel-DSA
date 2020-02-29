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