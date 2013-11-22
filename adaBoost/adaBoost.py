# -*- coding: cp936 -*-
'''
1���������������� ��׮������
2���������ݼ��������������������
'''
#import numpy as np
from numpy import *

def stumpClassify(dataSet,attr_id,threshold,ineq):
    flag = (ineq=="lt")
    num = dataSet.shape[0]
    labels = ones(num)
    '''
    ����Ҫ��ô�鷳��ֱ������index������
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
                #ע�������error�ȼ���ƽ���Ĵ����ʣ��������/�ܸ���,ֻ��������
                #ÿ����¼�ò�ͬ��Ȩ�أ�������ƽ��������ÿ����¼Ȩ1.0
                error = (weights*(currEst!=labels)).sum()
                if error<minError:
                    minError = error
                    bestStump['attr_id']=i
                    bestStump['threshold']=threshold
                    bestStump['ineq'] = inequal
                    bestEst = currEst.copy()
    return bestStump,minError,bestEst



def adaTrain(dataSet,labels):
    print "ѵ����...."
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
        #���Ԥ����ȷ��bestEst*labelsΪ1��ָ��Ϊ-��
        #���Ԥ�������bestEst*labelsΪ-1��ָ��Ϊ��
        weights = weights * exp(-1*alpha*(weakEst*labels))
        weights/=weights.sum()
        #��Ϊ����ada��������Ҫ����ÿ��weakclassify��Ԥ��ֵ��Ȩƽ��(�����������ŷ���)��
        #��ˣ�����Ҫһ��aggreEst����ʾ��ǰ����������������ϵ�ǿ�������ķ���׼ȷ��
        #��aggreEstԤ��Ĵ�����Ϊ0���ߵ����ﵽ�����ʱ��ֹ��
        #ע�⣬aggreEst�Ѿ�����-1,1������������floatֵ�ˣ������Ҫ��sign()���ж���
        #�����ţ��Ӷ�ת����-1,1
        aggreEst+=alpha*weakEst
        x=(sign(aggreEst)!=labels).sum()/float(num)
        if x < 1e-20:
            break
    print "ѵ������"
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
        ����ֱ��append�������õ���Ԫ�ض���string����
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
    print "Ԥ����"
    print "�ܸ���:"+str(totalNum)
    print "Ԥ���������"+str(error)
    print "��ȷ��:"+str(error/float(totalNum))
    '''
    �����
    �ܸ���:67
    Ԥ���������15
    ������:0.223880597015
    '''
    
    
