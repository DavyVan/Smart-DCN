from __future__ import division
import caffe
import numpy
import time
import datetime

MODEL = "wless" #""
ITERATION = 168048

PRETRAINED_MODEL = "_iter_"+ str(ITERATION) + ".caffemodel"
#TEST_MODEL_PATH = 'smart-net-test.protobuf'
TEST_MODEL_PATH = 'smart-net-wmw-test.protobuf'

TRAIN_DATA_SIZE = 10000
TEST_DATA_SIZE = 1000
DATA_SIZE = TRAIN_DATA_SIZE + TEST_DATA_SIZE
TRAIN_DATA_START = 0
TEST_DATA_START = 19000
TEST_DATA_OFFSET = TRAIN_DATA_SIZE

DEMAND_ROW_NUM = 8
TOPO_ROW_NUM = 8
def readDemand(filename):
    demand = [[0.0 for col in range(0, DEMAND_ROW_NUM)] for row in range(0, DEMAND_ROW_NUM)]
    f = open(filename, 'r')
    for i in range(0, DEMAND_ROW_NUM):
        line = f.readline().split(" ")
        for j in range(0, DEMAND_ROW_NUM):
            demand[i][j] = float(line[j])
    f.close()
    return demand

def readTopo(filename):
    topo = [[0.0 for col in range(0, TOPO_ROW_NUM)] for row in range(0, TOPO_ROW_NUM)]
    f = open(filename, 'r')
    for i in range(0, TOPO_ROW_NUM):
        line = f.readline().split(" ")
        for j in range(0, TOPO_ROW_NUM):
            topo[i][j] = float(line[j])
    f.close()
    return topo

    
