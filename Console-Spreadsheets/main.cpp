#include <iostream>
#include <memory>
#include "CellFactory.h"
#include "Table.h"
#include "MyString.h"

using namespace std;

int main() {
    cout << "=== Table Test Demo ===" << endl << endl;

    // Create a 4x4 table
    Table myTable(4, 4);

    cout << "Empty table:" << endl;
    myTable.display();
    cout << endl;

    // Fill some cells with different types
    cout << "Adding values to cells..." << endl;

    // Add some integers
    myTable.setCell(0, 0, MyString("42"));      // A1 = 42
    myTable.setCell(0, 1, MyString("100"));     // B1 = 100
    myTable.setCell(1, 0, MyString("-15"));     // A2 = -15

    // Add some booleans
    myTable.setCell(1, 1, MyString("true"));    // B2 = true
    myTable.setCell(1, 2, MyString("false"));   // C2 = false

    // Add some strings
    myTable.setCell(2, 0, MyString("\"Hello\"")); // A3 = "Hello"
    myTable.setCell(2, 1, MyString("\"World\"")); // B3 = "World"
    myTable.setCell(0, 2, MyString("\"Test\""));  // C1 = "Test"

    // Add unquoted text
    myTable.setCell(3, 3, MyString("End"));     // D4 = End

    cout << "Table with values:" << endl;
    myTable.display();
    cout << endl;

    // Test getting individual cells
    cout << "Testing getCell():" << endl;
    BaseCell* cell = myTable.getCell(0, 0);
    if (cell) {
        cout << "Cell A1: Type=" << cell->getType()
            << ", Value=" << cell->toString()
            << ", Numeric=" << cell->evaluate() << endl;
    }

    cell = myTable.getCell(1, 1);
    if (cell) {
        cout << "Cell B2: Type=" << cell->getType()
            << ", Value=" << cell->toString()
            << ", Numeric=" << cell->evaluate() << endl;
    }

    cell = myTable.getCell(2, 0);
    if (cell) {
        cout << "Cell A3: Type=" << cell->getType()
            << ", Value=" << cell->toString()
            << ", Numeric=" << cell->evaluate() << endl;
    }

    cout << endl;

    // Test invalid position
    cout << "Testing invalid position:" << endl;
    myTable.setCell(10, 10, MyString("Invalid"));

    cout << endl << "Demo complete!" << endl;
    return 0;
}