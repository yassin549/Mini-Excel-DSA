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

                rowHead = rowHead->left;
            }

            if (rowHead->down){
                // if there is a row below the selected row

                Cell<T> *nextRowHead = rowHead->down;
                Cell<T> *nextPointer = nextRowHead->right;
                Cell<T> *currPointer = rowHead->right;

                Cell <T> *newRowHead = new Cell<T>();
                nextRowHead->up = newRowHead;
                newRowHead->down = nextRowHead;
                newRowHead->up = rowHead;
                rowHead->down = newRowHead;

                Cell<T> *newRowPointer = newRowHead;

                for (int i = 1; i < cols; i++){

                    Cell<T> *newCell = new Cell<T>();
                    newRowPointer->right = newCell;
                    newCell->left = newRowPointer;

                    nextPointer->up = newCell;
                    newCell->down = nextPointer;

                    currPointer->down = newCell;
                    newCell->up = currPointer;

                    newRowPointer = newCell;
                    nextPointer = nextPointer->right;
                    currPointer = currPointer->right;

                }
            }
            else{
                // this is the last row
                makeNewRowAtBottom();

            }
            rows++;
        }

        void makeNewRowAtBottom(){
            Cell<T> *rowHead = head;
            while (rowHead->down != nullptr){
                rowHead = rowHead->down;
            }

            Cell<T> *newRowHead = new Cell<T>();
            Cell<T> *temp = rowHead->right;
            rowHead->down = newRowHead;
            newRowHead->up = rowHead;

            Cell<T> *curr = newRowHead;
            for (int i = 1; i < cols; i++){
                Cell<T> *newCell = new Cell<T>();
                newCell->left = curr;
                curr->right = newCell;
                temp->down = newCell;
                newCell->up = temp;
                temp = temp->right;
                curr = newCell;
            }
        }

        void insertRight(){
            
            // get the first cell of the selected column
            Cell<T> *colHead = selected;
            while (colHead->up != nullptr){
                colHead = colHead->up;
            }

            if (colHead->right){
                // if there is a column to the right of the selected column

                Cell<T> *nextColHead = colHead->right;
                Cell<T> *nextPointer = nextColHead->down;
                Cell<T> *currPointer = colHead->down;
                Cell<T> *newColHead = new Cell<T>();
                newColHead->left = colHead;
                colHead->right = newColHead;
                newColHead->right = nextColHead;
                nextColHead->left = newColHead;

                for (int i = 1; i < rows; i++){
                    Cell<T> *newCell = new Cell<T>();
                    newColHead->down = newCell;
                    newCell->up = newColHead;
                    nextPointer->left = newCell;
                    newCell->right = nextPointer;
                    currPointer->right = newCell;
                    newCell->left = currPointer;
                    newColHead = newCell;
                    nextPointer = nextPointer->down;
                    currPointer = currPointer->down;
                }
            }
            else{
                // if it is the last column
                makeNewColumnAtRight();
            }

            cols++;
        }

        void makeNewColumnAtRight(){
            Cell<T> *colHead = head;
            while (colHead->right != nullptr){
                colHead = colHead->right;
            }

            Cell<T> *newColHead = new Cell<T>();
            Cell<T> *temp = colHead->down;
            colHead->right = newColHead;
            newColHead->left = colHead;
            Cell<T> *curr = newColHead;

            for (int i = 1; i < rows; i++){
                Cell<T> *newCell = new Cell<T>();
                curr->down = newCell;
                newCell->up = curr;
                temp->right = newCell;
                newCell->left = temp;

                curr = newCell;
                temp = temp->down;
            }
        }

        void insertLeft(){

            // get the first cell of the selected column
            Cell<T> *colHead = selected;
            while (colHead->up != nullptr){
                colHead = colHead->up;
            }

            if (colHead->left){
                // if there is a column to the left of the selected column

                Cell<T> *prevColHead = colHead->left;
                Cell<T> *prevPointer = prevColHead->down;
                Cell<T> *currPointer = colHead->down;
                Cell<T> *newColHead = new Cell<T>();
                newColHead->right = colHead;
                colHead->left = newColHead;
                newColHead->left = prevColHead;
                prevColHead->right = newColHead;

                for (int i = 1; i < rows; i++){
                    Cell<T> *newCell = new Cell<T>();
                    newColHead->down = newCell;
                    newCell->up = newColHead;
                    prevPointer->right = newCell;
                    newCell->left = prevPointer;
                    currPointer->left = newCell;
                    newCell->right = currPointer;

                    newColHead = newCell;
                    prevPointer = prevPointer->down;
                    currPointer = currPointer->down;
                }
            }
            else{
                // if this is the first column
                makeNewColumnAtLeft();
                
            }
            cols++;
        }

        void makeNewColumnAtLeft(){
            Cell<T> *colHead = head;

            Cell<T> *newColHead = new Cell<T>();
            Cell<T> *temp = colHead->down;
            colHead->left = newColHead;
            newColHead->right = colHead;
            head = newColHead;

            for (int i = 1; i < rows; i++){
                Cell<T> *newCell = new Cell<T>();
                newColHead->down = newCell;
                newCell->up = newColHead;
                temp->left = newCell;
                newCell->right = temp;

                newColHead = newCell;

                temp = temp->down;
            }
        }

        void insertCellByRightShift(){

            // insert a new column at the right most end first
            Cell<T> *temp = selected;
            
            // getting to the right most end top cell
            while (temp->right != nullptr){
                temp = temp->right;
            }

            while (temp->up != nullptr){
                temp = temp->up;
            }

            // now temp is the top right most cell
            // inserting a new column at the right most end
            makeNewColumnAtRight();

            // from selected cell, right shift all the cells
            Cell<T> *current = selected;
            temp = current->left;

            Cell<T> *newCell = new Cell<T>();
            if (temp){
                temp->right = newCell;
                newCell->left = temp;
            }
            newCell->right = current;
            current->left = newCell;

            temp = newCell;

            // if it is the first row
            if (!current->up){
                Cell<T> *below = current->down;

                if (selected == head){
                    head = newCell;
                }

                while (below){
                    temp->down = below;
                    below->up = temp;

                    temp = temp->right;
                    below = below->right;
                }

            }
            else if (!current->down){
                // if it is the last row
                Cell<T> *above = current->up;

                while (above){
                    temp->up = above;
                    above->down = temp;

                    temp = temp->right;
                    above = above->right;
                }

            } 
            else {
                // if it is a middle row
                Cell<T> *above = current->up;
                Cell<T> *below = current->down;

                while (above){
                    temp->up = above;
                    above->down = temp;
                    below->up = temp;
                    temp->down = below;

                    temp = temp->right;
                    above = above->right;
                    below = below->right;
                }

            }
            temp->left->right = nullptr;
            cols++;
        }

        void insertCellByDownShift(){
            // insert a new row at the bottom most end first
            Cell<T> *temp = selected;

            // get to the bottom most end left cell
            while (temp->down != nullptr){
                temp = temp->down;
            }
            while (temp->left != nullptr){
                temp = temp->left;
            }

            // now temp is the bottom left most cell
            // inserting a new row at the bottom most end
            makeNewRowAtBottom();

            // from selected cell, down shift all the cells
            Cell<T> *current = selected;
            temp = current->up;

            Cell<T> *newCell = new Cell<T>();
            if (temp){
                temp->down = newCell;
                newCell->up = temp;
            }
            newCell->down = current;
            current->up = newCell;

            temp = newCell;

            if (!current->left){
                // it is the first column
                Cell<T> *right = current->right;

                if (selected == head){
                    head = newCell;
                }

                while (right){
                    temp->right = right;
                    right->left = temp;

                    temp = temp->down;
                    right = right->down;
                }
            }
            else if (!current->right){
                // it is the last column
                Cell<T> *left = current->left;

                while (left){
                    temp->left = left;
                    left->right = temp;

                    temp = temp->down;
                    left = left->down;
                }
            }
            else{
                // it is a middle column
                Cell<T> *left = current->left;
                Cell<T> *right = current->right;

                while (right){
                    temp->right = right;
                    right->left = temp;
                    left->right = temp;
                    temp->left = left;

                    temp = temp->down;
                    right = right->down;
                    left = left->down;
                }
            }

            temp->up->down = nullptr;
            rows++;
        }

        void deleteCellByLeftShift(){
            Cell<T> *current = selected;

            // a new cell will be added to the end of the selected row
            Cell<T> *temp = current;

            while (temp->right != nullptr){
                temp = temp->right;
            }

            Cell<T> *newCell = new Cell<T>();
            temp->right = newCell;
            newCell->left = temp;

            // deleting the selected cell
            temp = current->left;
                
            if (temp){
                temp->right = current->right;
                current->right->left = temp;    
            }
            else{
                // if it is the first column
                current->right->left = nullptr;
            }
            // left shifting the cells
            if (!current->up){
                // it is the first row
                Cell<T> *below = current->down;
                temp = current->right;

                while (temp){
                    below->up = temp;
                    temp->down = below;

                    temp = temp->right;
                    below = below->right;
                }

                if (selected == head){
                    head = current->right;
                }
            }
            else if (!current->down){
                // it is the last row
                Cell<T> *above = current->up;
                temp = current->right;

                while (temp){
                    above->down = temp;
                    temp->up = above;

                    temp = temp->right;
                    above = above->right;
                }
            }
            else{
                // it is a middle row
                Cell<T> *below = current->down;
                Cell<T> *above = current->up;
                temp = current->right;

                while (temp){
                    below->up = temp;
                    temp->down = below;
                    above->down = temp;
                    temp->up = above;

                    temp = temp->right;
                    below = below->right;
                    above = above->right;
                }
            }

            selected = current->right;
        }

        void deleteCellByUpShift(){
            Cell<T> *current = selected;
            Cell<T> *temp = current;

            // a new cell will be added to the end of the selected column
            while (temp->down != nullptr){
                temp = temp->down;
            }

            Cell<T> *newCell = new Cell<T>();
            temp->down = newCell;
            newCell->up = temp;

            // deleting the selected cell
            temp = current->up;

            if(temp){
                temp->down = current->down;
                current->down->up = temp;
            }
            else{
                // if it is the first row
                current->down->up = nullptr;
            }

            // up shifting the cells
            if (!current->left){
                // it is the first column
                Cell<T> *right = current->right;
                temp = current->down;

                while(temp){
                    temp->right = right;
                    right->left = temp;

                    temp = temp->down;
                    right = right->down;
                }

                if (selected == head){
                    head = current->down;
                }
            }
            else if (!current->right){
 
               // it is the last column
                Cell<T> *left = current->left;
                temp = current->down;

                while(temp){
                    temp->left = left;
                    left->right = temp;

                    temp = temp->down;
                    left = left->down;
                }
            }
            else{
                // it is a middle column
                Cell<T> *right = current->right;
                Cell<T> *left = current->left;
                temp = current->down;

                while(temp){
                    temp->right = right;
                    right->left = temp;
                    temp->left = left;
                    left->right = temp;

                    temp = temp->down;
                    right = right->down;
                    left = left->down;
                }
            }

            selected = current->down;
        }

        void deleteColumn(){
            Cell<T> *current = selected;
            Cell<T> *temp = current;

            // getting to the top of selected column
            while (temp->up != nullptr){
                temp = temp->up;
            }

            // deleting the selected column
            if (!temp->left){
                // it is the first column
                head = temp->right;
                Cell<T> *right = temp->right;

                while (right){
                    right->left = nullptr;
                    right = right->down;
                }

                selected = current->right;
            }
            else if (!temp->right){
                // it is the last column
                Cell<T> *left = temp->left;

                while (left){
                    left->right = nullptr;
                    left = left->down;
                }

                selected = current->left;
            }
            else{
                // it is a middle column
                Cell<T> *left = temp->left;
                Cell<T> *right = temp->right;

                while (left){
                    left->right = right;
                    right->left = left;

                    left = left->down;
                    right = right->down;
                }

                selected = current->left;
            }

            cols--;
        }

        void deleteRow(){
            Cell<T> *current = selected;
            Cell<T> *temp = current;

            // getting to the extreme left of selected row
            while (temp->left != nullptr){
                temp = temp->left;
            }

            // deleting the selected row
            if (!temp->up){
                // it is the first row
                head = head->down;
                Cell<T> *down = temp->down;

                while (down){
                    down->up = nullptr;
                    down = down->right;
                }

                selected = current->down;
            }
            else if (!temp->down){
                // it is the last row
                Cell<T> *up = temp->up;

                while (up){
                    up->down = nullptr;
                    up = up->right;
                }

                selected = current->up;
            }
            else{
                // it is a middle row
                Cell<T> *up = temp->up;
                Cell<T> *down = temp->down;

                while (up){
                    up->down = down;
                    down->up = up;

                    up = up->right;
                    down = down->right;
                }

                selected = current->up;
            }

            rows--;
        }

        void clearColumn(){
            Cell<T> *current = selected;

            // getting to the top of column
            while (current->up != nullptr){
                current = current->up;
            }

            // clearing the column
            while (current){
                current->data = "0";
                current = current->down;
            }
        }

        void clearRow(){
            Cell<T> *current = selected;

            // getting to the left of row
            while (current->left != nullptr){
                current = current->left;
            }

            while (current){
                current->data = "0";
                current = current->right;
            }
        }

        void print(){
            Cell<T> *temp = head;
            Cell<T> *rowTraverser;
            for (int i = 0; i <= cols; i++){
                cout << i << "  ";
            }
            cout << endl;
            for (int i = 0; i < rows; i++){
                cout << i + 1 << " |";
                rowTraverser = temp;
                for (int j = 0; j < cols; j++){
                    cout << rowTraverser->color << rowTraverser->data << " |";
                    rowTraverser = rowTraverser->right;
                }
                cout << endl;
                temp = temp->down;
            }
        }
};

