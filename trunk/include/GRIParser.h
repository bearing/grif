#ifndef GRIPARSER_H
#define GRIPARSER_H

#include <list>
#include "QString"
#include "QHash"

struct linkParsingDetails {
    QString writer;
    QString reader;
    QString dataBlock;
};

struct objectParsingDetails {
    QString objectName;
    QString className;
    std::list<struct linkParsingDetails*> *links;
};

class GRIParser {
public:
    /*
     * parses the XML file path provided by filePath.  Sets up the
     * objectsAndLinks data structure which contains the details
     * of the parsing.  This data is used by GRILoader.  Use
     * the getObjectsAndLinks() method to retrieve the data.
     *
     * parse() returns true if the parsing was successful and
     * false otherwise
     */
    bool Parse(QString FilePath);

    /* returns the objects and links necessary for initRegulatorDetails() */
    std::list<struct objectParsingDetails> get_objs_and_links() {
      return objs_and_links_;
    }

private:
    QHash<QString, struct objectParsingDetails *> object_hash_;
    std::list<struct objectParsingDetails> objs_and_links_;
};

#endif // GRIPARSER_H
