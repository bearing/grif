#include "GRILoader.h"

GRILoader::GRILoader(GRIRegulator* regulator)
{
    this->regulator = regulator;

}

GRILoader::~GRILoader()
{

}


// IS FILEPATH NECESSARY???
list<GRIProcessThread*>* GRILoader::initProcessThreads(list<string> names, list<string> filepaths)
{
    // create a process thread list
    list<GRIProcessThread*>* processList = new list<GRIProcessThread*>;

    // create a process thread variable
    GRIProcessThread* p;

    // for however many process threads there are
    for(int i = 0; i < names.size(); i++)
    {
        // load the process thread
        p = load(names.front(), filepaths.front());

        filepaths.pop_front();
        names.pop_front();

        // add to list
        processList->push_front(p);
    }

    // return list
    return processList;
}

GRIProcessThread* GRILoader::load(string process_name, string xml_file)
{
    GRIProcessThread* p;
    // sadly, you can't use strings in c++ switch statements
    if(process_name == "hemi")   {
//        for example
//        p = new HemiDaq();
    }

    return p;
}



list<GRIParam*>* GRILoader::readNewParamList(QString filePath, list<GRIParam*>* paramList)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        cout<<"Fail to Open RUNTIME PARAMS  File!"<<endl;
    }

    QXmlStreamReader xml(&file);

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
                  QVariant size = (unsigned int) paramList->size();
                   paramList->push_back(this->readParameter(xml, size.toString()));
//                    head->addChild(this->readParameter(xml, head));
            }
        }

    }

    xml.clear();

    return paramList;
}
GRIParam* GRILoader::readParameter(QXmlStreamReader& xml, QString paramIndex)
{
    GRIParam* param = new GRIParam();

    param->index = paramIndex.toStdString();

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
//                if(xml.name() == "IS_MENU") {
//                        this->addElementToParam(xml, param);
//
////                        //debug-------------------------------------------------------
////                        this->pauseProgram(("FINISHED ADDING is_menu, TAG NOW = '" + xml.name().toString().toStdString() + "'"));
////                        string isEndElement;
////                        (xml.isEndElement())?  isEndElement= "true": isEndElement = "false";
////                        this->pauseProgram("is end tag = " + isEndElement);
//
//                }
                /* We've found CHILDREN */
//                if(xml.name() == "CHILD_PARAMS") {
//
//                        this->addElementToParam(xml, param);
////
////                        //debug-------------------------------------------------------
////                        this->pauseProgram(("FINISHED ADDING CHILDREN, TAG NOW = '" + xml.name().toString().toStdString() + "'"));
////                        string isEndElement;
////                        (xml.isEndElement())?  isEndElement= "true": isEndElement = "false";
////                        this->pauseProgram("is end tag = " + isEndElement);
//
//
//                }
            }
            /* ...and next... */
            xml.readNext();
    }

    return param;
}

void GRILoader::addElementToParam(QXmlStreamReader& xml, GRIParam* param)
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
//    if(xml.tokenType() != QXmlStreamReader::Characters && elementName != "CHILD_PARAMS") {
//            return;
//    }

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
//    else if(elementName == "IS_MENU")
//    {
//
//        for (int i = 0; i < text.length(); ++i)
//        {
//            text[i]=toupper(text[i]);
//        }
//        (text == "TRUE")? param->isSubMenu = true: param->isSubMenu = false;
//    }
//    else if(elementName == "CHILD_PARAMS")
//    {
//
//        this->addChildParams(xml, param);
//    }
    else
    {
        cout << "Unknown Element..." << endl;
    }

}
void GRILoader::addChildParams(QXmlStreamReader& xml, GRIParam* head)
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

//            head->addChild(this->readParameter(xml,head));
            xml.readNextStartElement();
//            this->pauseProgram(("After child, tag = " + xml.name().toString().toStdString()));
    }

//    xml.readNext();
//    xml.readNext();


}


