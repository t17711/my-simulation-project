import numpy as np
from store import *
import time
import random
#import matplotlib.pyplot as plt

event_list=[]
store_list=[]
   
restock_time =[]
enter_time = []
exit_time=[]
delay_time=[]
exit_order = []

CLOCK=0
CUSTOMERS = 1

Min_time = 0.0001 # 1 seconds is smallest arrival interval

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
    
    # here current name is store name
    store_index = get_upper_store(curr_event.curr_store.name)

    stock  = get_stock_to_refill(store_index)
   
    curr_event.stock_takable = stock

    t = get_time_high(curr_event.curr_store.name)
    t = Min_time + random.uniform(0,t);

    # if store index is 0 just have double the travel time and return
    # warehouse has infinite stock and 0 queue
    if store_index < 0:
        curr_event.update("Returned from Refill", refill_return , CLOCK+2*t)
        curr_event.curr_store.shelf_exit_time+= CLOCK+2*t
        curr_event.curr_store.shelf_exit_time+= CLOCK+2*t
        event_list.append(curr_event)

    else:        
        # if not go to upper store
        curr_event.store = store_list[store_index]                            
        curr_event.update("Reached Refill", refill_take, CLOCK+t)
        curr_event.curr_store.shelf_exit_time+= CLOCK+t
        event_list.append(curr_event)

def refill_take(curr_event):
    random.seed()
    t = get_time_high(curr_event.curr_store.name)

    t2= get_time_low(curr_event.curr_store.name)
   
    store_index = get_upper_store(curr_event.curr_store.name)
    stor2 =  store_list[store_index]

    curr_event.curr_store = stor2

    # this is refill event
    if stor2.curr_shelf.stock > curr_event.stock_takable:
        stor2.curr_shelf.stock -= curr_event.stock_takable
        curr_event.curr_store.shelf_exit_time =CLOCK+t2
        t2 = CLOCK+random.uniform(0,t2)   + t2  
        curr_event.update("Refill Return", refill_return ,CLOCK+t2)

    else:
        t = CLOCK+random.uniform(0,t) + t       
        curr_event.update("Refill Queue",refill_take ,CLOCK+t)
        curr_event.curr_store.shelf_exit_time =CLOCK+t 

        if stor2.manager_contacted is False:
            # create new get event
            evn = event(stor2.name,stor2,refill_go, CLOCK)
            evn.type = "Refill Left"
            # to print out info do execute   sane refill_go(curr_event)
            evn.execute(CLOCK)
            stor2.manager_contacted = True
            stor2.shelf_exit_time = CLOCK+t

    event_list.append(curr_event)





def refill_return(curr_event):

    restock_time.append(CLOCK)

    #set the store to be same as name
    curr_event.curr_store = store_list[curr_event.name]

    # here just increase value of stock
    val = curr_event.stock_takable
    curr_event.curr_store.restock(val)
    # set the refill flag off
    curr_event.curr_store.manager_contacted =False  
    curr_event.set_shelf_status(NOT_BUSY)

def clerk_arrive(curr_event):
    #free shelf
    curr_event.set_shelf_status(NOT_BUSY)
    random.seed()
    temp =curr_event.get_clerk_status()
    # free shelf
    t = Min_time +random.expovariate(CLERK_RATE)
    if temp is NOT_BUSY:
        curr_event.update("CLERK EXIT", clerk_exit ,CLOCK+t)

        if (curr_event.curr_store.curr_clerk_queue >0):
            curr_event.curr_store.curr_clerk_queue-=1
            curr_event.curr_store.clerk_exit_time+=t
        else:
            curr_event.curr_store.clerk_exit_time = CLOCK +t

        if (curr_event.curr_store.curr_clerk_queue==0):   # if this costumer was last in queue
            curr_event.curr_store.clerk_exit_time = CLOCK +t

        #now set the shelf busy which will be not busy when shelf exit is executed
        curr_event.set_clerk_status(BUSY)                                              

    else:            
        if (curr_event.curr_store.curr_clerk_queue==0):   # if this costumer was last in queue
            curr_event.curr_store.clerk_exit_time = CLOCK+t          
       
        if curr_event.in_clerk is False:
            # add event to the shelf queue of store, 1 stuff is still in system 
            curr_event.curr_store.curr_clerk_queue  += 1
        curr_event.curr_store.clerk_exit_time = CLOCK +t
        curr_event.update("IN CLERK QUEUE", curr_event.function, curr_event.curr_store.clerk_exit_time)
        curr_event.in_clerk = True

    #send modified event to event list
    event_list.append(curr_event)

