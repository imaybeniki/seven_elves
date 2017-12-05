#include <mutex>

#include "../includes/baker.h"
#include "../includes/box.h"
#include "../includes/externs.h"
#include "../includes/PRINT.h"

using namespace std;

//ID is just a number used to identify this particular baker
//(used with PRINT statements)
Baker::Baker(int id):id(id)
{

}

Baker::~Baker()
{

}
//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	Box boxer;
	DONUT doughBoi;
	for(int i = 1; i <= anOrder.number_donuts; i++)
	{
		boxer.addDonut(doughBoi);
		if(i%12 == 0 || i== anOrder.number_donuts)
		{
			anOrder.boxes.push_back(boxer);
			boxer.clear();
		}
	}

}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {

	PRINT2("Thread waiting before loop, my id is ", id);
	while(true)
	{
		PRINT4("queue size is ", order_in_Q.size(), " this is thread ", id);
		//threads getting stuck here. Waiting on a notify_all from waiter AFTER waiter has exited execution. Need to only wait for a
		//notify all once, to indicate that orders have been placed in the in_order_queue
		if(b_WaiterIsFinished == false)
		{
			unique_lock<mutex> lck(mutex_order_inQ);
			cv_order_inQ.wait(lck); //working?
		}
		if(order_in_Q.empty() && b_WaiterIsFinished == true)
		{
			PRINT2("Thread done ", id);
			break;
		}
		while (order_in_Q.empty() && b_WaiterIsFinished == false)
		{
			unique_lock<mutex> lck(mutex_order_inQ);
			PRINT2("Thread waiting ", id);
			cv_order_inQ.wait(lck); //working?
		}
		unique_lock<mutex> lck1(mutex_order_outQ);
		if(!order_in_Q.empty())
		{
			PRINT3("Thread ", id, " is working.");
			ORDER tempOrder = order_in_Q.front();
			order_in_Q.pop();
			lck1.unlock();
			bake_and_box(tempOrder);
			lck1.lock();
			order_out_Vector.push_back(tempOrder);
			PRINT3("Thread ", id, " finished working");
			lck1.unlock();
		}
	}
}