template <typename T>
class Range{
    public:
        Excel<T> *excel;
        Cell<T> *start;
        Cell<T> *end;
        vector<Cell<T>*> *cells;
        vector<T> *data;

        Range(Excel<T> excel){
            this->excel = excel;
            this->start = nullptr;
            this->end = nullptr;
            cells = new vector<Cell<T>>();
        }

        Range(){
            excel = nullptr;
            start = nullptr;
            end = nullptr;
            cells = new vector<Cell<T>*>();
        }

        void CellLabeler(){
            // giving each cell row and column number
            int r = excel->rows;
            int c = excel->cols;
            Cell<T> *temp = excel->head;
            Cell<T> *rowTraverser;

            for (int i = 0; i < r; i++){
                rowTraverser = temp;
                for (int j = 0; j < c; j++){
                    rowTraverser->row = i;
                    rowTraverser->col = j;
                    rowTraverser = rowTraverser->right;
                }
                temp = temp->down;
            }
        }

        void fillVector(){
            // if end is at right of start and above
            if (start->col <= end->col && start->row >= end->row){
                Cell<T> *temp = start;
                Cell<T> *rowTraverser = temp;
                for (int i = start->row; i >= end->row; i--){
                    for (int j = start->col; j <= end->col; j++){
                        cells->push_back(temp);
                        temp = temp->right;
                    }
                    rowTraverser = rowTraverser->up;
                    temp = rowTraverser;
                }
            }
            else if (start->col <= end->col && start->row < end->row){
                // if end is at right of start and below
                Cell<T> *temp = start;
                Cell<T> *rowTraverser = temp;
                for (int i = start->row; i <= end->row; i++){
                    for (int j = start->col; j <= end->col; j++){
                        cells->push_back(temp);
                        temp = temp->right;
                    }
                    rowTraverser = rowTraverser->down;
                    temp = rowTraverser;
                }
            }
            else if (start->col > end->col && start->row > end->row){
                // if start is at right of end and below
                Cell<T> *temp = start;
                Cell<T> *rowTraverser = temp;
                for (int i = start->row; i >= end->row; i--){
                    for (int j = start->col; j >= end->col; j--){
                        cells->push_back(temp);
                        temp = temp->left;
                    }
                    rowTraverser = rowTraverser->up;
                    temp = rowTraverser;
                }
            }
            else if (start->col > end->col && start->row < end->row){
                // if start is at right of end and above
                Cell<T> *temp = start;
                Cell<T> *rowTraverser = temp;
                for (int i = start->row; i <= end->row; i++){
                    for (int j = start->col; j >= end->col; j--){
                        cells->push_back(temp);
                        temp = temp->left;
                    }
                    rowTraverser = rowTraverser->down;
                    temp = rowTraverser;
                }
            }
        }

