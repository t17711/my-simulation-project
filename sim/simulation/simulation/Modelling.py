import pydotplus
from simulation import *

graph_out = pydotplus.Dot(graph_type='digraph')
floyd(weighed_graph)

# need to do code like this later "dot -Tsvg main.dot -o t3.svg"
key = graph.keys()

graph_nodes = [1, 5, 6, 7, 9]  # size is  always 5

nodes = []

for i in key:
    nodes.append(pydotplus.Node(i, style="filled", fillcolor="green"))

for i in graph_nodes:
    nodes.append(pydotplus.Node(i, style="filled", fillcolor="red"))

for i in nodes:
    graph_out.add_node(i)

for i in key:
    assert isinstance(graph, dict)
    for j in graph[i]:
        edge = pydotplus.Edge(nodes[i-1], nodes[j-1], label=str(weighed_graph[i-1][j-1]))
        graph_out.add_edge(edge)

# this creates our initial graph which i create later from graphviz
graph_out.write('main.dot', 'raw')

simulation_start(graph_nodes, 200)
avg_delay = sum(delay_time) / len(delay_time)

print(avg_delay)
