import numpy as np
from store import *
import time
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
    
    np.random.seed()
    r = np.random.exponential()


    # here current name is Store name
    store_index = get_upper_store(curr_event.curr_store.name)

    stock  = get_stock_to_refill(curr_event.name)
    curr_event.stock_takable = stock

    t = r*get_time_high(curr_event.curr_store.name)

    # if Store index is 0 just have double the travel time and return
    # warehouse has infinite stock and 0 queue
    if store_index < 0:
        curr_event.update("Returned from Refill", refill_return , CLOCK+2*t)
        event_list.append(curr_event)

    else:        
        # if not go to upper Store
        curr_event.store = store_list[store_index]
        curr_event.update("Reached Refill", shelf_arrive, CLOCK+2*t)
        event_list.append(curr_event)

def refill_take(curr_event):
    # if current customer is refiller not 
    #if current Event is refill # need to do stuff here
    # if there is not enough stock send upstream
    np.random.seed()
    r = np.random.exponential()
    t = CLOCK+r*get_time_high(curr_event.curr_store.name)

    t2=0
    i =curr_store.name
    while (i!=curr_store.name):
        t2 +=CLOCK+r*get_time_low(i)
        i-=1

    # this is refill Event
    if curr_event.stock_take is True:
        curr_event.update("Refill Return", refill_return ,CLOCK+t2)
    else:
        curr_event.update("Refill upwards", refill_go ,CLOCK+t)

def refill_return(curr_event):

    restock_time.append(CLOCK)

    #set the Store to be same as name
    curr_event.curr_store = store_list[curr_event.name]

    # here just increase value of stock
    val = get_stock_to_refill(curr_event.name)
    store_list[curr_event.name].restock(val)

    # set the refill flag off
    store_list[curr_event.name].manager_contacted =False  
      
def clerk_arrive(curr_event):
    np.random.seed()
    r = np.random.uniform()
    temp =curr_event.get_clerk_status()
    # free Shelf
    curr_event.set_shelf_status(NOT_BUSY)
    if temp is NOT_BUSY:
        t =r*2.0/CLERK_RATE
        curr_event.update("CLERK EXIT", clerk_exit ,CLOCK+t)
        #now set the Shelf busy which will be not busy when Shelf exit is executed
        curr_event.set_clerk_status(BUSY)

        #now update time in Store  so that i can calculate time for net costumer
        if  curr_event.curr_store.shelf_exit_time is 0:
            # if clock is not set at first then next Shelf arrival from queue to system is clock + random
            curr_event.curr_store.clerk_exit_time = CLOCK +t
    else:
        # add Event to the Shelf queue of Store, 1 stuff is still in system
        curr_event.curr_store.curr_clerk_queue+=1
        t = r*2.0/CLERK_RATE

        #set the execution time of this Event to be at next Shelf exit time
        curr_event.time = curr_event.curr_store.clerk_exit_time

        # update next Shelf exit time
        curr_event.curr_store.clerk_exit_time+=t

    #send modified Event to Event list
    event_list.append(curr_event)
 
