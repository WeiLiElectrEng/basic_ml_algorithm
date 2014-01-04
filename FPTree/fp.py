def print_dict(d):
    for (k,v) in d.items():
        print k,' ',v


#childs成员变量保存子节点的名称以及所指向的子树
class TreeNode:
    def __init__(self,name,count,father):
        self.name = name
        self.count = count
        self.father = father
        self.childs = {}        
        self.next = None
    def display(self,pre):
        print ' '*pre,self.name,' ',self.count
        for c in self.childs.values():
            c.display(pre+2)

#两个易错的地方line135、line141：1、headTable为空时要直接返回
#2、dataSet中的某个transaction在headTable被裁减后，其元素可能在headTable中找不到对应了！
def buildTree(dataSet,minSupp = 100000):
    headTable = {}
    root = TreeNode('NULL',1,None)
    #统计每个item出现的频次
    for trans in dataSet:
        for item in trans:
            headTable[item] = headTable.get(item,0) + dataSet[trans]    
    #删除低于支持度阈值的item
    for item in headTable.keys():
        if(headTable[item]<minSupp) : del(headTable[item])
    #在headtable中加入一个指向item链的指针
    for item in headTable.keys():
        headTable[item] = [headTable[item],None]
    #print_dict(headTable)
    if(len(headTable)==0) : return None,None    
    for trans,count in dataSet.items():
        #先把item按照headTable中的频次降序排列
        tmpTrans = {}
        for item in trans:
            if(item in headTable) :tmpTrans[item] = headTable[item][0]
        if(len(tmpTrans)==0) : continue
        orderTrans = [v[0] for v in sorted(tmpTrans.items(),key=lambda p :p[1],reverse=True)]
        updateTree(root,headTable,orderTrans,count)
    
    root.display(0)
    for h in headTable.values():
        node = h[1]
        while(node!=None):
            print node.name,' ',node.count
            node = node.next
        print '-'*10
    return root,headTable

#注意：只有新加入节点时才需要更新headTable的item链
def updateTree(father,headTable,trans,cnt):
    item = trans[0]
    if(item in father.childs.keys()):
        father.childs[item].count += cnt
        #print '已经存在，增加count: ',father.childs[item].count
    else:
        #print '增加节点',item
        father.childs[item] = TreeNode(item,cnt,father)
        updateHeadTable(father.childs[item],headTable)
    if(len(trans)>1):
        updateTree(father.childs[item],headTable,trans[1:],cnt)

def updateHeadTable(node,headTable):
    if(headTable[node.name][1]!=None):
        curr = headTable[node.name][1]
        while(curr.next!=None): curr = curr.next
        curr.next = node
    else:
        headTable[node.name][1] = node

def backTracking(node,path):
    node = node.father
    while(node.name!='NULL'):
        path.append(node.name)
        node = node.father
        
    
def condPatternBase(item,headTable):
    cpb = {}
    node = headTable[item][1]
    while(node!=None):
        path = []
        backTracking(node,path)
        if(len(path)>0) : cpb[frozenset(path)] = node.count
        node = node.next
    #root.display(0)
    #print item,'的条件模式基'
    #print_dict(cpb)
    #a = raw_input()
    return cpb

def mine(root,headTable,freqItemSet,baseSet):
    #print headTable
    print 'headTable',headTable
    #先要对headTable按照频次升序排列
    items = [v[0] for v in sorted(headTable.items(),key = lambda p:p[1])]
    for item in headTable.keys():
        '''
        print 'item',item
        print 'mine baseset',baseSet,'!!!'
        print 'freqset',freqItemSet
        '''
        #aa = raw_input()
        newBaseSet = baseSet.copy()
        newBaseSet.add(item)
        freqItemSet.append(newBaseSet)
        cpb = condPatternBase(item,headTable)
        print 'cpb'
        print_dict(cpb)
        #print item,'>>>',cpb,'<<<'
        newRoot,newHeadTable = buildTree(cpb)
        if(newRoot==None) :
            print '子树为空'
            continue
        print 'newRoot,newHeadTable'
        newRoot.display(0)
        print_dict(newHeadTable)
        print '---'
        if(newRoot!=None) : mine(newRoot,newHeadTable,freqItemSet,newBaseSet)
        #a = raw_input()
    #print 'freqItemset>>>',freqItemSet,'<<<'
    ''''''
        
    
#dataSet是一个dict,key为由项集组成的frozenset，value为该项集出现的次数
def loadDataSet():
    d = open('kosarak.dat')
    data = {}
    for line in d.readlines():
        x = line.split()
        data[frozenset(x)] = data.get(frozenset(x),0) + 1
    #print data
    '''
    data = [['r', 'z', 'h', 'j', 'p'],
               ['z', 'y', 'x', 'w', 'v', 'u', 't', 's'],
               ['z'],
               ['r', 'x', 'n', 'o', 's'],
               ['y', 'r', 'x', 'z', 'q', 't', 'p'],
               ['y', 'z', 'x', 'e', 'q', 's', 't', 'm']]
    dataSet = {}
    for d in data:
        dataSet[frozenset(d)] = 1
    '''
    return data




if __name__ == "__main__":
    data = loadDataSet()
    print type(data)
    i = 0
    for k,v in data.items():
        print k,v
        i += 1
        if (i>10) : break
    root,headTable = buildTree(data) 
    freqItemSet = []
    baseSet = set([])
    mine(root,headTable,freqItemSet,baseSet)
    print freqItemSet
    