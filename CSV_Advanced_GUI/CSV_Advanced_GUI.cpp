#include <iostream>
#include "Utils.h"
#include "DataIterator.h"

typedef struct TableData {
    Data::DataIterator* di;
    int showing_page;
    int max_visible;
};

void printParamError(char *arg);
void clearConsole();
void addLines(int cn);
Data::DataIterator* requesIterator(std::string path, char separator);
void printTable(TableData* td);
void mainCicle(TableData* td);
void sortTable(TableData* td);
void setNewMax(TableData* td);
void searchVal(TableData* td);
void writeFile(TableData* td);
void printHelp();

int main(int argc, char* argv[]){
    TableData td;
    Utils::Functions* f = new Utils::Functions;
    std::string path;
    char sep = ',';
    bool path_provided = false;;
    if (argc > 1) {
        for(int i=1;i<argc;i++){
            std::string arg = argv[i];
            if (!f->compare(&arg,&std::string("-p"))){
                if (i + 1 < argc) {
                    path_provided = true;
                    path = argv[i+1];
                    i++;
                    continue;
                }
                printParamError(argv[i]);
                break;
            }else if (!f->compare(&arg, &std::string("-s"))) {
                if (i + 1 < argc) {
                    sep = argv[i+1][0];
                    i++;
                    continue;
                }
                printParamError(argv[i]);
                break;
            }
            else if (!f->compare(&arg, &std::string("-h")) || !f->compare(&arg, &std::string("-help")) || !f->compare(&arg, &std::string("-?"))) {
                printHelp();
                return 0;
            } else {
                std::cout << "Invalid argument "<< argv[i] << std::endl;
                break;
            }
        }
    }
    if(!path_provided){
        std::cout << "Missing path to CSV" << std::endl;
        return 0;
    }
    td.di = requesIterator(path, sep);
    td.max_visible = 10;
    td.showing_page = 0;
    delete f;
    mainCicle(&td);
}

void mainCicle(TableData *td) {
    char c = '\n';
    td->di->parseData();
    do {
        switch (c){
            case 'f':
                addLines(2);
                clearConsole();
                searchVal(td);
                break;
            case 'm':
                addLines(2);
                setNewMax(td);
                clearConsole();
                break;
            case 's':
                addLines(2);
                sortTable(td);
                clearConsole();
                break;
            case 'p':
                clearConsole();
                td->showing_page--;
                break;
            case 'n':
                clearConsole();
                td->showing_page++;
                break;
            case 'w':
                clearConsole();
                writeFile(td);
                break;
            case 'i':
                clearConsole();
                td->di->printInfo();
                break;
            default:
                clearConsole();
                break;
        }
        printTable(td);
        std::cout << "\nType:\n\t-q : to quit;\n\t-s : to sort a column;\n\t-w : write out the modified CSV;\n\t-f : find a value in a column;\n\t-p : to move to previous result page;\n\t-n : to movo to next result page;\n\t-m : set max rows per page;\n\t-i : show info about the CSV;\nEnter operation : ";
        do {
            std::cin >> c;
        } while (c == '\n');
    } while (c != 'q');
    delete td->di;
}

void printParamError(char* arg) {
    std::cout << "Missing expected argument for " << arg << std::endl;
}

void clearConsole() {
    std::cout << std::endl << std::endl 
              << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl 
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl
        << std::endl << std::endl;
}

void addLines(int cn) {
    for (int i = 0; i < cn; i++) {
        std::cout << std::endl;
    }
}

Data::DataIterator* requesIterator(std::string path,char separator) {
    Data::DataIterator* di = new Data::DataIterator(path, separator);
    return di;
}

void printTable(TableData* td) {
    td->di->printTable(td->max_visible,td->showing_page);
}

void sortTable(TableData* td) {
    int inp = 0;
    do {
        std::cout << "Which column do you want to sort (ascending) [1-" << td->di->getColumnSize() << "] ? (0 to undo) ";
        std::cin >> inp;
    } while (inp > td->di->getColumnSize() || inp < 0);
    if (!inp) {
        return;
    }
    td->di->sortCSV(inp - 1);
}

void setNewMax(TableData* td) {
    int inp = 0;
    do {
        std::cout << "Max amount of rows per page [5-" << td->di->getRowSize() << "] ? (0 to undo) ";
        std::cin >> inp;
        if (inp == 0) {
            break;
        }
    } while (inp > td->di->getRowSize() || inp < 5);
    if (!inp) {
        return;
    }
    td->max_visible = inp;
}

void searchVal(TableData* td) {
    std::string to_search;
    int column;
    std::cout << "Search value position in a specified column, value : " ;
    std::getchar();
    std::getline(std::cin,to_search);
    do {
        std::cout << "In which column do you want to look [1-" << td->di->getColumnSize() << "] ? ";
        std::cin >> column;
    } while (column > td->di->getColumnSize() || column < 1);
    Data::DataPos dp = td->di->getPosOf(to_search, column-1);
    if (dp.row < 0) {
        std::cout << "\"" << to_search << "\" not found in column n." << column<<std::endl;
    }
    else {
        std::cout << "\"" << to_search << "\" found in column n." << column << " and row n." << dp.row +1 << std::endl;
    }
}

void writeFile(TableData* td) {
    bool success = td->di->writeOut();
    if (success) {
        std::cout << "Data saved to file..." << std::endl;
    } else {
        std::cout << "Error saving data to file..." << std::endl;
    }
}

void printHelp() {
    std::cout << "HELP -- acsv " << std::endl 
        << "Usage: acsv [options] -p <path to file>" << std::endl << std::endl
        <<"Options:" << std::endl 
        << "\t-s define the separator used in the CSV file." << std::endl
        << "\t-p define the path to the file to be interpreted." << std::endl
        << "\t-? -h -help show this help page." << std::endl
        <<std::endl
        <<"If the file path contains spaces use \" \" to encolose the parameter."
        <<std::endl<<std::endl
        <<"Visualizer help:"
        << std::endl << "\tType 'q' to quit the programm."
        << std::endl << "\tType 'i' to get basic info about the opened file."
        << std::endl << "\tType 'm' to update the default (20) amount of rows shown per page."
        << std::endl << "\tType 'p' go to previous page (a set of empty rows is shown if page doesn't exists)."
        << std::endl << "\tType 'n' go to the next page (no rows are shown if no more rows are present)."
        << std::endl << "\tType 's' to execute an alphabetical oreder sort of a single column."
        << std::endl << "\tType 'f' to get coordinates of a value in a column (if present)" 
                << std::endl << "\t\t after being sorted (this mode uses binary search)."
        << std::endl << "\tType 'w' to overwrite the file with the current modifications.";
}