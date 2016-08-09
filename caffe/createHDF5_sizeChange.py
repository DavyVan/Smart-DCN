import numpy as np
#import lmdb
#import caffe
import h5py

filenum = 100
linenum_d = 32
linenum_t = 80
topopath = './data/Topo/topo'
demandpath = './data/Demand/demand'
respath = './data/Res/res'
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
			res_n = [0 for x in range(0,linenum_t)]
			f = open(topopath + str(i)+'.txt')
			for k in range(0,linenum_t):
				res = (f.readline().split(' '))[0:linenum_t]
				for j in range(0,linenum_t):
					res[j] = int(res[j])
				topo.append(res)
			f.close()
			f = open(demandpath + str(i)+'.txt')
			for k in range(0,linenum_t):
				res_n = [0 for x in range(0,linenum_t)]
				if k < linenum_d:
					res = (f.readline().split(' '))[0:linenum_d]
					for j in range(0,linenum_t):
						if j < linenum_d:
							res_n[j] = int(res[j])
						else:
							res_n[j] = int(0)
				else:
					for j in range(0,linenum_t):
						res_n[j] = int(0)
				demand.append(res_n)
			f.close()

			f = open(respath + str(i)+'.txt')
			t = float(f.readline())
			f.close()
			x = [topo, demand]
			X.append(x)
			Y.append(t)
		X = np.array(X)
		Y = np.array(Y)
		tohdf5(X,Y,filename,linenum_t)