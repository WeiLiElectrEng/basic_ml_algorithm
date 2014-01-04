def print_dict(d):
    for (k,v) in d.items():
        print k,' ',v


#childs��Ա���������ӽڵ�������Լ���ָ�������
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

#�����״�ĵط�line135��line141��1��headTableΪ��ʱҪֱ�ӷ���
#2��dataSet�е�ĳ��transaction��headTable���ü�����Ԫ�ؿ�����headTable���Ҳ�����Ӧ�ˣ�
def buildTree(dataSet,minSupp = 100000):
    headTable = {}
    root = TreeNode('NULL',1,None)
    #ͳ��ÿ��item���ֵ�Ƶ��
    for trans in dataSet:
        for item in trans:
            headTable[item] = headTable.get(item,0) + dataSet[trans]    
    #ɾ������֧�ֶ���ֵ��item
    for item in headTable.keys():
        if(headTable[item]<minSupp) : del(headTable[item])
    #��headtable�м���һ��ָ��item����ָ��
    for item in headTable.keys():
        headTable[item] = [headTable[item],None]
    #print_dict(headTable)
    if(len(headTable)==0) : return None,None    
    for trans,count in dataSet.items():
        #�Ȱ�item����headTable�е�Ƶ�ν�������
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

#ע�⣺ֻ���¼���ڵ�ʱ����Ҫ����headTable��item��
def updateTree(father,headTable,trans,cnt):
    item = trans[0]
    if(item in father.childs.keys()):
        father.childs[item].count += cnt
        #print '�Ѿ����ڣ�����count: ',father.childs[item].count
    else:
        #print '���ӽڵ�',item
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
    #print item,'������ģʽ��'
    #print_dict(cpb)
    #a = raw_input()
    return cpb

def mine(root,headTable,freqItemSet,baseSet):
    #print headTable
    print 'headTable',headTable
    #��Ҫ��headTable����Ƶ����������
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
            print '����Ϊ��'
            continue
        print 'newRoot,newHeadTable'
        newRoot.display(0)
        print_dict(newHeadTable)
        print '---'
        if(newRoot!=None) : mine(newRoot,newHeadTable,freqItemSet,newBaseSet)
        #a = raw_input()
    #print 'freqItemset>>>',freqItemSet,'<<<'
    ''''''
        
    
#dataSet��һ��dict,keyΪ�����ɵ�frozenset��valueΪ������ֵĴ���
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
    