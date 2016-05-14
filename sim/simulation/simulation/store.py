# store elements
BUSY = True
NOT_BUSY = False
QUEUE_LIMIT=2000
NEXT_TIME=0.0
SHELF_NUMBER = 1
# for 1 item
CUSTOMER_ARRIVAL_RATE = SHELF_NUMBER*(60) # 2 PER 10 MIN
SHELF_CAPACITY = 10
SHELF_RATE = SHELF_NUMBER*(50)

CLERK_NUMBER = 4
CLERK_RATE = 20*CLERK_NUMBER # 1 PER MIN

SHELF_NUMBER = 1

RESTOCKING = False
class event:
    def __init__(self,CUSTOMERS,store, funct, time):
        self.type = "ENTERED STORE"
        self.name = CUSTOMERS
        # what is the event that has to happen here
        self.function = funct
        self.curr_store =  store
        self.arrival_time = time
        self.time = time
        self.stock_takable=1
        self.refill= False
        self.in_clerk =False
        self.in_shelf = True

    def execute(self,CLOCK):
        print("TIME: %f, Customer %d reached %s"%(CLOCK, self.name, self.type))
        self.curr_store.print_s()
        self.function(self)

    def stock_take(self):
        curr = self.curr_store.curr_shelf.stock
        if (curr >= self.stock_takable):
            self.curr_store.curr_shelf.stock-=self.stock_takable
            return True
        else:
            return False

    def restock(self):
        self.curr_store.restock(self.stock_takable)
 

    def update(self,type,funct,time):
        self.type =type
        self.function = funct
        self.time = time
    
    def set_shelf_status(self,val):
        self.curr_store.curr_shelf.status = val

    def set_clerk_status(self,val):
        self.curr_store.curr_clerk.status = val

    def get_clerk_status(self):
        # if busy dont return
        return self.curr_store.curr_clerk.status

    def get_shelf_status(self):            
        return self.curr_store.curr_shelf.status



class shelf:
    def __init__(self):
        self.self_capacity = SHELF_CAPACITY
        self.stock = SHELF_CAPACITY
        #every shelf has a queue
        self.status= NOT_BUSY

class clerk:
    def __init__(self):
        self.status= NOT_BUSY

class store:
    def __init__(self,num):
        self.name =  num
        # for stock
        self.curr_shelf = shelf()
        self.curr_clerk = clerk()
        self.curr_clerk_queue =0
        self.curr_shelf_queue =0
        self.manager_contacted = False

        self.shelf_exit_time = 0
        self.clerk_exit_time = 0
   
    def restock(self, val):
        self.curr_shelf.stock = val


    def print_s(self):
        print("Cureent store name is %d, the Shelf queue is %d, the Clerk queue is %d, stock is %d\n"
        % (self.name,self.curr_shelf_queue, self.curr_clerk_queue, self.curr_shelf.stock))
    
   