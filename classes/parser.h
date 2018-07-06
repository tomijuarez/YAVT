#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <QMap>
#include <QQueue>
#include <QString>
#include "parseddata.h"


class Parser {
    public:
        Parser();

        ~Parser();

        void getAlgorithmData(ParsedData & algorithmData, QList<char*> & algorithmErrors, unsigned int & numTest);
        void getData(QQueue<std::string> & data, QList<char*> & formulaErrors);
        void parse(char condition[]);
        void parse(QString condition);
    private:
        void _cleanParserData();
};

#endif // PARSER_H
