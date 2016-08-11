import numpy as np
# import lmdb
# import caffe
import h5py
import sys

DATASET_SIZE = 1000
TOPO_ROW_NUM = 80
DEMAND_ROW_NUM = 32
TOPO_PATH = "./Sample_Data/topo/topo"
DEMAND_PATH = "./Sample_Data/demand/demand"
FULL_TOPO_PATH = "output_full_topo.txt"
OUTPUT_FILE_NAME = "smart_net_train.hdf5"

# write data from multi-dimentional array into HDF5 file
def toHDF5(data_topo, data_demand, label, label_size):
	data_topo = data_topo.reshape(DATASET_SIZE, 1, TOPO_ROW_NUM, TOPO_ROW_NUM)
	data_demand = data_demand.reshape(DATASET_SIZE, 1, DEMAND_ROW_NUM, DEMAND_ROW_NUM)
	label = label.reshape(DATASET_SIZE, label_size)

	with h5py.File(OUTPUT_FILE_NAME, "w") as f:
		f["data_topo"] = data_topo.astype(np.float32)
		f["data_demand"] = data_demand.astype(np.float32)
		f["label"] = label.astype(np.float32)

# main function
def main(argv):
	# Parse argvs
	if (len(argv) < 3):
		print "Argument(s) missed!"
		print "Usage: python HDF5Generator.py layer_no time_involved"
		return 1
	layer_no = int(argv[1])
	time_involved = int(argv[2])

	# read full topo
	full_topo = [[0 for col in range(0, TOPO_ROW_NUM)] for row in range(0, TOPO_ROW_NUM)]
	full_topo_file = open(FULL_TOPO_PATH)
	for line in range(0, TOPO_ROW_NUM):
		line_data = (full_topo_file.readline().split(" "))[0:TOPO_ROW_NUM]
		for item in range(0, TOPO_ROW_NUM):
			full_topo[line][item] = int(line_data[item])
	full_topo_file.close()

	data_topo = []
	data_demand = []
	label = []

	for i in range(0, DATASET_SIZE):
		topo = []
		demand = []
		label_topo = []

		# read topo
		topo_file = open(TOPO_PATH + str((i if time_involved == 0 else i-1)) + ".txt")	# input topo use (i - 1)th
		for line in range(0, TOPO_ROW_NUM):
			line_data = (topo_file.readline().split(' '))[0:TOPO_ROW_NUM]
			for item in range(0, TOPO_ROW_NUM):
				line_data[item] = int(line_data[item])
			topo.append(line_data)
		topo_file.close()

		# read demand
		demand_file = open(DEMAND_PATH + str(i) + ".txt")
		for line in range(0, DEMAND_ROW_NUM):
			line_data = (demand_file.readline().split(" "))[0:DEMAND_ROW_NUM]
			for item in range(0, DEMAND_ROW_NUM):
				line_data[item] = int(line_data[item])
			demand.append(line_data)
		demand_file.close()

		# read label(i_th topo)
		label_size = 0
		label_topo_file = open(TOPO_PATH + str(i) + ".txt")
		for line in range(0, TOPO_ROW_NUM):
			line_data = (label_topo_file.readline().split(" "))[0:TOPO_ROW_NUM]
			for item in range(0, TOPO_ROW_NUM):
				if (item > line and full_topo[line][item] == layer_no):		# upper triangle
					label_topo.append(int(line_data[item]))
					label_size += 1
		label_topo_file.close()

		# re-organize data format
		data_topo.append(topo)
		data_demand.append(demand)
		label.append(label_topo)
		if(i % 50 == 0):
			print "processing " + str(i)
	data_topo = np.array(data_topo)
	data_demand = np.array(data_demand)
	label = np.array(label)
	toHDF5(data_topo, data_demand, label, label_size)

if __name__ == "__main__":
	main(sys.argv)
