# coding=utf-8
import random

import numpy as np

from store import *

# these are the global lists and values for project
event_list = []  # stores events of Object class Event
store_list = []  # this contains the stores Object Store

restock_time = []
enter_time = []
exit_time = []
delay_time = []
exit_order = []

CLOCK = 0
CUSTOMERS = 1

# now for  modelling create a graph of store area
store_weight = [0.1, 0.3, 0.5, 0.8, 1.0]  # [0.1,0.2,0.3,0.5,0.7,0.8,1.0]

store_exists_at_node = []

# every nodes have same length for convenience
top =0
graph = {1: [2, 6],
         2: [1, 3, 5],
         3: [2, 4, 5],
         4: [3, 7],
         5: [2, 3, 7, 10],
         6: [1, 7, 8],
         7: [4, 5, 6, 8],
         8: [6, 7, 9],
         9: [8, 10],
         10: [9, 5]
         }  # size should be 10 for modelling

# now weighed graph to show average time it takes to directly go from one point to other
#            1,      2,         3,        4,         5,          6,          7,         8   ,       9,      10
'''       [np.inf,  np.inf, np.inf,   np.inf,   np.inf,     np.inf,     np.inf,    np.inf,     np.inf,    np.inf],'''
# the time from distribution to store is time from t to store + 1
weighed_graph = \
    [[0,       1,     np.inf,   np.inf,   np.inf,     2,         np.inf,    np.inf,     np.inf,    np.inf],
     [1,      0,       2,      np.inf,   3,         np.inf,     np.inf,    np.inf,     np.inf,    np.inf],
     [np.inf,  2,      0,       2,       3,         np.inf,     np.inf,    np.inf,     np.inf,    np.inf],
     [np.inf,  np.inf,  2,      0,        np.inf,     np.inf,     1,        np.inf,     np.inf,    np.inf],
     [np.inf,  3,      np.inf,  np.inf,   0,          np.inf,     np.inf,    np.inf,     np.inf,        3],
     [2,      np.inf,  np.inf,  np.inf,   np.inf,     0,          1,        2,         np.inf,    np.inf],
     [np.inf,  np.inf, np.inf,   1,       2,         1,         0,         1,         np.inf,    np.inf],
     [np.inf,  np.inf, np.inf,   np.inf,   np.inf,     2,         1,        0,          2,        np.inf],
     [np.inf,  np.inf, np.inf,   np.inf,   np.inf,     np.inf,     np.inf,    2,         0,             1],
     [np.inf,  np.inf, np.inf,   np.inf,   3,         np.inf,     np.inf,    np.inf,     1,             0]]

Min_time = 0.005  # 1 seconds is smallest arrival interval


# start simulation
def simulation_start(graph_nodes: list, time: float) -> object:
    # create stores
    global top
    global store_exists_at_node
    global store_list

    # set top item
    top = graph_nodes[0]

    # generate stores
    for i in range(len(store_weight)):
        str = Store(i, graph_nodes[i])
        str.restock(SHELF_CAPACITY* (1.1 - store_weight[i]))
        store_list.append(str)  # crate a store on name num and addr from graph
        e = Event(i, str, daily_refill, REFILL_TIME + get_random(2))
        e.type = "Daily Refill"
        event_list.append(e)

    # now show where in graph node that stuff exists
    for i in range(10):
        store_exists_at_node.append(False)

    for i in graph_nodes:
        store_exists_at_node[i] = True

    customer_arrivals(time)


# just floyd to calculate time before simulation, later i add some random number to this
def floyd(mat_input):
    l = len(mat_input)
    for k in range(l):
        for i in range(l):
            for j in range(l):
                mat_input[i][j] = min(mat_input[i][j], mat_input[i][k] + mat_input[k][j])


# generates random exponential time
def get_random(time_get):
    np.random.seed()
    x = np.random.exponential(1 / time_get)
    return x


# this just reads store_weight
def get_time(store1, store2):
    # just get time from weighed which has gone through floyd algorithm
    if store1 < 0:
        str1 = store_list[store2].address
        time_get = weighed_graph[str1][1] + 1

    elif store2 < 0:
        str1 = store_list[store1].address
        time_get = weighed_graph[str1][1] + 1
    else:
        str1 = store_list[store1].address
        str2 = store_list[store2].address
        time_get = weighed_graph[str1][str2]

    time_get += Min_time + random.uniform(0, time_get)

    return time_get


# this just look at addresses and go upside of graph
def get_upper_store(store1):

    str1 = store_list[store1].address
    if str1 is top:
        return -1

    minimum = 0

    for i in range(len(weighed_graph)):
        # for all address with value less than current address find the minimum address where a store exists
        if i < str1 and weighed_graph[str1][i] < weighed_graph[str1][minimum] and store_exists_at_node[i] is True:
            minimum = i

    if minimum == 0:
        return 0

    # get store
    for i in store_list:
        if i.address is minimum:
            minimum = i.name
            break
    return minimum


