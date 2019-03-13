from simulation import *

s_map = Map()
s_map.addNode(0, 0, 0)
s_map.addNode(1, 30, 0)
s_map.addNode(2, 60, 0)
s_map.addNode(3, 90, 0)
s_map.addNode(4, 0, 40)
s_map.addNode(5, 30, 40)
s_map.addNode(6, 60, 40)
s_map.addNode(7, 90, 40)
s_map.addNode(8, 0, 80)
s_map.addNode(9, 30, 80)
s_map.addNode(10, 60, 80)
s_map.addNode(11, 90, 80)
s_map.addNode(12, 0, 120)
s_map.addNode(13, 30, 120)
s_map.addNode(14, 60, 120)
s_map.addNode(15, 90, 120)
s_map.addNode(16, 90, 180)

s_map.addRoad(9, 10)
s_map.addRoad(8, 12)
s_map.addRoad(12, 13)
s_map.addRoad(13, 14)
s_map.addRoad(14, 10)
s_map.addRoad(10, 11)
s_map.addRoad(11, 7)
s_map.addRoad(15, 16, False)
s_map.addRoad(14, 16, False)

s_map.deleteRoad(14, 16, False)

s_map.deleteNode(16)

s_map.saveMap("test_map_1")

e_map = Map()
e_map.loadMap("test_map_1")

print(s_map)
print(e_map)

s = Simulation()
s.setMap(s_map)



s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 3, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 11, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 8, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 2, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 7, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 15, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 3, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 6, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 8, 10)
s.addGenerator([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], 5, 10)

s.startSimulation(100)
s.delGenerator(3)


#s.print_segments()
