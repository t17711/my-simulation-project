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
graph = []  # size should be 15 for modelling


def simulation_start(graph_nodes: list, time: object) -> object:
    # create stores
    for i in range(len(store_weight)):
        store_list.append(Store(i, graph_nodes[i]))  ## crate a store on name num and addr from graph
    customer_arrivals(time)


Min_time = 0.005  # 1 seconds is smallest arrival interval


# generates random exponential time
def get_random(t):
    np.random.seed()
    x = np.random.exponential(1 / t)
    return x


def get_time(store1, store2):
    # to do

    t = abs((store2 - store1) * 2)
    if store2 < 0:
        t *= 2
    t = Min_time + random.uniform(0, t)
    return t


def get_upper_store(store1):
    # to do
    return store1 - 1


def get_stock_to_refill(store1):
    # to do
    return (len(store_list) - store1) * 100 + 100


def refill_go(curr_event):
    print("%d Store arrived for refill from %d at %f" % (curr_event.name, curr_event.curr_store.name, CLOCK))

    store_index = get_upper_store(curr_event.curr_store.name)
    t = get_time(curr_event.curr_store.name, store_index)

    # if Store index is 0 just have double the travel time and return
    # warehouse has infinite stock and 0 queue
    if store_index < 0:
        print("%d went to distribution " % curr_event.name)
        curr_event.update("Returned from Refill distribution center", refill_return, CLOCK + t)
    else:
        # if not go to upper Store
        print("%d went to %d  at %f" % (curr_event.name, store_index, CLOCK))
        curr_event.update("Reached Refill", refill_take, CLOCK + t)
        curr_event.curr_store = store_list[store_index]
        #  store_list[curr_event.name].shelf_exit_time = CLOCK+ t +Min_time

    # store_list[curr_event.name].shelf_exit_time= CLOCK+t +Min_time
    event_list.append(curr_event)


def refill_take(curr_event):
    random.seed()
    stor2 = curr_event.curr_store

    # this is refill Event
    if stor2.curr_shelf.stock > curr_event.stock_takable:
        t = get_time(curr_event.name, curr_event.curr_store.name)
        t = random.uniform(0, t) + t

        print("%d got refill from %d  at %f" % (curr_event.name, stor2.name, CLOCK))
        stor2.curr_shelf.stock -= curr_event.stock_takable
        # store_list[curr_event.name].shelf_exit_time+= t +Min_time
        curr_event.update("Refill Return", refill_return, CLOCK + t)
        event_list.append(curr_event)

    else:
        curr_event.update("Refill go up", refill_go, CLOCK)
        curr_event.execute(CLOCK)


def refill_return(curr_event):
    print("%d returned with stock at%f  " % (curr_event.name, CLOCK))
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


def customer_arrivals(max_clock: int) -> object:
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