# this calculates amount of stock that a store gets
def get_stock_to_refill(store1):
    val = 1000* (1.1 - store_weight[store1])
    return val


# this adds refill event to list
def refill_go(curr_event):
    print("%d Store (address %d) arrived for refill from %d (addr %d) at %f" %
          (curr_event.name, store_list[curr_event.name].address,
           curr_event.curr_store.name, store_list[curr_event.curr_store.name].address, CLOCK))

    store_index = get_upper_store(curr_event.curr_store.name)
    t = get_time(curr_event.curr_store.name, store_index)

    # if Store index is 0 just have double the travel time and return
    # warehouse has infinite stock and 0 queue
    if store_index < 0:
        print("%d went to distribution " % curr_event.name)
        curr_event.update("Returned from Refill distribution center", refill_return, CLOCK + t)
    else:
        # if not go to upper Store
        print("%d (address %d) went to %d (address %d) at %f" %
              (curr_event.name, store_list[curr_event.name].address,  store_index, store_list[store_index].address, CLOCK))
        curr_event.update("Reached Refill", refill_take, CLOCK + t)
        curr_event.curr_store = store_list[store_index]
        #  store_list[curr_event.name].shelf_exit_time = CLOCK+ t +Min_time

    # store_list[curr_event.name].shelf_exit_time= CLOCK+t +Min_time
    event_list.append(curr_event)


# this takes stock from distribution or upper store
def refill_take(curr_event):
    random.seed()
    stor2 = curr_event.curr_store

    # this is refill Event
    if stor2.curr_shelf.stock * .25 > curr_event.stock_takable:
        t = get_time(curr_event.name, curr_event.curr_store.name)
        t = random.uniform(0, t) + t

        print("%d (address %d) got refill from %d (address %d) at %f" %
              (curr_event.name, store_list[curr_event.name].address, stor2.name, store_list[stor2.name].address,CLOCK))
        stor2.curr_shelf.stock -= curr_event.stock_takable
        # store_list[curr_event.name].shelf_exit_time+= t +Min_time
        curr_event.update("Refill Return", refill_return, CLOCK + t)
        event_list.append(curr_event)

    else:
        curr_event.update("Refill go up", refill_go, CLOCK)
        curr_event.execute(CLOCK)


# this adds stock to original
def refill_return(curr_event):
    print("%d (address %d)returned with stock at%f  "
          % (curr_event.name, store_list[curr_event.name].address, CLOCK))
    restock_time.append(CLOCK)

    # set the Store to be same as name
    store_curr = store_list[curr_event.name]

    # here just increase value of stock
    val = curr_event.stock_takable
    store_curr.restock(val)
    # set the refill flag off
    store_curr.manager_contacted = False
    store_curr.curr_shelf.status = NOT_BUSY
    time_diff = CLOCK - curr_event.arrival_time + Min_time
    for m in store_curr.reserve_event:
        m.time += time_diff
        event_list.append(m)

    store_curr.shelf_exit_time = event_list[-1].time

    del store_curr.reserve_event[:]
    curr_event.exited = True
    curr_event = None
    del curr_event


# customer event reaches clerk
def clerk_arrive(curr_event):
    # free Shelf
    curr_event.set_shelf_status(NOT_BUSY)
    # free Shelf
    t = Min_time + get_random(CLERK_RATE)

    if curr_event.curr_store.curr_clerk_queue == 0:  # if this costumer is in queue
        curr_event.curr_store.clerk_exit_time = CLOCK + t
    else:
        curr_event.curr_store.clerk_exit_time += t

    temp = curr_event.get_clerk_status()

    if temp is NOT_BUSY:
        curr_event.update("CLERK EXIT", clerk_exit, CLOCK + t)
        if curr_event.curr_store.curr_clerk_queue > 0:
            curr_event.curr_store.curr_clerk_queue -= 1

        # now set the Shelf busy which will be not busy when Shelf exit is executed
        curr_event.set_clerk_status(BUSY)
        event_list.append(curr_event)
    else:
        if curr_event.in_clerk is False:
            # add Event to the Shelf queue of Store, 1 stuff is still in system
            curr_event.curr_store.curr_clerk_queue += 1
            curr_event.in_clerk = True
        curr_event.update("IN CLERK QUEUE", clerk_arrive, curr_event.curr_store.clerk_exit_time)
        # send modified Event to Event list
        event_list.append(curr_event)


# customer event exits simulation
def clerk_exit(curr_event):
    # set Clerk status not busy
    curr_event.set_clerk_status(NOT_BUSY)

    # add Event to the Shelf queue of stor    # Store exit time
    delay_time.append(CLOCK - curr_event.arrival_time)
    exit_order.append(curr_event.name)
    curr_event.exited = True
    exit_time.append(CLOCK)

    curr_event = None
    del curr_event
    # n = gc.collect()
    # print("cleared %d"%(n))