def shelf_arrive(curr_event):
    random.seed()
    global CLOCK
    # now check if shelf is free
    Current_status =curr_event.get_shelf_status()
    t = Min_time+random.expovariate(SHELF_RATE)

    if Current_status is NOT_BUSY:
        # check stock and if successfully taken it returns true
        if curr_event.stock_take() is True:
                #now set the shelf busy which will be not busy when shelf exit is executed
                curr_event.set_shelf_status(BUSY)

                # send next shelf queue event to event list if there is a queue
                if (curr_event.curr_store.curr_shelf_queue>0):
                    curr_event.curr_store.curr_shelf_queue-=1
                    curr_event.curr_store.shelf_exit_time+=t
                else:
                    curr_event.curr_store.shelf_exit_time = CLOCK +t
                    generate_arrival_event()      # if this costumer came to this from gentrator generate next

                if (curr_event.curr_store.curr_shelf_queue>0):   # if this costumer was last in queue
                    curr_event.curr_store.shelf_exit_time+=t

                # this will get to clerk at clock + t time 
                curr_event.update("CLERK ARRIVE", clerk_arrive ,CLOCK+t)
                event_list.append(curr_event)
                return

        else:
            curr_event.set_shelf_status(BUSY)
            # else if there is no more stock
            #now create new event checking if stock is called to be refilled
            if curr_event.curr_store.manager_contacted is False:
                # create restock event
                curr_event.curr_store.manager_contacted =True
                # this will also change next shelf time for current store
                evn = event(curr_event.curr_store.name,curr_event.curr_store,refill_go, CLOCK)
                evn.type = "Refill Left"
                # to print out info do execute   sane refill_go(curr_event)
                evn.execute(CLOCK)

                if curr_event.in_shelf is False:
                    generate_arrival_event()
                    curr_event.curr_store.curr_shelf_queue+=1

                curr_event.update("IN SHELF QUEUE", curr_event.function,  curr_event.curr_store.shelf_exit_time)   
                curr_event.in_shelf = True              
                # put modified event to list
                event_list.append(curr_event)  
                return

    if (curr_event.curr_store.curr_shelf_queue==0):   # if this costumer is in queue
        curr_event.curr_store.shelf_exit_time = CLOCK+t

    curr_event.curr_store.shelf_exit_time+=t

    # generate next arrival  and add to list
    if curr_event.in_shelf is False:
        generate_arrival_event()
        curr_event.curr_store.curr_shelf_queue+=1

    curr_event.update("IN SHELF QUEUE", curr_event.function,  curr_event.curr_store.shelf_exit_time)   
    curr_event.in_shelf = True              
    # put modified event to list
    event_list.append(curr_event)   
 
def clerk_exit(curr_event):
    #set clerk status not busy
    curr_event.set_clerk_status(NOT_BUSY)

    # add event to the shelf queue of stor    # store exit time
    delay_time.append(CLOCK  - curr_event.arrival_time)
    exit_order.append(curr_event.name)

def generate_arrival_event():
    global event_list
    global CUSTOMERS
    random.seed()
    r = random.uniform(0.0,1.0)
    a = len(event_list)
    for i in range(len(store_weight)):
        if (r < store_weight[i]):
            t = Min_time + random.expovariate(CUSTOMER_ARRIVAL_RATE)

            # send customer to shelf
            #where they will meet a queue or free shelf                             
            event_list.append(event(CUSTOMERS,store_list[i], shelf_arrive, CLOCK + t))
            enter_time.append(CLOCK + t)
            CUSTOMERS+=1
            break
    if len(event_list) == a:
        print("no create")

def customer_arrivals(MAX_CLOCK):
    global CLOCK 
    global CUSTOMERS

    CUSTOMERS=1
    # print("new customer\n")
    CLOCK = 0
    generate_arrival_event()  
    
    # generate customer, it will add arrival to list    
    """Create new customers until the sim time reaches Max."""
    while (CLOCK < MAX_CLOCK):

        next_event = 0

        next_event_time = event_list[next_event].time

        x = len(event_list) 
        #check if this time is smaller than next event if so got to next event
        for i in range(x):
            if event_list[i].time < next_event_time:
                next_event_time = event_list[i].time
                next_event = i
             
        #increment clock
        CLOCK = next_event_time       

        # if next event is to execute events then execute
        # execute next event 
        event_list[next_event].execute(CLOCK)

        # remove it from event list , this will end this event but will create new event
        #for example arrive event creates shelf event and theat creates clerk event
        event_list.pop(next_event)
        print("events in list  %d"%(len(event_list)))

    
store_weight= [0.1,0.2,0.3,0.5,0.7,0.8,1.0]

for i in range(len(store_weight)):
    store_list.append(store(i))
    
customer_arrivals(100) 
avg_delay =   sum(delay_time)/len(delay_time)
print("average delay is %f"%(avg_delay))

#x = np.linspace(1,len(enter_time)+1,len(enter_time))     # customer # starts at 1 and ends 1 step above

#num_exited=len(delay_time)
#y =np.linspace(1,num_exited,num_exited)

#num_restock = len(restock_time)
#z =   np.linspace( 1,num_restock,num_restock)
                             

##for i in range(len(exit_order)):
#    exit_time.append(enter_time[exit_order[i]]+delay_time[i])

#plt.scatter(restock_time, z)
#plt.scatter(x,enter_time,c='red',marker =3)
#plt.scatter(exit_order ,exit_time, c ='magenta'  ,  marker =3)

#plt.show()
