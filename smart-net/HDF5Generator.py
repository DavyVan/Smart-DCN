import numpy as np
import lmdb
import caffe
import h5py

filenum = 25000
linenum = 50
topopath = './topo/topo'
demandpath = './demand/demand'
respath = './res/res'
createName = 'train.hdf5'
def tohdf5(X,y,name,size):
	N = len(y)
	print N
	X = X.reshape(N,2,size,size)
	Y = np.zeros((N,1))
	for i in range(0,N):
		Y[i] = y[i]
	with h5py.File(name, 'w') as f:  
		f['data'] = X.astype(np.float32)  
		f['label'] = Y.astype(np.float32)  

if __name__ == "__main__":

	for n in range(0,10):
		filename = 'train'+str(n)+'.hdf5'
		X = []
		Y = []
		for m in range(0,filenum):
			i = n * filenum + m
			print i
			topo = []
			demand = []
			t = 0
			f = open(topopath + str(i))
			for k in range(0,linenum):
				res = (f.readline().split(' '))[0:linenum]
				for j in range(0,linenum):
					res[j] = int(res[j])
				topo.append(res)
			f.close()
			f = open(demandpath + str(i))
			for k in range(0,linenum):
				res = (f.readline().split(' '))[0:linenum]
				for j in range(0,linenum):
					res[j] = int(res[j])
				demand.append(res)
			f.close()

			f = open(respath + str(i))
			t = float(f.readline())
			f.close()
			x = [topo, demand]
			X.append(x)
			Y.append(t)
		X = np.array(X)
		Y = np.array(Y)
		tohdf5(X,Y,filename,linenum)


