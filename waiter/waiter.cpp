#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/file_IO.h"
#include "../includes/externs.h"
#include <queue>

using namespace std;
//ID is just a number used to identify this particular baker
//(used with PRINT statements)
//filename is what waiter reads in orders from
Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order from file_IO
//if return == SUCCESS then anOrder
//contains new order
//otherwise return contains error
int Waiter::getNext(ORDER &anOrder){
	int success = myIO.getNext(anOrder);
	return success;
}
//contains a loop that will get orders from filename one at a time
	//then puts them in order_in_Q then signals baker(s) using cv_order_inQ
	//so they can be consumed by baker(s)
	//when finished exits loop and signals baker(s) using cv_order_inQ that
	//it is done using b_WaiterIsFinished
void Waiter::beWaiter() {
	ORDER tempOrder;
	int success = getNext(tempOrder);
	unique_lock<mutex> lck(mutex_order_inQ);
	while(success == SUCCESS){
		lck.lock();
		order_in_Q.push(tempOrder); //need a lock around this
		lck.unlock();
		cv_order_inQ.notify_all(); //there is no way this is right
		int success = getNext(tempOrder);
	}
	unique_lock<mutex> lck(m);
	b_WaiterIsFinished = true;
}


