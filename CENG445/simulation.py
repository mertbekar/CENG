from Map import *
import random
import time
import threading
from RSegment import *
import os

tack = threading.Thread()
id_ = 0

terminator = 0

class Generator:
    def __init__(self, source_list, target_list, period, number, simulation, _map):
        self.tick_number = 0
        self.number = number
        self.source_list = source_list
        self.target_list = target_list
        self.period = period
        self.number = number
        self.simulation = simulation
        self._map = _map

    def run(self):
        self.tick_number += 1
        if self.tick_number % self.period == 0:
            source_id = random.choice(self.source_list)
            target_id = random.choice(self.target_list)
            source = find_junc_by_id(self._map, source_id)
            target = find_junc_by_id(self._map, target_id)
            shortest_path = self._map.getShortestPath(source_id, target_id)
            if shortest_path != []:
                for i in self.simulation.rsegments:
                    _id1 = i.start_at.id
                    _id2 = i.end_at.id
                    if _id1 == shortest_path[0] and _id2 == shortest_path[1] and self.number>0:
                        global id_
                        vehicle = Vehicle(id_, i, shortest_path[1:], 0)
                        id_ += 1
                        threading.Thread(target=i.insertVehicle, args=[vehicle]).start()
                        self.number -= 1


class Simulation:
    def __init__(self):
        self.map = None
        self.generators = []
        self.rsegments = []
        self.tickperiod = 0
        self.thread_list = []
        self.completed_vehicle = 0
        self.terminator = 0

    def setMap(self, simulation_map):
        self.map = simulation_map

    def addGenerator(self, source_list, target_list, period, number):
        this_generator = Generator(source_list, target_list, period, number, self, self.map)
        self.generators.append(this_generator)

    def startSimulation(self, tickperiod=0):
        global id_
        id_ = 0
        self.tickperiod = float(tickperiod)/1000
        for node in self.map.graph:
            for road in self.map.graph[node]:
                self.rsegments.append(RSegment(road[2]['nlines'], road))
        global tack
        tack = threading.Thread(target=self.tick)
        tack.start()

    def tick(self):
        while self.terminator == 0:
            for j in range(len(self.rsegments)):
                _len = len(self.rsegments[j].q)
                k = 0
                while k < _len:
                    self.rsegments[j].q[k].completed_length += (self.tickperiod * self.rsegments[j].q[k].speed)
                    vehicle_number = self.rsegments[j].getFVehicles(self.rsegments[j].q[k])
                    self.rsegments[j].q[k].speed = getSpeed(self.rsegments[j], self.rsegments[j].q[k].kspeed, vehicle_number)
                    if self.rsegments[j].q[k].completed_length >= self.rsegments[j].len:
                        if len(self.rsegments[j].q[k].remaining_rsegment) <= 1:
                            temp_vehicle = Vehicle(self.rsegments[j].q[k].id_,
                                                   self.rsegments[j].q[k].current_rsegment,
                                                   self.rsegments[j].q[k].remaining_rsegment, 0,
                                                   self.rsegments[j].q[k].kspeed)
                            self.completed_vehicle += 1

                        if len(self.rsegments[j].q[k].remaining_rsegment) > 1:
                            temp_vehicle = Vehicle(self.rsegments[j].q[k].id_, self.find_Rsegment(self.rsegments[j].q[k].remaining_rsegment[0], self.rsegments[j].q[k].remaining_rsegment[1]), self.rsegments[j].q[k].remaining_rsegment, 0, self.rsegments[j].q[k].kspeed)
                        threading.Thread(target=self.rsegments[j].deleteVehicle, args=[k]).start()
                        _len -= 1
                        if len(temp_vehicle.remaining_rsegment) > 1:
                            temp_rsegment = self.find_Rsegment(temp_vehicle.remaining_rsegment[0], temp_vehicle.remaining_rsegment[1])
                            temp_vehicle.remaining_rsegment = temp_vehicle.remaining_rsegment[1:]
                            threading.Thread(target=temp_rsegment.insertVehicle, args=[temp_vehicle]).start()
                    else:
                        k += 1
            time.sleep(self.tickperiod)
            for i in range(0, len(self.generators)):
                if self.generators[i].number > 0:
                    self.generators[i].run()
            if self.tickperiod == 0:
                break;
    def find_Rsegment(self, departure, arrival):
        for i in range(0, len(self.rsegments)):
            if self.rsegments[i].start_at.id == departure and self.rsegments[i].end_at.id == arrival:
                return self.rsegments[i]

    def getGenerators(self):
        return self.generators

    def delGenerator(self, num):
        del self.getGenerators()[num]

    def terminate(self):
        global tack
        self.terminator = 1
        tack.join()

    def print_segments(self):
        for m in range(0, len(self.rsegments)):
            with self.rsegments[m].mutex:
                for n in range(0, len(self.rsegments[m].q)):
                    print('segment:', self.rsegments[m].start_at.id, "to", self.rsegments[
                            m].end_at.id, ', vehicle_id:', self.rsegments[m].q[n].id_, ', speed:', self.rsegments[m].q[
                            n].speed, 'completed:', self.rsegments[m].q[n].completed_length, "length of segment= ", len(self.rsegments[m].q))

    def getStats(self):
        print("Number of vehicle completed: ", self.completed_vehicle)
        k = 0
        total_speed = 0
        for m in range(0, len(self.rsegments)):
            for n in range(0, len(self.rsegments[m].q)):
                total_speed += self.rsegments[m].q[n].speed
                k += 1
        print("Number of vehicle currently on map: ", k)
        print("Total number of vehicle: ", k + self.completed_vehicle)
        if k > 0:
            print("Average speed: ", total_speed / k)
        else:
            print("Average speed: ", 0)

    def wait(self):
        while True:
            k = 0
            c = 0;
            for m in range(0, len(self.rsegments)):
                for n in range(0, len(self.rsegments[m].q)):
                    k += 1
            for i in range(0, len(self.generators)):
                c += self.generators[i].number
            if self.tickperiod == 0:
                if c+k==0:
                    return True
                else:
                    return False
            if c+k==0:
                return True
            #else:
             #   self.print_segments()
              #  time.sleep(self.tickperiod)
