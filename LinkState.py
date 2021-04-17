import sys
import math
import copy 

DIST_LIMIT = 99999999

# def findMin(u=0,v=0,u2v=0,v2u=0):
#     return min(v2u, u2v + )
#     print("Inside findMind")

# def initNodes(s=0,g=[]):
#     for i in range(len(g)):
#         if((i+1) == s):
#             g[i] = 0
#         else:
#             g[i] = -1

# class Queue:

#     def __init__(self,vals=[]):
#         self.vals = vals
    
#     def enqueue(self,val = 0):
#         self.vals.append(val)
    
#     def dequeue(self):
#         return self.vals.pop(0)

# class Node:
    
#     def __init__(self,dist=1,prev=1,vis='n'):
#         self.dist = dist
#         self.prev = prev
#         self.vis = vis

# class Graph:

#     def __init__(self,dimensions=0):
#         self.vals = Node*[dimensions]


def Djikstras(source = 0,graph = [[]],dim=0):
    visited = []
    notVis = copy.deepcopy(dim)
    currMinInd = -1
    
    tempMin = []
    tempAdj = []
    
    distance = [] 
    previous = [source] * dim 
    solution = []

    tempMin = copy.deepcopy(graph[source-1])
    distance = copy.deepcopy(tempMin)

    print(distance)

    while (notVis > 0):
        currMin = DIST_LIMIT
        for j in range(len(tempMin)): 
            if (int(tempMin[j]) < int(currMin) and int(tempMin[j]) > 0):
                # print(tempMin[j],"!=",currMin) 
                currMin = tempMin[j]
                currMinInd = j

        
        #After finding the minimum node from the graph
        if(currMinInd < len(tempMin)):
            if(int(tempMin[currMinInd]) < DIST_LIMIT):
                # solution.append(currMinInd)
                notVis -= 1
                tempMin[currMinInd] = DIST_LIMIT
                tempAdj = copy.deepcopy(graph[currMinInd])
                # print("tempAdj: ", tempAdj)
                # print("len(tempAdj): ", len(tempAdj))

                for i in range(len(tempAdj)):
                    if(int(tempAdj[i]) > 0):
                        if(int(graph[source-1][currMinInd]) > 0 and int(graph[currMinInd][i]) > 0):
                            path = int(graph[source-1][currMinInd]) + int(graph[currMinInd][i])
                            # print(graph[source-1][i]," > ",path)
                            # print("graph[source-1][i]:", graph[source-1][i])
                            # print("graph[i][currMinInd]",graph[i][currMinInd])

                            if (path < int(graph[source-1][i])):
                                # if(distance[i] > path):
                                distance[i] = int(path)
                                print(dim/2)
                                if (currMinInd+1 < dim / 2):
                                    previous[i] = str(currMinInd+1)
                                else:
                                    previous[i] = str(currMinInd)
                                
                                print("dist: ",distance[i])
                                print("prev: ", previous[i])

                                solution.append(previous)
            else:
                notVis -= 1
    return distance, previous, solution

def main():
    fileData = sys.stdin.read()
    initArr = fileData.split("\n")

    graphRepresentation = []
    gateways = []

    j = 0
    orf = int(initArr[0])

    for i in initArr[1:]:
        
        if(len(i.split(" ")) == orf):
            graphRepresentation.append(i.split(" "))
        
        if((0 < len(i.split(" ")) < orf)):
            for k in i.split(" "):
                if(k != ''):
                    gateways.append(k)
        
    dist,prev,sol = Djikstras(1,graphRepresentation,orf)
    # print("cost: ",dist)
    # print("next hop: ",prev)
    # print("sol: ", sol)
main()