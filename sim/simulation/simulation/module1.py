import random
import simpy

import numpy as np
import matplotlib.pyplot as plt

# yield puts function in future event list

CUSTOMER_ARRIVAL_RATE = 5 # 20 PER HR
SHELF_CAPACITY = 20
SHELF_RATE = 10  # 60 PER HR
CLERK_NUMBER = 1
CLERK_RATE = 10 # 30 per hr

SHELF_NUMBER = 1

RESTOCKING = TRUE

restock_time =[]
enter_time = []
exit_time=[]
shelf_delay = []
clerk_delay = []
class store:
    def __init__(self, env):
        self.env = env
        self.shelf = simpy.Resource(env,capacity=SHELF_NUMBER)
        self.clerk = simpy.Resource(env,capacity=CLERK_NUMBER)
        self.stock = simpy.Container(env, capacity=SHELF_CAPACITY, init = SHELF_CAPACITY)

    def print_s(self):
        print("\n %d remaining in shelf \t shelf line is %d \t clerk line % d\n"
        % (self.stock.level,len(self.shelf.queue),len(self.clerk.queue)))

    def get(self,n, customer):
        # print("user %d generated at %s: " %(customer,env.now))

        with self.shelf.request() as shelf_line:
            print("SHELF\t %d reaches SHELF queue at: \t %s" % (customer, self.env.now))
            self.print_s()         
            now = self.env.now
            enter_time.append(now)

            yield shelf_line
            # line if there is no stock
            with self.stock.get(n) as stock_line:
                print("SHELF\t %d reaches stock queue at: \t %s" % (customer, self.env.now))
                self.print_s()         
                yield stock_line
                
                # go to take stuff
                yield self.env.timeout(1.0/SHELF_RATE)  # time in hrs

                ''' this get line creates queue that only releases if 
                stock level is bigger than 0 and if not they stay in queue'''

                print("SHELF\t %d took 1 from SHELF at: \t  %s" % (customer, self.env.now))
                self.print_s()         


                # no need to yield this since this function creates another event inside it

                shelf_delay.append(self.env.now - now)

                self.env.process(self.clerk_go(customer))



    def clerk_go(self,customer):
        with self.clerk.request() as clerk_line:
            print("CLERK\t %d reached CLERK queue at : \t %s" % (customer, self.env.now))         
            self.print_s() 

            # wait
            yield clerk_line

            # print("CLERK\n %d customer arrived at CLERK %s queue line is %d"%(customer, self.env.now,len(self.clerk.queue)))
            yield self.env.timeout(1.0/ CLERK_RATE)  # per hr to redo
            print("CLERK\t %d bought 1 from CLERK at : \t %s" % (customer, self.env.now))
            self.print_s() 

            exit_time.append(self.env.now)

   
    def restock(self):
         if not self.stock.level > 0 and not RESTOCKING:
            RESTOCKING = True
            restock_time.append(self.env.now)

            ''' need to do restock function
            this queues the stuff until restocked'''

            print("restock start at %s"% (self.env.now)) 
            yield self.env.timeout(2.0)  # 2 hr to redo
            yield self.stock.put(SHELF_CAPACITY)
            self.print_s() 

            print("%d put to stock  at %s" % (self.stock.level,self.env.now)) 

            RESTOCKING = False

            # here stock is full so put

    def buy(self, customer):
        yield self.env.process(self.get(1, customer))


def customer_arrivals(env, store):
    """Create new customers until the sim time reaches 120."""
    i = 1 
    while 1:
        # print("new customer\n")
        r = random.uniform(0.0,1.0)
        yield env.timeout(r*2.0/SHELF_RATE)  # avg arrival is per hr
        env.process(store.get(1, i))
        env.process(store.restock())
        i+=1



random.seed(9999)
env = simpy.Environment()
store1= store(env)

buy=env.process(customer_arrivals(env,store1))

env.run(until = 10.0) # stop at 100 hr

y = [ 3 for x in range(len(restock_time))]

z = [ 1 for x in range(len(enter_time))]

t =  [ 2 for x in range(len(exit_time))]


plt.scatter(restock_time, y)
plt.scatter(enter_time, z,c='red',marker =3)
plt.scatter(exit_time, t , c ='magenta'  ,  marker =3)

plt.show()