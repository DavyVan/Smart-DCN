import caffe
import numpy
import os

PRETRAINED_MODEL = "_iter_100000.caffemodel"
TEST_MODEL_PATH = 'smart-net-test.protobuf'
TEST_DATA_SIZE = 200
TEST_DATA_OFFSET = 800
DEMAND_ROW_NUM = 32

def readDemand(filename):
    demand = [[0.0 for col in range(0, DEMAND_ROW_NUM)] for row in range(0, DEMAND_ROW_NUM)]
    f = open(filename, 'r')
    for i in range(0, DEMAND_ROW_NUM):
        line = f.readline().split(" ")
        for j in range(0, DEMAND_ROW_NUM):
            demand[i][j] = float(line[j])
    f.close()
    return demand

def main():
    net = caffe.Classifier(TEST_MODEL_PATH, PRETRAINED_MODEL)

    for test_index in range(0, TEST_DATA_SIZE):
        demand_file_name = "/home/fanquan/Desktop/Sample_Data/demand/demand" + str(test_index + TEST_DATA_OFFSET) + ".txt"
        demand = readDemand(demand_file_name)
        net.blobs['demand'].data[...] = demand
        print str(test_index) + "**********************data prep done******************"

        # Which layer we want to extract
        layer = "output1"
        result = net.forward()[layer][0]    # It's an array

        # normalization and round
        # find min and max
        _min = 1
        _max = 0
        for item in result:
            if item < _min:
                _min = item
            elif item > _max:
                _max = item

        #normalize
        for index, value in enumerate(result):
            result[index] = round((value - _min) / (_max - _min))

        # Construct result topo
        result_topo = [[0 for col in range(0, DEMAND_ROW_NUM)] for row in range(0, DEMAND_ROW_NUM)]
        index = 0
        for i in range(0, DEMAND_ROW_NUM):
            for j in range(0, DEMAND_ROW_NUM):
                if(j > i):
                    result_topo[i][j] = int(result[index])
                    result_topo[j][i] = int(result[index])
                    index += 1

        # Write result_topo into files
        f = open("/home/fanquan/Desktop/Result/result" + str(test_index + TEST_DATA_OFFSET) + ".txt", 'w')
        for i in range(0, DEMAND_ROW_NUM):
            for j in range(0, DEMAND_ROW_NUM):
                f.write(str(result_topo[i][j]) + " ")
            f.write("\n")
        f.close()
    print "All Done."

if __name__ == "__main__":
    main()
