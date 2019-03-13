import math
import sqlite3
import sys
import json
# import pickle
# import _pickle as cPickle


def find_junc_by_id(s_map, id):
    keys = s_map.graph.keys()
    for key in keys:
        if id == key.id:
            return key


class Junction:
    def __init__(self, id1, x, y):
        self.id = id1
        self.x = x
        self.y = y


class Map:
    conn = sqlite3.connect('maps.db')
    c = conn.cursor()
    # c.execute("""CREATE TABLE maps(name text, map text)""")
    conn.commit()
    conn.close()

    def __init__(self):
        self.graph = {}
        self.matrix = [[]]

    def __str__(self):
        for node in self.graph:
            print("Node id:", node.id, "at (x:",node.x, ",y:",node.y, ")")
            if len(self.graph[node]) > 0:
                for tupl in self.graph[node]:
                    print("\t to node id:", tupl[1].id, "dict:", tupl[2])
        return " "

    def addNode(self, id0, x, y):
        this_junction = Junction(id0, x, y)
        self.graph[this_junction] = []

    def deleteNode(self, id0):
        junc = find_junc_by_id(self, id0)
        keys = self.graph.keys()
        for key in keys:
            self.deleteRoad(key.id, id0, bidir=True)
        del self.graph[junc]

    def addRoad(self, id1, id2, nlines=1, bidir=True):
        junc1 = find_junc_by_id(self, id1)
        junc2 = find_junc_by_id(self, id2)
        x1, y1, x2, y2 = junc1.x, junc1.y, junc2.x, junc2.y
        distance = math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)

        if bidir == False:
            self.graph[junc1].append((junc1, junc2, {'nlines': nlines, 'distance': distance}))
        elif bidir == True:
            self.graph[junc1].append((junc1, junc2, {'nlines': nlines, 'distance': distance}))
            self.graph[junc2].append((junc2, junc1, {'nlines': nlines, 'distance': distance}))

    def deleteRoad(self, id1, id2, bidir=True):
        junc1 = find_junc_by_id(self, id1)
        junc2 = find_junc_by_id(self, id2)

        if bidir == False:
            for tupl in self.graph[junc1]:
                if tupl[1] == junc2:
                    self.graph[junc1].remove(tupl)

        if bidir == True:
            for tupl in self.graph[junc1]:
                if tupl[1] == junc2:
                    self.graph[junc1].remove(tupl)
            for tupl in self.graph[junc2]:
                if tupl[1] == junc1:
                    self.graph[junc2].remove(tupl)

    def findPathLength(self, id1, id2):
        junc1 = find_junc_by_id(self, id1)
        junc2 = find_junc_by_id(self, id2)

        for tupl in self.graph[junc1]:
            if tupl[1] == junc2:
                return tupl[2]['distance']

        return -1

    def getShortestPath(self, id1, id2):
        index_obj_dict = {}
        path = {}
        index1 = 0
        index2 = 0
        i = 0
        for junc in self.graph:
            index_obj_dict[i] = junc
            i += 1
        dict_len = len(index_obj_dict)
        matrix = [[None for x in range(dict_len)] for y in range(dict_len)]
        list_matrix = [[None for x in range(dict_len)] for y in range(dict_len)]
        for j in range(0, dict_len):
            junc1 = index_obj_dict.get(j)
            for k in range(0, dict_len):
                junc2 = index_obj_dict.get(k)
                dist = self.findPathLength(junc1.id, junc2.id)
                if j == k:
                    matrix[j][k] = 0
                    list_matrix[j][k] = [index_obj_dict[j].id]
                elif dist > 0:
                    matrix[j][k] = dist
                    list_matrix[j][k] = [index_obj_dict[j].id]
                else:
                    matrix[j][k] = sys.maxsize
                    list_matrix[j][k] = [index_obj_dict[j].id]
        junc8 = find_junc_by_id(self, id1)
        junc9 = find_junc_by_id(self, id2)
        for m in range(0, dict_len):
            if junc8==index_obj_dict[m]:
                index1 = m
            if junc9==index_obj_dict[m]:
                index2 = m

        for s in range(0, dict_len):
                for j in range(0, dict_len):
                    junc1 = index_obj_dict.get(j)
                    for k in range(0, dict_len):
                        junc2 = index_obj_dict.get(k)
                        if matrix[j][s]+matrix[s][k] < matrix[j][k]:
                            matrix[j][k] = matrix[j][s]+matrix[s][k]
                            list_matrix[j][k] = list_matrix[j][s] + list_matrix[s][k]
        list_matrix[index1][index2].append(id2)
        if matrix[index1][index2] == 0 or matrix[index1][index2] == sys.maxsize:
            return []
        else:
            return list_matrix[index1][index2]

    # def saveMap(self, name):
    #     pdata = cPickle.dumps(self, cPickle.HIGHEST_PROTOCOL) #, cPickle.HIGHEST_PROTOCOL)
    #     conn = sqlite3.connect('maps.db')
    #     c = conn.cursor()
    #     c.execute("insert into maps (name, map) values ('" + name + "', :map)", (sqlite3.Binary(pdata),))
    #
    #     conn.commit()
    #     conn.close()

    def saveMap(self, name):
        dict = {}
        dict['junc_infos'] = []
        dict['road_infos'] = []
        for junc in self.graph:
            dict['junc_infos'].append((junc.id, junc.x, junc.y))
        for junc in self.graph:
            for road in self.graph[junc]:
                dict['road_infos'].append((road[0].id, road[1].id, road[2].get('nlines')))
        js = json.dumps(dict)
        conn = sqlite3.connect('maps.db')
        c = conn.cursor()
        c.execute("insert into maps (name, map) values ('" + name + "', '" + js + "')")
        conn.commit()
        conn.close()

    def loadMap(self, name):
        conn = sqlite3.connect('maps.db')
        c = conn.cursor()
        c.execute("select map from maps where name = '" + name + "' limit 1")
        data = c.fetchall()
        data = json.loads(data[0][0])
        self.graph = {}
        for junc_info in data['junc_infos']:
            self.addNode(junc_info[0], junc_info[1], junc_info[2])
        for road_info in data['road_infos']:
            self.addRoad(road_info[0], road_info[1], road_info[2], bidir=False)

    def deleteMap(self, name):
        conn = sqlite3.connect('maps.db')
        c = conn.cursor()
        # c.execute("delete from maps where name = '" + name + "'")
        c.execute("delete from maps")
        conn.commit()
        conn.close()

    # def loadMap(self, name):
    #     conn = sqlite3.connect('maps.db')
    #     c = conn.cursor()
    #     c.execute("select map from maps where name = '" + name + "' limit 1")
    #     data = None
    #     for row in c:
    #         data = row
    #     d = cPickle.loads(str(data[0]))
    #     conn.commit()
    #     conn.close()
    #     return d

    def printMapFromDatabase(self, name):
        conn = sqlite3.connect('maps.db')
        c = conn.cursor()
        
        # c.execute("select * from maps where name = '" + name + "'")
        c.execute("select * from maps")

        print(c.fetchall())

        conn.commit()
        conn.close()