import numpy as np
# import lmdb
# import caffe
import h5py

DATASET_SIZE = 10000
TOPO_ROW_NUM = 80
DEMAND_ROW_NUM = 32
TOPO_PATH = "./Sample_Data/topo/topo"
DEMAND_PATH = "./Sample_Data/demand/demand"
OUTPUT_FILE_NAME = "smart_net_train.hdf5"

# write data from multi-dimentional array into HDF5 file
def toHDF5(data_topo, data_demand, label):
	data_topo = data_topo.reshape(DATASET_SIZE, 1, TOPO_ROW_NUM, TOPO_ROW_NUM)
	data_demand = data_demand.reshape(DATASET_SIZE, 1, DEMAND_ROW_NUM, DEMAND_ROW_NUM)
	label = label.reshape(DATASET_SIZE, 1, TOPO_ROW_NUM, TOPO_ROW_NUM)

	with h5py.File(OUTPUT_FILE_NAME, "w") as f:
		f["data_topo"] = data_topo.astype(np.float32)
		f["data_demand"] = data_demand.astype(np.float32)
		f["label"] = label.astype(np.float32)

# main function
def main():
	data_topo = []
	data_demand = []
	label = []

	for i in range(0, DATASET_SIZE):
		topo = []
		demand = []
		label_topo = []

		# read topo
		topo_file = open(TOPO_PATH + str(i - 1) + ".txt")	# input topo use (i - 1)th
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
		label_topo_file = open(TOPO_PATH + str(i) + ".txt")
		for line in range(0, TOPO_ROW_NUM):
			line_data = (label_topo_file.readline().split(" "))[0:TOPO_ROW_NUM]
			for item in range(0, TOPO_ROW_NUM):
				line_data[item] = int(line_data[item])
			label_topo.append(line_data)
		label_topo_file.close()

		# re-organize data format
		data_topo.append(topo)
		data_demand.append(demand)
		label.append(label_topo)
		print "processing " + str(i)
	data_topo = np.array(data_topo)
	data_demand = np.array(data_demand)
	label = np.array(label)
	toHDF5(data_topo, data_demand, label)

if __name__ == "__main__":
	main()
