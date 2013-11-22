# -*- coding: cp936 -*-
'''
1、构建弱分类器： 树桩分类器
2、根据数据集，迭代添加弱分类器类
'''
#import numpy as np
from numpy import *

def stumpClassify(dataSet,attr_id,threshold,ineq):
    flag = (ineq=="lt")
    num = dataSet.shape[0]
    labels = ones(num)
    '''
    不需要这么麻烦，直接利用index更方便
    for i in range(num):
        if(flag):
            if(dataSet[:,i]>threshold):      labels[i] = -1.0
        else:
            if(dataSet[:,i]<threshold):       labels[i] = -1.0
    '''
    if(flag):
        labels[dataSet[:,attr_id]<=threshold]=-1.0
    else:
        labels[dataSet[:,attr_id]>threshold]=-1.0
    return labels


def buildStump(dataSet,labels,weights):
    step = 10
    bestStump = {}
    bestEst = 0
    minError = inf
    n = dataSet.shape[1]
    for i in range(n):
        minVal = dataSet[:,i].min()
        maxVal = dataSet[:,i].max()        
        stepLen = (maxVal-minVal)/step
        for j in range(-1,step+1):
            for inequal in ['lt','gt']:
                threshold = (minVal+float(j)*stepLen)
                currEst = stumpClassify(dataSet,i,threshold,inequal)
                #注意这里的error等价于平常的错误率：错误个数/总个数,只不过这里
                #每个记录用不同的权重，而不是平常所见的每个记录权1.0
                error = (weights*(currEst!=labels)).sum()
                if error<minError:
                    minError = error
                    bestStump['attr_id']=i
                    bestStump['threshold']=threshold
                    bestStump['ineq'] = inequal
                    bestEst = currEst.copy()
    return bestStump,minError,bestEst



def adaTrain(dataSet,labels):
    print "训练中...."
    maxLoop = 100
    weakClassify = []
    num = dataSet.shape[0]
    weights = ones(num,)/num
    bestEst=0
    aggreEst=zeros(num,)
    for loop in range(maxLoop):
        if loop%20==0:
            print "loop:"+str(loop)
        stump,error,weakEst = buildStump(dataSet,labels,weights)
        #print "error"+str(error)
        alpha = float(0.5*math.log((1-error)/max(error,1e-16)))
        stump['alpha'] = alpha
        weakClassify.append(stump)
        #如果预测正确则bestEst*labels为1，指数为-α
        #如果预测错误则bestEst*labels为-1，指数为α
        weights = weights * exp(-1*alpha*(weakEst*labels))
        weights/=weights.sum()
        #因为最终ada分类器需要根据每个weakclassify的预测值加权平均(并根据正负号分类)，
        #因此，还需要一个aggreEst来表示当前已有弱分类器所组合的强分类器的分类准确性
        #当aggreEst预测的错误率为0或者迭代达到最大步数时终止。
        #注意，aggreEst已经不是-1,1，而是连续的float值了，因此需要用sign()来判断其
        #正负号，从而转化成-1,1
        aggreEst+=alpha*weakEst
        x=(sign(aggreEst)!=labels).sum()/float(num)
        if x < 1e-20:
            break
    print "训练结束"
    return weakClassify


def adaClassify(dataSet,classifier):
    classEst = zeros(dataSet.shape[0],)
    for i in range(len(classifier)):
        weak = classifier[i]
        classEst+=(weak['alpha']*stumpClassify( dataSet,weak['attr_id'],weak['threshold'],weak['ineq'],))        
    return sign(classEst)

def loadData(filename):
    dataSet=[]
    labels=[]
    train=open(filename)
    for line in train.readlines():
        arr = line.strip().split('\t')
        tmp=[]
        for i in range(len(arr)-1):
            tmp.append(float(arr[i]))
        dataSet.append(tmp)
        labels.append(float(arr[len(arr)-1]))
        '''
        不能直接append，这样得到个元素都是string类型
        dataSet.append(arr[:-1])
        labels.append(arr[-1:][0])
        '''
    dataSet = array(dataSet)
    #print labels
    labels = array(labels)
    return dataSet,labels
def justdoit():
    trainData,trainLabels=loadData('train.txt')
    classifiers=adaTrain(trainData,trainLabels)    
    testData,testLabels=loadData('test.txt')
    predictLabels=adaClassify(testData,classifiers)
    error=0
    totalNum=len(testLabels)
    for i in range(totalNum):
        if testLabels[i]!=predictLabels[i]:
            error+=1
    print "预测结果"
    print "总个数:"+str(totalNum)
    print "预测错误数："+str(error)
    print "正确率:"+str(error/float(totalNum))
    '''
    结果：
    总个数:67
    预测错误数：15
    错误率:0.223880597015
    '''
    
    
