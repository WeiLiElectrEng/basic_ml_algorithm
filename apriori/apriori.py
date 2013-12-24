# -*- coding: UTF-8 -*- 
import sys
import numpy as np

def loadDataSet():
    return [[1,3,4],[2,3,5],[1,2,3,5],[2,5]]


def getC1(data):
    C1 = []
    n = len(data)
    for i in range(n):
        m = len(data[i])
        for j in range(m):
            item = data[i][j]
            if ( [item] not in C1):
                C1.append([item])
    C1.sort()
    return map(frozenset,C1)
    
#preLK：K-1项频繁项集，可以用一个set保存
def valid(LK,preLK):
    n = len(LK)
    if (n==1) : return True
    for item in LK:
        tmp = set(LK.copy())
        tmp.remove(item)
        if tmp not in preLK: return False
    return True
    

'''
删除CK中的非频繁项
首先去掉某一K-1子集不在K-1频繁项集的序列
再去掉小于支持度的序列
'''
def prune(dataSet,CK,k,L,minSupp):
    preLK = []
    if(k>1) : preLK = L[k-2]
    LK = {}
    n = len(CK)
    m = len(dataSet)
    for i in range(n):        
        if(not valid(CK[i],preLK) ): continue
        tmp = 0
        for j in range(m):
            if CK[i].issubset(dataSet[j]): tmp+=1
        if(tmp>minSupp):
            LK[CK[i]] = tmp
    #频繁项集
    rst = []
    #项集 支持度映射
    supp = {} 
    total = float(len(dataSet))
    for can in LK.keys():
        print can,'   ',LK[can]
        val = LK[can]/total
        if(val > minSupp):
            rst.append(can)
            supp[can] = val
    return rst,supp

#根据K项频繁集生成K+1项候选集
#注意参数k是当前频繁项集的item个数，而不是要生成的k+1
def join(LK,k):
    rst = []
    n = len(LK)
    for i in range(n):
        np.sort(LK[i])
    
    for i in range(n):
        for j in range(i+1,n):
            if(list(LK[i])[:k-1]==list(LK[j])[:k-1]):
                rst.append(LK[i] | LK[j])
    return map(frozenset,rst)
                

def apriori(dataSet):
    L = []
    supp = {}
    CK = getC1(dataSet)
    LK,supp = prune(dataSet,CK,1,L,0.3)
    k = 1    
    while(True):        
        if (len(LK)<=0) : break
        L.append(LK)        
        CK = join(LK,k)
        k += 1 
        LK,tmpSupp= prune(dataSet,CK,k,L,0.3)
        supp.update(tmpSupp)
    return L,supp
    

def getRules(L,supp):
    rules = []
    for i in range(1,len(L)):
        for freqSet in L[i]:
            consequent = [frozenset([item]) for item in freqSet]
            if i>1 : 
                print '!!!',consequent
                getConfB(freqSet,consequent,supp,rules,0.5)
            else   : getConfA(freqSet,consequent,supp,rules,0.5)
    for i in range(len(rules)):
        print rules[i]

#剪枝
def getConfA(freqSet,consequent,supp,rules,minConf=0.7):
    pConse = []     #修剪后的规则后件集合
    for rhs in consequent:
        print freqSet,'---',rhs
        conf = supp[freqSet]/supp[freqSet-rhs]
        if( conf >minConf ):
            rules.append( (freqSet-rhs,rhs,conf) )
            pConse.append(rhs)
    
    return pConse
    
#合并
def getConfB(freqSet,consequent,supp,rules,minConf=0.7):
    k = len(consequent[0])
    if(len(freqSet)>k+1):
        consequent = join(consequent,k)
        print 'conse',consequent
        a = raw_input()
        consequent = getConfA(freqSet,consequent,supp,rules,minConf)
        if(len(consequent)>1):
            getConfB(freqSet,consequent,supp,rules,minConf)

    
if __name__ == "__main__":
    dataSet = loadDataSet()
    L,supp = apriori(dataSet)
    getRules(L,supp)
        