# customer event arrives at store
def shelf_arrive(curr_event):
    random.seed()
    global CLOCK
    # now check if Shelf is free
    current_status = curr_event.get_shelf_status()
    t = Min_time + get_random(SHELF_RATE)

    if curr_event.in_shelf is False:
        generate_arrival_event()

    if curr_event.curr_store.curr_shelf_queue == 0:  # if this costumer is in queue
        curr_event.curr_store.shelf_exit_time = CLOCK + t
    else:
        curr_event.curr_store.shelf_exit_time += t

    if current_status is NOT_BUSY:
        curr_event.set_shelf_status(BUSY)

        # check stock and if successfully taken it returns true
        if curr_event.stock_take is True:
            # now set the Shelf busy which will be not busy when Shelf exit is executed

            # send next Shelf queue Event to Event list
            # if there is a queue decrement
            if curr_event.curr_store.curr_shelf_queue > 0:
                curr_event.curr_store.curr_shelf_queue -= 1

            # this will get to Clerk at clock + t time
            curr_event.update("CLERK ARRIVE", clerk_arrive, CLOCK + t)
            event_list.append(curr_event)
            return

        else:
            # else if there is no more stock
            # now create new Event checking if stock is called to be refilled
            if curr_event.curr_store.manager_contacted is False:
                # create restock Event
                curr_event.curr_store.manager_contacted = True

                # this will also change next Shelf time for current Store
                evn = Event(curr_event.curr_store.name, curr_event.curr_store, refill_go, CLOCK)
                evn.type = "Refill Left"

                # to print out info do execute   sane refill_go(curr_event)
                # here find stockable value
                stock = get_stock_to_refill(evn.name)
                evn.stock_takable = stock
                evn.execute(CLOCK)

                if curr_event.in_shelf is False:
                    curr_event.curr_store.curr_shelf_queue += 1
                    curr_event.in_shelf = True

                curr_event.update("IN SHELF QUEUE", shelf_arrive, curr_event.curr_store.shelf_exit_time)
                # put modified Event to list
                curr_event.reserve()
                return

    # generate next arrival  and add to list
    if curr_event.in_shelf is False:
        curr_event.curr_store.curr_shelf_queue += 1
        curr_event.in_shelf = True

    curr_event.update("IN SHELF QUEUE", shelf_arrive, curr_event.curr_store.shelf_exit_time)
    # put modified Event to list

    # if gone for refill then add to reserve
    if curr_event.curr_store.manager_contacted is True:
        curr_event.reserve()
    else:
        event_list.append(curr_event)


# this generates customer arrival events
def generate_arrival_event():
    global event_list
    global CUSTOMERS
    r = random.uniform(0.0, 1.0)
    a = len(event_list)
    # print(CUSTOMERS)
    for event_in_list in range(len(store_weight)):
        if r <= store_weight[event_in_list]:
            t = Min_time + get_random(CUSTOMER_ARRIVAL_RATE)

            # send customer to Shelf
            # where they will meet a queue or free Shelf
            event_list.append(Event(CUSTOMERS, store_list[event_in_list], shelf_arrive, CLOCK + t))
            enter_time.append(CLOCK + t)
            CUSTOMERS += 1
            break
    if len(event_list) == a:
        print("no create")


# this generates refill event
def daily_refill(curr_event):
    print("%d (address %d) Got daily refill at %f  "
          % (curr_event.name, store_list[curr_event.name].address, CLOCK))

    restock_time.append(CLOCK)

    # set the Store to be same as name
    store_curr = curr_event.curr_store

    # here just increase value of stock
    val = get_stock_to_refill(store_curr.name)
    store_curr.restock(val)
    # set the refill flag off

    curr_event.time = CLOCK + REFILL_TIME
    event_list.append(curr_event)


# this is main simulation loop
# #######################################################
# ######################################################
def customer_arrivals(max_clock: float) -> object:
    global CLOCK
    global CUSTOMERS
    global event_list

    # print("new customer\n")
    CLOCK = 0
    CUSTOMERS = 1

    generate_arrival_event()
    # generate customer, it will add arrival to list
    """Create new customers until the sim time reaches Max."""
    while CLOCK < max_clock:
        event_list.sort(key=lambda s: s.time, reverse=True)
        evn = event_list.pop(-1)
        assert isinstance(evn.time, object)
        next_event_time = evn.time

        if CLOCK > next_event_time:
            for events_in_list in event_list:
                if events_in_list.exited is True:
                    assert isinstance(events_in_list, object)
                    event_list.remove(events_in_list)
            continue

        CLOCK = next_event_time
        # if next Event is to execute events then execute
        # execute next Event
        evn.execute(CLOCK)

        # remove it from Event list , this will end this Event but will create new Event
        # for example arrive Event creates Shelf Event and theat creates Clerk Event
        # print("events in list  %d"%(len(event_list)))


