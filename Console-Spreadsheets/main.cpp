#include "ConsoleUI.h"
#include "Table.h"
#include <iostream>

int main() {

    Table table(5, 5);

    ConsoleUI ui(&table);

    ui.run();

    return 0;
}