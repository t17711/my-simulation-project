import pydotplus
from simulation import *

graph_nodes = []


def print_nodes(title):
    graph_out = pydotplus.Dot(graph_type='digraph')

    nodes = []
    key = graph.keys()

    for i in key:
        nodes.append(pydotplus.Node(i, style="filled", fillcolor="green"))
    j = 0
    for i in graph_nodes:
        nodes.append(pydotplus.Node(i, label="store {0} address {1}".format(str(j), str(i)),
                                    style="filled", fillcolor="red"))
        j += 1

    for i in nodes:
        graph_out.add_node(i)

    for i in key:
        assert isinstance(graph, dict)
        for j in graph[i]:
            edge = pydotplus.Edge(nodes[i - 1], nodes[j - 1], label=str(weighed_graph[i - 1][j - 1]))
            graph_out.add_edge(edge)

    # this creates our initial graph which i create later from graphviz
    graph_out.write('Graph_' + str(title) + '.dot', 'raw')
    # need to do code like this later "dot -Tsvg main.dot -o t3.svg"

#
# print_nodes("map")

graph_area = [[1, 2, 3], [4, 5, 6, 7], [8, 9, 10]]
addresses = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
stores = [0, 1, 2, 3, 4]
floyd(weighed_graph)


# greedy algorithm, place all the stores on the best spots in an area

optimal_pos = [[],
               [],
               []]
# 1 is always placed on top
minimum = 0
err = np.inf
rerun = 1
graph_nodes = [3, 5, 9]

# while True:
#     tm = simulation_start(graph_nodes, 336)
#     print(tm)

while rerun < 10:
    rerun += 1
    for addresses in range(len(graph_area)):  # go through each store areas
        minimum = graph_area[addresses][0]    # set some value to start
        for possibles in graph_area[addresses]:  # go through each possible addresses
            graph_nodes[addresses] = possibles
            print("Simulating for  store %d at area:  " % addresses)
            print(graph_nodes)
            temp = simulation_start(graph_nodes, 336)  # do simulation for 2 weeks

            print(temp)
            if temp < err: # find minimum error address
                err = temp
                minimum = possibles
        err = np.inf
        print("Optimal node is: %d" % minimum)
        graph_nodes[addresses] = minimum # set address to minimum for faster computation
        optimal_pos[addresses].append(minimum) # store this address as one of the possible addresses

final = []
for fin in optimal_pos:
    # create a dictionary of all values then find which key gives minimum value
    d = {
        1: 0,
        2: 0,
        3: 0,
        4: 0,
        5: 0,
        6: 0,
        7: 0,
        8: 0,
        9: 0,
        10: 0
    }

    for t in fin:
        d[t] += 1

    maxm = 0
    max_key= 0
    for j in d.keys():
        if d[j] > maxm:
            maxm = d[j]
            max_key = j
    final.append(max_key)

print(final)
graph_nodes = final

print_nodes("Optimized")