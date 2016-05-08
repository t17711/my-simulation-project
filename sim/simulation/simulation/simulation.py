import numpy as np
from store import *
import time
import matplotlib.pyplot as plt



event_list=[]
store_list=[]
   
restock_time =[]
enter_time = []
exit_time=[]
delay_time=[]
exit_order = []

CLOCK=0
CUSTOMERS = 1

def get_upper_store(store1):
    # to do
    return store1-1

def get_time_high(store1):
    #TO DO
    return 1.0

def get_time_low(store1):
    #TO DO
    return 2.0

def get_stock_to_refill(store1):
    #to do
    return 100

def refill_go(curr_event):
    store_list[curr_event.name].manager_contacted =True

    time = get_time_high(curr_event.name)

    # here current name is store name
    store_index = get_upper_store(curr_event.name)

    stock  = get_stock_to_refill(curr_event.name)
    curr_event.stock_takable = stock


    # if store index is 0 just have double the travel time and return
    # warehouse has infinite stock and 0 queue
    if store_index < 0:
        curr_event.update("Returned from Refill", refill_return , CLOCK+2*time)
        event_list.append(curr_event)

    else:        
        # if not go to upper store
        curr_event.store = store_list[store_index]
        curr_event.update("Reached Refill", shelf_arrive, CLOCK+2*time)
        event_list.append(curr_event)

def refill_return(curr_event):
    restock_time.append(CLOCK)
    #set the store to be same as name
    curr_event.curr_store = store_list[curr_event.name]

    # here just increase value of stock
    val = get_stock_to_refill(curr_event.name)
    store_list[curr_event.name].restock(val)

    # set the refill flag off
    store_list[curr_event.name].manager_contacted =False  

    # release queue does same thing as clerk exit but not update stastic
    #set clerk status not busy
    curr_event.set_clerk_status(NOT_BUSY)

    # add event to the shelf queue of stor    # store exit time
    delay_time.append(CLOCK  - curr_event.arrival_time)
    exit_order.append(curr_event.name)

    # send next clerk queue event to event list if there is a queue
    if (len(curr_event.curr_store.curr_clerk_queue)>0):
        event_next = curr_event.clerk_next_queue()
        # only need to change time
        event_next.time = CLOCK+1 / CLERK_RATE
        event_list.append(event_next)  
      


def clerk_arrive(curr_event):
    temp =curr_event.get_clerk_status()
    if temp is NOT_BUSY:
        curr_event.update("CLERK EXIT", clerk_exit ,CLOCK+1/CLERK_RATE)
        event_list.append(curr_event)
        #now set the shelf busy which will be not busy when shelf exit is executed
        curr_event.set_clerk_status(BUSY)
    else:
        curr_event.clerk_add_queue(curr_event)

def shelf_arrive(curr_event):

    # now check if shelf is free
    temp =curr_event.get_shelf_status()
    if temp is NOT_BUSY:
        if curr_event.stock_take() is True:
            if curr_event.refill is True:
                
                # this is refill event
                t =get_time_high(curr_event.name)
                curr_event.update("Refill Return", refill_return ,CLOCK+t)

            else:
                # now spend shelf_rate time 
                curr_event.update("SHELF EXIT", shelf_exit ,CLOCK+1/SHELF_RATE)
                event_list.append(curr_event)
                #now set the shelf busy which will be not busy when shelf exit is executed
                curr_event.set_shelf_status(BUSY)
        else:
            # else if there is no more stock
            # add event to the shelf queue of store
            curr_event.shelf_add_queue(curr_event)

            #now create new event
            if curr_event.curr_store.manager_contacted is False:
                # create restock event
                curr_event.curr_store.manager_contacted =True
                evn = event(curr_event.curr_store.name,curr_event.curr_store,refill_go, CLOCK)
                evn.type = "Refill Left"
                evn.execute(CLOCK)
    else:
        # add event to the shelf queue of store
        curr_event.shelf_add_queue(curr_event)
   
    # generate next arrival
    # generate customer, it will add arrival to list
    generate_arrival_event()  

 
def clerk_exit(curr_event):

    #set clerk status not busy
    curr_event.set_clerk_status(NOT_BUSY)

    # add event to the shelf queue of stor    # store exit time
    delay_time.append(CLOCK  - curr_event.arrival_time)
    exit_order.append(curr_event.name)

    # send next clerk queue event to event list if there is a queue
    if (len(curr_event.curr_store.curr_clerk_queue)>0):
        event_next = curr_event.clerk_next_queue()
        # only need to change time
        event_next.time = CLOCK+1 / CLERK_RATE
        event_list.append(event_next)  

def shelf_exit(curr_event):
    # since it is exiting now shelf is not busy
    curr_event.set_shelf_status(NOT_BUSY)

    # send this event to event list for clerk
    clerk_arrive(curr_event)

    # send next shelf queue event to event list if there is a queue
    if (len(curr_event.curr_store.curr_shelf_queue)>0):
        event_next = curr_event.shelf_next_queue()
        # only need to change time
        event_next.time = CLOCK+1 / SHELF_RATE
        event_list.append(event_next)


def generate_arrival_event():
    global CUSTOMERS
    np.random.seed()
    r = np.random.uniform(0.0001, 1.0)

    for i in range(len(store_weight)):
        if (r < store_weight[i]):
            t = r*2.0/CUSTOMER_ARRIVAL_RATE

            # send customer to shelf
            #where they will meet a queue or free shelf                             
            event_list.append(event(CUSTOMERS,store_list[i], shelf_arrive, CLOCK + t))
            enter_time.append(CLOCK + t)
            CUSTOMERS+=1
            break;



def customer_arrivals(MAX_CLOCK):
    global CLOCK
    np.random.seed( 99999)
    CUSTOMERS=1
    # print("new customer\n")
    CLOCK = 0
    
    # generate customer, it will add arrival to list
    generate_arrival_event()  

    """Create new customers until the sim time reaches Max."""
    while (CLOCK < MAX_CLOCK):

        next_event = 0
        if len(event_list) is 0:
            generate_arrival_event()  

        next_event_time = event_list[0].time

        #check if this time is smaller than next event if so got to next event
        for i in range(len(event_list)):
            if event_list[i].time < next_event_time:
                next_event_time = event_list[i].time
                next_event = i
             
        #increment clock
        CLOCK = next_event_time;

        # if next event is to execute events then execute
        # execute next event 
        event_list[next_event].execute(CLOCK)

        # remove it from event list , this will end this event but will create new event
        #for example arrive event creates shelf event and theat creates clerk event
        event_list.pop(next_event)

    
store_weight= [0.1,0.2,0.3,0.5,0.7,0.8,0.999]

for i in range(len(store_weight)):
    store_list.append(store(i))
    
customer_arrivals(10) 
x = np.linspace(1,len(enter_time)+1,len(enter_time))     # customer # starts at 1 and ends 1 step above

num_exited=len(delay_time)
y =np.linspace(1,num_exited,num_exited)

num_restock = len(restock_time)
z =   np.linspace( 1,num_restock,num_restock)
                             
avg_delay =   sum(delay_time)/len(delay_time)
print("average delay is %f"%(avg_delay))

for i in range(len(exit_order)):
    exit_time.append(enter_time[exit_order[i]]+delay_time[i])

plt.scatter(restock_time, z)
plt.scatter(x,enter_time,c='red',marker =3)
plt.scatter(exit_order ,exit_time, c ='magenta'  ,  marker =3)

plt.show()
