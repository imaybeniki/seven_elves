#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/file_IO.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"
#include <queue>

using namespace std;
//ID is just a number used to identify this particular baker
//(used with PRINT statements)
//filename is what waiter reads in orders from
Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){

}

Waiter::~Waiter(){

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
	//while FileIO can find another order
	while(success == SUCCESS){
		//set the unique lock
		unique_lock<mutex> lck(mutex_order_inQ);
		//push our temp order onto the order q
		PRINT1("This is waiter, I have added an order!")
		order_in_Q.push(tempOrder);
		//notify bakers that the order is ready
		cv_order_inQ.notify_all(); //there is no way this is right
		//reinitalize tempOrder to the next order
		success = getNext(tempOrder);
	}
	//give everyone one last notification to free up any threads that are stuck waiting on new orders.
	unique_lock<mutex> lck(mutex_order_inQ);
	b_WaiterIsFinished = true;
	cout << "Waiter done!" << endl;
	cv_order_inQ.notify_all();
}
