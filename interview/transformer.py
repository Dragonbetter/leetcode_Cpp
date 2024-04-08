import torch
import torch.nn as nn
import torch.nn.functional as F
import math
"""
transformer机制整体理解 手写 着重在于self-attention机制 
1.理清transformer的结构图
基本的单头注意力-》多头注意力-》掩码注意力
  Encoder:
  最基本的
    input->linear/MLP
    position-encoder
    add
    qkv->三个linear(注意维度变化)
    矩阵乘法
    q*k/sqrt(dk)
    softmax
    *v
    linear
  add
  layernorm
  linear
  add
  layernorm
"""

# 假设batch_size=2, seq_len=100, d_model=256, heads=8
# 这里Q,K,V维度都是相同的，由于分头了，将d_model例如拆成heads份，所以维数是[2, 8, 100, 32]
# 前面的batch 和 head维度一般都不变 而后续的 seq_len与d_model 表示的则是有seq_len个词语 每个词语的维度为32

def attention(query , key , value , mask=None , dropout=None):
    # 取query的最后一维的维度 
    d_k = query.size(-1)
    #按照注意力公式，将query与key的转置相乘，这里面key是将最后两个维度进行转置，再除以缩放系数得到注意力得分张量scores
    # 如果query是[len, embed], 那么socres是[len, len]
    scores = torch.matmul(query,key.transpose(-2,-1))/math.sqrt(d_k)
    # 考虑mask的性质
    if mask is not None:
        # mask 也是对应的[len,len] 与scores每个位置一一比较，如果mask[i][j]=0，则将scores[i][j]改为-1e9
        # 因为softmax中 极大的负数 几乎为0
        # masked_fill 是一种常用的操作，用于根据一个布尔掩码对张量中的元素进行条件替换。
        scores = scores.masked_fill(mask==0,-1e9)
    # 对最后一维进行softmax => 注意力分数矩阵的维度是len x len，其中每一行代表特定位置对所有位置的注意力分数，
    # 对于序列中的每个位置，我们都得到一个分布，表示每个位置对其他位置的关注程度。
    # 因此我们在每行上（即最后一个维度）应用 softmax
    scores = F.softmax(scores,-1)
    # todo droupout?
    if dropout is not None:
        scores = nn.Dropout(scores)
    # [B,Len,d+model] =>如果是多头则为 [B,Head,Len,d_model]
    atten = torch.matmul(scores,value)
    return atten,scores

class MultiHeadAttention(nn.Module):
    def __init__(self,d_model,h,dropout=0.1):
        super(MultiHeadAttention,self).__init__()
         # 判断h是否能被d_model整除，这是因为我们之后要给每个头分配等量的词特征
        assert d_model%h==0
        # 得到每个头获得的分割词向量维度d_k
        self.d_k = d_model //h
        self.h = h
        # 4个linear
        self.w_key = nn.Linear(d_model,d_model)
        self.w_query = nn.Linear(d_model,d_model)
        self.w_value = nn.Linear(d_model,d_model)
        self.fc_output = nn.Linear(d_model,d_model)

        self.dropout = nn.Dropout(dropout)
        self.atten = None # 返回的attention张量，现在还没有，保存给可视化使用
    
    def forward(self,query,key,value,mask=None):
        if mask is not None:
            # 使用 unsqueeze(1)：mask.unsqueeze(1) 操作在掩码的第二个维度（即 1）处添加了一个额外的维度，
            # 将掩码的形状从 [batch_size, seq_len] 变为 [batch_size, 1, seq_len]。
            # 这个额外的维度是为了匹配多头注意力中的 num_heads 维度。
            mask = mask.unsqueeze(1) # head导致的query等多了一维
        batch_size = query.size(0)
        # 重塑维度 将d_model 拆分开来 而后transpose回到正常的（Batch_size,Head,Len,d_model）
        # 计算QK时 并无需就已经设计对应的多头 因为linear相对而言 单个或多个都无关 ？？ 故而如果要对其attention 不影响
        # 多头只在计算attention score时起了作用 
        query = self.w_query(query).view(batch_size,-1,self.h,self.d_k).transpose(1,2)
        key = self.w_key(key).view(batch_size, -1, self.h, self.d_k).transpose(1,2)
        value = self.w_value(value).view(batch_size, -1, self.h, self.d_k).transpose(1,2)
        x,self.atten = attention(query,key,value,mask,self.dropout)
        # 多头的情况下 [B,Head,Len,d_model]-》[batch_size, seq_len, num_heads, head_dim]
        # .contiguous() 方法的作用是确保张量在内存中是连续的。
        # 这通常在对张量进行一系列操作（如转置、重塑、切片等）之后使用，因为这些操作可能导致张量在内存中变得不连续。
        x = x.transpose(1, 2).contiguous().view(batch_size, -1, self.h * self.d_k)
        x =self.fc_output(x)
        return x  

class FeedForwardLayer(nn.Module):
    def __init__(self, d_model, forward_expansion):
        super(FeedForwardLayer, self).__init__()
        # 先将维度提升forward_expansion倍，经过relu激活函数，又将维度降回来
        self.w1 = nn.Linear(d_model, d_model*forward_expansion)
        self.w2 = nn.Linear(d_model*forward_expansion, d_model)

    def forward(self, x):
        # 部分还会继续添加对应的dropout代码
        return self.w2((F.relu(self.w1(x))))

class LayerNorm(nn.Module):
    def __init__(self, embedding_dim, eps=1e-6):  # embedding_dim: 是一个size, 例如[batch_size, len, embedding_dim], 也可以是embedding_dim。。
        super(LayerNorm, self).__init__()
        # 用 parameter 封装，代表模型的参数，作为调节因子
        self.a = nn.Parameter(torch.ones(embedding_dim))
        self.b = nn.Parameter(torch.zeros(embedding_dim))
        self.eps = eps

    def forward(self, x):
        # 其实就是对最后一维做标准化
        mean = x.mean(-1, keepdim=True)
        std = x.std(-1, keepdim=True)
        return self.a * (x-mean) / (std+self.eps) + self.b

class PositionEmbedding(nn.Module):
    def __init__(self, d_model, max_len=1000): # max_len是每个句子的最大长度
        super(PositionEmbedding, self).__init__()

        pe = torch.zeros(max_len, d_model)
        position = torch.arange(max_len).unsqueeze(1)
        div_term = torch.exp(torch.arange(0, d_model, 2) * -(math.log(10000.0)/d_model))
        x = position * div_term
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        pe = pe.unsqueeze(0)  # pe: [max_len, d_model]
        self.register_buffer('pe', pe)

    def forward(self, x):
        x = x + Variable(self.pe[:, :x.size(1)], requires_grad=False)
        return x