        int calculateSum(){
            int sum = 0;
            string temp = "";
            for (int i = 0; i < cells->size(); i++){
                temp = tostring(cells->at(i)->data);
               sum += stoi(temp);
            }
            return sum;
        
        }

        float calculateAverage(){
            return round(((float)calculateSum() / cells->size()) * 10) / 10.0;
        }

        int calculateCount(){
            return cells->size();
        }

        int calculateMax(){
            int max = INT_MIN;
            string temp = "";
            for (int i = 0; i < cells->size(); i++){
                temp = tostring(cells->at(i)->data);
                if (max < stoi(temp)){
                    max = stoi(temp);
                }
            }
            return max;
        }

        int calculateMin(){
            int min = INT_MAX;
            string temp = "";
            for (int i = 0; i < cells->size(); i++){
                temp = tostring(cells->at(i)->data);
                if (min > stoi(temp)){
                    min = stoi(temp);
                }
            }
            return min;
        }

        void copy(){
            data = new vector<T>();

            for (int i = 0; i < cells->size(); i++){
                data->push_back(cells->at(i)->data);
            }
        }

        void cut(){
            // copying the data
            copy();

            // deleting the cells
            for (int i = 0; i < cells->size(); i++){
                cells->at(i)->data = "0";
            }
        }

