#include <iostream>
#include "LL1.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    TScanner* scan;
    int type;
    LEX lex;

    char filename[] = "input.txt";

    scan = new TScanner(filename);

    LL1* gram = new LL1(scan);
    gram->LL_1();

    type = scan->Scanner(lex);

    if (type != TEnd)
    {
        scan->PrintError("Лишний текст в конце программы");
    }
    else
    {
        printf("\n\nОшибки не найдены.\n\n");
    }

    printf("\n\n\nСЕМАНТИЧЕСКОЕ ДЕРЕВО\n\n");
    gram->PrintTree();

    delete scan;
    delete gram;

    return 0;
}
