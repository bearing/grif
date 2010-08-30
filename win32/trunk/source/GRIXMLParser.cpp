#include "gri_xmlparser.h"


GRIXMLParser::GRIXMLParser() : QWidget(NULL)
{
}


GRIParamList* GRIXMLParser::readExampleAnalysisXMLFile()
{
    GRIParamList *head = new GRIParamList(NULL, "Main Menu", "", "", "", "", "","", true, NULL);

    QFile file(":/runtime_params.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        cout<<"Fail to Open RUNTIME PARAMS  File!"<<endl;

    }

    QXmlStreamReader xml(&file);

    //debug
//    this->pauseProgram("OPENED PARAMETER XML FILE");

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue; }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            /* If it's named PARAMETER_LIST, we'll go to the next.*/
            if(xml.name() == "PARAMETER_LIST") {
                    continue;
            }
            /* If it's named parameter, we'll dig the information from there.*/
            if(xml.name() == "PARAMETER") {

                //debug-------------------------------------------------------
//                this->pauseProgram("reading tag 'PARAMETER'");

                    head->addChild(this->readParameter(xml, head));
            }
        }

    }

    /* Error handling. */
    if(xml.hasError()) {
            QMessageBox::critical(this,
                                  "QXSRExample::parseXML",
                                  xml.errorString(),
                                  QMessageBox::Ok);
    }
    xml.clear();

    return head;
}
GRIParamList* GRIXMLParser::readParameter(QXmlStreamReader& xml, GRIParamList* head)
{
    GRIParamList* param = new GRIParamList();

    //set parent
    param->parent = head;


//    //debug-------------------------------------------------------
//    this->pauseProgram(("xml name = " + xml.name().toString().toStdString()));


    /* Let's check that we're really getting a parameter. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
       xml.name() == "PARAMETER") {
            return param;
    }

    /* Next element... */
    xml.readNext();

    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named parameter.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                    xml.name() == "PARAMETER")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                //debug-------------------------------------------------------
//                this->pauseProgram(("READING TAG " + xml.name().toString().toStdString()));

                /* We've found name. */
                if(xml.name() == "NAME") {
                        this->addElementToParam(xml, param);
                }
                /* We've found DEFAULT VALUE. */
                if(xml.name() == "DEFAULT") {
                        this->addElementToParam(xml, param);
                }
                /* We've found DATA. */
                if(xml.name() == "DATA") {
                        this->addElementToParam(xml, param);
                }
                /* We've found MIN VALUE. */
                if(xml.name() == "MIN") {
                        this->addElementToParam(xml, param);
                }
                /* We've found MAX VALUE. */
                if(xml.name() == "MAX") {
                        this->addElementToParam(xml, param);
                }
                /* We've found UNITS. */
                if(xml.name() == "UNITS") {
                        this->addElementToParam(xml, param);
                }
                /* We've found DATA TYPE. */
                if(xml.name() == "DATA_TYPE") {
                        this->addElementToParam(xml, param);
                }
                /* We've found IS_MENU. */
                if(xml.name() == "IS_MENU") {
                        this->addElementToParam(xml, param);

//                        //debug-------------------------------------------------------
//                        this->pauseProgram(("FINISHED ADDING is_menu, TAG NOW = '" + xml.name().toString().toStdString() + "'"));
//                        string isEndElement;
//                        (xml.isEndElement())?  isEndElement= "true": isEndElement = "false";
//                        this->pauseProgram("is end tag = " + isEndElement);

                }
                /* We've found CHILDREN */
                if(xml.name() == "CHILD_PARAMS") {

                        this->addElementToParam(xml, param);
//
//                        //debug-------------------------------------------------------
//                        this->pauseProgram(("FINISHED ADDING CHILDREN, TAG NOW = '" + xml.name().toString().toStdString() + "'"));
//                        string isEndElement;
//                        (xml.isEndElement())?  isEndElement= "true": isEndElement = "false";
//                        this->pauseProgram("is end tag = " + isEndElement);


                }
            }
            /* ...and next... */
            xml.readNext();
    }

//    param->display();



    return param;
}

void GRIXMLParser::addElementToParam(QXmlStreamReader& xml, GRIParamList* param)
{
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement) {
            return;
    }
    /* Let's read the name... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /*
     * This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave.
     */
    if(xml.tokenType() != QXmlStreamReader::Characters && elementName != "CHILD_PARAMS") {
            return;
    }

    string text = xml.text().toString().toStdString();

//    //debug-------------------------------------------------------
//    this->pauseProgram(("READING elementName " + elementName.toStdString() + ": " + text));

    if(elementName == "NAME")
    {
        param->name = text;
    }
    else if(elementName == "DEFAULT")
    {
        param->defaultValue = text;
    }
    else if(elementName == "DATA")
    {
        param->data = text;
    }
    else if(elementName == "MIN")
    {
        param->min = text;
    }
    else if(elementName == "MAX")
    {
        param->max = text;
    }
    else if(elementName == "UNITS")
    {
        param->units = text;
    }
    else if(elementName == "DATA_TYPE")
    {
        param->data_type = text;
    }
    else if(elementName == "IS_MENU")
    {

        for (int i = 0; i < text.length(); ++i)
        {
            text[i]=toupper(text[i]);
        }
        (text == "TRUE")? param->isSubMenu = true: param->isSubMenu = false;
    }
    else if(elementName == "CHILD_PARAMS")
    {

        this->addChildParams(xml, param);
    }
    else
    {
        cout << "Unknown Element..." << endl;
    }

}
void GRIXMLParser::addChildParams(QXmlStreamReader& xml, GRIParamList* head)
{

    //debug-------------------------------------------------------
//    this->pauseProgram(("INSIDE ADD CHILDPARAMS "));



    //debug-------------------------------------------------------
//    this->pauseProgram(("READING TAG = '" + xml.name().toString().toStdString() + "' IN CHILD PARAMS "));

    /* ...go to the next. */
    xml.readNext();



    //debug-------------------------------------------------------
//    this->pauseProgram(("READING TAG = '" + xml.name().toString().toStdString() + "' IN CHILD PARAMS "));

    /* If it's named parameter, we'll dig the information from there.*/
    while(xml.name() == "PARAMETER")
    {
            //debug-------------------------------------------------------
//            this->pauseProgram(("ADDING A CHILD PARAM "));

            head->addChild(this->readParameter(xml, head));
            xml.readNextStartElement();
//            this->pauseProgram(("After child, tag = " + xml.name().toString().toStdString()));
    }

//    xml.readNext();
//    xml.readNext();


}

void GRIXMLParser::pauseProgram(string message)
{
    cout << endl << message << endl;
    cin.ignore(100,'\n');
}