from simulation import *
import matplotlib.pyplot as plt

graph_nodes = [1, 5, 7, 8, 12]  # size is  always 5
simulation_start(graph_nodes, 200)
avg_delay = sum(delay_time) / len(delay_time)

print("average delay is %f" % avg_delay)
# t= len(enter_time)
# x = np.ones((1,t))
# x = x.tolist()
# x = x[0]

# plt.scatter(restock_time, z)
# plt.scatter(enter_time,x,c='blue',marker =3)
plt.scatter(exit_order, delay_time, c='magenta', marker=3)

plt.show()
