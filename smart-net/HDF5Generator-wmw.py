import numpy as np
# import lmdb
# import caffe
import h5py
import sys
import os

#DATASET_SIZE = 1100
TRAINING_DATA_START = 0
TRAINING_DATA_END = 19000	# 0 - 39999
TEST_DATA_START = 19000
TEST_DATA_END = 20000 #DATASET_SIZE	# 40000 - 49999

TOPO_ROW_NUM = 8
DEMAND_ROW_NUM = 8

TOPO_PATH = "../../Sample_Data/wireless/topo_sample/topo"
DEMAND_PATH = "../../Sample_Data/wireless/demand/demand"
FULL_TOPO_PATH = "./Full_Topo_Preprocessor/output_full_topo4.txt"

OUTPUT_FILE_NAME = "/home/fanquan/Desktop/source_hdf5_wireless/smart_net4_train_wmw_19000.hdf5"
OUTPUT_TEST_FILE_NAME = "/home/fanquan/Desktop/source_hdf5_wireless/smart_net4_test_wmw_19000.hdf5"

# write data from multi-dimentional array into HDF5 file
def toHDF5(data_topo, data_demand, label, label_size, isTrain):
	data_size = 0
	if(isTrain):
		data_size = TRAINING_DATA_END - TRAINING_DATA_START
	else:
		data_size = TEST_DATA_END - TEST_DATA_START
	data_topo = data_topo.reshape(data_size, 1, TOPO_ROW_NUM, TOPO_ROW_NUM)
	data_demand = data_demand.reshape(data_size, 1, DEMAND_ROW_NUM, DEMAND_ROW_NUM)
	label = label.reshape(data_size, label_size)

	with h5py.File((OUTPUT_FILE_NAME if isTrain else OUTPUT_TEST_FILE_NAME), "w") as f:
		f["matrix"] = data_topo.astype(np.float32)
		f["demand"] = data_demand.astype(np.float32)
		f["label"] = label.astype(np.float32)

def generate(isTrain, layer_no, time_involved):
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

	for i in range((TRAINING_DATA_START if isTrain else TEST_DATA_START), (TRAINING_DATA_END if isTrain else TEST_DATA_END)):
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
#		for line in range(0, TOPO_ROW_NUM):
#			res_n = [0 for x in range(0,TOPO_ROW_NUM)]
#			if line < DEMAND_ROW_NUM:
#				line_data = (demand_file.readline().split(" "))[0:DEMAND_ROW_NUM]
#				for j in range(0,DEMAND_ROW_NUM):
#					if j < DEMAND_ROW_NUM:
#						res_n[j] = int(line_data[j])
#					else:
#						res_n[j] = int(0)
#			else:
#				for j in range(0,TOPO_ROW_NUM):
#					res_n[j] = int(0)			
#			demand.append(res_n)
		demand_file.close()

		# read label(i_th topo)
		label_size = 0
		label_topo_file = open(TOPO_PATH + str(i) + ".txt")
		for line in range(0, TOPO_ROW_NUM):
			line_data = (label_topo_file.readline().split(" "))[0:TOPO_ROW_NUM]
			for item in range(0, TOPO_ROW_NUM):
				if (item > line and full_topo[line][item] == layer_no):		# upper triangle
					#label_topo.append(int(line_data[item])*2-1)
					label_topo.append(int(line_data[item]))
					label_size += 1
		
		#os.system("pause")
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
	toHDF5(data_topo, data_demand, label, label_size, isTrain)

# main function
def main(argv):
	# Parse argvs
	if (len(argv) < 3):
		print "Argument(s) missed!"
		print "Usage: python HDF5Generator.py layer_no time_involved"
		return 1
	layer_no = int(argv[1])
	time_involved = int(argv[2])

	print "processing training data..."
	generate(True, layer_no, time_involved)
	print "processing test data..."
	generate(False, layer_no, time_involved)

if __name__ == "__main__":
	main(sys.argv)