        void paste(){
            int idx = data->size() - 1;

            // end is at the top left corner of selection rectangle
            if (end->col <= start->col && end->row <= start->row){
                Cell<T> *temp = excel->selected;
                Cell<T> *rowTraverser = temp;

                for (int i = end->row; i <= start->row; i++){
                    for (int j = end->col; j <= start->col; j++){
                        rowTraverser->data = data->at(idx);
                        idx--;
                        if (rowTraverser->right == nullptr){
                            excel->makeNewColumnAtRight();
                            excel->cols++;
                        }
                        rowTraverser = rowTraverser->right;
                    }
                    if (rowTraverser->down == nullptr){
                        excel->makeNewRowAtBottom();
                        excel->rows++;
                    }
                    temp = temp->down;
                    rowTraverser = temp;
                }
            }

            // end is left and below start
            if (start->col > end->col && start->row < end->row){
                Cell<T> *temp = excel->selected;
                Cell<T> *rowTraverser = temp;

                int idx = start->col - end->col + 1;
                int c = 1;

                for (int i = start->row; i <= end->row; i++){
                    for (int j = start->col; j >= end->col; j--){
                        rowTraverser->data = data->at(idx-1);
                        idx--;
                        if (rowTraverser->right == nullptr){

                             excel->makeNewColumnAtRight();
                            excel->cols++;
                        }
                        rowTraverser = rowTraverser->right;
                    }
                    c++;
                    idx = c * (start->col - end->col + 1);
                    if (rowTraverser->down == nullptr){
                        excel->makeNewRowAtBottom();
                        excel->rows++;
                    }
                    temp = temp->down;
                    rowTraverser = temp;
                }
            }

            // end is right and below start
            if (start->col <= end->col && start->row < end->row){
                Cell<T> *temp = excel->selected;
                Cell<T> *rowTraverser = temp;

                int idx = 0;

                for (int i = start->row; i <= end->row; i++){
                    for (int j = start->col; j <= end->col; j++){
                        rowTraverser->data = data->at(idx);
                        idx++;
                        if (rowTraverser->right == nullptr){
                            excel->makeNewColumnAtRight();
                            excel->cols++;
                        }
                        rowTraverser = rowTraverser->right;
                    }
                    if (rowTraverser->down == nullptr){
                        excel->makeNewRowAtBottom();
                        excel->rows++;
                    }
                    temp = temp->down;
                    rowTraverser = temp;
                }
            }

            // end is right and above start
            if (start->col <= end->col && start->row >= end->row){
                Cell<T> *temp = excel->selected;
                Cell<T> *rowTraverser = temp;

                int c = start->row - end->row;
                int idx = c * (end->col - start->col + 1);

                for (int i = start->row; i >= end->row; i--){
                    for (int j = start->col; j <= end->col; j++){
                        rowTraverser->data = data->at(idx);
                        idx++;
                        if (rowTraverser->right == nullptr){
                            excel->makeNewColumnAtRight();
                            excel->cols++;
                        }
                        rowTraverser = rowTraverser->right;
                    }
                    c--;
                    idx = c * (end->col - start->col + 1);
                    if (rowTraverser->down == nullptr){
                        excel->makeNewRowAtBottom();
                        excel->rows++;
                    }
                    temp = temp->down;
                    rowTraverser = temp;
                }
            }
        }
};