def main():
    net = caffe.Classifier(TEST_MODEL_PATH, PRETRAINED_MODEL)
    caffe.set_mode_gpu()
    linkNum_train = [[0 for length in range(0,TRAIN_DATA_SIZE)]for resultType in range(0, 2)]
    linkNum_test = [[0 for length in range(0,TEST_DATA_SIZE)]for resultType in range(0, 2)]
    same_topo_num = [0 for i in range(0,2)]
    loss = [0 for length in range(0,DATA_SIZE)]
    time_forward = 0
    # Write loss into files
    fLoss = open("/home/fanquan/Desktop/loss/loss" + "_TRAIN_DATA_SIZE_" + str(TRAIN_DATA_SIZE) + str(PRETRAINED_MODEL) + ".txt", 'w')
    for test_index in range(0, DATA_SIZE):
	#if test_index == 2000:	
		# readDemand and readTopo
		if test_index < TRAIN_DATA_SIZE:
			plus = TRAIN_DATA_START
		else:
			plus = TEST_DATA_START - TEST_DATA_OFFSET
		demand_file_name = "/home/fanquan/Desktop/Sample_Data/wireless/demand/demand" + str(test_index+plus) + ".txt"
		demand = readDemand(demand_file_name)
		net.blobs['demand'].data[...] = demand
		
		topo_file_name = "/home/fanquan/Desktop/Sample_Data/wireless/topo_sample/topo" + str(test_index+plus) + ".txt"
		topo = readTopo(topo_file_name)
		STR = str(test_index+plus) + "**********************data prep done******************"
		print STR
		fLoss.write(STR)
		fLoss.write("\n")
	
		# Which layer we want to extract
		layer = "output1"
		if test_index < TRAIN_DATA_SIZE:		
			result = net.forward()[layer][0]    # It's an array
		else :
			#start = time.time()
			start = datetime.datetime.now()
			result = net.forward()[layer][0]    # It's an array
			#end = time.time()
			end = datetime.datetime.now()
			
			time_forward += (end - start).microseconds

		print len(result)
	
		# compute loss
		sol_ref = [0 for i in range(0,len(result))]
		ind = 0
		for i in range(0,DEMAND_ROW_NUM):
		    for j in range(0,DEMAND_ROW_NUM):
			if j > i:
			   sol_ref[ind] = topo[i][j]
			   ind += 1
			    
		for index in range(0,len(result)):
		    t = result[index] - sol_ref[index]
		    t = t*t
		    loss[test_index] += t
		loss[test_index] = loss[test_index]/2 
		#loss[test_index] = loss[test_index]/(2*len(result))
		STR = "%d th sample: loss = %f" %(test_index+plus,loss[test_index])
		print STR
		fLoss.write(STR)
		fLoss.write("\n")

		# Write rawOutput into files
		if test_index >= TRAIN_DATA_SIZE:
			f = open("/home/fanquan/Desktop/rawOutput-wireless/rawOutput" + str(test_index + plus) + ".txt", 'w')
			for item in result:
				f.write(str(item) + " ")
			f.write("\n")
			f.close()

		# normalization and round
		# find min and max
		#_min = 100
		#_max = -500
		#for item in result:
		#    if item < _min:
		#        _min = item
		#    if item > _max:
		#        _max = item
		if test_index < TRAIN_DATA_SIZE:
			_min = min(result)
			_max = max(result)
	
			STR = "min = %f, max = %f" %(_min ,_max)
			print STR
			fLoss.write(STR)
			fLoss.write("\n")
			#normalize
			for index, value in enumerate(result):
		    		temp = (value - _min) / (_max - _min)
		    		#print "index = %d,normValue = %f" %(index,temp)
		    		#temp = value
		        	result[index] = round(temp)
		else:
			start = datetime.datetime.now()
			_min = min(result)
			_max = max(result)
	
			STR = "min = %f, max = %f" %(_min ,_max)
			# print STR
			fLoss.write(STR)
			fLoss.write("\n")
			#normalize
			for index, value in enumerate(result):
		    		temp = (value - _min) / (_max - _min)
		    		#print "index = %d,normValue = %f" %(index,temp)
		    		#temp = value
		        	result[index] = round(temp)	
			end = datetime.datetime.now()
			#time_forward += (end - start).microseconds		

		# Construct result topo
		result_topo = [[0 for col in range(0, DEMAND_ROW_NUM)] for row in range(0, DEMAND_ROW_NUM)]
		index = 0

		for i in range(0, DEMAND_ROW_NUM):
		    for j in range(0, DEMAND_ROW_NUM):
		        if(j > i):
		            result_topo[i][j] = int(result[index])
		            result_topo[j][i] = int(result[index])
		            index += 1
	
		# compute the accuracy of result topo
		for i in range(0, DEMAND_ROW_NUM):
		    for j in range(0, DEMAND_ROW_NUM):
		        if(j > i) and (result_topo[i][j] == 1):
				if test_index < TRAIN_DATA_SIZE:			
					linkNum_train[0][test_index] += 1
					#print "%d %d linkNum = %d" %(i,j,linkNum_train[0][test_index])
			    		if result_topo[i][j] == topo[i][j]: 
						linkNum_train[1][test_index] += 1
						if linkNum_train[0][test_index] == 4 and linkNum_train[1][test_index]==4:
							same_topo_num[0] = same_topo_num[0] + 1
						#print "%d %d correctlinkNum = %d" %(i,j,linkNum_train[1][test_index])
				else:
					linkNum_test[0][test_index-TEST_DATA_OFFSET] += 1
					#print "%d %d linkNum = %d" %(i,j,linkNum_test[0][test_index-TEST_DATA_OFFSET])
			    		if result_topo[i][j] == topo[i][j]: 
						linkNum_test[1][test_index-TEST_DATA_OFFSET] += 1
						if linkNum_test[0][test_index-TEST_DATA_OFFSET] == 4 and linkNum_test[1][test_index-TEST_DATA_OFFSET]==4:
							same_topo_num[1] = same_topo_num[1] + 1
						#print "%d %d correctlinkNum = %d" %(i,j,linkNum_train[1][test_index-TEST_DATA_OFFSET])
		if test_index < TRAIN_DATA_SIZE:
			STR = "train set: %d th inferLinkNum = %d, CorrectLinkNum = %d" %(test_index+plus,linkNum_train[0][test_index-TEST_DATA_OFFSET] ,linkNum_train[1][test_index-TEST_DATA_OFFSET])
		else:			
			STR = "test set: %d th inferLinkNum = %d, CorrectLinkNum = %d" %(test_index+plus,linkNum_test[0][test_index-TEST_DATA_OFFSET] ,linkNum_test[1][test_index-TEST_DATA_OFFSET])	
		# print STR
		fLoss.write(STR)
		fLoss.write("\n")

		# Write result_topo into files
		if test_index >= TRAIN_DATA_SIZE:
			f = open("/home/fanquan/Desktop/Result4/result" + str(test_index+plus) + ".txt", 'w')
			for i in range(0, DEMAND_ROW_NUM):
			    for j in range(0, DEMAND_ROW_NUM):
				f.write(str(result_topo[i][j]) + " ")
			    f.write("\n")
			f.close()
    fLoss.close()

    # compute result
    inferLinkNum_mean = [0 for i in range(0,2)] # 0 for train set, 1 for test set
    correctLinkNum_mean = [0 for i in range(0,2)]
    accuracy = [0 for i in range(0,2)]
    loss_mean = [0 for i in range(0,2)]
    topo_accuracy =[0 for i in range(0,2)]
    inferLinkNum_mean[0] = sum(linkNum_train[0])/TRAIN_DATA_SIZE
    inferLinkNum_mean[1] = sum(linkNum_test[0])/TEST_DATA_SIZE
    correctLinkNum_mean[0] = sum(linkNum_train[1])/TRAIN_DATA_SIZE
    correctLinkNum_mean[1] = sum(linkNum_test[1])/TEST_DATA_SIZE
    accuracy[0] = correctLinkNum_mean[0]/inferLinkNum_mean[0]
    accuracy[1] = correctLinkNum_mean[1]/inferLinkNum_mean[1]
    loss_mean[0] =  sum(loss[0:TRAIN_DATA_SIZE])/TRAIN_DATA_SIZE
    loss_mean[1] =  sum(loss[TRAIN_DATA_SIZE:DATA_SIZE])/TEST_DATA_SIZE
    topo_accuracy[0] = same_topo_num[0]/TRAIN_DATA_SIZE
    topo_accuracy[1] = same_topo_num[1]/TEST_DATA_SIZE
    print "********************************************************************************"
    print "train set : inferLinkNum_mean = %f, CorrectLinkNum_mean = %f" %(inferLinkNum_mean[0] ,correctLinkNum_mean[0])
    print "test  set : inferLinkNum_mean = %f, CorrectLinkNum_mean = %f" %(inferLinkNum_mean[1] ,correctLinkNum_mean[1])
    print "train set ACCURACY = %f,test set ACCURACY = %f" %( accuracy[0],accuracy[1])
    print "trainLoss_mean = %f, testLoss_mean = %f" %(loss_mean[0],loss_mean[1])
    print "train topo ACCURACY = %f, test topo ACCURACY = %f" %(topo_accuracy[0],topo_accuracy[1])
    print "forward time = %f" %(time_forward)
    print "All Done."

    # write LOG into files    
    fLog = open("/home/fanquan/Desktop/LOG" + ".txt", 'a')
    #fLog.write("")
    STR = MODEL + "\t %5d\t %5d\t %8d\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t"  %(TRAIN_DATA_SIZE,TEST_DATA_SIZE,ITERATION,loss_mean[0],loss_mean[1],inferLinkNum_mean[0] ,correctLinkNum_mean[0],accuracy[0],inferLinkNum_mean[1] ,correctLinkNum_mean[1],accuracy[1],topo_accuracy[0],topo_accuracy[1],time_forward)    
    fLog.write(STR)
    fLog.write("\n")
	
    fLog.close()
if __name__ == "__main__":
    main()