std::list<ProcessDetails*> GRILoader::readPathXML(QString rootXMLFile)
{
    list<ProcessDetails*> locPathVector;


        ProcessDetails daq_info[200];
        ProcessDetails ana_info[200];

        int daq_index = 0;
        int ana_index = 0;
        int no_of_file = 0;

        QDomDocument doc("CONFIG_FILE_PATH");

        QFile file(rootXMLFile);
        if(!file.open(QIODevice::ReadOnly))
        {
            cout<<"Fail to Open FILEPATH File!"<<endl;
            //return 0;
        }
        if(!doc.setContent(&file))
        {
            file.close();
            cout<<"Fail to set file content!"<<endl;
            //return 0;
        }
        file.close();

        QDomElement root = doc.documentElement();

        if( root.tagName()!="CONFIG_FILE_PATH")
        {
            cout<<"Not right file!"<<endl;
            //return 0;
        }

        QDomNode n = root.firstChild();

        while(!n.isNull())
        {
                if(n.nodeName()=="DAQ_INFO")
                {
                    QDomElement temp = n.toElement();
                    QDomNode temp_node = temp.firstChild();
                    while(!temp_node.isNull())
                    {
                        if(temp_node.nodeName()=="FILE")
                        {
                            QDomElement info_node = temp_node.toElement();
                            daq_info[daq_index].name = info_node.attribute("name", "").toStdString();
                        }
                        else if( temp_node.nodeName()=="LOCATION")
                        {
                            QDomElement info_node = temp_node.toElement();
                            daq_info[daq_index].xml_path = info_node.attribute("path", "").toStdString();
                        }
                        temp_node = temp_node.nextSibling();
                    }
                    (++daq_index);

                }
                else
                {
                    QDomElement temp = n.toElement();
                    QDomNode temp_node = temp.firstChild();
                    while(!temp_node.isNull())
                    {
                        if(temp_node.nodeName()=="FILE")
                        {
                            QDomElement info_node = temp_node.toElement();
                            ana_info[ana_index].name = info_node.attribute("name", "").toStdString();
                        }
                        else if( temp_node.nodeName()=="LOCATION")
                        {
                            QDomElement info_node = temp_node.toElement();
                            ana_info[ana_index].xml_path = info_node.attribute("path", "").toStdString();
                        }
                        temp_node = temp_node.nextSibling();
                    }
                    (++ana_index);
                }
            n = n.nextSibling();
        }

        no_of_file = daq_index + ana_index;


        for(int j = 0; j<daq_index; j++)
        {
            ProcessDetails *temp = new ProcessDetails;
            temp->isDaq = true;
            temp->name = daq_info[j].name;
            temp->xml_path = daq_info[j].xml_path;

            locPathVector.push_back(temp);
        }
        for(int k = 0; k<ana_index; k++)
        {
            ProcessDetails *temp = new ProcessDetails;
            temp->isDaq = false;
            temp->name = ana_info[k].name;
            temp->xml_path = ana_info[k].xml_path;

            locPathVector.push_back(temp);
        }
        return locPathVector;

}

std::list<AnalysisStructureObject*> GRILoader::readAnalysisStructureXML()
{
        AnalysisStructureObject *temp_struc;
        std::list<AnalysisStructureObject*> list;


        int struc_index = 0;
        int No_DAQ_depen;
        int No_ANA_depen;
        int i = 0;

        QDomDocument doc("DATA_ANALYSIS_STRUCTURE");

        QFile file(":/struc_config.xml");
        if(!file.open(QIODevice::ReadOnly))
        {
            cout<<"Fail to Open Analysis Structure File!"<<endl;

        }
        if(!doc.setContent(&file))
        {
            file.close();
            cout<<"Fail to set file content!"<<endl;

        }
        file.close();

        QDomElement root = doc.documentElement();
        if(root.tagName()!="DATA_ANALYSIS_STRUCTURE")
        {
            cout<<"Wrong File Read!"<<endl;

        }

        QDomNode n = root.firstChild();

        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            QDomNode node = e.firstChild();
            No_DAQ_depen = 0;
            No_ANA_depen = 0;
            temp_struc = new AnalysisStructureObject;

            while(!node.isNull())
            {
                QDomElement temp = node.toElement();
                if(node.nodeName()=="DATA")
                {
                    temp_struc->data = temp.firstChild().toText().data().toStdString();
                }
                else if(node.nodeName()=="FROM")
                {
                    temp_struc->From = temp.firstChild().toText().data().toStdString();
                }
                else if(node.nodeName()=="TO")
                {
                    temp_struc->To.push_front(temp.firstChild().toText().data().toStdString());
                }
                else if(node.nodeName()=="ISDAQ")
                {
//                    temp_struc->isdaq = temp.firstChild().toText().data().toUInt();
                }
                node = node.nextSibling();
            }
            list.push_front(temp_struc);
            struc_index++;
            i++;
            n = n.nextSibling();

        }

        return list;

}


