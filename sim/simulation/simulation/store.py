# store elements

# for easy comparision
BUSY = True
NOT_BUSY = False

REFILL_TIME = 48
# store constants
QUEUE_LIMIT = 200
NEXT_TIME = 0.0
SHELF_NUMBER = 1

CUSTOMER_ARRIVAL_RATE = SHELF_NUMBER * 60  # 2 PER 10 MIN
SHELF_CAPACITY = 1000
SHELF_RATE = SHELF_NUMBER * 50

CLERK_NUMBER = 4
CLERK_RATE = 20 * CLERK_NUMBER  # 1 PER MIN

SHELF_NUMBER = 1

RESTOCKING = False


class Event:
    def __init__(self, customers, curr_store: object, function, time: float):

        self.type = "ENTERED STORE"
        self.name = customers
        # what is the Event that has to happen here
        self.function = function
        self.curr_store = curr_store
        self.arrival_time = time
        self.time = time
        self.stock_takable = 1
        self.refill = False
        self.in_clerk = False
        self.in_shelf = False
        self.exited = False

    def reserve(self):
        self.curr_store.reserve_event.append(self)

    def execute(self, clock):
        # print(u"TIME: {0:f}, ".format(clock))
        # print("TIME: %f, Customer %d reached %s"%(clock, self.name, self.type))
        # self.curr_store.print_s()
        self.function(self)

    @property
    def stock_take(self):
        curr = self.curr_store.curr_shelf.stock
        if curr >= self.stock_takable:
            self.curr_store.curr_shelf.stock -= self.stock_takable
            return True
        else:
            return False

    def restock(self):
        self.curr_store.restock(self.stock_takable)

    def update(self, type_new, function_new, time_new):
        self.type = type_new
        self.function = function_new
        self.time = time_new

    def set_shelf_status(self, val: object) -> object:
        self.curr_store.curr_shelf.status = val

    def set_clerk_status(self, val):
        self.curr_store.curr_clerk.status = val

    def get_clerk_status(self):
        # if busy dont return
        return self.curr_store.curr_clerk.status

    def get_shelf_status(self):
        return self.curr_store.curr_shelf.status


class Shelf:
    def __init__(self):
        self.self_capacity = SHELF_CAPACITY
        self.stock = SHELF_CAPACITY
        # every Shelf has a queue
        self.status = NOT_BUSY


class Clerk:
    def __init__(self):
        self.status = NOT_BUSY


class Store:
    def __init__(self, num: int, address: int) -> object:
        assert isinstance(num, int)
        self.name = num
        # for stock
        self.curr_shelf = Shelf()
        self.curr_clerk = Clerk()
        self.curr_clerk_queue = 0
        self.curr_shelf_queue = 0
        self.manager_contacted = False

        self.shelf_exit_time = 0
        self.clerk_exit_time = 0

        self.reserve_event = []

        assert isinstance(address, int)
        self.address = address

    def restock(self, val):
        self.curr_shelf.stock = val

    def print_s(self):
        print("Cureent Store name is %d, the Shelf queue is %d, the Clerk queue is %d, stock is %d\n"
              % (self.name, self.curr_shelf_queue, self.curr_clerk_queue, self.curr_shelf.stock))
