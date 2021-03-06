//
// Created by Jakub Trstensky on 6. 5. 2020.
//

#include "db_handler.h"
#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <fstream>
#include <utility>
#include "../Database/nlohmann/json.hpp"
#include "../data/pathToConfig.h"
#include "../CSV/csv_handler.h"

using json = nlohmann::json;
using namespace pqxx;

db_handler::db_handler(string connectionString) : connection_string((std::move(connectionString))) {}

void db_handler::connectToDatabase() {
    this->Conn = new pqxx::connection(getConnectionString());
    if (!this->Conn->is_open()){
        cerr << "Can't open database";
    }
}

const string &db_handler::getConnectionString() const {
    return connection_string;
}

void db_handler::setConnectionString(const string &connectionString) {
    connection_string = connectionString;
}

void db_handler::closeDbConnection() {
    this->Conn->close();
}

db_handler::~db_handler() {
    delete this->Conn;
    cout << "\nDeleted Conn";
}

pqxx::connection *db_handler::getConn() const {
    return Conn;
}

const string &db_handler::getTableName() const {
    return table_name;
}

void db_handler::setTableName(const string &tableName) {
    table_name = tableName;
}

const vector<string> &db_handler::getColumnNames() const {
    return column_names;
}

void db_handler::setColumnNames(const vector<string> &columnNames) {
    column_names = columnNames;
}

int db_handler::pushDataFromCSVToDatabase() {
    auto *csvHandler = new csv_handler();
    string csv_filename = csvHandler->getCsvFilename();
    try{
        connectToDatabase();
        work transaction(*this->getConn());

        ifstream csv_file(csv_filename);
        if (!csv_file.is_open()){
            cerr << "File cannot be opened!\n";
            return -1;
        }
        string line;
        stream_to stream(transaction,getTableName(),getColumnNames());

        while(getline(csv_file,line)){
            vector<string> values = csvHandler->splitCSVline(line);

            string value = values[0];
            string timestamp = values[1];

            stream << make_tuple(value,timestamp);
        }
        stream.complete();
        transaction.commit();

        csv_file.close();
        closeDbConnection();
    }catch(const exception &e){
        cerr << "PQXX exception: " << e.what() << endl;
        return -2;
    }
    return 0;
}