template <typename T>
class FrontEnd{

    public:

        Excel<T> *excel;
    
        FrontEnd(Excel<T> excel){
            this->excel = excel;
        }

        FrontEnd(){
            excel = nullptr;
        }

        void printKeyManual(){
        cout << "Use arrow keys to navigate" << endl;
        cout << "Use space to enter data" << endl;
        cout << "Use escape to exit" << endl;
        cout << "Press CTRL + A to insert row above selected cell" << endl;
        cout << "Press CTRL + B to insert row below selected cell" << endl;
        cout << "Press CTRL + R to insert column to the right of selected cell" << endl;
        cout << "Press CTRL + L to insert column to the left of selected cell" << endl;
        cout << "Press CTRL + I to insert cells by right shift" << endl;
        cout << "Press CTRL + K to insert cells by down shift" << endl;
        cout << "Press CTRL + O to delete cells by left shift" << endl;
        cout << "Press CTRL + U to delete cells by up shift" << endl;
        cout << "Press CTRL + D to delete column" << endl;
        cout << "Press CTRL + E to delete row" << endl;
        cout << "Press CTRL + M to clear column" << endl;
        cout << "Press CTRL + N to clear row" << endl;
        cout << "Press F1 key to select range of cells" << endl;
        }

        void RangeSelection(Excel<T> *excel, Range<T> *range){
            // storing current selected cell
            Cell<T> *current = excel->selected;

            // selecting cells
            cout << "Select Starting cell..." << endl;

            bool selecting = true, modify = false;
            Sleep(100);
            while (selecting){
                arrowMovement("\33[35m", modify);
                if (GetAsyncKeyState(VK_F1)){
                    if (range->start == nullptr){
                        range->start = excel->selected;
                        cout << "Start cell selected" << endl;
                        cout << "Select End cell..." << endl;
                        modify = true;
                    }
                    else{
                        range->end = excel->selected;
                        cout << "End cell selected" << endl;
                        selecting = false;
                        range->start->color = "\33[37m";
                        range->end->color = "\33[37m";
                    }
                }
                Sleep(100);

                if (modify){
                    modify = false;
                    system("cls");
                    printKeyManual();
                    excel->print();
                    if (range->start != nullptr){
                        range->start->color = "\33[35m";
                        cout << "Start cell selected" << endl;
                        cout << "Select end cell" << endl;
                    }
                    else{
                        cout << "Select start cell" << endl;
                    }
                }
            }

            // resetting the selected cell to default
            excel->selected = current;
        }

        void rangeOption(Range<T> *range){
            string option;
            system("cls");
            cout << "1. Sum" << endl;
            cout << "2. Average" << endl;
            cout << "3. Count" << endl;
            cout << "4. Max" << endl;
            cout << "5. Min" << endl;
            cout << "6. Copy/Paste" << endl;
            cout << "7. Cut/Paste" << endl;
            cout << "0. Back" << endl;
            cout << "Enter option:";
            cin >> option;

            if (option == "1"){
                // sum
                excel->selected->data = validString(to_string(range->calculateSum()));
            }
            else if (option == "2"){
                // avg
                excel->selected->data = validString(to_string(range->calculateAverage()));
            }
            else if (option == "3"){
                // count
                excel->selected->data = validString(to_string(range->calculateCount()));
            }
            else if (option == "4"){
                // max
                excel->selected->data = validString(to_string(range->calculateMax()));
            }
            else if (option == "5"){
                // min
                excel->selected->data = validString(to_string(range->calculateMin()));
            }
            else if (option == "6"){
                // copy
