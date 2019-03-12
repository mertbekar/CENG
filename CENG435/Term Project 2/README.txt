Dogancan Kartal 2036028
Mert Bekar 2035749


First arrange route tables:

Router 1 (R1):
	sudo route add -net 10.10.3.2 netmask 255.255.255.255 gw 10.10.3.2
	sudo route add -net 10.10.2.1 netmask 255.255.255.255 gw 10.10.2.1
Router2 (R2):
	sudo route add -net 10.10.5.2 netmask 255.255.255.255 gw 10.10.5.2
	sudo route add -net 10.10.4.1 netmask 255.255.255.255 gw 10.10.4.1

######################### HOW TO RUN: ############################################

We have 3 source codes:
	s.py : Source Node Implementation
	b.py : Broker Node Implementation
	d.py : Destination Node Implementation

We also have 2 helper script:
	vs_graph.py : Plots the Graphs
	experiments.py : Stores Results of Our Experiments

Put the d.py to Destination node, b.py to Broker Node, s.py to Source Node and run them respectively.
Run the following commands after link configurations and routing table commands respectively:
	"python d.py" on Destination Node
	"python b.py" on Broker Node
	"python s.py" on Source Node

Link configurations are shown below.

############## LINK CONFIGURATIONS FOR EACH EXPERIMENT ####################################


Experiment 1 link configurations:

	Packet Loss(%0.5):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0.5% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
				sudo tc qdisc add dev eth3 root netem loss 0.5% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0.5% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0.5% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%

	Packet Loss(%10):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 10% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
				sudo tc qdisc add dev eth3 root netem loss 10% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 10% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 10% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%

	Packet Loss(%20):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 20% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
				sudo tc qdisc add dev eth3 root netem loss 20% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 20% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 20% corrupt 0% duplicate 0% delay 3 ms reorder 0% 0%


Experiment 2 link configurations:

	Corruption Percentage(%0.2):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0.2% duplicate 0% delay 3 ms reorder 0% 0%
				sudo tc qdisc add dev eth3 root netem loss 0% corrupt 0.2% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0.2% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0% corrupt 0.2% duplicate 0% delay 3 ms reorder 0% 0%

	Corruption Percentage(%10):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 10% duplicate 0% delay 3 ms reorder 0% 0%
				sudo tc qdisc add dev eth3 root netem loss 0% corrupt 10% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 10% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0% corrupt 10% duplicate 0% delay 3 ms reorder 0% 0%

	Corruption Percentage(%20):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 20% duplicate 0% delay 3 ms reorder 0% 0%
				sudo tc qdisc add dev eth3 root netem loss 0% corrupt 20% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 20% duplicate 0% delay 3 ms reorder 0% 0%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0% corrupt 20% duplicate 0% delay 3 ms reorder 0% 0%


Experiment 3 link configurations:

	Reordering Percentage(%1):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 1% 50%
				sudo tc qdisc add dev eth3 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 1% 50%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 1% 50%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 1% 50%

	Reordering Pertentage(%10):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 10% 50%
				sudo tc qdisc add dev eth3 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 10% 50%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 10% 50%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 10% 50%

	Reordering Percentage(%35):
		On Broker Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 35% 50%
				sudo tc qdisc add dev eth3 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 35% 50%
		On Router 1 (R1) Node:
				sudo tc qdisc add dev eth1 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 35% 50%
		On Router 2 (R2) Node:
				sudo tc qdisc add dev eth2 root netem loss 0% corrupt 0% duplicate 0% delay 3 ms reorder 35% 50%