#ifndef GRIPROCESSTHREAD_H
#define GRIPROCESSTHREAD_H

#define PROCESS_THREAD_DEBUG

#include <QThread>
#include <QHash>
#include <QString>

#include <string>

#include "GRIRegulator.h"
#include "GRIDataBlock.h"

using namespace std;

class GRIRegulator;
class GRIDataBlock;

#define DEFAULT_PACKETS_TO_SATURATION 1
#define DEFAULT_PACKETS_FROM_SATURATION 1

typedef struct process_details
	{
		string xml_path;
		string name;
		bool isDaq;
	} process_details_t;

/*
 * GRIProcessThread class is a common interface for DAQ thread & analysis thread.
 *
 * Load balancing is being partially done in here (ie: the actual changing of thread's
 * priority). It is done by having two sets of variables. One is # of packets before we
 * are allowed to adjust the priority of this process away/to saturation. Saturation
 * being defined as time-critical priority or idle priority. The other one is when is the
 * last time this process adjusts its priority away/to saturation. If the latter is
 * greater than the former, priority adjustment will be done
 */
class GRIProcessThread : public QThread
	{
		
		friend class GRICommandAndControl;
		
	public:
		
		GRIProcessThread(QObject* obj);
		
		~GRIProcessThread();
		
		/*
		 * getID() returns the id of this process
		 */
		unsigned int getID();
		
		/*
		 * get_type() returns the type of this process (either DAQ or analysis)
		 */
		bool get_type();
		
		/*
		 * set_detail() sets i
		 */
		void set_detail(GRIRegulator* reg, process_details* proc_detail);
		
		/*
		 * get_name() returns the name of this process
		 */
		string get_name();
		
		/*
		 * get_xml_path() returns the path to the xml file of this process
		 */
		string get_xml_path();
		
		/*
		 * set_load_balancing_vars() allows the user to customize the number of packets need to be
		 * written/read before this thread's priority could be adjusted again
		 */
		void set_load_balancing_vars(int num_packets_to_saturation,
									 int num_packets_from_saturation);
		
		/*
		 * set_link() sets up the pointers to the processes objects that are directly involved
		 * with this process (ie: those who will be writtten to or read by this process
		 */
		void set_link(list<GRIDataBlock*>* data_blocks);
		
		/*
		 * adds a data block that this process is going to use. Whether it's a buffer that this
		 * process is writing to or reading from will be dictated by type (OUT or IN).
		 */
		void add_data_block(string data_block, bool is_output);
		
		/*
		 * change_priority() decides whether to change the thread's priority or not
		 */
		bool change_priority(bool is_up);
		
		/*
		 * increment_packet_count() is being called everytime a packet is being read/written
		 * by this process. It will increment the load balancing variables, ie:
		 * last_adjustment_to_saturation & last_adjustment_from_saturation;
		 */
		void increment_packet_count();
		
		/*
		 * find_data_block() finds a data block that this process is possibly writing to/
		 * reading from. It will return NULL when it could not find the data block.
		 */
		GRIDataBlock* find_data_block(string data_block_name);
		
		/*
		 * The following 3 methods are get/sets for the parameters of any class inheriting GRIProcessThread.
		 * Usage:
		 *
		 * //Derived Class:
		 * class GRIDAQThread : public GRIProcessThread
		 * {
		 *      public:
		 *      GRIDAQThread();  //May possibly initialize parameters or whatever.
		 *      double parameter1;
		 *      int parameter2;
		 *      string *parameter3;
		 * }
		 *
		 * //Instantiation of Derived Class:
		 * GRIDAQThread *someDAQ = new GRIDAQThread();
		 *
		 * //Make parameters callable.  Will probably be done in loader.
		 * someDAQ->addParam<double>("FirstParameter", someDAQ->parameter1);
		 * someDAQ->addParam<int>("SecondParameter", someDAQ->parameter2);
		 * someDAQ->addParam<string *>("ThirdParameter", someDAQ->parameter3);
		 *
		 * //Gets and Sets for parameters.  Should be instructive.
		 * cout<< *(someDAQ->getParameter<string *>("ThirdParameter")) <<endl;  //prints the string in parameter3
		 *
		 * getParameter<int>("SecondParameter") = 5;  // does not set parameter2 in someDAQ
		 *                                            // because getParameter returns a copy.
		 *
		 * someDAQ->setParam<int>("SecondParameter", 5);  //sets parameter2 to 5 in someDAQ
		 * string *aNewString = new string("wooogly wooooogly!");
		 * someDAQ->setParam<string *>("ThirdParameter", aNewString);  //sets parameter3 to wooogly wooooogly!
		 * aNewString = new string("booogly boooooogly@@@");
		 * cout<< *(someDAQ->getParameter<string *>("ThirdParameter") <<endl; //prints wooogly wooooogly!
		 *                                                                   //because setParameter sets
		 *                                                                   //parameter3 to a copy of the
		 *                                                                   //aNewString pointer.
		 *
		 * cout<< *(someDAQ->parameter3) <<endl; //also prints wooogly wooooogly!
		 *     //As you can see, calling setParam also sets the parameter originally passed in using addParam.
		 *
		 * How it works:
		 * GRIProcessThread now contains a hashtable.  Every time you do addParam, a <"Name", parameter> pair
		 * is added to the hashtable where parameter is a reference to the parameter passed in.  Thus, setParam
		 * actually sets the parameter referenced in the hashtable (usually a variable in a DAQThread or analysisThread).
		 * getParam returns a copy of the parameter in the hashtable.  This is usually faster than doing a massive
		 * if...elseif statement to check which parameter the user is trying to look up.
		 */
		template <class T> void addParam(QString Key, T& value);
		template <class T> T getParam(QString Key);
		template <class T> void setParam(QString Key, T value);
		
		
		//Identical to regulator... essentially just abstracts the regulator away.
		//Passing strings by constant reference to avoid using copy constructor.
		/*
		 * readMemory() reads one packet from memory in the location specified
		 * by process_name & bufferName
		 */
		template <class T> T* readMemory(string const &bufferName);
		
		/*
		 *
		 * writeMemory() writes a data given in the char array to the location specified
		 * by process_name & bufferName
		 *
		 */
		template <class T> bool writeMemory(string const &bufferName, unsigned int size, T dataArray[]);
		
		/*
		 *
		 * currentPacketPosition() returns the current index of the packet marker. This is in most cases the last
		 * packet to be read next unless setPacketPosition() has been called.
		 *
		 */
		unsigned int currentPacketPosition(string const &bufferName);
		
		/*
		 *
		 * lastPacket() returns the index of the last packet in the specified buffer. This is equivalent to
		 * the buffer size minus one.
		 *
		 */
		unsigned int lastPacket(string const &bufferName);
		
		unsigned int sizeofPacket(string const &bufferName, unsigned int packetNumber);
		
		unsigned int sizeofBuffer(string const &bufferName);
		
		
#ifdef PROCESS_THREAD_DEBUG
		void display_current_state();
#endif // PROCESS_THREAD_DEBUG
		
	protected:
		
		typedef struct data
			{
				string name;
				GRIDataBlock* data_block;
			} data_t;
		
		string name;
		
		string xml_path;
		
		unsigned int thread_id; // id of this thread
		
		static unsigned int counter; // to keep track what id needs to be given to a new thread
		
		bool is_daq; // indicates whether this process is a daq or analysis
		
		list<data_t*> data_outs; // list of data blocks this process writing to
		
		list<data_t*> data_ins; // list of data blocks this process reading from
		
		// Load balancing variables: refer to the description of the class for more details
		int num_packets_to_saturation;
		int num_packets_from_saturation;
		
		int last_adjustment_to_saturation;
		int last_adjustment_from_saturation;
	private:
		GRIRegulator* reg;
		QHash<QString, void *> *hashTable;
	};


template<class T> void GRIProcessThread::addParam(QString Key, T& value){
    hashTable->insert(Key, &value);
}

template<class T> T GRIProcessThread::getParam(QString Key){
    return *((T *) hashTable->value(Key));
}

template<class T> void GRIProcessThread::setParam(QString Key, T value){
    *((T *) hashTable->value(Key)) = value;
}

/*
 * readMemory() reads one packet from memory in the location specified
 * by process_name & bufferName
 */
template<class T> T* GRIProcessThread::readMemory(string const &bufferName){
    return ((T *) reg->readMemory(bufferName));
}

/*
 *
 * writeMemory() writes a data given in the char array to the location specified
 * by process_name & bufferName
 *
 */
template<class T> bool GRIProcessThread::writeMemory(string const &bufferName, unsigned int size, T dataArray[]){
    return reg->writeMemory(bufferName, size * sizeof(T), (char) dataArray[] );
}

#endif // GRIPROCESSTHREAD_H
