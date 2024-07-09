import torch.nn as nn
import torch
import math
import torch.nn.function as F
class attention(nn.module):
    def __init__(self,d_model):
        super(attention,self).__init__()
        self.d_model = d_model
        self.w_query = nn.linear(d_model,d_model)
        self.w_key = nn.linear(d_model,d_model)
        self.w_value = nn.linear(d_model,d_model)
        self.out_put = nn.linear(d_model,d_model)

    def forward(self,query,key,value,mask):
        batch_size = query.size(0)
        # batch-size,seq-length,d_model
        query = self.w_query(query)
        key = self.w_key(key)
        value = self.w_value(value)
        atten = torch.matmul(query,key.transpose(-2,-1))/math.sqrts(self.d_model)
        if mask is not None:
            atten = atten.masked_fill(mask=0,-1e9)
        atten = F.softmax(atten,-1)
        output = torch.matmul(atten,value)
        output = self.out_put(output)
        return output
