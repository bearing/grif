#include "GRIUserLoaderAux.h"

//GCG process selector code for GRIUserLoader.cpp
GRIProcessThread *get_new_process(QString class_name, QString instance_name){

	GRIProcessThread *p = NULL;

	if(class_name.contains("SIMDAQThread")){
		p = new SIMDAQThread();
		return p;
	}

	return p;
}