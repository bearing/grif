#include <QtCore/QCoreApplication>

#include "GRIDataBlock.h"
#include "GRIRegulator.h"
#include "dummy_thread1.h"
#include "dummy_thread2.h"

typedef struct reader
{
    long read_counter;
    GRIProcessThread* reader;
    string reader_name;
} reader_t;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    GRIMemoryManager* mm = new GRIMemoryManager();
    GRIRegulator* reg = new GRIRegulator(mm);

    process_details_t* new_detail = new process_details_t;
    new_detail->isDaq = true;
    new_detail->name = "SISDAQ";
    new_detail->xml_path = "SISDAQ XML";

    DummyThread1* sisdaq = new DummyThread1(NULL);
    sisdaq->set_detail(reg, new_detail);
    sisdaq->add_data_block("ENERGY", 1);

    new_detail = new process_details_t;
    new_detail->isDaq = true;
    new_detail->name = "A1";
    new_detail->xml_path = "A1 XML";

    DummyThread2* A1 = new DummyThread2(NULL);
    A1->set_detail(reg, new_detail);
    A1->add_data_block("ENERGY", 0);

    list<GRIProcessThread*> processes;
    processes.push_back(sisdaq);
    processes.push_back(A1);

    struct AnalysisStructureObject* obj = new struct AnalysisStructureObject;
    obj->data = "ENERGY";
    obj->From = "SISDAQ";
    (obj->To).push_back("A1");

    GRIDataBlock data(reg, obj);

    list<GRIDataBlock*> data_blocks;
    data_blocks.push_back(&data);

    reg->init_config(&data_blocks, &processes);

    sisdaq->start(QThread::NormalPriority);
    A1->start(QThread::NormalPriority);

//    sleep(1);
//    data.delete_packet();
//
//    sisdaq->exit(*(new int));
//    A1->exit(*(new int));
//
//    sisdaq->start(QThread::NormalPriority);
//    A1->start(QThread::NormalPriority);
//
//    sleep(1);
//    data.delete_packet();

    return app.exec();
}
