import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import math
import sys

def loadDataSet(fileName):
    dim = len(open(fileName).readline().split())-1
    dataArr = []
    labelArr = []
    lines = open(fileName).readlines()
    for line in lines:
        arr = line.split()
        data = []
        for i in range(dim):
            data.append(float(arr[i]))
        labelArr.append(float(arr[dim]))
        dataArr.append(data)
    dataMat = np.mat(dataArr)
    labelMat = np.mat(labelArr).reshape(dataMat.shape[0],1)
    return dataMat,labelMat

def regression(dataMat,weight):
    return dataMat*weight

#����correlation coefficient
def correCoeff(label,labelEst):
    return np.corrcoef(label.T,labelEst.T)

def error(label,labelEst):
    diff = label - labelEst
    return ((diff.T*diff)**2).sum()

#ע���׼���е�һ��С���⣬�����ȡ�����붼��������ôpython���matrix������
#��Ϊint,��ʱ��������һ��float����ֵ��matrix�л��и��С������沿�֣�����
#0~1֮�������ȫ�����0��Ӧ��ʹ��astype()����float�͡�
def normalization(dataMat):
    n,m = dataMat.shape
    dataMat = dataMat.astype('f8')
    for i in range(m):
        tmp = dataMat[:,i]
        dataMat[:,i] = (tmp-np.mean(tmp))/np.std(tmp)
    return dataMat
        


#ƽ�������С�ع飨��С���˻ع飩Least squares regression 
def LSR(dataMat,labelMat):  
    n,dim = dataMat.shape 
    tmp = dataMat.T*dataMat
    if np.linalg.det(tmp) == 0:
        print "XT*X���죬��С���˷��޷�����"
        return
    return tmp.I*dataMat.T*labelMat

#�ֲ���Ȩ���Իع�
def LWLR(dataMat,labelMat,data,k=0.01):
    n = dataMat.shape[0]
    V = np.eye(n)
    for i in range(n):
        diff = data - dataMat[i]
        V[i][i] = math.exp(diff*diff.T/(float(-2)*k**2))
    XVX = dataMat.T*V*dataMat
    if np.linalg.det(XVX) == 0:
        print "�������죬�ֲ���Ȩ���Իع��޷�����"
        return
    return XVX.I*dataMat.T*V*labelMat
    
def ridgeRegression(dataMat,labelMat,lamb=0.2):
    n = dataMat.shape[1]
    I = np.eye(n)
    for k in range(n):
        I[k][k] = lamb
    tmp = dataMat.T*dataMat+I
    if np.linalg.det(tmp) == 0:
        print "�������죬��ع��޷�����"
        return
    return (tmp).I*dataMat.T*labelMat
    
def stageWise(dataMat,labelMat,eps=0.001,maxItr=5000):
    n,dim = dataMat.shape 
    minError = np.inf
    weight = np.zeros(dim)
    itrWeight = np.zeros(dim) 
    tryWeight = np.zeros(dim) 
    for loop in range(maxItr):
        minError = np.inf
        for i in range(dim):
            for j in [1,-1] :
                tryWeight = weight.copy()
                tryWeight[i]+=(j*eps)
                currError = error(labelMat,regression(dataMat,np.mat(tryWeight.reshape(dim,1))))   #.flatten().A[0]
                if currError < minError:
                    minError = currError
                    itrWeight = tryWeight
        if loop%100 ==0 : print ("loop %f: error %f" % (loop,minError))
        weight = np.copy(itrWeight)
    return np.mat(tryWeight.reshape(dim,1))

#���ԣ�����չʾ���ݵ�һά�ͻع�ֵyֱ�ӵĹ�ϵ
def displayLSR(dataMat,labelMat,weight):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    x = dataMat.A[:,1]
    y = labelMat.flatten().A[0]
    ax.scatter(x,y,c='r')
    yEst = regression(dataMat,weight)
    yEst = yEst.flatten().A[0]
    ax.plot(x,yEst)
    plt.show()
    
def displayLWLR(dataMat,labelMat):
    x = dataMat.A[:,1]
    y = labelMat.flatten().A[0]
    srtInd = x.argsort()
    yEst = []
    n = dataMat.shape[0]
    for i in range(n):
        weight = LWLR(dataMat,labelMat,dataMat[i])
        yEst.append(regression(dataMat[i],weight).A[0][0])
    yEst = np.array(yEst)
    print yEst.shape
    print type(yEst)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(x[srtInd],y[srtInd],c='r')   
    ax.plot(x[srtInd],yEst[srtInd])
    plt.show()

#ע��\a��python��ת���ַ�
if __name__ == "__main__":
    #dataMat,labelMat = loadDataSet('data\ex0.txt')
    #weight = LSR(dataMat,labelMat)
    #regression(dataMat,weight)
    #displayLSR(dataMat,labelMat,weight)
    #correCoeff(labelMat,regression(dataMat,weight))
    #data = np.mat(np.array([1.0,0.981083]))
    #weight = LWLR(dataMat,labelMat,data)
    #print regression(data,weight)
    #displayLWLR(dataMat,labelMat)
    
    #���� ǰ80%����Ϊѵ���� ��20%����Ϊ���Լ�
    ''''''
    dataMat,labelMat = loadDataSet('data\\abalone.txt')    
    trainNum = dataMat.shape[0]*0.8
    testNum = dataMat.shape[0]*0.25
    #weight = LSR(dataMat[:trainNum],labelMat[:trainNum])
    #weight = ridgeRegression(dataMat[:trainNum],labelMat[:trainNum])
    dataMat = normalization(dataMat)
    weight = stageWise(dataMat,labelMat)
    #print weight
    labelEst = regression(dataMat[trainNum:],weight)
    #print error(labelMat[trainNum:],labelEst)/testNum