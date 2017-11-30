from flask import Flask, render_template, request, flash
from collections import defaultdict

app = Flask(__name__)
app.secret_key = 'some_secret'
app.debug = False
start = 0
end = 0
directions = ""


# Class to represent a graph
turns = [[-1, -1, 1, 14], [0,-1,2,14], [1,-1,3,7], [2,-1,4,6], [3,-1,-1,5],[6,4,-1,-1],[-1,3,5,9],[-1,2,8,-1],[7,-1,9,13],[8,6,-1,10],[11,9,-1,-1],[12,-1,10,-1],[-1,13,11,-1],[14,-1,8,12],[0,1,13,-1]]
turnDirections = ["U","L","F","R"]
history = []

class Graph:
    retVal = ""
    # A utility function to find the vertex with minimum dist value, from
    # the set of vertices still in queue
    def minDistance(self, dist, queue):
        # Initialize min value and min_index as -1
        minimum = float("Inf")
        min_index = -1
        # from the dist array,pick one which has min value and is till in queue
        for i in range(len(dist)):
            if dist[i] < minimum and i in queue:
                minimum = dist[i]
                min_index = i
        return min_index

    # Function to print shortest path from source to j
    # using parent array

    def printPath(self, parent, j):
        if parent[j] == -1:  # Base Case : If j is source
            self.retVal += str(j) + " "
            return
        self.printPath(parent, parent[j])
        self.retVal += str(j) + " "

    # A utility function to print the constructed distance
    # array
    # input the end goal
    def printSolution(self, dist, parent, end):
        self.printPath(parent, end)
        return self.retVal

    '''Function that implements Dijkstra's single source shortest path
	algorithm for a graph represented using adjacency matrix
	representation'''

    def dijkstra(self, graph, src, end):

        row = len(graph)
        col = len(graph[0])

        # The output array. dist[i] will hold the shortest distance from src to i
        # Initialize all distances as INFINITE
        dist = [float("Inf")] * row

        # Parent array to store shortest path tree
        parent = [-1] * row

        # Distance of source vertex from itself is always 0
        dist[src] = 0

        # Add all vertices in queue
        queue = []
        for i in range(row):
            queue.append(i)

        # Find shortest path for all vertices
        while queue:

            # Pick the minimum dist vertex from the set of vertices
            # still in queue
            u = self.minDistance(dist, queue)

            # remove min element
            queue.remove(u)

            # Update dist value and parent index of the adjacent vertices of
            # the picked vertex. Consider only those vertices which are still in
            # queue
            for i in range(col):
                '''Update dist[i] only if it is in queue, there is
                an edge from u to i, and total weight of path from
                src to i through u is smaller than current value of
                dist[i]'''
                if graph[u][i] and i in queue:
                    if dist[u] + graph[u][i] < dist[i]:
                        dist[i] = dist[u] + graph[u][i]
                        parent[i] = u

        # print the constructed distance array
        return self.printSolution(dist, parent, end)


graph = [[0, 135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 123],
         [135, 0, 106, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156],
         [0, 106, 0, 88, 0, 0, 0, 113, 0, 0, 0, 0, 0, 0, 0],
         [0, 0, 88, 0, 59, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0],
         [0, 0, 0, 59, 0, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 68, 0, 68, 0, 0, 0, 0, 0, 0, 0, 0],
         [0, 0, 0, 64, 0, 68, 0, 0, 0, 118, 0, 0, 0, 0, 0],
         [0, 0, 113, 0, 0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 0, 0, 59, 0, 1200, 0, 0, 0, 97, 0],
         [0, 0, 0, 0, 0, 0, 118, 0, 1200, 0, 72, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 105, 0, 0, 0],
         [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 0, 110, 0, 0],
         [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110, 0, 105, 0],
         [0, 0, 0, 0, 0, 0, 0, 0, 97, 0, 0, 0, 105, 0, 102],
         [123, 156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 0]
         ]

def getTurn(prev,at,to):
    atArray = turns[int(at)]
    prevIndex = 0
    toIndex = 0
    for i in range(0,len(atArray)):
        if atArray[i] == int(prev):
            prevIndex = i
        if atArray[i] == int(to):
            toIndex = i

    val = (toIndex - prevIndex) % 4
    return turnDirections[val]



@app.route('/', methods=['GET', 'POST'])
def index():
    global start
    global end
    global directions
    global history
    if request.method == 'POST':
        g = Graph()
        start = int(request.form['start'])
        end = int(request.form['end'])
        print request.form
        if 'reset' in request.form:
            history = []

        nodes = g.dijkstra(graph, int(start), int(end)).split(" ")

        directions = ""
        if not history:
            directions += "F"
        else:
            directions += getTurn(history[len(history)-1], int(nodes[0]), int(nodes[1]))

        history.extend(nodes[:len(nodes)-2])
        print history

        for i in range (1,len(nodes)-2):
            directions += getTurn(nodes[i-1], nodes[i], nodes[i+1])
        directions +="S"
        flash("Starting at node " + nodes[0] + " and face towards node " + nodes[1] + " Directions:" + directions, 'success')
        return render_template('index.html')
    else:
        return render_template('index.html')

@app.route('/id')
def route():
    global start
    global end
    global directions
    id = request.args.get("id")
    return str(id) +":"+directions

if __name__ == '__main__':
    app.run()
