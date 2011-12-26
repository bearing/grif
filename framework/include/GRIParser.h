#ifndef GRIF_FRAMEWORK_INCLUDE_GRIPARSER_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIPARSER_H_

#include <QHash>
#include <QList>
#include <QString>

struct linkParsingDetails {
    QString writer;
    QString reader;
    QString dataBlock;
};

struct objectParsingDetails {
    QString objectName;
    QString className;
    QList<struct linkParsingDetails*> *links;
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
    QList<struct objectParsingDetails> get_objs_and_links() {
      return objs_and_links_;
    }

private:
    QHash<QString, struct objectParsingDetails *> object_hash_;
    QList<struct objectParsingDetails> objs_and_links_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRIPARSER_H_
