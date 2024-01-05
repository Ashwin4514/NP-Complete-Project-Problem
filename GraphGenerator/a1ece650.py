#!/usr/bin/env python3
import sys
import re
from itertools import islice
import math

# YOUR CODE GOES HERE

operations = {'add','mod','gg','rm'}
streetDict = dict()
Vertices = dict()
Edges = set()

class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '({0:.2f}, {1:.2f})'.format(self.x, self.y)
   
    def __str__(self):
        return repr(self)

    def __eq__(self, other):

        if isinstance(other, Point):
            return self.x == other.x and self.y == other.y
        return False
    
    def __hash__(self):
        return hash((self.x, self.y))

class Line(object):
    def __init__(self, p1, p2):
        self.src = p1
        self.dst = p2
    
    def __repr(self):
        return '({0}, {1})'.format(repr(self.src), repr(self.dist))
    
    def __iter__(self):
        yield self.src
        yield self.dst

def intersect(l1,l2):

    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xden = float((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    yden = xden

    if xden == 0:
        return None    #Parallel Lines.

    xnum = float(((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4)))
    ynum = float(((x1*y2-y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)))
    
    xcoor = float(xnum / xden)
    ycoor = float(ynum / yden)
    
    # The following condition is required to check whether the line segments are not collinear and its purely their intersection lying on both the line segments.
    # Here as you can see we are creating a bounding box using four points, two of each line segments and strictly restricting our point of intersection inside it.

    if (min(x1, x2) <= xcoor <= max(x1, x2) and  min(y1, y2) <= ycoor <= max(y1, y2) and 
            min(x3, x4) <= xcoor <= max(x3, x4) and min(y3, y4) <= ycoor <= max(y3, y4)):

            source1 = Point(x1,y1)
            destination1 = Point(x2,y2)
            source2 = Point(x3,y3)
            destination2 = Point(x4,y4)
            intersection = Point(xcoor, ycoor)

            
            if(intersection == source1 or intersection == source2 or intersection == destination1 or intersection == destination2):
               if((source1 != destination2 and source1 != source2) and (destination1 != source2 and destination1 != destination2)):
                return Point(xcoor, ycoor)
               else: 
                return None
            else:
                return Point(xcoor, ycoor)

    return None


def commandChecker(allText):
    """Parse the input command and check for errors."""

    # if any(item.isspace() for item in allText):
    #     raise Exception('Something is missing or there is an extra space in the command.')

    if allText[0] not in operations:
        raise Exception('Not a valid Operation')
    
    if allText[0] == 'gg' :
        return True
    
    if allText[0] == 'rm' and allText[1] != '' :
        return True

    if(allText[1] == ""):
        
        raise Exception('Please include the streetname.')

    if(len(allText) < 2 and (allText[0]=='add' or allText[0]=='mod')):
        
        raise Exception('Please include the coordinates.')

    if(len(allText) ==  3):
        
        raise Exception('Just a single coordinate?')

    for i in range(2,len(allText)):
        
        try:
            coordinate = allText[i].strip('(').strip(')').split(',')
            x, y = coordinate
            x = float(x)
            y = float(y)

        except ValueError as ve:
            raise Exception('Please check the type of values passed as coordinates.')
        
        except Exception as ex :
            print(f'{ex}')
            raise Exception('Please give correct type of coordinates.')
        
    
    return True


def cast_to_Point(coord):

    xcord, ycord = coord.strip('()').split(',')

    return Point(xcord, ycord)

def addormodOperation(LineAsList):

    Points = []
    
    try:
        for itr in range(2,len(LineAsList)):
            
            coordinate = cast_to_Point(LineAsList[itr])
            Points.append(coordinate)
            
        streetDict[LineAsList[1]] = Points

    except Exception as ex:
        raise Exception(f'Error: {ex}')

def rmOperation(LineAsList):

    try:
        del streetDict[LineAsList[1]]

    except Exception as ex :

        raise Exception('Error: Check Remove Function.')

def crossProductChecker(Li, Pointer):

    if Pointer == Li.src or Pointer == Li.dst:
        return False

    # Define a small positive number for epsilon
    epsilon = 1e-6

    # Calculate the cross product
    cross_product = (Pointer.y - Li.src.y) * (Li.dst.x - Li.src.x) - (Pointer.x - Li.src.x) * (Li.dst.y - Li.src.y)

    # Check if the point is collinear with the line segment
    if abs(cross_product) < epsilon:
        # Check if the point is within the bounding box (excluding endpoints)
        if (min(Li.src.x, Li.dst.x) <= Pointer.x <= max(Li.src.x, Li.dst.x)) and (min(Li.src.y, Li.dst.y) <= Pointer.y <= max(Li.src.y, Li.dst.y)):
            return True
    
    return False

def min_distance_calc(vertex,listOFVertices):
    min_distance = float('inf')
    reqPoint = None

    for i in listOFVertices:
        if(vertex!=i):
            dist = math.sqrt((vertex.x - i.x)**2 + (vertex.y - i.y)**2)
            if(dist < min_distance):
                min_distance = dist
                reqPoint = i

    return reqPoint

def create_graph(lines):

    vertices = dict()
    vert_counter = 1
    edges = set()
    line_intersections = set()
    tempEdges = set() 

    for i, line1 in enumerate(lines):
        for j, line2 in enumerate(lines):
            if i < j:

                intersectionPoint = intersect(line1,line2)
                
                #We proceed only when there is an intersection point.
                #if yes, the following steps will help you to connect all the points involved here with the intersection point as edges.
                if intersectionPoint:

                    line_intersections.add(intersectionPoint)

                    for point in [line1.src, line1.dst, line2.src, line2.dst]:
                        if point not in vertices.values():
                           vertices[vert_counter] = point
                           vert_counter += 1

                    if intersectionPoint not in vertices.values():
                        vertices[vert_counter] = intersectionPoint
                        vert_counter += 1

                    for point in line1: 

                        src_num, src_vert = next((n,v) for n,v in vertices.items() if v == point)         
                        dst_num, dst_vert = next((n,v) for n,v in vertices.items() if v == intersectionPoint)
                        l1 = Line(src_vert,dst_vert)
                        edges.add(tuple(sorted([src_num, dst_num])))

                    for point in line2:
                        src_num, src_vert = next((n,v) for n,v in vertices.items() if v == point)                 
                        dst_num, dst_vert = next((n,v) for n,v in vertices.items() if v == intersectionPoint)
                        l2 = Line(src_vert,dst_vert)
                        edges.add(tuple(sorted([src_num, dst_num])))

                    ## To Remove Duplicate/Overlapping lines, Take All edges, consider those as line and loop through your final edges.
                    ## If there is an issue, remove that edge.


    #Recalculating all the edges after collecting all the new intersections.
    ########Highly Important Step. 
    for eg in edges:
        startPoint = vertices[eg[0]]
        endPoint   = vertices[eg[1]]
        tempL = Line(startPoint,endPoint)
        pointsInQuestion = []
        pointsInQuestion.append(startPoint)
        pointsInQuestion.append(endPoint)

        for intersection in line_intersections:
            if(crossProductChecker(tempL,intersection)):
                pointsInQuestion.append(intersection)

        ##Making Final Edges
        for mainVert in pointsInQuestion:
            mainIdx, _ = next((n,v) for n,v in vertices.items() if v==mainVert)
            connectingVert = min_distance_calc(mainVert,pointsInQuestion)
            connectingIdx, _ = next((n,v) for n,v in vertices.items() if v == connectingVert)
            tempEdges.add(tuple(sorted([mainIdx,connectingIdx])))
        
    return vertices, tempEdges

def console_and_file(otp):
       print(otp)

def printOutput(vertices, edges):
    
   
    console_and_file("V = {")
    for key, value in vertices.items():
        if(key < 10):
            console_and_file("  {0}:  ({1},{2})".format(key,round(value.x,2),round(value.y,2)))
        else:
            console_and_file("  {0}: ({1},{2})".format(key,round(value.x,2),round(value.y,2)))

    console_and_file("}")    

    console_and_file("E = {")

    if(edges):
        n = len(edges)
        Edges = list(edges)
        for itr in Edges[:n-1]:
            it = list(itr)
            console_and_file(f"  <{it[0]},{it[1]}>,")
          
        lastElement = list(Edges[len(Edges)-1])
        console_and_file(f"  <{lastElement[0]},{lastElement[1]}>")
    
    console_and_file("}")


def main():
    # YOUR MAIN CODE GOES HERE
    # Take input as a series of commands that describe streets.
    # add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
    # Break the input into three strings, Function - add, mod, rm, gg
    # Only the four functions allowed - Write a Handler for it
    # No special characters in the coordinates. Write a Handler for it
    # Always take some street with the command input. Write a Handler for it.
    # Let it slide in case of 'gg'
    #command = 'add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)'
    
    while(True):
        
        line = sys.stdin.readline()

        if line:
            line = ' '.join(line.split())
            print("read a line: ", line)
                    
            allText = re.findall(r'".+?"|\S+|  +', line)
                    
            line = ""
                    
            try:
                        
                if (commandChecker(allText)):
                    if(allText[0] == 'add'):
                        if(allText[1] in streetDict):
                            raise Exception("Street Already Exists!")
                        addormodOperation(allText)
                        
                    elif(allText[0] == 'mod'):
                        if(allText[1] not in streetDict):
                             raise Exception("Street does not exist for modification!")
                        addormodOperation(allText)    

                    elif(allText[0] == 'rm'):
                        if(allText[1] not in streetDict):
                             raise Exception("Street not yet added!")
                        rmOperation(allText)

                    elif(allText[0] == 'gg'):
                        AllLines = []
                            #    for key, value in streetDict.items():
                            
                            #        print(f"{key} -> {value}")
                        
                        for key, value in streetDict.items():
                             aux = []
                            
                             for itr in range(len(value)):
                                        if(len(aux) == 0):
                                          aux.append(value[itr])
                                        
                                        elif(len(aux) == 1):
                                           L = Line(aux[0],value[itr])
                                           del aux[0]
                                           aux.append(value[itr])
                                           if L not in AllLines:
                                              AllLines.append(L)

                        vertices,edges = create_graph(AllLines)
                        printOutput(vertices,edges)
                    
            except Exception as ve :
                print(f"Error: {ve}")
                with open('error_log.txt', 'a') as file:
                    file.write(str(ve) + '\n')

        else:
            break 

        print("Finished Reading the input.")

    # return exit code 0 on successful termination
    sys.exit(0)

if __name__ == "__main__":
    
    main()
