import math
import threading
import time


def fcrowd(v, l, d):
    fmin, fmax = 0.05, 1.0
    cmin, cmax = 10, 100
    c = float(v)/(float(l)*float(d))
    if c <= cmin:
        return fmax
    elif c < cmax:
        return fmax - (c - cmin) * (fmax - fmin) / (cmax - cmin)
    else:
        return fmin

def getSpeed(rsegment, kspeed, vehicle_number):
    return fcrowd(vehicle_number, rsegment.nlanes, rsegment.len) * kspeed


class Vehicle:
    def __init__(self, id_, current_rsegment, remaining_rsegment, completed_length=0, kspeed=60.0):
        self.id_ = id_
        self.completed_length = completed_length
        self.kspeed = kspeed
        self.current_rsegment = current_rsegment
        self.speed = getSpeed(current_rsegment, kspeed, current_rsegment.getFVehicles(self))
        self.remaining_rsegment = remaining_rsegment


class RSegment:
    def __init__(self, nlanes, edge):
        self.nlanes = nlanes
        self.edge = edge
        self.start_at = edge[0]
        self.end_at = edge[1]
        self.len = edge[2]['distance']
        self.vehicles = []
        self.capacity = math.ceil(150.0 * float(self.len) * float(self.nlanes))
        self.q = []
        self.mutex = threading.RLock()

    def insertVehicle(self, vehicle):
        with self.mutex:
            if len(self.q) < self.capacity:
                self.vehicles.append(vehicle)
                self.q.append(vehicle)

    def deleteVehicle(self, k):
        with self.mutex:
            del self.q[k]

    def getFVehicles(self, vehicle):
        k = 0
        with self.mutex:
            for i in range(0, len(self.q)):
                if self.q[i].completed_length > vehicle.completed_length:
                    k += 1
        return k

    def getInfo(self):
        return self.len, self.nlanes, self.start_at, self.end_at

    def getNVehicles(self):
        with self.mutex:
            return len(self.q)

    def getCapacity(self):
        return math.ceil(150.0 * float(self.len) * float(self.nlanes))

    def full(self):
        return len(self.q) == self.capacity

    def waitCapacity(self):
        pass

    def getStats(self):
        pass
