demand0：拓扑需求情况
demand为10*10的矩阵，(i,j)表示i到j的流量需求大小

Ed_topo0：实用Edmond算法得到的拓扑
topo1：使用搜索算法得到的拓扑
topo为10*10的矩阵，（i，j）表示i到j的连接情况，0表示不连接，1表示连接

route_Ed2：Edmond2算法中需要生成的子流情况
route_Sample：搜索算法中需要生成的子流情况
每2行对一个子流进行说明：
	第1行格式为i to j : demand，说明了流从i传到j，需要传的大小为demand
	第2行格式为若干个整数，以i开始，以j结尾，说明了该子流的传播路径