def shelf_arrive(curr_event):
    np.random.seed()
    r = np.random.exponential()

    # now check if Shelf is free
    Current_status =curr_event.get_shelf_status()
    if Current_status is NOT_BUSY:
        # check stock and if successfully taken it returns true
        if curr_event.stock_take is True:
                # now spend shelf_rate time 
                t = r*2.0/SHELF_RATE
                
                #now update time in Store  so that i can calculate time for net costumer
                if  curr_event.curr_store.shelf_exit_time is 0:
                    # if clock is not set at first then next Shelf arrival from queue to system is clock + random
                    curr_event.curr_store.shelf_exit_time = CLOCK +t
                # send next Shelf queue Event to Event list if there is a queue
                if (curr_event.curr_store.curr_shelf_queue>0):
                    curr_event.curr_store.curr_shelf_queue-=1

                #event_list.append(curr_event)
                #now set the Shelf busy which will be not busy when Shelf exit is executed
                curr_event.set_shelf_status(BUSY)
                # this will get to Clerk at clock + t time
                curr_event.update("CLERK ARRIVE", clerk_arrive ,CLOCK+t)
                # curr_event.execute() #it happens immediately, just to print it out i call execute
        else:
            # else if there is no more stock
            #now create new Event checking if stock is called to be refilled
            if curr_event.curr_store.manager_contacted is False:
                # create restock Event
                curr_event.curr_store.manager_contacted =True
                # this will also change next Shelf time for current Store
                evn = Event(curr_event.curr_store.name, curr_event.curr_store, refill_go, CLOCK)
                evn.type = "Refill Left"
                
                # to print out info do execute   sane refill_go(curr_event)
                evn.execute(CLOCK)
            else:
                # add Event to the Shelf queue of Store, 1 stuff is still in system
                curr_event.curr_store.curr_shelf_queue+=1
                t = r*2.0/SHELF_RATE

                #set the execution time of this Event to be at next Shelf exit time
                curr_event.time = curr_event.curr_store.shelf_exit_time

                # update next Shelf exit time
                curr_event.curr_store.shelf_exit_time+=t
    else:
        # add Event to the Shelf queue of Store, 1 stuff is still in system
        curr_event.curr_store.curr_shelf_queue+=1
        t = r*2.0/SHELF_RATE

        #set the execution time of this Event to be at next Shelf exit time
        curr_event.time = curr_event.curr_store.shelf_exit_time

        # update next Shelf exit time
        curr_event.curr_store.shelf_exit_time+=t

    # put modified Event to list
    event_list.append(curr_event)   
    # generate next arrival
    # generate customer, it will add arrival to list
    
    generate_arrival_event()  
 
def clerk_exit(curr_event):
    #set Clerk status not busy
    curr_event.set_clerk_status(NOT_BUSY)

    # add Event to the Shelf queue of stor    # Store exit time
    delay_time.append(CLOCK  - curr_event.arrival_time)
    exit_order.append(curr_event.name)

    # send next Clerk queue Event to Event list if there is a queue
    if (curr_event.curr_store.curr_clerk_queue>0):
        curr_event.curr_store.curr_clerk_queue-=1

def shelf_exit(curr_event):
    #set Clerk status not busy
    curr_event.set_shelf_exit(NOT_BUSY)

    # add Event to the Shelf queue of stor    # Store exit time
    delay_time.append(CLOCK  - curr_event.arrival_time)
    exit_order.append(curr_event.name)

    # send next Clerk queue Event to Event list if there is a queue
    if (curr_event.curr_store.curr_clerk_queue>0):
        curr_event.curr_store.curr_clerk_queue-=1

def generate_arrival_event():
    global CUSTOMERS
    np.random.seed()
    r = np.random.exponential()

    for i in range(len(store_weight)):
        if (r < store_weight[i]):
            t = r*2.0/CUSTOMER_ARRIVAL_RATE

            # send customer to Shelf
            #where they will meet a queue or free Shelf
            event_list.append(Event(CUSTOMERS, store_list[i], shelf_arrive, CLOCK + t))
            enter_time.append(CLOCK + t)
            CUSTOMERS+=1
            break

def customer_arrivals(MAX_CLOCK):
    global CLOCK
    np.random.seed()
    CUSTOMERS=1
    # print("new customer\n")
    CLOCK = 0
    
    # generate customer, it will add arrival to list
      

    """Create new customers until the sim time reaches Max."""
    while (CLOCK < MAX_CLOCK):

        next_event = 0
        if len(event_list) is 0:
            generate_arrival_event()  

        if (len(event_list)   ) is 0:
            generate_arrival_event() 

        next_event_time = event_list[0].time

        #check if this time is smaller than next Event if so got to next Event
        for i in range(len(event_list)):
            if event_list[i].time < next_event_time:
                next_event_time = event_list[i].time
                next_event = i
             
        #increment clock
        CLOCK = next_event_time

        # if next Event is to execute events then execute
        # execute next Event
        event_list[next_event].execute(CLOCK)

        # remove it from Event list , this will end this Event but will create new Event
        #for example arrive Event creates Shelf Event and theat creates Clerk Event
        event_list.pop(next_event)

    
store_weight= [0.1,0.2,0.3,0.5,0.7,0.8,0.999]

for i in range(len(store_weight)):
    store_list.append(Store(i))
    
customer_arrivals(10) 
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
